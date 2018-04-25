class PluginManager
{	
	private ref array<typename>					m_PluginRegister;	// list of modules for creation
	private ref map<typename, ref PluginBase>	m_PluginsPtrs;		// plugin, plugin pointer
	
	//=================================
	// Constructor
	//=================================
	void PluginManager()
	{
		m_PluginRegister	= new array<typename>;
		m_PluginsPtrs		= new map<typename, ref PluginBase>;
	}
	
	//=================================
	// Destructor
	//=================================
	void ~PluginManager()
	{
		int i;
		PluginBase plugin;
		
		for ( i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				plugin.OnDestroy();
				delete plugin;
			}
		}
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.MainOnUpdate);
	}
	
	//=================================
	// Init
	//=================================
	void Init()
	{	
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 						Client	Server
		//----------------------------------------------------------------------
		RegisterPlugin( "PluginHorticulture",					true, 	true );
		RegisterPlugin( "PluginRepairing",						true, 	true );
		RegisterPlugin( "PluginPlayerStatus",					true, 	true );
		RegisterPlugin( "PluginMessageManager",					true, 	true );
		RegisterPlugin( "PluginLifespan",						true, 	true );
		RegisterPlugin( "PluginVariables",						true, 	true );
		RegisterPlugin( "PluginObjectsInteractionManager",		false, 	true );
		RegisterPlugin( "PluginRecipesManager",					true, 	true );
		RegisterPlugin( "PluginTransmissionAgents",				true, 	true );
		RegisterPlugin( "PluginAdditionalInfo",					true,	true ); //TODO clean up after Gamescom
		RegisterPlugin( "PluginConfigEmotesProfile",			true, 	true );
		
		// Only In Debug / Internal
		RegisterPluginDebug( "PluginConfigViewer",				true, 	true );
		RegisterPluginDebug( "PluginLocalEnscriptHistory",		true, 	true );
		RegisterPluginDebug( "PluginKeyBinding",				true, 	false );
		RegisterPluginDebug( "PluginDeveloper",					true, 	true );
		RegisterPluginDebug( "PluginDeveloperSync",				true, 	true );
		
		RegisterPluginDebug( "PluginSceneManager",				true, 	true );
		RegisterPluginDebug( "PluginConfigScene",				true, 	true );
		RegisterPluginDebug( "PluginMissionConfig",				true, 	true );
		RegisterPluginDebug( "PluginConfigEmotesProfile",		true, 	true );
		RegisterPluginDebug( "PluginConfigDebugProfile",		true, 	true );
		RegisterPluginDebug( "PluginConfigDebugProfileFixed",	true, 	true );
		
		RegisterPluginDebug( "PluginItemDiagnostic",			true, 	true );
		RegisterPluginDebug( "PluginDayzPlayerDebug",			true, 	true );
		RegisterPluginDebug( "PluginDiagMenu",					true, 	true );
		RegisterPluginDebug( "PluginDayZCreatureAIDebug",		true, 	true );
		RegisterPluginDebug( "PluginDoorRuler",					true, 	false );
		RegisterPluginDebug( "PluginPermanentCrossHair",		true,	false );
		RegisterPluginDebug( "PluginRemotePlayerDebugClient",	true,	false );
		RegisterPluginDebug( "PluginRemotePlayerDebugServer",	false,	true );
		RegisterPluginDebug( "PluginDrawCheckerboard",			true,	false );
		//RegisterPluginDebug( "PluginSoundDebug",				true,	false );
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(this.MainOnUpdate);
	}
	
	//=================================
	// PluginsInit
	//=================================
	void PluginsInit()
	{
		int i = 0;
		
		for ( i = 0; i < m_PluginRegister.Count(); ++i)
		{
			typename plugin_type = m_PluginRegister.Get(i);
			
			PluginBase module_exist = GetPluginByType( plugin_type );
			if ( module_exist )
			{
				m_PluginsPtrs.Remove( plugin_type );
			}
			
			PluginBase module_new = plugin_type.Spawn();
			m_PluginsPtrs.Set(plugin_type, module_new);
		}
		
		for ( i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			PluginBase plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				plugin.OnInit();
			}
		}
		
		PluginSceneManager oPlugin = GetPluginByType(PluginSceneManager);
		
		oPlugin.EditorToggle();
	}
	
	//=================================
	// MainOnUpdate
	//=================================
	void MainOnUpdate(float delta_time)
	{
		for ( int i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			PluginBase plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				plugin.OnUpdate(delta_time);
			}
		}
	}
	
	/**
	\brief Returns registred plugin by class type, better is to use global funtion GetPlugin(typename plugin_type)
		\param module_tpye \p typename class type of plugin
		\return \p PluginBase
		@code
			PluginRepairing plugin = GetPluginManager().GetPluginByType(PluginRepairing);
		@endcode
	*/
	//=================================
	// GetPluginByType
	//=================================
	PluginBase GetPluginByType( typename plugin_type )
	{
		if ( m_PluginsPtrs.Contains( plugin_type ) )
		{
			return m_PluginsPtrs.Get( plugin_type );
		}
		
		return NULL;
	}
	
	/**
	\brief Register new PluginBase to PluginManager for storing and handling plugin.
		\param module_tpye \p typename class type of plugin
		\return \p void
		@code
			class PluginRepairing extends PluginBase
			{
			...
			}
			
			RegisterPlugin(PluginRepairing);
		@endcode
	*/
	//=================================
	// RegisterPlugin
	//=================================
	protected void RegisterPlugin( string plugin_class_name, bool reg_on_client, bool reg_on_server, bool reg_on_release = true )
	{
		if ( !reg_on_client )
		{
			if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
			{
				return;
			}
		}
		
		if ( !reg_on_server )
		{
			if ( GetGame().IsMultiplayer() )
			{
				if ( GetGame().IsServer() )
				{
					return;
				}
			}
		}
		
		if ( !reg_on_release )
		{
			if ( !GetGame().IsDebug() )
			{
				return;
			}
		}
		
		m_PluginRegister.Insert( plugin_class_name.ToType() );
	}
	
	/**
	\brief Register new PluginBase to PluginManager for storing and handling plugin.
		\param module_tpye \p typename class type of plugin
		\return \p void
		@code
			class PluginRepairing extends PluginBase
			{
			...
			}
			
			RegisterPlugin(PluginRepairing);
		@endcode
	*/
	//=================================
	// RegisterPlugin
	//=================================
	protected void RegisterPluginDebug( string plugin_class_name, bool reg_on_client, bool reg_on_server )
	{
		RegisterPlugin(plugin_class_name, reg_on_client, reg_on_server, false);
	}
	
	//---------------------------------
	// UnregisterPlugin
	//---------------------------------
	protected bool UnregisterPlugin( string plugin_class_name )
	{
		typename plugin_type = plugin_class_name.ToType();
		
		if ( m_PluginRegister.Find( plugin_type ) != -1 )
		{
			m_PluginRegister.RemoveItem( plugin_type );
			return true;
		}
		
		return false;
	}
}

