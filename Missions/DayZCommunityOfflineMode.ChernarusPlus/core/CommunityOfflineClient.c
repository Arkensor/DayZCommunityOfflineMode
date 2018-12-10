class CommunityOfflineClient : MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_bLoaded;

	protected bool m_CanPause = true;

	protected bool m_IsOpenPauseMenu = false;

	void CommunityOfflineClient()
	{
	    Print( "CommunityOfflineClient::CommunityOfflineClient()" );
	    m_bLoaded = false;

		NewModuleManager();
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

        DayZPlayerCameras.RegisterTransitionTime(DayZPlayerCameras.DAYZCAMERA_1ST, DayZPlayerCameras.DAYZCAMERA_OPTICS, DayZPlayerCameras.TIME_CAMERACHANGE_02, true);

		GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\BarrelCrosshair\\gui\\layouts\\BarrelCrosshair.layout" );
	
		GetDayZGame().SetMissionPath("$saves:CommunityOfflineMode\\"); // CameraToolsMenu
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
        GetModuleManager().RegisterModules();
        GetModuleManager().OnInit();
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
		if ( IsPaused() )
		{
			return;
		} 

		if ( CanPause() )
		{
			CloseAllMenus();
			
			// open ingame menu
			GetUIManager().EnterScriptedMenu(MENU_INGAME, NULL);
		}
	}

	#ifdef MODULE_UIEXTENDER
	override UIScriptedMenu CreateScriptedMenu( int id )
	{
		UIScriptedMenu menu = super.CreateScriptedMenu( id );

		UIExtender om = UIExtender.Cast(GetModuleManager().GetModuleByName("UIExtender"));

		if ( om )
		{
			UIScriptedMenu tempMenu = om.CreateScriptedMenu( id, menu );

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

		//GetModuleManager().OnMouseButtonRelease( button );
	}

	override void OnMouseButtonPress( int button )
	{
		super.OnMouseButtonPress( button );

		//GetModuleManager().OnMouseButtonPress( button );
	}

	override void OnKeyPress( int key )
	{
		super.OnKeyPress(key);

		//GetModuleManager().OnKeyPress( key );
		
		if ( key == KeyCode.KC_M )
		{ 
			Object physicsObj = GetGame().CreateObject( "Apple", GetPlayer().GetPosition() + "0 4 0" );

			if ( physicsObj == NULL ) return;

			dBodyDestroy( physicsObj );

			autoptr PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateSphere( 0.1 ), "material/default", 0xffffffff)};
			dBodyCreateDynamicEx( physicsObj , "0 0 0", 1.0, geoms );
		}
		
//		if ( key == KeyCode.KC_N )
//		{
//			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
//			if (player && player.GetItemInHands() && !GetUIManager().GetMenu())
//			{
//				ActionManagerClient manager = ActionManagerClient.Cast(player.GetActionManager());
//				manager.ActionDropItemStart(player.GetItemInHands(),null);
//			}
//		}
	}

	override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		//GetModuleManager().OnKeyRelease( key );
		
		if ( key == KeyCode.KC_PERIOD )
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				//TODO reconnect when appropriate
				GesturesMenu.CloseMenu();
			}
		}
	}

    void SpawnPlayer()
    {
		#ifndef MODULE_PERSISTENCY
		GetGame().SelectPlayer( NULL, CreateCustomDefaultCharacter() );
		#endif

		#ifdef DISABLE_PERSISTENCY
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
