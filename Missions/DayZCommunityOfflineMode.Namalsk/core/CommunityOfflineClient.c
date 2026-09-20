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
#ifdef DISABLE_PERSISTENCY
        GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
#endif
    }

    void InitHive()
    {
        if ( GetGame().IsClient() && GetGame().IsMultiplayer() ) return;
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
        /* Keep Namalsk mission date persisted by the storage. */
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