ref PluginManager g_Plugins;

/**
\brief Returns registred plugin by class type, better is to use global funtion GetPlugin(typename plugin_type)
	\param module_tpye \p typename class type of plugin
	\return \p PluginBase
	@code
		PluginRepairing plugin = GetPluginManager().GetPluginByType(PluginRepairing);
	@endcode
*/
PluginManager GetPluginManager()
{	
	/*
	if ( g_Plugins == NULL )
	{
		PluginManagerInit();
	}
	*/
	
	return g_Plugins;
}


void PluginManagerInit()
{	
	if (g_Plugins == NULL)
	{
		g_Plugins = new PluginManager;
		g_Plugins.Init();
		g_Plugins.PluginsInit();
	}
}

void PluginManagerDelete()
{	
	if ( g_Plugins )
	{
		delete g_Plugins;
		g_Plugins = NULL;
	}
}

bool IsPluginManagerExists()
{
	if ( g_Plugins != NULL )
	{
		return true;
	}	
	
	return false;
}

PluginBase GetPlugin(typename plugin_type)
{
	PluginBase plugin = NULL;
	
	if ( IsPluginManagerExists () )
	{
		plugin = GetPluginManager().GetPluginByType(plugin_type);
	
		if ( plugin == NULL )
		{
			if ( GetGame().IsDebug() && IsPluginManagerExists() )
			{
				PrintString("Module " + plugin_type.ToString() + " is not Registred in PluginManager.c!");
				DumpStack();
			}
			return NULL;
		}
	}
	
	return plugin;
}

bool IsModuleExist(local typename plugin_type)
{
	if ( IsPluginManagerExists() )
	{
		return ( GetPlugin(plugin_type) != NULL );
	}
	
	return false;
}
