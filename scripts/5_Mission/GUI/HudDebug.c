// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class HudDebugEventHandler extends ScriptedWidgetEventHandler
{
	HudDebug m_HudDebug;
	
	void HudDebugEventHandler( HudDebug hud_debug )
	{
		m_HudDebug = hud_debug;
	}
	
	HudDebug GetHudDebug()
	{
		return m_HudDebug;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		return GetHudDebug().OnClick( w, x, y, button );
	}
}

class HudDebug extends Hud
{
	static const int HUD_WIN_UNDEFINED			= 0;
	static const int HUD_WIN_CHAR_STATS			= 1;
	static const int HUD_WIN_CHAR_MODIFIERS		= 2;
	static const int HUD_WIN_CHAR_AGENTS		= 3;
	static const int HUD_WIN_CHAR_DEBUG			= 4;
	
	Widget							m_WgtRoot;
	Widget							m_Crosshair;
	ref array<ref HudDebugWinBase>	m_Panels;
	ref Timer						m_TimerUpdate;
	ref HudDebugEventHandler 		m_HudDebugHandler;
	ref HudDebugWinCharModifiers 	m_WinCharModifiers;
	
	//============================================
	// HudDebug
	//============================================
	void HudDebug()
	{
	}

	//============================================
	// ~HudDebug
	//============================================
	void ~HudDebug()
	{
		delete m_WgtRoot;
		m_TimerUpdate.Stop();
	}
	
	//============================================
	// Init
	//============================================
	override void Init( Widget hud_panel_widget )
	{
		m_WgtRoot = hud_panel_widget;
		m_WgtRoot.Show( true );
		
		// Crosshair widget root
		m_Crosshair = m_WgtRoot.FindAnyWidget( "wdw_Crosshair" );
		
		m_Panels = new array<ref HudDebugWinBase>;
		
		// Register Window Character Stats
		HudDebugWinCharStats win_char_stats = new HudDebugWinCharStats( m_WgtRoot.FindAnyWidget( "wdw_CharacterStats" ) );
		m_Panels.Insert( win_char_stats );
		
		// Register Window Chracter Modifiers
		m_WinCharModifiers = new HudDebugWinCharModifiers( m_WgtRoot.FindAnyWidget( "wdw_CharacterModifiers" ) );
		m_Panels.Insert( m_WinCharModifiers );
		
		// Register Window Chracter Agents
		HudDebugWinCharAgents win_char_agents = new HudDebugWinCharAgents( m_WgtRoot.FindAnyWidget( "wdw_CharacterAgents" ) );
		m_Panels.Insert( win_char_agents );
		
		// Register Window Chracter Debug
		HudDebugWinCharDebug win_char_debug = new HudDebugWinCharDebug( m_WgtRoot.FindAnyWidget( "wdw_CharacterDebug" ) );
		m_Panels.Insert( win_char_debug );
		
		RefreshByLocalProfile();
		RefreshCrosshairVisibility();
		
		m_TimerUpdate = new Timer();
		m_TimerUpdate.Run( 1.0, this, "Update", NULL, true );
		
		//set ui event handler
		m_HudDebugHandler = new HudDebugEventHandler( this );
		m_WgtRoot.SetHandler( m_HudDebugHandler );
	}
	
	//============================================
	// Update
	//============================================
	override void Update()
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			if ( m_Panels.Get( i ).IsVisible() )
			{
				m_Panels.Get( i ).Update();
			}
		}
	}

	//============================================
	// SetPanetVisible
	//============================================
	void SetPanelVisible(int panel_type, bool visible)
	{
		if ( visible )
		{
			PanelShow( panel_type );
		}
		else
		{
			PanelHide( panel_type );
		}
	}
	
	//============================================
	// PanelShow
	//============================================
	void PanelShow(int panel_type)
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			HudDebugWinBase panel = m_Panels.Get( i );
			
			if ( panel.GetType() == panel_type )
			{
				panel.Show();
			}
		}
	}

	//============================================
	// PanelHide
	//============================================
	void PanelHide(int panel_type)
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			HudDebugWinBase panel = m_Panels.Get( i );
			
			if ( panel.GetType() == panel_type )
			{
				panel.Hide();
			}
		}
	}

	//============================================
	// RefreshCrosshairVisibility
	//============================================
	void RefreshCrosshairVisibility()
	{
		PluginConfigDebugProfile module_cfg_profile = PluginConfigDebugProfile.Cast( GetPlugin( PluginConfigDebugProfile ) );
		
		if ( module_cfg_profile )
		{
			PluginDeveloper modul_dev = PluginDeveloper.Cast( GetPlugin( PluginDeveloper ) );

			if ( modul_dev.IsEnabledFreeCamera() )
			{
				m_Crosshair.Show( module_cfg_profile.GetFreeCameraCrosshairVisible() );
			}
			else
			{
				m_Crosshair.Show( false );
			}
		}
	}

	
	//============================================
	// HideCrosshairVisibility
	//============================================
	void HideCrosshairVisibility()
	{
		PluginConfigDebugProfile module_cfg_profile = PluginConfigDebugProfile.Cast( GetPlugin( PluginConfigDebugProfile ) );
		
		if ( module_cfg_profile )
		{
			PluginDeveloper modul_dev = PluginDeveloper.Cast( GetPlugin( PluginDeveloper ) );

			if ( modul_dev.IsEnabledFreeCamera() )
			{
				m_Crosshair.Show( false );
			}
		}
	}

	//============================================
	// RefreshByLocalProfile
	//============================================
	void RefreshByLocalProfile()
	{
		PluginConfigDebugProfile module_cfg_profile = PluginConfigDebugProfile.Cast( GetPlugin( PluginConfigDebugProfile ) );
		
		if ( module_cfg_profile )
		{
			SetPanelVisible( HudDebug.HUD_WIN_CHAR_STATS, 		module_cfg_profile.GetCharacterStatsVisible() );
			SetPanelVisible( HudDebug.HUD_WIN_CHAR_MODIFIERS, 	module_cfg_profile.GetCharacterModifiersVisible() );
			SetPanelVisible( HudDebug.HUD_WIN_CHAR_AGENTS,	 	module_cfg_profile.GetCharacterAgentsVisible() );
			SetPanelVisible( HudDebug.HUD_WIN_CHAR_DEBUG, 		module_cfg_profile.GetCharacterDebugVisible() );
		}
	}
	
	//============================================
	// IsInitialized
	//============================================
	bool IsInitialized()
	{
		if ( m_WgtRoot == NULL )
		{
			return false;
		}
		
		return false;
	}	
	
	//============================================
	// OnClick
	//============================================
	bool OnClick( Widget w, int x, int y, int button )
	{	
		//send OnClick to HudDebugWinCharModifiers
		if ( m_WinCharModifiers )
		{
			m_WinCharModifiers.OnClick( w, x, y, button );

			return true;
		}
		
		return false;
	}
}
