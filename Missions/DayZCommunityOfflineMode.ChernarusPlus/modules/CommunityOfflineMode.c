#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\WelcomeMenu.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\PositionMenu.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\WeatherMenu.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\ObjectMenu.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\BarrelCrosshair.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\Module.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\ModuleManager.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\KeyMouseBinding.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\COMModule\\COMModule.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\ObjectEditorModule\\ObjectEditorModule.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\CameraModule\\CameraModule.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\SaveModule.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\patches\\PluginLifespan.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\patches\\DebugMonitor.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\StaticFunctions.c"

class CommunityOfflineMode : MissionGameplay
{
	protected bool DISABLE_RESPAWN_ONRESTART = true; // disable(true) / enable(false) - Player Respawn on every Mission Restart
	protected bool DISABLE_HIVE = false;	 		// disable(true) / enable(false) - Local Hive / Economy 

	ref PluginLifespanPatched cpl;

	//Modules
	protected ref ModuleManager m_ModuleManager;
	
	//Patches
	protected ref DebugMonitorPatched m_debugMonitorPatched;

	//Until we find a better way
	protected bool m_bLoaded = false;

	//For freecam and utils
	protected Camera m_oCamera;
	
	protected bool m_bDebugMonitor = false;
	protected bool m_bGodMode = false;
	protected bool m_bWelcome = false;
	protected int m_nAutoWalkMode = 0;

	// For Aiming like 0.62
	bool m_bOldAiming = false;

	//For keyhandler
	protected bool m_IsCtrlHolding = false;
	protected bool m_IsWinHolding = false;
	protected bool m_IsLeftAltHolding = false;
	protected bool m_IsRightAltHolding = false;
	protected bool m_IsLeftShiftHolding = false;
	protected bool m_IsRightShiftHolding = false;

	void CommunityOfflineMode()
	{
		Print( "CommunityOfflineMode::CommunityOfflineMode()" );

		m_ModuleManager = new ModuleManager();

		RegisterModules();
		
		// for beard
		g_Game.SetMissionState(DayZGame.MISSION_STATE_GAME);
		SetDispatcher(new DispatcherCaller);
		cpl = new PluginLifespanPatched();
	}

	void ~CommunityOfflineMode()
	{
		Print( "CommunityOfflineMode::~CommunityOfflineMode()" );
		
		if( GetHive() )
		{
			DestroyHive();
		}
	}

	void RegisterModules()
	{
		m_ModuleManager.RegisterModule( new COMModule(this) );
		m_ModuleManager.RegisterModule( new ObjectEditorModule(this) );
		m_ModuleManager.RegisterModule( new CameraModule(this) );
		m_ModuleManager.RegisterModule( new SaveModule(this) );
	}

	override void OnInit()
	{
		super.OnInit();

        SetupWeather();

		SpawnPlayer();
		
		InitHive();

		m_ModuleManager.InitializeModules();
		
		GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\layouts\\BarrelCrosshair.layout" );
		
		DayZPlayerCameras.RegisterTransitionTime( DayZPlayerCameras.DAYZCAMERA_1ST, DayZPlayerCameras.DAYZCAMERA_OPTICS, 0.15, true ); // Temp fix for snapping camera transitions
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
		GetGame().GetUIManager().CloseMenu( MENU_INGAME );

		m_ModuleManager.OnMissionStart();

		CreateDebugMonitor();
		
		m_debugMonitorPatched.Hide();
	}

	override void OnMissionFinish()
	{	
		GetGame().GetUIManager().CloseMenu( MENU_INGAME );

		CloseAllMenus();

		DestroyAllMenus();

		m_ModuleManager.OnMissionFinish();

		super.OnMissionFinish();
	}

    void OnMissionLoaded()
    {
		m_ModuleManager.OnMissionLoaded();

        GetGame().GetUIManager().ScreenFadeOut( 0 );
    }

	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

		m_ModuleManager.OnUpdate( timeslice );

        if( !m_bLoaded && !GetDayZGame().IsLoading())
        {
            m_bLoaded = true;
            OnMissionLoaded();
        }

		PlayerBase oPlayer = GetGame().GetPlayer();

