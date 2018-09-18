class CommunityOfflineClient : MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_bLoaded;

	protected bool m_CanPause = true;

	void CommunityOfflineClient()
	{
	    Print( "CommunityOfflineClient::CommunityOfflineClient()" );
	    m_bLoaded = false;

		GetModuleManager();
	}

	void ~CommunityOfflineClient()
	{
	    Print( "CommunityOfflineClient::~CommunityOfflineClient()" );
	}

	override void OnInit()
	{
		super.OnInit();

        InitHive();

        SetupWeather();

		SpawnPlayer();

        GetModuleManager().OnInit();
		
		g_Game.SetProfileOption(EDayZProfilesOptions.GAME_MESSAGES, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
        GetModuleManager().OnMissionStart();
		
		GetGame().GetUIManager().CloseMenu( MENU_INGAME );
	}

	override void OnMissionFinish()
	{
        GetModuleManager().OnMissionFinish();
		
		CloseAllMenus();

		DestroyAllMenus();

		GetGame().GetUIManager().CloseMenu( MENU_INGAME );

		if( GetHive() )
		{
			DestroyHive();
		}

		super.OnMissionFinish();
	}

    void OnMissionLoaded()
    {		
		GetModuleManager().OnMissionLoaded();
    }

	void SetCanPause( bool can )
	{
		m_CanPause = can;
	}
	
	bool CanPause()
	{
		return m_CanPause;
	}

	override void Pause()
	{
		if ( IsPaused() || !CanPause() )
		{
			return;
		} 
		
		CloseAllMenus();
		
		// open ingame menu
		GetUIManager().EnterScriptedMenu(MENU_INGAME, NULL);
		m_IsOpenPauseMenu = true;
	}

	#ifdef MODULE_OVERRIDEMENUS
	override UIScriptedMenu CreateScriptedMenu( int id )
	{
		UIScriptedMenu menu = super.CreateScriptedMenu( id );

		OverrideMenus om = OverrideMenus.Cast(GetModuleManager().GetModuleByName("OverrideMenus"));

		if ( om )
		{
			UIScriptedMenu tempMenu = om.CreateScriptedMenu( id );

			if ( tempMenu )
			{
				menu = tempMenu;
			}
		}

		if ( menu )
		{
			menu.SetID( id );
		}

		return menu;
	}
	#endif

	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

        GetModuleManager().OnUpdate( timeslice );

        if( !m_bLoaded && !GetDayZGame().IsLoading() )
        {
            m_bLoaded = true;
            OnMissionLoaded();
        }
	}

	override void OnMouseButtonRelease( int button )
	{
		super.OnMouseButtonRelease( button );

		GetModuleManager().OnMouseButtonRelease( button );
	}

	override void OnMouseButtonPress( int button )
	{
		super.OnMouseButtonPress( button );

		GetModuleManager().OnMouseButtonPress( button );
	}

	override void OnKeyPress( int key )
	{
		super.OnKeyPress(key);

		GetModuleManager().OnKeyPress( key );
	}

	override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		GetModuleManager().OnKeyRelease( key );
	}

    void SpawnPlayer()
    {
		#ifndef MODULE_PERSISTENCY
		GetGame().SelectPlayer( NULL, CreateCustomDefaultCharacter() );
		#endif
    }

	void InitHive()
	{
		if ( GetGame().IsClient() && GetGame().IsMultiplayer() ) return;

		// RD /s /q "storage_-1" > nul 2>&1
		if ( !HIVE_ENABLED ) return;
	
		Hive oHive = GetHive();
		
		if( !oHive )
		{
			oHive = CreateHive();
		}

		if( oHive )
		{
			oHive.InitOffline();
		}

		oHive.SetShardID("100");
		oHive.SetEnviroment("stable");
	}

    static void SetupWeather()
    {
        Weather weather = g_Game.GetWeather();

        weather.GetOvercast().SetLimits( 0.0 , 2.0 );
        weather.GetRain().SetLimits( 0.0 , 2.0 );
        weather.GetFog().SetLimits( 0.0 , 2.0 );

        weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );

        weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
        weather.GetRain().SetForecastTimeLimits( 600 , 600 );
        weather.GetFog().SetForecastTimeLimits( 600 , 600 );

        weather.GetOvercast().Set( 0.0, 0, 0 );
        weather.GetRain().Set( 0.0, 0, 0 );
        weather.GetFog().Set( 0.0, 0, 0 );

        weather.SetWindMaximumSpeed( 50 );
        weather.SetWindFunctionParams( 0, 0, 1 );
    }
}
