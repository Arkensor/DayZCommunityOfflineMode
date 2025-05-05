class CommunityOfflineServer : MissionServer
{
    protected bool m_loaded;

	void CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::CommunityOfflineServer()" );
	    m_loaded = false;

		COM_GetModuleManager();
	}

	void ~CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::~CommunityOfflineServer()" );
	}

	override void OnInit()
	{
		super.OnInit();

        SetupWeather();

        COM_GetModuleManager().OnInit();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
        COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
        COM_GetModuleManager().OnMissionFinish();

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

	override void OnMouseButtonRelease( int button )
	{
		super.OnMouseButtonRelease( button );

		//COM_GetModuleManager().OnMouseButtonRelease( button );
	}

	override void OnMouseButtonPress( int button )
	{
		super.OnMouseButtonPress( button );

		//COM_GetModuleManager().OnMouseButtonPress( button );
	}

	override void OnKeyPress( int key )
	{
		super.OnKeyPress(key);

		//COM_GetModuleManager().OnKeyPress( key );
    }

    override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		//COM_GetModuleManager().OnKeyRelease( key );
	}

    static void SetupWeather()
    {
        //Rostow specific
        Weather weather = g_Game.GetWeather();

		weather.MissionWeather(false);    // false = use weather controller from Weather.c

		weather.GetOvercast().Set( Math.RandomFloatInclusive(0.02, 0.1), 1, 0);
		weather.GetRain().Set( 0, 1, 0);
		weather.GetFog().Set( 0, 1, 0);

		//Rostow specific date
		int year, month, day, hour, minute;
		int reset_month = 8, reset_day = 10;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		if (month != reset_month)
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
    }
}
