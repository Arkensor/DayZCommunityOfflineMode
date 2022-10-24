class CommunityOfflineClient extends MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_loaded;

	void CommunityOfflineClient()
	{
	    m_loaded = false;

		NewModuleManager();
	}

	override void OnInit()
	{
		startCOM();
		super.OnInit();
        InitHive();
        SetupWeather();
		SpawnPlayer();
		GetDayZGame().SetMissionPath(BASE_COM_DIR + "\\");
		
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
        COM_GetModuleManager().OnInit();
		COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
		stopCOM();
        COM_GetModuleManager().OnMissionFinish();

		CloseAllMenus();

		DestroyAllMenus();

		if( GetHive() )
		{
			DestroyHive();
		}

		super.OnMissionFinish();
	}

    void OnMissionLoaded()
    {
		COM_GetModuleManager().OnMissionLoaded();
    }

	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

        COM_GetModuleManager().OnUpdate( timeslice );

        if( !m_loaded && !GetDayZGame().IsLoading() )
        {
            m_loaded = true;
            OnMissionLoaded();
        }
	}

    void SpawnPlayer()
    {
//		#ifndef MODULE_PERSISTENCY
//		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif

//		#ifdef DISABLE_PERSISTENCY
		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif
    }

	void InitHive()
	{
		if ( GetGame().IsClient() && GetGame().IsMultiplayer() ) return;

		// RD /s /q "storage_-1" > nul 2>&1
		if (settings.hiveEnabled != 1) return;
	
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
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if(id == EditorMenu.MENU_ID)
        {
            return new EditorMenu();
        }
        
        return super.CreateScriptedMenu(id);
    }
}
