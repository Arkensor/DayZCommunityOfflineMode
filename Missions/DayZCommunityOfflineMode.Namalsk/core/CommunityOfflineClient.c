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
		super.OnInit();

        InitHive();

        SetupWeather();

		SpawnPlayer();

		GetDayZGame().SetMissionPath( "$saves:CommunityOfflineMode\\" ); // CameraToolsMenu
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

        COM_GetModuleManager().OnInit();
		COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
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
        /*
          [Namalsk] Weather init
           Warning: DO NOT ALTER following values as they are interconnected with other Namalsk-specific systems!
           To ensure correct functionality, it is necessary to include weaher init AFTER the hive init.
        */
        Weather weather = GetGame().GetWeather();
        weather.MissionWeather( true );
        weather.GetOvercast().SetLimits( 0.10, 0.30 );
        weather.GetRain().SetLimits( 0.0, 0.0 );
        weather.GetFog().SetLimits( 0.0, 0.0 );
        weather.GetOvercast().SetForecastChangeLimits( 0.1, 0.3 );
        weather.GetOvercast().SetForecastTimeLimits( 1600, 2100 );
        weather.GetOvercast().Set( Math.RandomFloatInclusive( 0.1, 0.2 ), 0, 0 );	// ignored if storage is present
        weather.GetRain().Set( 0, 0, 0 );											// ignored if storage is present
        weather.GetFog().Set( 0, 0, 0 );											// ignored if storage is present
        weather.SetWindMaximumSpeed( 30 );
        weather.SetWindFunctionParams( 0.1, 1.0, 42 );
        weather.SetStorm( 0, 1, 1 );
        weather.SetRainThresholds( 0.0, 1.0, 0 );
        
        /*
          [Namalsk] Mission time init
           after CE init to determine if storage mission type is outside of the required time-frame
           currently recommended time-frame is:
            11/1 -> 11/30
            keep in mind that gameplay features are tied to the mission date (stored in the storage) and that it SHOULD remain this period!
           while using:
            day accelerated 6 times (serverTimeAcceleration=6), resulting in an average 78 min of day-time (RL)
            night accelerated 24 times (serverNightTimeAcceleration=4), resulting in an average of 26 min of night-time (RL)
        */
        int year, month, day, hour, minute;
        GetGame().GetWorld().GetDate( year, month, day, hour, minute );

        if ( ( month < 11 ) || ( month >= 12 ) )
        {
            year = 2011;
            month = 11;
            day = 1;
            
            GetGame().GetWorld().SetDate( year, month, day, hour, minute );
        }
    }
};