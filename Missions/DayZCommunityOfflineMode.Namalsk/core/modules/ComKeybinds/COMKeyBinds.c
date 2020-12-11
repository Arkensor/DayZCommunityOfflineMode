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
		KeyMouseBinding toggleCursor    = new KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "Toggles the cursor."   , true     );
		KeyMouseBinding toggleCOMEditor = new KeyMouseBinding( GetModuleType() , "ShowCOMEditor" ,  "Opens the COM Editor."           );
		KeyMouseBinding teleport	    = new KeyMouseBinding( GetModuleType() , "TeleportCursor",  "Teleport to cursor position."    );
		KeyMouseBinding reload          = new KeyMouseBinding( GetModuleType() , "Reload"        ,  "Instantly reloads mag."		  );
        KeyMouseBinding spawnZ          = new KeyMouseBinding( GetModuleType() , "SpawnZ"        ,  "Spawns infected."                );
        KeyMouseBinding hideHud         = new KeyMouseBinding( GetModuleType() , "HideHud"       ,  "Hides ui completely."            );
        KeyMouseBinding printPlayer     = new KeyMouseBinding( GetModuleType() , "PrintPlayer"   ,  "Print current player position."  );
        KeyMouseBinding autoRun         = new KeyMouseBinding( GetModuleType() , "AutoRun"       ,  "Toggle autorun."                 );
//        KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );
        KeyMouseBinding closeMenu       = new KeyMouseBinding( GetModuleType() , "CloseOpenMenu" ,  "Close the menu on esc.", true    );

		toggleCursor   .AddBinding( "kU" );
		toggleCOMEditor.AddBinding( "kY" );
		teleport       .AddBinding( "kEnd" );
		reload         .AddBinding( "kR" );
        spawnZ         .AddBinding( "kO" );
        hideHud        .AddBinding( "kHome" );
        printPlayer    .AddBinding( "kP" );
        autoRun        .AddBinding( "kX" );
//        keyFrame       .AddBinding( "kPrior" );
        closeMenu      .AddBinding( "kEscape" );

		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleCOMEditor );
		RegisterKeyMouseBinding( teleport );
		RegisterKeyMouseBinding( reload );
        RegisterKeyMouseBinding( spawnZ );
        RegisterKeyMouseBinding( hideHud );
        RegisterKeyMouseBinding( printPlayer );
        RegisterKeyMouseBinding( autoRun );
//        RegisterKeyMouseBinding( keyFrame );
        RegisterKeyMouseBinding( closeMenu );
	}

