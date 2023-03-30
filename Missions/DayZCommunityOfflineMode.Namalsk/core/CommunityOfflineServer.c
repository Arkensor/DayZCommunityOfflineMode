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
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if(id == EditorMenu.MENU_ID)
        {
            return new EditorMenu();
        }
        
        return super.CreateScriptedMenu(id);
    }
}
