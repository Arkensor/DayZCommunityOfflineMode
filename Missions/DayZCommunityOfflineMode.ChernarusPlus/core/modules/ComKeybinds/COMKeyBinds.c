/*
	Manages all keybinds for COM
*/
class COMKeyBinds extends Module 
{
	bool m_IsHudVisible = true;
	protected int m_nAutoWalkMode = 0;

	void COMKeyBinds()
	{
	}

	void ~COMKeyBinds()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
	    UpdateAutoWalk();

	    UpdateGodMode();
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCursor    = new KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "[U]"    , "Toggles the cursor."   , true   );
		KeyMouseBinding toggleCOMEditor = new KeyMouseBinding( GetModuleType() , "ShowCOMEditor" , "[Y]"    , "Opens the COM Editor."          );
		KeyMouseBinding teleport	    = new KeyMouseBinding( GetModuleType() , "TeleportCursor", "[END]"    , "Teleport to cursor position."   );
		KeyMouseBinding reload          = new KeyMouseBinding( GetModuleType() , "Reload"        , "[R]"    , "Instantly reloads mag."		   );
        KeyMouseBinding spawnZ          = new KeyMouseBinding( GetModuleType() , "SpawnZ"        , "[O]"    , "Spawns infected."               );
        KeyMouseBinding hideHud         = new KeyMouseBinding( GetModuleType() , "HideHud"       , "[HOME]" , "Hides ui completely."           );
        KeyMouseBinding printPlayer     = new KeyMouseBinding( GetModuleType() , "PrintPlayer"   , "[P]"    , "Print current player position." );
        KeyMouseBinding autoRun         = new KeyMouseBinding( GetModuleType() , "AutoRun"       , "[X]"    , "Toggle autorun." );
        KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  , "[=]"    , "Toggle dayz dev cinematic tool." );

		toggleCursor   .AddKeyBind( KeyCode.KC_U,    KeyMouseBinding.KB_EVENT_PRESS   );
		toggleCOMEditor.AddKeyBind( KeyCode.KC_Y,    KeyMouseBinding.KB_EVENT_PRESS );
		teleport       .AddKeyBind( KeyCode.KC_END,  KeyMouseBinding.KB_EVENT_PRESS   );
		reload         .AddKeyBind( KeyCode.KC_R,    KeyMouseBinding.KB_EVENT_PRESS );
        spawnZ         .AddKeyBind( KeyCode.KC_O,    KeyMouseBinding.KB_EVENT_PRESS );
        hideHud        .AddKeyBind( KeyCode.KC_HOME, KeyMouseBinding.KB_EVENT_PRESS );
        printPlayer    .AddKeyBind( KeyCode.KC_P,    KeyMouseBinding.KB_EVENT_PRESS );
        autoRun        .AddKeyBind( KeyCode.KC_X,    KeyMouseBinding.KB_EVENT_PRESS );
        keyFrame       .AddKeyBind( KeyCode.KC_EQUALS, KeyMouseBinding.KB_EVENT_PRESS );

		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleCOMEditor );
		RegisterKeyMouseBinding( teleport );
		RegisterKeyMouseBinding( reload );
        RegisterKeyMouseBinding( spawnZ );
        RegisterKeyMouseBinding( hideHud );
        RegisterKeyMouseBinding( printPlayer );
        RegisterKeyMouseBinding( autoRun );
        RegisterKeyMouseBinding( keyFrame );
	}

    void OpenKeyframe() 
    {
        GetGame().GetUIManager().ShowScriptedMenu( GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL );
    }

    void ToggleCursor()
    {
        if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
        {
            GetGame().GetInput().ChangeGameFocus( 1 );
            GetGame().GetUIManager().ShowUICursor( true );
            FreeDebugCamera.GetInstance().SetFreezed(true);
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor( false );
            GetGame().GetInput().ResetGameFocus();
            FreeDebugCamera.GetInstance().SetFreezed(false);
        }
         Message(GetDayZGame().GetMissionFolderPath());
    }

    void ShowCOMEditor()
    {
        GetGame().GetUIManager().ShowScriptedMenu( new EditorMenu() , NULL );
    }

    void TeleportCursor()
    {
        Print( "COMKeyBinds::TeleportCursor()" );

//		if ( CameraTool.Cast( m_Mission.GetModule( CameraTool ) ).IsUsingCamera() ) //Todo renable after module manager is done
//		{
//			GetPlayer().MessageStatus( "You can not teleport while you are inside the freecam!" );
//			return;
//		}
		
        vector hitPos = GetCursorPos();

        float distance = vector.Distance( GetPlayer().GetPosition(), hitPos );

        if ( distance < 5000 )
        {
            EntityAI oVehicle = GetPlayer().GetDrivingVehicle();

            if( oVehicle )
            {
                GetPlayer().MessageStatus( "Get out of the vehicle first!" );
            }
            else
            {
                GetPlayer().SetPosition( hitPos );
                GetPlayer().MessageStatus( "Teleported!" );
            }
        }
        else
        {
            GetPlayer().MessageStatus( "Distance for teleportation is too far!" );
        }
    }

    void Reload()
    {
        EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();

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

    void SpawnZ() 
    {
        if( CTRL() )
        {
            GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos(), false, true );
        }
        else if( SHIFT() )
        {
            GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), GetCursorPos(), false, true );
        }
        else
        {
            GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), GetCursorPos(), false, true );
        }
    }

    void HideHud() 
    {
        Widget hudWidget = IngameHud.Cast(GetClientMission().GetHud()).GetHudPanelWidget();
        hudWidget.Show(!hudWidget.IsVisible());
    }

    void PrintPlayer()
    {
        Print( "Position:" + GetPlayer().GetPosition().ToString() );
        Print( "Orientation" + GetPlayer().GetOrientation().ToString() );
        Message( "POS X:" + GetPlayer().GetPosition()[0] + " Y:" + GetPlayer().GetPosition()[2] + " Z:" + GetPlayer().GetPosition()[1] );
        Message( "Player position and orientation vector were written to the game logs too." );
    }

    void AutoRun()
    {
        if( m_nAutoWalkMode && !SHIFT() )
        {
            m_nAutoWalkMode = 0;
            GetPlayer().GetInputController().OverrideMovementSpeed( false, 0 );
            GetPlayer().GetInputController().OverrideMovementAngle( false, 0 );
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
    }

    void UpdateAutoWalk()
    {
        if( m_nAutoWalkMode )
        {
            if( ( GetPlayer().GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 1 ) )
            {
                GetPlayer().GetInputController().OverrideMovementSpeed( true, 2 );
            }
            else
            {
                GetPlayer().GetInputController().OverrideMovementSpeed( true, 3 );
            }

            GetPlayer().GetInputController().OverrideMovementAngle( true, 1 );
        }
    }

    void UpdateGodMode()
    {
        // just putting this here for now
        if ( m_GodMode ) // located in staticfunctions
        {
            GetPlayer().SetAllowDamage( false );

            GetPlayer().SetHealth( GetPlayer().GetMaxHealth( "", "" ) );
            GetPlayer().SetHealth( "","Blood", GetPlayer().GetMaxHealth( "", "Blood" ) );
            GetPlayer().SetHealth( "","Shock", GetPlayer().GetMaxHealth( "", "Shock" ) );

            GetPlayer().GetStaminaHandler().SyncStamina(1000, 1000);
            GetPlayer().GetStatStamina().Set(GetPlayer().GetStaminaHandler().GetStaminaCap());
            GetPlayer().GetStatEnergy().Set(1000);
            GetPlayer().GetStatWater().Set(1000);
//            GetPlayer().GetStatStomachVolume().Set(300);
//            GetPlayer().GetStatStomachWater().Set(300);
//            GetPlayer().GetStatStomachEnergy().Set(300);
            GetPlayer().GetStatHeatComfort().Set(0);

            EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();

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
    }
}