//    void OpenKeyframe()
//    {
//        GetGame().GetUIManager().ShowScriptedMenu( COM_GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL );
//    }

    void COM_ToggleCursor()
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
         COM_Message(GetDayZGame().GetMissionFolderPath());
    }

    void CloseOpenMenu()
    {
        if( GetGame().GetUIManager().GetMenu() && ( GetGame().GetUIManager().GetMenu().GetID() == 133742 ) )
        {
            GetGame().GetUIManager().Back();
        }
    }

    void ShowCOMEditor()
    {
        GetGame().GetUIManager().ShowScriptedMenu( new EditorMenu(), NULL );
    }

    void TeleportCursor()
    {
        Print( "COMKeyBinds::TeleportCursor()" );

//		if ( CameraTool.Cast( m_Mission.GetModule( CameraTool ) ).IsUsingCamera() ) //Todo renable after module manager is done
//		{
//			COM_GetPB().MessageStatus( "You can not teleport while you are inside the freecam!" );
//			return;
//		}
		
        vector hitPos = COM_GetCursorPos();
        
        Print("hitpos: " + hitPos);
        
        float distance = vector.Distance( COM_GetPB().GetPosition(), hitPos );

        if ( distance < 5000 )
        {
            EntityAI oVehicle = COM_GetPB().GetDrivingVehicle();

            if( oVehicle )
            {
                COM_GetPB().MessageStatus( "Get out of the vehicle first!" );
            }
            else
            {
                COM_GetPB().SetPosition( hitPos );
                COM_GetPB().MessageStatus( "Teleported!" );
            }
        }
        else
        {
            COM_GetPB().MessageStatus( "Distance for teleportation is too far!" );
        }
    }

    void Reload()
    {
        auto wpn = COM_GetPB().GetHumanInventory().GetEntityInHands();

        if( wpn )
        {
            auto magAtt = wpn.GetAttachmentByConfigTypeName( "DefaultMagazine" );

            if( magAtt )
            {
                auto mag = Magazine.Cast( magAtt );

                if( mag && mag.IsMagazine() )
                {
                    mag.LocalSetAmmoMax();
                }
            }

            auto suppAtt = wpn.GetAttachmentByConfigTypeName( "ItemSuppressor" );

            if( suppAtt )
            {
                auto supp = Object.Cast( suppAtt );

                if( supp )
                {
                    supp.SetHealth( supp.GetMaxHealth( "", "" ) );
                }
            }
        }
    }

    void SpawnZ() 
    {
        if( COM_CTRL() )
        {
            GetGame().CreateObject( "Animal_CanisLupus_Grey", COM_GetCursorPos(), false, true );
        }
        else if( COM_SHIFT() )
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), COM_GetCursorPos(), false, true );
        }
        else
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "ZombieBase", 2 ), COM_GetCursorPos(), false, true );
        }
    }

    void HideHud() 
    {
        Widget hudWidget = IngameHud.Cast(COM_GetClientMission().GetHud()).GetHudPanelWidget();
        hudWidget.Show(!hudWidget.IsVisible());
    }

    void PrintPlayer()
    {
        Print( "Position:" + COM_GetPB().GetPosition().ToString() );
        Print( "Orientation" + COM_GetPB().GetOrientation().ToString() );
        COM_Message( "POS X:" + COM_GetPB().GetPosition()[0] + " Y:" + COM_GetPB().GetPosition()[2] + " Z:" + COM_GetPB().GetPosition()[1] );
        COM_Message( "Player position and orientation vector were written to the game logs too." );
    }

    void AutoRun()
    {
        if( m_nAutoWalkMode && !COM_SHIFT() && !COM_CTRL() )
        {
            m_nAutoWalkMode = 0;
            COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
            COM_GetPB().GetInputController().OverrideMovementAngle( false, 0 );
        }
        else
        {
            if( COM_SHIFT() )
            {
                m_nAutoWalkMode = 3;
            }
            else if( COM_CTRL() )
            {
                m_nAutoWalkMode = 1; //CTRL == slow mode
            }
            else
            {
                m_nAutoWalkMode = 2;
            }
        }
    }

    void UpdateAutoWalk()
    {
        if( m_nAutoWalkMode > 0 )
        {
            if( m_nAutoWalkMode == 1 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 1 );
            }
            else if( ( COM_GetPB().GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 2 ) )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 2 );
            }
            else if( m_nAutoWalkMode == 3 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 3 );
            }

            COM_GetPB().GetInputController().OverrideMovementAngle( true, 1 );
        }
    }

    void UpdateGodMode()
    {
        // just putting this here for now
        if ( m_COM_GodMode ) // located in staticfunctions
        {
            COM_GetPB().SetHealth( COM_GetPB().GetMaxHealth( "", "" ) );
            COM_GetPB().SetHealth( "","Blood", COM_GetPB().GetMaxHealth( "", "Blood" ) );
            COM_GetPB().SetHealth( "","Shock", COM_GetPB().GetMaxHealth( "", "Shock" ) );

            //GetPlayer().GetStaminaHandler().SyncStamina(1000, 1000);
            COM_GetPB().GetStatStamina().Set(COM_GetPB().GetStaminaHandler().GetStaminaCap());
            COM_GetPB().GetStatEnergy().Set(1000);
            COM_GetPB().GetStatWater().Set(1000);
//            COM_GetPB().GetStatStomachVolume().Set(300);
//            COM_GetPB().GetStatStomachWater().Set(300);
//            COM_GetPB().GetStatStomachEnergy().Set(300);
            COM_GetPB().GetStatHeatComfort().Set(0);

            EntityAI oWeapon = COM_GetPB().GetHumanInventory().GetEntityInHands();

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