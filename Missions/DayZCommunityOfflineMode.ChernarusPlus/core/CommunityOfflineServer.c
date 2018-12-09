class CommunityOfflineServer : MissionServer
{
    protected bool m_bLoaded;

	void CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::CommunityOfflineServer()" );
	    m_bLoaded = false;

		GetModuleManager();
	}

	void ~CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::~CommunityOfflineServer()" );
	}

	override void OnInit()
	{
		super.OnInit();

        SetupWeather();

        GetModuleManager().OnInit();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
        GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
        GetModuleManager().OnMissionFinish();

		super.OnMissionFinish();
	}

    void OnMissionLoaded()
    {
		GetModuleManager().OnMissionLoaded();
    }

	void SetCanPause( bool can )
	{
	}

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
    }

    override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		//GetModuleManager().OnKeyRelease( key );
	}

    static void SetupWeather()
    {
        //Offical DayZ SA weather code
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
