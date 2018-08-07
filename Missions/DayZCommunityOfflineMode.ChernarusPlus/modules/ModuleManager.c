class ModuleManager
{

	private ref set<ref Module> m_Modules;

	//For mousehandler - keeps track of last press/release time
	ref array<ref MouseButtonInfo> m_MouseButtons; 
	const int CLICK_TIME			= 200; //ms
	const int HOLD_CLICK_TIME_MIN	= 200; //ms
	const int DOUBLE_CLICK_TIME		= 300; //ms

    void ModuleManager()
    {
		Print( "ModuleManager::ModuleManager()" );

		m_MouseButtons = new array<ref MouseButtonInfo>;
		
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.LEFT ) );
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.RIGHT ) );
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.MIDDLE ) );

		m_Modules 	   = new set<ref Module>;
    }

    void RegisterModule( ref Module oModule )
    {
		oModule.SetModuleManager( this );
        m_Modules.Insert( oModule );
		Print("Registered module: " + oModule.GetModuleName());
    }

    void InitializeModules()
    {
		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).Init();
			Print("Initialized module: " + m_Modules.Get(i).GetModuleName());
		}
    }

    ref Module GetModule( typename module_Type )
	{
		for ( int i = 0; i < m_Modules.Count(); ++i )
		{
			ref Module module = m_Modules.Get(i);
			if ( module.GetModuleType() == module_Type)
			{
				return module;
			}
		}

		return NULL;
	}

	ref Module GetModuleByName( string module_name )
	{
		for ( int i = 0; i < m_Modules.Count(); ++i )
		{
			ref Module module = m_Modules.Get( i );

			if (module.GetModuleName() == module_name) 
			{
				return module;
			}
		}

		return NULL;
	}

    void OnMissionStart()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionStart();
		}
    }

    void OnMissionFinish()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionFinish();
		}
    }

    void OnMissionLoaded()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionLoaded();
		}
    }

    void OnUpdate( float timeslice )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i) 
		{
			Module module = m_Modules.Get(i);
			
			for ( int kb = 0; kb < module.GetBindings().Count(); ++kb )
			{
				KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);
				
				if ( k_m_Binding.IsRecurring() )
				{
					if (k_m_Binding.Check()) 
					{
						int mouseButton = -1;
	
						bool hasDrag = false;
						for ( int mb = 0; mb < k_m_Binding.GetMouseBinds().Count(); ++mb) 
						{
							int mouseBind = k_m_Binding.GetMouseBinds().GetKey(mb);
							int mouseEvent = k_m_Binding.GetMouseBinds().Get(mouseBind);
							
							if ( mouseEvent == MB_EVENT_DRAG ) 
							{
								hasDrag = true;
								mouseButton = mouseBind;
							}
							else if ( mouseEvent == MB_EVENT_HOLD ) 
							{
								mouseButton = mouseBind;
							}
						}
						
						if ( mouseButton > -1 )
						{
							MouseButtonInfo info = m_MouseButtons.Get( mouseButton );
							if ( info ) 
							{
								if ( info.IsButtonDown() ) 
								{
									int time_curr = GetGame().GetTime();
									int time_hold = info.GetTimeLastPress() + HOLD_CLICK_TIME_MIN;
									
									if ( time_hold < time_curr )
									{
										if ( hasDrag ) 
										{
											GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
										}
										else if ( k_m_Binding.ContainsButtonEvent( mouseButton, MB_EVENT_HOLD) )
										{
											GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
										}
									}
								}
							}
						}
						
						if ( k_m_Binding.ContainsButton( MouseState.WHEEL ) )
						{
							if ( GetMouseState (MouseState.WHEEL) != 0 )
							{
								GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, GetMouseState( MouseState.WHEEL ) );
							}
						}
					}
				}
			}

			module.onUpdate( timeslice );
		}
    }

    private void ModuleMouseCheck( int button, int mouseEvent ) 
	{
		MouseButtonInfo button_info = GetMouseButtonInfo( button );
		if (button_info == NULL) return;
		if ( mouseEvent == MB_EVENT_PRESS ) button_info.Press();
		
		int time_curr			= GetGame().GetTime();
		int time_last_press		= button_info.GetTimeLastPress();
		int time_last_release	= button_info.GetTimeLastRelease();	
		int time_delta_press	= time_curr - time_last_press;
		int time_delta_release	= time_curr - time_last_release;
		
		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			Module module = m_Modules.Get(i);

			for ( int kb = 0; kb < module.GetBindings().Count(); ++kb ) 
			{
				KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);
				
				if ( GetGame().GetUIManager().GetMenu() ) 
				{
					if ( !k_m_Binding.canUseInMenu() ) 
					{
						continue;
					}
				}
				
				if ( k_m_Binding.ContainsButton( button ) ) 
				{
					if ( k_m_Binding.Check() ) 
					{
						if ( mouseEvent == MB_EVENT_RELEASE ) 
						{
							if ( time_delta_release < DOUBLE_CLICK_TIME )
							{
								if ( k_m_Binding.ContainsButtonEvent( button, MB_EVENT_DOUBLECLICK ) )
								{
									GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
								}
							}
							else if ( time_delta_press < CLICK_TIME )
							{
								if ( k_m_Binding.ContainsButtonEvent( button, MB_EVENT_CLICK ) )
								{
									GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
								}
							} 
							else 
							{
								if ( k_m_Binding.ContainsButtonEvent( button, MB_EVENT_RELEASE ) )
								{
									GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
								} 
							}
						} 
						else if ( k_m_Binding.ContainsButtonEvent( button, MB_EVENT_PRESS ) ) 
						{
								GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
						}
					}
				}
			}
			
			switch ( mouseEvent ) 
			{
				case MB_EVENT_PRESS:
					module.onMouseButtonPress( button );
					break;
				case MB_EVENT_RELEASE:
					module.onMouseButtonRelease( button );
					break;
			}
		}
		
		if ( mouseEvent == MB_EVENT_RELEASE ) button_info.Release();
	}
	
	private void ModuleKeyCheck( int key, int keyEvent ) 
	{
		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			Module module = m_Modules.Get(i);

			for ( int kb = 0; kb < module.GetBindings().Count(); ++kb ) 
			{
				KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);
				
				if ( GetGame().GetUIManager().GetMenu() ) 
				{
					if ( !k_m_Binding.canUseInMenu() ) 
					{
						continue;
					}
				}
				
				if ( k_m_Binding.ContainsKeyEvent( key, keyEvent ) ) 
				{
					if ( k_m_Binding.Check() ) 
					{
						GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
					}
				}
			}
			
			switch ( keyEvent ) 
			{
				case KB_EVENT_PRESS:
					module.onKeyPress( key ); //extra utility
					break;
				case KB_EVENT_RELEASE:
					module.onKeyRelease( key );
					break;
			}
		}
	}

    private MouseButtonInfo GetMouseButtonInfo( int button )
	{	
		for ( int i = 0; i < m_MouseButtons.Count(); ++i )
		{
			MouseButtonInfo info = m_MouseButtons.Get(i);
			
			if ( info.GetButtonID() == button )
			{
				return info;
			}
		}
		return NULL;
	}

    void OnMouseButtonRelease(int button)
	{		
		ModuleMouseCheck( button, MB_EVENT_RELEASE );
	}

	void OnMouseButtonPress( int button )
	{		
		ModuleMouseCheck( button, MB_EVENT_PRESS );
	}

	void OnKeyPress( int key )
	{
		ModuleKeyCheck( key, KB_EVENT_PRESS );
    }

	void OnKeyRelease(int key)
	{
		ModuleKeyCheck( key, KB_EVENT_RELEASE );
    }
}