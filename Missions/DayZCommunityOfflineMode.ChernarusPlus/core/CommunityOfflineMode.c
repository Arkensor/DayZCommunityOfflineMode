#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\ModuleManager.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\StaticFunctions.c"

class CommunityOfflineMode : MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_bLoaded;

	void CommunityOfflineMode()
	{
	    Print( "CommunityOfflineMode::CommunityOfflineMode()" );
	    m_bLoaded = false;

		GetModuleManager();
	}

	void ~CommunityOfflineMode()
	{
	    Print( "CommunityOfflineMode::~CommunityOfflineMode()" );
	}

	override void OnInit()
	{
		super.OnInit();

        InitHive();

        SetupWeather();

		SpawnPlayer();

        GetModuleManager().OnInit();

//		GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\BarrelCrosshair\\gui\\layouts\\BarrelCrosshair.layout" );
		
//		DayZPlayerCameras.RegisterTransitionTime( DayZPlayerCameras.DAYZCAMERA_1ST, DayZPlayerCameras.DAYZCAMERA_OPTICS, 0.15, true ); // Temp fix for snapping camera transitions
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
//        GetGame().GetUIManager().ScreenFadeOut( 0 );
		
		GetModuleManager().OnMissionLoaded();
    }
	
	bool CanPause()
	{
		return GetModuleManager().CanPause();
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

//			GetPlayer().SetHealth( GetPlayer().GetMaxHealth( "", "" ) );
//			GetPlayer().SetHealth( "","Blood", GetPlayer().GetMaxHealth( "", "Blood" ) );
//			GetPlayer().SetHealth( "","Shock", GetPlayer().GetMaxHealth( "", "Shock" ) );
//			// GetPlayer().SetStamina(1000, 1000);
//			GetPlayer().GetStatStamina().Set(1000);
//			GetPlayer().GetStatEnergy().Set(1000);
//			GetPlayer().GetStatWater().Set(1000);
//			GetPlayer().GetStatStomachSolid().Set(300);
//			GetPlayer().GetStatStomachWater().Set(300);
//			GetPlayer().GetStatStomachEnergy().Set(300);
//			GetPlayer().GetStatHeatComfort().Set(0);
//
//			EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();
//
//			if( oWeapon )
//			{
//				Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
//
//				if( oMag && oMag.IsMagazine() )
//				{
//					oMag.LocalSetAmmoMax();
//				}
//
//				Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
//
//				if( oSupressor )
//				{
//					oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
//				}
//			}
//
//		UpdateAutoWalk();

//		if(m_oPlayer) {
//		    m_oPlayer.StatUpdateByTime("playtime");
//
//			if (m_bOldAiming) {
//				m_oPlayer.OverrideShootFromCamera(false);
//			}
//		}
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
//
//
//
//        if( !m_bWelcome )
//        {
//            m_bWelcome = true;
//
//            WelcomeMenu oWelcomeMenu = new WelcomeMenu;
//            oWelcomeMenu.Init();
//            GetGame().GetUIManager().ShowScriptedMenu( oWelcomeMenu, NULL );
//        }
//
//		// dannydog: port over old keybinds and functions to new module system
//
		switch( key )
		{
//			case KeyCode.KC_LCONTROL:
//			{
//				m_IsCtrlHolding = true;
//				break;
//			}
//
//			case KeyCode.KC_LWIN:
//			{
//				m_IsWinHolding = true;
//				break;
//			}
//
//			case KeyCode.KC_LMENU:
//			{
//				m_IsLeftAltHolding = true;
//				break;
//			}
//
//			case KeyCode.KC_RMENU:
//			{
//				m_IsRightAltHolding = true;
//				break;
//			}
//
//			case KeyCode.KC_LSHIFT:
//			{
//				m_IsLeftShiftHolding = true;
//				break:
//			}
//
//			case KeyCode.KC_RSHIFT:
//			{
//				m_IsRightShiftHolding = true;
//				break:
//			}
//
//
//			//Gestures [.]
//			case KeyCode.KC_PERIOD:
//			{
//				//open gestures menu
//				if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
//				{
//					GesturesMenu.OpenMenu();
//				}
//
//				break;
//			}
//
//
//			//Radial Quickbar [,]
//			case KeyCode.KC_COMMA:
//			{
//				//open radial quickbar menu
//				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
//				{
//					RadialQuickbarMenu.OpenMenu();
//				}
//
//				break;
//			}
//
//			case KeyCode.KC_O:
//			{
//				if( CTRL() )
//				{
//					GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos(), false, true );
//				}
//				else if( SHIFT() )
//				{
//					GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), GetCursorPos(), false, true );
//				}
//				else
//				{
//					GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), GetCursorPos(), false, true );
//				}
//
//				break;
//			}
//
//			case KeyCode.KC_P:
//			{
//				Print( m_oPlayer.GetPosition().ToString() );
//				Print( m_oPlayer.GetDirection().ToString() );
//
//				Print( "POS X:" + m_oPlayer.GetPosition()[0] + " Y:" + m_oPlayer.GetPosition()[2] + " Z:" + m_oPlayer.GetPosition()[1] );
//				m_oPlayer.MessageStatus( "POS X:" + m_oPlayer.GetPosition()[0] + " Y:" + m_oPlayer.GetPosition()[2] + " Z:" + m_oPlayer.GetPosition()[1] );
//
//				m_oPlayer.MessageStatus( "Player position and direction vector were written to the game logs too." );
//
//				break;
//			}
//
//            case KeyCode.KC_X:
//            {
//                if( m_nAutoWalkMode && !SHIFT() )
//                {
//                    m_nAutoWalkMode = 0;
//                    m_oPlayer.GetInputController().OverrideMovementSpeed( false, 0 );
//                    m_oPlayer.GetInputController().OverrideMovementAngle( false, 0 );
//                }
//                else
//                {
//                    if( SHIFT() )
//                    {
//                        m_nAutoWalkMode = 2;
//                    }
//                    else
//                    {
//                        m_nAutoWalkMode = 1;
//                    }
//                }
//
//                break;
//            }
//
//
			case KeyCode.KC_K:
			{
				if (!CTRL() && !SHIFT())
				// GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( GetGame().RestartMission );
				break;
			}
//
//
//			case KeyCode.KC_N:
//			{
//				TStringArray attArr = {
//				"CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel",
//				"CarBattery","CarRadiator","EngineBelt","SparkPlug","CivSedanHood",
//				"CivSedanTrunk","CivSedanDoors_Driver","CivSedanDoors_CoDriver",
//				"CivSedanDoors_BackLeft","CivSedanDoors_BackRight",
//				};
//
//				EntityAI oCar = EntityAI.Cast( GetGame().CreateObject( "CivilianSedan", GetCursorPos(), false, true ) );
//
//				for (int j = 0; j < attArr.Count(); j++) { oCar.GetInventory().CreateAttachment( attArr.Get(j) ); }
//
//				oCar.SetAllowDamage( false );
//
//				break;
//			}
//
//			case KeyCode.KC_END:
//			{
//				if ( m_bGodMode )
//				{
//					m_oPlayer.MessageStatus( "God mode disabled." );
//					m_oPlayer.SetAllowDamage( true );
//					m_bGodMode = false;
//				}
//				else
//				{
//					m_oPlayer.MessageStatus( "God mode enabled." );
//					m_oPlayer.SetAllowDamage( false );
//					m_bGodMode = true;
//				}
//
//				break;
//			}
//
//			case KeyCode.KC_L:
//			{
//				if ( m_bOldAiming )
//				{
//					m_oPlayer.MessageStatus( "Old Aiming System disabled." );
//					m_bOldAiming = false;
//					m_oPlayer.OverrideShootFromCamera(true);
//				}
//				else
//				{
//					m_oPlayer.MessageStatus( "Old Aiming System enabled." );
//					m_bOldAiming = true;
//				}
//
//				break;
//			}
//
//
//			case KeyCode.KC_DELETE:
//			{
//
//                UIScriptedMenu oMenu = NULL;
//
//				if( CTRL() )
//				{
//				    oMenu = new PositionMenu;
//				}
//				else if( SHIFT() )
//				{
//                    oMenu = new WeatherMenu;
//				}
//				else
//				{
//				    oMenu = new ObjectMenu;
//				}
//
//				oMenu.Init();
//                GetGame().GetUIManager().ShowScriptedMenu( oMenu, NULL );
//
//				break;
//			}
//
//
//			case KeyCode.KC_HOME:
//			{
//				DamageSystem.ExplosionDamage( NULL, NULL, "Dummy_Heavy", GetCursorPos() );
//				Particle.Play( ParticleList.EXPLOSION_TEST, GetCursorPos() );
//				break;
//			}
		}
	}

	override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		GetModuleManager().OnKeyRelease( key );