		if( m_bGodMode && oPlayer )
		{
			
			oPlayer.SetHealth( oPlayer.GetMaxHealth( "", "" ) );
			oPlayer.SetHealth( "","Blood", oPlayer.GetMaxHealth( "", "Blood" ) );
			oPlayer.SetHealth( "","Shock", oPlayer.GetMaxHealth( "", "Shock" ) );
			// oPlayer.SetStamina(1000, 1000);
			oPlayer.GetStatStamina().Set(1000);
			oPlayer.GetStatEnergy().Set(1000);
			oPlayer.GetStatWater().Set(1000);
			oPlayer.GetStatStomachSolid().Set(300);		
			oPlayer.GetStatStomachWater().Set(300);
			oPlayer.GetStatStomachEnergy().Set(300);
			oPlayer.GetStatHeatComfort().Set(0);
			
			EntityAI oWeapon = oPlayer.GetHumanInventory().GetEntityInHands();

			if( oWeapon )
			{
				Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );

				if( oMag && oMag.IsMagazine() )
				{
					oMag.LocalSetAmmoMax();
				}
				
				Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );

				if( oSupressor )
				{
					oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
				}
			}
			
		}
		
		UpdateDebugMonitor();

		UpdateAutoWalk();

		// UpdateEditor(); --- Missing / Removed ??
        
		if(oPlayer) {
		    oPlayer.StatUpdateByTime("playtime");
		    cpl.UpdateLifespan( oPlayer, true );
		    cpl.UpdateBloodyHandsVisibility(oPlayer, oPlayer.HasBloodyHands());

			if (m_bOldAiming) {
				oPlayer.OverrideShootFromCamera(false);
			}
		}
	}

	override void OnMouseButtonRelease(int button)
	{
		super.OnMouseButtonRelease(button);

		m_ModuleManager.OnMouseButtonPress(button);
	}

	override void OnMouseButtonPress( int button )
	{
		super.OnMouseButtonPress( button );

		m_ModuleManager.OnMouseButtonPress(button);
	}
	
	override void OnKeyPress( int key )
	{
		super.OnKeyPress(key);

		m_ModuleManager.OnKeyPress(key);
		
		PlayerBase oPlayer = GetGame().GetPlayer();

        if( !m_bWelcome )
        {
            m_bWelcome = true;

            WelcomeMenu oWelcomeMenu = new WelcomeMenu;
            oWelcomeMenu.Init();
            GetGame().GetUIManager().ShowScriptedMenu( oWelcomeMenu, NULL );
        }

		// dannydog: port over old keybinds and functions to new module system

		switch( key )
		{
			case KeyCode.KC_LCONTROL:
			{
				m_IsCtrlHolding = true;
				break;
			}
			
			case KeyCode.KC_LWIN:
			{
				m_IsWinHolding = true;
				break;
			}
			
			case KeyCode.KC_LMENU:
			{
				m_IsLeftAltHolding = true;
				break;	
			}
			
			case KeyCode.KC_RMENU:
			{
				m_IsRightAltHolding = true;
				break;
			}
			
			case KeyCode.KC_LSHIFT:
			{
				m_IsLeftShiftHolding = true;
				break:
			}
			
			case KeyCode.KC_RSHIFT:
			{
				m_IsRightShiftHolding = true;
				break:
			}


			//Gestures [.]
			case KeyCode.KC_PERIOD:
			{
				//open gestures menu
				if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
				{
					GesturesMenu.OpenMenu();
				}
				
				break;
			}


			//Radial Quickbar [,]
			case KeyCode.KC_COMMA:
			{
				//open radial quickbar menu
				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
				{
					RadialQuickbarMenu.OpenMenu();
				}
				
				break;
			}

			case KeyCode.KC_P:
			{
				Print( oPlayer.GetPosition().ToString() );
				Print( oPlayer.GetDirection().ToString() );
				
				Print( "POS X:" + oPlayer.GetPosition()[0] + " Y:" + oPlayer.GetPosition()[2] + " Z:" + oPlayer.GetPosition()[1] );
				oPlayer.MessageStatus( "POS X:" + oPlayer.GetPosition()[0] + " Y:" + oPlayer.GetPosition()[2] + " Z:" + oPlayer.GetPosition()[1] );

				oPlayer.MessageStatus( "Player position and direction vector were written to the game logs too." );
				
				break;
			}


			case KeyCode.KC_B:
			{
                if( m_bDebugMonitor )
                {
                    m_debugMonitorPatched.Hide();
                    m_bDebugMonitor = false;
                }
                else
                {
                    m_debugMonitorPatched.Show();
                    m_bDebugMonitor = true;
                }

				break;
			}


            case KeyCode.KC_X:
            {
                if( m_nAutoWalkMode && !SHIFT() )
                {
                    m_nAutoWalkMode = 0;
                    oPlayer.GetInputController().OverrideMovementSpeed( false, 0 );
                    oPlayer.GetInputController().OverrideMovementAngle( false, 0 );
                }
                else
                {
                    if( SHIFT() )
                    {
                        m_nAutoWalkMode = 2;
                    }
                    else
                    {
                        m_nAutoWalkMode = 1;
                    }
                }

                break;
            }


			case KeyCode.KC_K:
			{
				if (!CTRL() && !SHIFT())
				GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( GetGame().RestartMission );
				break;
			}			

			case KeyCode.KC_END:
			{
				if ( m_bGodMode )
				{
					oPlayer.MessageStatus( "God mode disabled." );
					oPlayer.SetAllowDamage( true );
					m_bGodMode = false;
				}
				else
				{
					oPlayer.MessageStatus( "God mode enabled." );
					oPlayer.SetAllowDamage( false );
					m_bGodMode = true;
				}

				break;
			}		

			case KeyCode.KC_L:
			{
				if ( m_bOldAiming )
				{
					oPlayer.MessageStatus( "Old Aiming System disabled." );
					m_bOldAiming = false;
				}
				else
				{
					oPlayer.MessageStatus( "Old Aiming System enabled." );
					m_bOldAiming = true;
				}

				break;
			}	


			case KeyCode.KC_DELETE:
			{

                UIScriptedMenu oMenu = NULL;

				if( CTRL() )
				{
				    oMenu = new PositionMenu;
				}
				else if( SHIFT() )
				{
                    oMenu = new WeatherMenu;
				}
				else
				{
				    oMenu = new ObjectMenu;
				}

				oMenu.Init();
                GetGame().GetUIManager().ShowScriptedMenu( oMenu, NULL );

				break;
			}
		}	
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);

		m_ModuleManager.OnKeyRelease(key);
				
		switch( key )
		{
			case KeyCode.KC_LCONTROL:
			{
				m_IsCtrlHolding = false;
				break;
			}

			case KeyCode.KC_LWIN:
			{
				m_IsWinHolding = false;
				break;
			}

			case KeyCode.KC_LMENU:
			{
				m_IsLeftAltHolding = false;
				break;
			}

			case KeyCode.KC_RMENU:
			{
				m_IsRightAltHolding = false;
				break;
			}
			
			case KeyCode.KC_LSHIFT:
			{
				m_IsLeftShiftHolding = false;
				break:
			}
			
			case KeyCode.KC_RSHIFT:
			{
				m_IsRightShiftHolding = false;
				break:
			}
		}

		//Gestures [.]
		if ( key == KeyCode.KC_PERIOD )
		{
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.CloseMenu();
			}
		}

		//Radial Quickbar [,]
		if ( key == KeyCode.KC_COMMA )
		{
			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
			}
		}
	}

    override void CreateDebugMonitor()
    {
        if (!m_debugMonitorPatched)
        {
            m_debugMonitorPatched = new DebugMonitorPatched();
            m_debugMonitorPatched.Init();
        }
    }

    override void UpdateDebugMonitor()
    {
        if (!m_debugMonitorPatched) return;

		PlayerBase oPlayer = GetGame().GetPlayer();

        if (oPlayer)
        {
            m_debugMonitorPatched.SetHealth( oPlayer.GetHealth( "","" ) );
            m_debugMonitorPatched.SetBlood(  oPlayer.GetHealth( "","Blood" ) );
            m_debugMonitorPatched.SetLastDamage( "" );
            m_debugMonitorPatched.SetPosition( oPlayer.GetPosition() );
        }
    }

    void UpdateAutoWalk()
    {
        if( m_nAutoWalkMode )
        {
			PlayerBase oPlayer = GetGame().GetPlayer();
            if( ( oPlayer.GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 1 ) )
            {
                oPlayer.GetInputController().OverrideMovementSpeed( true, 2 );
            }
            else
            {
                oPlayer.GetInputController().OverrideMovementSpeed( true, 3 );
            }

            oPlayer.GetInputController().OverrideMovementAngle( true, 1 );
        }
    }

    void SetupWeather()
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

	bool CTRL() return m_IsCtrlHolding;
	bool WIN() return m_IsWinHolding;
	bool SHIFT() return ( m_IsLeftShiftHolding || m_IsRightShiftHolding );
	bool ALT() return ( m_IsLeftAltHolding || m_IsRightAltHolding );

	void SpawnPlayer()
    {
		PlayerBase oPlayer = CreateCustomDefaultCharacter();

		GetGame().SelectPlayer( NULL, oPlayer );
    }

	void InitHive()
	{
		if (DISABLE_HIVE) return;
	
		Hive oHive = GetHive();
		
		if( !oHive )
		{
			oHive = CreateHive();
		}
		if( oHive )
		{
			oHive.InitOffline();
		}
	}
}
