/*
	Manages all keybinds for COM
*/
class COMKeyBinds extends Module 
{
	bool m_IsHudVisible = true;
	protected int m_nAutoWalkMode = 0;
    string cursorPositions;

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
		scriptLog("Loading keyboard and mouse bindings for COMKeyBinds.c");
        KeyMouseBinding toggleCOMEditor = new KeyMouseBinding(GetModuleType(), "ShowCOMEditor", "Opens the COM Editor.");
        toggleCOMEditor.AddBinding(settings.keyCOMOpenMenu); RegisterKeyMouseBinding(toggleCOMEditor);
        KeyMouseBinding closeMenu       = new KeyMouseBinding(GetModuleType(), "CloseOpenMenu", "Close the menu on esc.", true);
        closeMenu.AddBinding(settings.keyCOMCloseMenu); RegisterKeyMouseBinding(closeMenu);
        KeyMouseBinding toggleCursor    = new KeyMouseBinding(GetModuleType(), "ToggleCursor", "Toggles the cursor.", true);
        toggleCursor.AddBinding(settings.keyCOMToggleCursor); RegisterKeyMouseBinding(toggleCursor);
        KeyMouseBinding teleport        = new KeyMouseBinding(GetModuleType(), "TeleportCursor", "Teleport to cursor position.");
        teleport.AddBinding(settings.keyCOMTeleportToCursor); RegisterKeyMouseBinding(teleport);
        KeyMouseBinding reload          = new KeyMouseBinding(GetModuleType(), "Reload", "Instantly reloads mag.");
        reload.AddBinding(settings.keyCOMInstantReload);RegisterKeyMouseBinding(reload);
        KeyMouseBinding spawnZ          = new KeyMouseBinding(GetModuleType(), "SpawnZ", "Spawns infected.");
        spawnZ.AddBinding(settings.keyCOMSpawnInfected); RegisterKeyMouseBinding(spawnZ);
        KeyMouseBinding spawnGift       = new KeyMouseBinding(GetModuleType(), "SpawnGift", "Spawns large gift box.");
        spawnGift.AddBinding(settings.keyCOMSpawnGiftBox); RegisterKeyMouseBinding(spawnGift);
        KeyMouseBinding hideHud         = new KeyMouseBinding(GetModuleType(), "HideHud", "Hides ui completely.");
        hideHud.AddBinding(settings.keyCOMHideHUD); RegisterKeyMouseBinding(hideHud);
        KeyMouseBinding printPlayer     = new KeyMouseBinding(GetModuleType(), "PrintPlayer", "Print current player position.");
        printPlayer.AddBinding(settings.keyCOMCopyPlayerPosition); RegisterKeyMouseBinding(printPlayer);
        KeyMouseBinding printCursor     = new KeyMouseBinding(GetModuleType(), "PrintCursor", "Print current cursor position.");
        printCursor.AddBinding(settings.keyCOMCopyCursorPositions); RegisterKeyMouseBinding(printCursor);
        KeyMouseBinding clearCursor     = new KeyMouseBinding(GetModuleType(), "ClearCursor", "Clear current cursor positions.");
        clearCursor.AddBinding(settings.keyCOMClearCursorPositions); RegisterKeyMouseBinding(clearCursor);
        KeyMouseBinding autoRun         = new KeyMouseBinding(GetModuleType(), "AutoRun", "Toggle autorun.");
        autoRun.AddBinding(settings.keyCOMAutoRun); RegisterKeyMouseBinding(autoRun);
        //KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );
        //keyFrame.AddBinding( "kPrior" ); RegisterKeyMouseBinding( keyFrame );
	}
    //void OpenKeyframe() { GetGame().GetUIManager().ShowScriptedMenu( COM_GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL ); }

    void CloseOpenMenu()
    {
        ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).saveGroupInputs();
        auto menu = GetGame().GetUIManager().GetMenu(); if(!menu) { return; }
        if(menu.GetID() == 133742) {
            GetGame().GetUIManager().Back();
	        deleteAllClicks = 0;
	        ToggleCursor(0);
        }
    }

    void ShowCOMEditor()
    {
        GetGame().GetUIManager().EnterScriptedMenu(EditorMenu.MENU_ID, NULL);
        ToggleCursor(1); isCOMOpen = true;
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
                auto supp = suppAtt;

                if( supp )
                {
                    supp.SetHealth( supp.GetMaxHealth( "", "" ) );
                }
            }
        }
    }

    void SpawnZ() 
    {
        vector cursorPos = COM_GetCursorPos();
        if(COM_CTRL()) {
            ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SpawnObjectAI( "Animal_CanisLupus_Grey", cursorPos, vector.Zero, 1, "Test" );
            COM_Message("Spawning Animal_CanisLupus_Grey at X = " + cursorPos[0] + ", Y = " + cursorPos[1] + ", Z = " + cursorPos[2]);
        } else if(COM_SHIFT()) {
            ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SpawnObjectAI( COM_GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), cursorPos, vector.Zero, 1, "Test" );
            COM_Message("Spawning Random Animal at X = " + cursorPos[0] + ", Y = " + cursorPos[1] + ", Z = " + cursorPos[2]);
        } else {
            ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SpawnObjectAI( COM_GetRandomChildFromBaseClass( "cfgVehicles", "ZombieBase", 2 ), cursorPos, vector.Zero, 1, "Test" );
            COM_Message("Spawning Random Infected at X = " + cursorPos[0] + ", Y = " + cursorPos[1] + ", Z = " + cursorPos[2]);
        }
    }
    void SpawnGift() {
        vector cursorPos = COM_GetCursorPos();
        COM_Message("Spawning GiftBox_Large_1 at X = " + cursorPos[0] + ", Y = " + cursorPos[1] + ", Z = " + cursorPos[2]);
        ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SpawnObjectAI("GiftBox_Large_1", cursorPos, vector.Zero, 1, "Test");
    }

    void HideHud() { 
        if(!COM_SHIFT() && !COM_CTRL()) { Widget hudWidget = IngameHud.Cast(COM_GetClientMission().GetHud()).GetHudPanelWidget(); hudWidget.Show(!hudWidget.IsVisible()); }
    }

    void PrintPlayer() {
        vector pos = COM_GetPB().GetPosition(); vector ypr = COM_GetPB().GetOrientation();
        scriptLog("Position: " + COM_VectorToString(pos)); scriptLog("Orientation: " + COM_VectorToString(ypr));
        string toCopy;
        if(COM_CTRL()) {
            string plyr_r = COM_FormatFloat(ypr[2], 6);
            string plyr_p = COM_FormatFloat(ypr[1], 6);
            string plyr_y = COM_FormatFloat(ypr[0], 6);
            if(plyr_r == "0" || plyr_r == "-0") { plyr_r = "0.000000"; }
            if(plyr_p == "0" || plyr_p == "-0") { plyr_p = "0.000000"; }
            if(plyr_y == "0" || plyr_y == "-0") { plyr_y = "0.000000"; }
            toCopy += "    <group name=\"Item_Survivor\" pos=\"" + COM_VectorToString( pos ) + "\" rpy=\"" + plyr_r + " " + plyr_p + " " + plyr_y + "\" />";
            
        } else if(COM_SHIFT()) {
            toCopy += "        <zone name=\"Rest\" smin=\"0\" smax=\"0\" dmin=\"1\" dmax=\"1\" x=\"" + pos[0].ToString() + "\" z=\"" + pos[2].ToString() + "\" r=\"45\"/>";
        } else { toCopy += "		<pos x=\"" + pos[0].ToString() + "\" z=\"" + pos[2].ToString() + "\" a=\"" + ypr[0].ToString() + "\" />"; }
        COM_Message("Copied to clipboard: " + toCopy);
        GetGame().CopyToClipboard("\n" + toCopy);
    }
    void TeleportCursor()
    {
        if(COM_GetPB().IsInVehicle()) { COM_Message("Exit the vehicle before teleporting."); return; }
        vector cursorPos = COM_GetCursorPos();
        if (vector.Distance(COM_GetPB().GetPosition(), cursorPos) > 5000) { COM_Message("You cannot teleport that far."); return; }
		cursorPos = COM_SnapToGround(cursorPos);
        COM_GetPB().SetPosition(cursorPos); 
        COM_Message("Teleported to X = " + cursorPos[0] + ", Y = " + cursorPos[1] + ", Z = " + cursorPos[2]);
		Camera cam = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).m_oCamera;
		if(cam) { cursorPos[1] = cursorPos[1] + 2; cam.SetPosition(cursorPos); }
    }
    void PrintCursor() {
        vector hitPos = COM_GetCursorPos();
        cursorPositions += "\n		<pos x=\"" + hitPos[0].ToString() + "\" z=\"" + hitPos[2].ToString() + "\" a=\"0\" />";
        GetGame().CopyToClipboard(cursorPositions);
        COM_Message("Added to cursor Positions: X = " + hitPos[0].ToString() + ", Y = " + hitPos[2].ToString());
    }
    void ClearCursor() {
        cursorPositions = "";
        COM_Message("Cleared cursor positions.");
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
        if (m_COM_GodMode) // located in staticfunctions
        {
            COM_GetPB().SetHealth(COM_GetPB().GetMaxHealth("", ""));
            COM_GetPB().SetHealth("", "Blood", COM_GetPB().GetMaxHealth("", "Blood"));
            COM_GetPB().SetHealth("", "Shock", COM_GetPB().GetMaxHealth("", "Shock"));

            COM_GetPB().GetStatEnergy().Set(COM_GetPB().GetStatEnergy().GetMax());
            COM_GetPB().GetStaminaHandler().SetStamina(COM_GetPB().GetStatStamina().GetMax());
            COM_GetPB().GetStatWater().Set(1000);
            //            COM_GetPB().GetStatStomachVolume().Set(300);
            //            COM_GetPB().GetStatStomachWater().Set(300);
            //            COM_GetPB().GetStatStomachEnergy().Set(300);
            COM_GetPB().GetStatHeatComfort().Set(0);

            EntityAI oWeapon = COM_GetPB().GetHumanInventory().GetEntityInHands();

            if (oWeapon)
            {
                oWeapon.SetHealth(oWeapon.GetMaxHealth("", ""));

                Magazine oMag = (Magazine.Cast(oWeapon.GetAttachmentByConfigTypeName("DefaultMagazine"));

                if (oMag && oMag.IsMagazine())
                {
                    oMag.LocalSetAmmoMax();
                }

                Object oSupressor = (Object)oWeapon.GetAttachmentByConfigTypeName("SuppressorBase");

                if (oSupressor)
                {
                    oSupressor.SetHealth(oSupressor.GetMaxHealth("", ""));
                }
            }
        }
    }
}