//		switch( key )
//		{
//			case KeyCode.KC_LCONTROL:
//			{
//				m_IsCtrlHolding = false;
//				break;
//			}
//
//			case KeyCode.KC_LWIN:
//			{
//				m_IsWinHolding = false;
//				break;
//			}
//
//			case KeyCode.KC_LMENU:
//			{
//				m_IsLeftAltHolding = false;
//				break;
//			}
//
//			case KeyCode.KC_RMENU:
//			{
//				m_IsRightAltHolding = false;
//				break;
//			}
//
//			case KeyCode.KC_LSHIFT:
//			{
//				m_IsLeftShiftHolding = false;
//				break:
//			}
//
//			case KeyCode.KC_RSHIFT:
//			{
//				m_IsRightShiftHolding = false;
//				break:
//			}
//		}
//
//		//Gestures [.]
//		if ( key == KeyCode.KC_PERIOD )
//		{
//			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
//			{
//				GesturesMenu.CloseMenu();
//			}
//		}
//
//		//Radial Quickbar [,]
//		if ( key == KeyCode.KC_COMMA )
//		{
//			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
//			{
//				RadialQuickbarMenu.CloseMenu();
//			}
//		}
	}

//    void UpdateAutoWalk()
//    {
//        if( m_nAutoWalkMode )
//        {
//            //m_oPlayer.GetStaminaHandler().GetStaminaNormalized()
//            if( ( m_oPlayer.GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 1 ) )
//            {
//                m_oPlayer.GetInputController().OverrideMovementSpeed( true, 2 );
//            }
//            else
//            {
//                m_oPlayer.GetInputController().OverrideMovementSpeed( true, 3 );
//            }
//
//            m_oPlayer.GetInputController().OverrideMovementAngle( true, 1 );
//        }
//    }

    void SpawnPlayer()
    {
		#ifndef MODULE_PERSISTENCY
		auto oPlayer = CreateCustomDefaultCharacter();
		
		GetGame().SelectPlayer( NULL, oPlayer );
		#endif
    }

	void InitHive()
	{
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
