

//-----------------------------------------------------------------------------------------------
// DayZPlayerTypeCreate - main dayz type config
//-----------------------------------------------------------------------------------------------

void DayZPlayerTypeCreate(DayZPlayerType pType)
{
//	Print("DayZPlayerTypeCreate called");

	//! register all item types
	DayZPlayerTypeRegisterItems(pType);

	//! register all sounds
	DayZPlayerTypeRegisterSounds(pType);
	
	//! register camera creators
	DayZPlayerCameras.RegisterCameras(pType);
 
	//! other configs 

	//! look limits 
	pType.SetLookLimits(-85, 85, -160, 160);	
	
	//! aiming limits 	
	pType.SetAimLimits(-85, 85, -180, 180);		

	//! camera shoot params
	pType.SetCameraShootParams(250, 0.1);
	

	//--------------------------------------------------------------
    // VIKTOR, JOHNNY - nastaveni Movu !!!

	//! set move settings: 
	SHumanCommandMoveSettings hmcs = pType.CommandMoveSettingsW();

	//! run sprint (SHIFT HOLD) filter 
	hmcs.m_fRunSpringTimeout	= 0.1;							//!< filter span value		[s]
	hmcs.m_fRunSpringMaxChange	= 4.0;							//!< filter speed value		[val/s]

	//! WSAD direction filter 
	hmcs.m_fDirFilterTimeout	= 0.1;						//!< direction filter timeout [s]
	hmcs.m_fDirFilterSpeed		= Math.DEG2RAD * 360; 		//!< direction filter max rot speed [rad/s]
	hmcs.m_fMaxSprintAngle		= Math.DEG2RAD * 45.01;		//!< max sprint angle [rad]

	hmcs.m_fTurnAngle 			= 45;						//!< this is when char starts tu turn
	hmcs.m_fTurnEndUpdateNTime	= 0.7;						//!< this is end norm time (turn amount is not updated after this time)
	hmcs.m_fTurnNextNTime		= 0.9;						//!< this is norm time when new turn from old turn is evaluated again (turn->turn)


	hmcs.m_fHeadingChangeLimiterIdle 	= 500000; 			//!<
	hmcs.m_fHeadingChangeLimiterWalk 	= 2000;				//!<
	hmcs.m_fHeadingChangeLimiterRun		= 1500;				//!<


	//--------------------------------------------------------------
    // VIKTOR, JOHNNY, PETERN - nastaveni speedu animaci

	// DayZPlayerUtils.DebugOverrideAnimationSpeed("dz/anims/anm/player/moves/rifles/p_rfl_erc_runF_low.anm", 2.0);
	// DayZPlayerUtils.DebugOverrideAnimationTranslation("dz/anims/anm/player/moves/rifles/p_rfl_erc_runF_low.anm", "0 0 10" );

}


//-----------------------------------------------------------------------------------------------
// ITEMS 
//-----------------------------------------------------------------------------------------------

class DayzPlayerItemBehaviorCfg extends HumanItemBehaviorCfg
{
	void DayzPlayerItemBehaviorCfg()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ALL;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] = DayZPlayerConstants.MOVEMENT_IDLE;

		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_INVALID;		//! per item camera user data to be uninitialized

		//! rotational delay to camera direction in move only (filter)
		m_fMoveHeadingFilterSpan 	= 0.15;	                // 
		m_fMoveHeadingFilterSpeed	= Math.DEG2RAD * 300;	// 720 stup / sek

		m_fMeleeEvadeHeadingFilterSpan	= 0.07;	                // 
		m_fMeleeEvadeHeadingFilterSpeed	= Math.DEG2RAD * 1440;	// 720 stup / sek

		m_bAttackLean = false;
	}

	void 	SetPistols()
	{
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, true, true, true);

		SetIK(DayZPlayerConstants.STANCEIDX_RAISEDERECT, DayZPlayerConstants.MOVEMENTIDX_SPRINT, false, true, false);
	}

	void 	SetFirearms()
	{
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, true, true, true);
	}

	void 	SetIKTwoHanded()
	{
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, false, false, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, false, false, false);
	}

	void	SetEmptyHanded()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_EMPTYHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		
		SetIKAll(false, false, false);

		m_bAttackLean = true;
	}
	
	void	SetTwoHanded()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
	  	m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;

		SetIKTwoHanded();

		m_bAttackLean = true;
	}

	void	SetToolsOneHanded()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_ONEHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		
		SetIKAll(false, true, false);
		
		m_bAttackLean = true;
	}
	
	void	SetHeavyItems()
	{
	  m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	  m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
	  m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;
	  
	  SetIKAll(false, true, true);
		
	  m_bAttackLean = true;
	  
	}
}


void DayZPlayerTypeRegisterItems(DayZPlayerType pType)
{
	GetGame().ProfilerStart("DayZPlayerTypeRegisterItems");
	
	//! behaviour 
	DayzPlayerItemBehaviorCfg 	emptyHanded = new DayzPlayerItemBehaviorCfg;
	emptyHanded.SetEmptyHanded();
	
	DayzPlayerItemBehaviorCfg 	twoHanded = new DayzPlayerItemBehaviorCfg;
	twoHanded.SetTwoHanded();
	
	DayzPlayerItemBehaviorCfg 	toolsOneHanded = new DayzPlayerItemBehaviorCfg;
	toolsOneHanded.SetToolsOneHanded();
	
	DayzPlayerItemBehaviorCfg 	heavyItemBehaviour = new DayzPlayerItemBehaviorCfg;
	heavyItemBehaviour.SetHeavyItems();
	
	//! set the default - use default instance set + default behaviour - empty hands
	pType.SetDefaultItemInHandsProfile("", emptyHanded);

	//! reset previous setup 
	pType.ResetItemInHandsProfiles();

	//! complete root of any item not specified here
	{
		pType.AddItemInHandsProfileIK("Inventory_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, "dz/anims/anm/player/ik/gear/apple.anm");
	}

	//! Pistols (RH,LH IK)
	{
		DayzPlayerItemBehaviorCfg 	pistolItemBehaviour = new DayzPlayerItemBehaviorCfg;

		pistolItemBehaviour.SetPistols();
		
		pType.AddItemInHandsProfileIK("1911_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_1911.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/fnp45.anm");
		pType.AddItemInHandsProfileIK("CZ75_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz75.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/CZ75.anm", "dz/anims/anm/player/reloads/CZ75/w_CZ75_states.anm");
		pType.AddItemInHandsProfileIK("DartGun", "dz/anims/workspaces/player/player_main/weapons/player_main_x2dartgun.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/X2.anm");
		pType.AddItemInHandsProfileIK("Derringer_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_derringer.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/derringer.anm");
		pType.AddItemInHandsProfileIK("Flaregun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_flaregun.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/flaregun.anm");
		pType.AddItemInHandsProfileIK("FNX_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnp45.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/fnp45.anm", "dz/anims/anm/player/reloads/FNP45/w_fnp45_states.anm" );
		pType.AddItemInHandsProfileIK("Glock19_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_glock.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Glock.anm");
		pType.AddItemInHandsProfileIK("LongHorn_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_longhorn.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/longhorn.anm");
		pType.AddItemInHandsProfileIK("Magnum_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_magnum.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/magnum.anm");
		pType.AddItemInHandsProfileIK("MKII_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_rugermk2.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/rugerMKII.anm");
		pType.AddItemInHandsProfileIK("pb6p9", "dz/anims/workspaces/player/player_main/weapons/player_main_pb6p9.asi",pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/PB6P9.anm");
		pType.AddItemInHandsProfileIK("MakarovIJ70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ij70.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/ij70.anm", "dz/anims/anm/player/reloads/IJ70/w_IJ70_states.anm");
		pType.AddItemInHandsProfileIK("P1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_p1.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/P1.anm");
		pType.AddItemInHandsProfileIK("Red9_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mauserred9.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Red9.anm");
		pType.AddItemInHandsProfileIK("Shockpistol_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_taser.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Shockpistol.anm");
	}

	//! Firearms (RH,LH IK)		
	{
		DayzPlayerItemBehaviorCfg 	fireArmsItemBehaviour = new DayzPlayerItemBehaviorCfg;
		
		fireArmsItemBehaviour.SetFirearms();
		
		pType.AddItemInHandsProfileIK("AK74_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak74u.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/akm.anm");
		pType.AddItemInHandsProfileIK("AK101_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak74u.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/akm.anm");
		pType.AddItemInHandsProfileIK("AKM_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_akm.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/akm.anm","dz/anims/anm/player/reloads/akm/w_akm_states.anm");
		pType.AddItemInHandsProfileIK("AugSteyr_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_aug.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/aug.anm");
		pType.AddItemInHandsProfileIK("B95_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_b95.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/b95.anm");
		pType.AddItemInHandsProfileIK("CZ61_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz61.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/CZ61.anm", "dz/anims/anm/player/reloads/cz61/w_cz61_states.anm");
		pType.AddItemInHandsProfileIK("CZ527_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz527.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/cz527.anm");
		pType.AddItemInHandsProfileIK("FAL_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnfal.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/fn_fal.anm");
		pType.AddItemInHandsProfileIK("Izh18_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh18.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/izh18.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		pType.AddItemInHandsProfileIK("M4A1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");
		pType.AddItemInHandsProfileIK("M249_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m249.asi",fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/M249.anm");
		pType.AddItemInHandsProfileIK("Mosin9130_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mosin9130.anm","dz/anims/anm/player/reloads/Mosin_1903/w_mosin1903_states.anm");
		pType.AddItemInHandsProfileIK("SawedoffMosin9130_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mosin9130.anm");
		pType.AddItemInHandsProfileIK("MP5K_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp5.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp5k.anm","dz/anims/anm/player/reloads/mp5/w_mp5k_states.anm");
		pType.AddItemInHandsProfileIK("PM73Rak_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_pm73.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/pm63rak.anm");
		pType.AddItemInHandsProfileIK("Repeater_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_repeater.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/repeater.anm");
		pType.AddItemInHandsProfileIK("Ruger1022_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ruger1022.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ruger1022.anm");
		pType.AddItemInHandsProfileIK("ScoutBase", "dz/anims/workspaces/player/player_main/player_main_scout.asi",fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/Scout.anm");
		pType.AddItemInHandsProfileIK("SKS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_sks.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/SKS.anm");
		pType.AddItemInHandsProfileIK("SVD_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_svd.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/svd.anm","dz/anims/anm/player/reloads/SVD/w_SVD_states.anm");
		pType.AddItemInHandsProfileIK("Trumpet_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_trumpet.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/trumpet.anm");
		pType.AddItemInHandsProfileIK("UMP45_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ump45.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ump.anm","dz/anims/anm/player/reloads/UMP45/w_UMP45_states.anm");
		pType.AddItemInHandsProfileIK("VSS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_vss.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/VSS.anm");
		pType.AddItemInHandsProfileIK("Winchester70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_winchester70.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/winchester70.anm");

		//! Shotguns
		pType.AddItemInHandsProfileIK("Izh43Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh43.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/izh43.anm");
		pType.AddItemInHandsProfileIK("Mp133Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp133.anm");
		pType.AddItemInHandsProfileIK("Mp133Shotgun_PistolGrip", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133grip.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp133_pistol_grip.anm");
		pType.AddItemInHandsProfileIK("Saiga12KShotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh43.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/Saiga.anm");
	}
	
	//! Archery (RH,LH IK)
	{
		DayzPlayerItemBehaviorCfg 	archeryItemBehaviour = new DayzPlayerItemBehaviorCfg;
		//! Bows
		pType.AddItemInHandsProfileIK("PVCBow", "dz/anims/workspaces/player/player_main/player_main_bow.asi", archeryItemBehaviour,						"dz/anims/anm/player/ik/weapons/bow_erc_IK.anm");
		pType.AddItemInHandsProfileIK("QuickieBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_quicky.asi", archeryItemBehaviour, 	"dz/anims/anm/player/ik/weapons/bow_erc_IK.anm");
		pType.AddItemInHandsProfileIK("RecurveBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_recurve.asi", archeryItemBehaviour,	"dz/anims/anm/player/ik/weapons/bow_erc_recurve_IK.anm");
		pType.AddItemInHandsProfileIK("Crossbow", "dz/anims/workspaces/player/player_main/weapons/player_main_crossbow.asi", archeryItemBehaviour,		"dz/anims/anm/player/ik/weapons/crossbow.anm");
		
		//! Arrows
		pType.AddItemInHandsProfileIK("Ammo_ArrowComposite", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowBolt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowBoned", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowPrimitive", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_SharpStick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
	}
	
	//! Launchers
	{
		DayzPlayerItemBehaviorCfg 	launcherItemBehaviour = new DayzPlayerItemBehaviorCfg;

		//!pType.AddItemInHandsProfile("GP25Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		//!pType.AddItemInHandsProfile("LAWBase", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		//!pType.AddItemInHandsProfile("M203Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);
		//!pType.AddItemInHandsProfile("RPG7Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);
	}

	//! Ammunition
	{
		pType.AddItemInHandsProfileIK("Ammunition_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		
		
		//! Boxes
		pType.AddItemInHandsProfileIK("AmmoBox_9x19_25rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/9mm_25rnd_box.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_00buck_10rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/00Buck_10RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x39_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x54_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_22_50Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 	"dz/anims/anm/player/ik/ammunition/22_50RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_556x45_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/556x20_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_357_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 		"dz/anims/anm/player/ik/ammunition/357_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_545x39_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/545x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_380_35rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 		"dz/anims/anm/player/ik/ammunition/380auto_35RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_45ACP_25rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/45acp_25rnd_box.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_308Win_20rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/308Win_20RoundBox.anm");
		
		//! Loose Rounds
		pType.AddItemInHandsProfileIK("Ammo_9x19", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_357", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_380", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_45ACP", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x39", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x54", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x39_LooseRounds.anm");
		
		pType.AddItemInHandsProfileIK("Ammo_LAW_HE", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Ammo_LAW_HE", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured

	}

	//! Attachments
	{	
		//! light
		pType.AddItemInHandsProfileIK("UniversalLight", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pistolItemBehaviour,				"dz/anims/anm/player/ik/gear/weaponlight_universal.anm");
		pType.AddItemInHandsProfileIK("TLRLight", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pistolItemBehaviour,						"dz/anims/anm/player/ik/light/weaponlight_TLR.anm");
	
		//! magazine
		pType.AddItemInHandsProfileIK("Magazine_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/banana.anm");
		pType.AddItemInHandsProfileIK("Mag_IJ70_8Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_ij70.anm");
		pType.AddItemInHandsProfileIK("Mag_FNX45_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded,		"dz/anims/anm/player/ik/attachments/magazines/magazine_fnp45.anm");
		pType.AddItemInHandsProfileIK("Mag_MP5_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded,			"dz/anims/anm/player/ik/attachments/magazines/magazine_mp5_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_MP5_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded,			"dz/anims/anm/player/ik/attachments/magazines/magazine_mp5_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_MKII_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_mk2.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_Drum75Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_drum.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP762x39_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_762x39_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP762x54_5Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_762x54_5Rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP9x19_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_9x19_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ75_15Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_CZ75.anm");
		pType.AddItemInHandsProfileIK("Mag_P1_8Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/magazines/magazine_p1.anm");
		pType.AddItemInHandsProfileIK("Mag_PM73_15Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_pm73_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_PM73_25Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_pm73_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_1911_7Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_1911.anm");
		pType.AddItemInHandsProfileIK("Mag_762x54Snaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/snaploader_762.anm");
		pType.AddItemInHandsProfileIK("Mag_308WinSnaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/snaploader_762.anm");
		pType.AddItemInHandsProfileIK("Mag_12gaSnaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/snaploader.anm");
		pType.AddItemInHandsProfileIK("Mag_357Speedloader_6Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/357_speedloader.anm");
		pType.AddItemInHandsProfileIK("Mag_SVD_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_svd.anm");
		pType.AddItemInHandsProfileIK("Mag_UMP_25Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_ump45.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ61_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_cz61_20rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ527_5rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_cz527.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_30Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_Palm30Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_STANAG_30Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");

		//! muzzle
		pType.AddItemInHandsProfileIK("AK_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("M9A1_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("Mosin_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("SKS_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("Mosin_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/attachments/muzzle/compensator_mosin.anm");
		pType.AddItemInHandsProfileIK("MP5_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/attachments/muzzle/compensator_mp5k.anm");
		pType.AddItemInHandsProfileIK("ImprovisedSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/attachments/muzzle/suppressor_bottle.anm");
		pType.AddItemInHandsProfileIK("M4_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/attachments/muzzle/suppressor_556.anm");
		pType.AddItemInHandsProfileIK("AK_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/attachments/muzzle/suppressor_545.anm");
		pType.AddItemInHandsProfileIK("PistolSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/attachments/muzzle/suppressor_45acp.anm");
		pType.AddItemInHandsProfileIK("Groza_Barrel_Short", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("Groza_Barrel_Grip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Groza_Barrel_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	
		//! optic
		pType.AddItemInHandsProfileIK("M4_CarryHandleOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/optic/optic_m4_carryhandle.anm");
		pType.AddItemInHandsProfileIK("BUISOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_buis.anm");
		pType.AddItemInHandsProfileIK("M68Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/attachments/optic/optic_m68_cco.anm");
		pType.AddItemInHandsProfileIK("FNP45_MRDOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_fnp45_mrd.anm");
		pType.AddItemInHandsProfileIK("ACOGOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/attachments/optic/optic_acog.anm");
		pType.AddItemInHandsProfileIK("PUScopeOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_puscope.anm");
		pType.AddItemInHandsProfileIK("LongrangeOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_longrange.anm");
		pType.AddItemInHandsProfileIK("HuntingOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_hunting_scope_12x.anm");
		pType.AddItemInHandsProfileIK("PSO1Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pso1.anm");
		pType.AddItemInHandsProfileIK("PSO11Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pso1.anm");
		pType.AddItemInHandsProfileIK("M4_T3NRDSOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_t3n_rds.anm");
		pType.AddItemInHandsProfileIK("PistolOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pistol.anm");
		pType.AddItemInHandsProfileIK("KashtanOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_kashtan.anm");
		pType.AddItemInHandsProfileIK("Crossbow_RedpointOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/optic/optic_red_point.anm");
		pType.AddItemInHandsProfileIK("ReflexOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("GrozaOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_ots14.anm");
			
		//! support		
		pType.AddItemInHandsProfileIK("M4_OEBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_m4_oe.anm");
		pType.AddItemInHandsProfileIK("M4_MPBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_m4_mp.anm");
		pType.AddItemInHandsProfileIK("M4_CQBBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_m4_cqb.anm");
		pType.AddItemInHandsProfileIK("M4_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_m4_plastic.anm");
		pType.AddItemInHandsProfileIK("M4_RISHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m4_ris.anm");
		pType.AddItemInHandsProfileIK("M4_MPHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m4_mp.anm");
		pType.AddItemInHandsProfileIK("AK_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_ak_folding.anm");
		pType.AddItemInHandsProfileIK("AK_PlasticBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_ak_plastic.anm");
		pType.AddItemInHandsProfileIK("AK_WoodHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_rail.anm");
		pType.AddItemInHandsProfileIK("AK_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_plastic.anm");
		pType.AddItemInHandsProfileIK("AK74_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK74_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AKS74U_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_aks.anm");
		pType.AddItemInHandsProfileIK("MP5k_StockBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_mp5_stock.anm");
		pType.AddItemInHandsProfileIK("MP5_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/handguard_mp5k_plastic.anm");
		pType.AddItemInHandsProfileIK("MP5_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_mp5k_rail.anm");
		pType.AddItemInHandsProfileIK("Red9Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_ak_folding.anm");
		pType.AddItemInHandsProfileIK("Fal_OeBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_fal_oe.anm");
		pType.AddItemInHandsProfileIK("Fal_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_fal_folding.anm");
		pType.AddItemInHandsProfileIK("Saiga_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_saiga_folding.anm");
		pType.AddItemInHandsProfileIK("M249_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m249.anm");
		pType.AddItemInHandsProfileIK("M249_RisHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_m249_ris.anm");
		pType.AddItemInHandsProfileIK("M249_Bipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/bipod_m249.anm");
		pType.AddItemInHandsProfileIK("AtlasBipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/bipod_atlas.anm");
		pType.AddItemInHandsProfileIK("GhillieAtt_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/ghillie_wrap.anm");
		pType.AddItemInHandsProfileIK("Groza_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/banana.anm");
		pType.AddItemInHandsProfileIK("GrozaGL_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/banana.anm");
	}
	
	//! Explosives
	{
		pType.AddItemInHandsProfileIK("RGD5Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("M67Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/explosives/grenade.anm");
		pType.AddItemInHandsProfileIK("M18SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/explosives/smokegrenade.anm");
		pType.AddItemInHandsProfileIK("Grenade_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("FlashGrenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/explosives/flashbang.anm");
		pType.AddItemInHandsProfileIK("RDG2SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	}

	//! Tools
	{
		pType.AddItemInHandsProfileIK("Flashlight", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/flashlight.anm");
		pType.AddItemInHandsProfileIK("Pen_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/pen.anm");
		pType.AddItemInHandsProfileIK("CanOpener", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/CanOpener.anm");
		pType.AddItemInHandsProfileIK("Heatpack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Heatpack.anm");
		pType.AddItemInHandsProfileIK("MessTin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/mess_tin.anm");
	    pType.AddItemInHandsProfileIK("Pliers", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/pliers.anm");
		pType.AddItemInHandsProfileIK("Lockpick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/lockpick.anm");
		pType.AddItemInHandsProfileIK("WeaponCleaningKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/cleaning_kit_wood.anm");
		pType.AddItemInHandsProfileIK("SewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/sewing_kit.anm");
		pType.AddItemInHandsProfileIK("LeatherSewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/leather_sewing_kit.anm");
		pType.AddItemInHandsProfileIK("Handcuffs", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/handcuffs.anm");
		pType.AddItemInHandsProfileIK("HandcuffKeys", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/handcuff_keys.anm");
		pType.AddItemInHandsProfileIK("Cassette", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/cassette.anm");
		pType.AddItemInHandsProfileIK("FieldShovel", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/CSLA_Field_Shovel.anm");
		pType.AddItemInHandsProfileIK("Crowbar", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("Hammer", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/hammer_ik.anm");
		pType.AddItemInHandsProfileIK("MeatTenderizer", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/meat_tenderizer.anm");
		pType.AddItemInHandsProfileIK("Wrench", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/wrench.anm");
		pType.AddItemInHandsProfileIK("LugWrench", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/Lug_wrench.anm");
		pType.AddItemInHandsProfileIK("Pipe", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/metal_pipe.anm");
		pType.AddItemInHandsProfileIK("Screwdriver", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/screwdriver.anm");
		pType.AddItemInHandsProfileIK("Sickle", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/sickle.anm");
		pType.AddItemInHandsProfileIK("Hacksaw", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/hacksaw.anm");
		pType.AddItemInHandsProfileIK("KitchenKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("HuntingKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/kitchenknife.anm"); 
		pType.AddItemInHandsProfileIK("CombatKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/kitchenknife.anm"); 
		pType.AddItemInHandsProfileIK("SteakKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/steak_knife.anm");
		pType.AddItemInHandsProfileIK("HayHook", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/hay_hook.anm");
		pType.AddItemInHandsProfileIK("StoneKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/stone_knife.anm");
		pType.AddItemInHandsProfileIK("Iceaxe", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/Ice_Axe.anm"); // wrong pose
		pType.AddItemInHandsProfileIK("Hatchet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/wrench.anm"); 
		pType.AddItemInHandsProfileIK("Machete", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/kitchenknife.anm"); 
		pType.AddItemInHandsProfileIK("BrassKnuckles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/brassknuckles.anm");
		pType.AddItemInHandsProfileIK("TelescopicBaton", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/TelescopicBaton.anm");
		pType.AddItemInHandsProfileIK("CattleProd", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("StunBaton", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("Cleaver", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("FishingRod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/fishingrod.anm");
		pType.AddItemInHandsProfileIK("ImprovisedFishingRod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/ImprovisedFishingRod.anm");
		pType.AddItemInHandsProfileIK("ObsoleteFishingRod", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_light.asi", twoHanded, 		"dz/anims/anm/player/ik/gear/ImprovisedFishingRod.anm");
		pType.AddItemInHandsProfileIK("Mace", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/flanged_mace.anm");
		
		//! Tools two handed
		pType.AddItemInHandsProfileIK("PipeWrench", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("Shovel", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("FarmingHoe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 			"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("SledgeHammer", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 			"dz/anims/anm/player/ik/two_handed/sledge_hammer.anm");
		pType.AddItemInHandsProfileIK("Broom", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("Paddle", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm"); 
		pType.AddItemInHandsProfileIK("FirefighterAxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 		"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("WoodAxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/woodaxe.anm");
		pType.AddItemInHandsProfileIK("Pitchfork", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_light.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/pitchfork.anm");		
		pType.AddItemInHandsProfileIK("Pickaxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("Sword", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_sword.asi", twoHanded,					"dz/anims/anm/player/ik/two_handed/medieval_sword.anm");
		pType.AddItemInHandsProfileIK("BaseballBat", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_bat.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/baseballbat.anm");	
		pType.AddItemInHandsProfileIK("HockeyStick", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");	
		pType.AddItemInHandsProfileIK("Chainsaw", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/chainsaw.anm"); 
		pType.AddItemInHandsProfileIK("FireExtinguisher", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 	"dz/anims/anm/player/ik/two_handed/fire_extinguisher.anm");	
	}
	
	//! Camping
	{
		//! Camping 2handed
		pType.AddItemInHandsProfileIK("CableReel", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/cable_reel.anm");
		pType.AddItemInHandsProfileIK("Spotlight", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/spotlight.anm");
		pType.AddItemInHandsProfileIK("MediumTent", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tent_medium_packed.anm");
	
		//! Camping heavy	
		pType.AddItemInHandsProfileIK("Barrel_ColorBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("CarTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PartyTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("CamoNet", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("CamoNetShelter", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BatteryCharger", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfile("PowerGenerator", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);	
		// pType.AddItemInHandsProfileIK("PowerGenerator", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour, 			"dz/anims/anm/player/ik/heavy/generator.anm");
	}	
	
	//! Consumables	
	{	
		pType.AddItemInHandsProfileIK("Paper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("DuctTape", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/duct_tape.anm");
		pType.AddItemInHandsProfileIK("Bandana_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/BandanaFH_ground.anm");
		pType.AddItemInHandsProfileIK("Rag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/Rags.anm");
		pType.AddItemInHandsProfileIK("BurlapStrip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/burlap_strips.anm");
		pType.AddItemInHandsProfileIK("SmallStone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/SmallStone.anm");
		pType.AddItemInHandsProfileIK("BatteryD", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/battery_d.anm");
		pType.AddItemInHandsProfileIK("Battery9V", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/9v_battery.anm");
		pType.AddItemInHandsProfileIK("SmallGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/GasCannisterSmall.anm");
		pType.AddItemInHandsProfileIK("MediumGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		 			"dz/anims/anm/player/ik/gear/GasCannisterLarge.anm");  
		pType.AddItemInHandsProfileIK("LargeGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/GasCannisterLarge.anm");	
		pType.AddItemInHandsProfileIK("Spraycan_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/spraycan.anm");
		pType.AddItemInHandsProfileIK("Matchbox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Matchbox.anm");
		pType.AddItemInHandsProfileIK("NailBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Nails_box.anm");
		pType.AddItemInHandsProfileIK("Roadflare", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/roadflare.anm");
		pType.AddItemInHandsProfileIK("Chemlight_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/chemlight.anm");
		pType.AddItemInHandsProfileIK("Hook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 				"dz/anims/anm/player/ik/gear/bait_worm_pinned.anm");
		pType.AddItemInHandsProfileIK("BoneHook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/bone_hook.anm");
		pType.AddItemInHandsProfileIK("Nail", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/Nails_loose_10.anm");
		pType.AddItemInHandsProfileIK("Bone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Bait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/bait_worm_pinned.anm");
		pType.AddItemInHandsProfileIK("BoneBait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Netting", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("ButaneCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/ButanCanister.anm");	
		pType.AddItemInHandsProfileIK("Bark_Oak", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Bark_oak.anm");
		pType.AddItemInHandsProfileIK("Bark_Birch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/Bark_birch.anm");
		
			
		//! Consumables two handed	
		pType.AddItemInHandsProfileIK("Stone", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 									"dz/anims/anm/player/ik/two_handed/stone.anm");
		pType.AddItemInHandsProfileIK("Firewood", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/firewood.anm");
		pType.AddItemInHandsProfileIK("Pelt_Base", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("TannedLeather", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tanned_leather.anm");	
		pType.AddItemInHandsProfileIK("WoodenPlank", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PileOfWoodenPlanks", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("MetalPlate", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("GardenLime", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("CowPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Cow.anm");
		pType.AddItemInHandsProfileIK("DeerPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Deer.anm");
		pType.AddItemInHandsProfileIK("RabbitPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Rabbit.anm");
		pType.AddItemInHandsProfileIK("FoxPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Fox.anm");
		pType.AddItemInHandsProfileIK("WildboarPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 			"dz/anims/anm/player/ik/two_handed/Pelt_Wildboar.anm");
		pType.AddItemInHandsProfileIK("GoatPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Goat.anm");
		pType.AddItemInHandsProfileIK("PigPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Pig.anm");
		pType.AddItemInHandsProfileIK("MouflonPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 			"dz/anims/anm/player/ik/two_handed/Pelt_Mouflon.anm");


	}	
	
	//! Containers	
	{	
		pType.AddItemInHandsProfileIK("FirstAidKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/first_aid_kit.anm");	
		pType.AddItemInHandsProfileIK("SmallProtectorCase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/protector_case.anm");	
		pType.AddItemInHandsProfileIK("AmmoBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/AmmoBox.anm"); 
		pType.AddItemInHandsProfileIK("Bear_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/TeddyBear.anm");
		pType.AddItemInHandsProfileIK("ChildBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/child_g.anm");
		pType.AddItemInHandsProfileIK("SmershBag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/smersh_g.anm");
	
		//! Containers - 2handed		
		pType.AddItemInHandsProfileIK("WaterproofBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/WaterproofBag.anm");	
		pType.AddItemInHandsProfileIK("TaloonBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/taloon_g.anm");	
		pType.AddItemInHandsProfileIK("TortillaBag", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tortilla_g.anm");	
		pType.AddItemInHandsProfileIK("HuntingBag", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/hunting_g.anm");
		pType.AddItemInHandsProfileIK("AssaultBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/assault_g.anm");
		pType.AddItemInHandsProfileIK("MountainBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/mountain_g.anm");
		pType.AddItemInHandsProfileIK("CoyoteBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/coyote_g.anm");
		pType.AddItemInHandsProfileIK("AliceBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/alice_g.anm");
		
		
		//! Containers - Heavy	
		pType.AddItemInHandsProfileIK("Container_BarrelBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Barrel_ColorBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("TrashCan", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	}	
	
	//! Cooking	
	{	
		pType.AddItemInHandsProfileIK("ClutterCutterFireplace", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("BarrelHoles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Tripod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Cooking_Tripod.anm");		
		pType.AddItemInHandsProfileIK("FryingPan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/frying_pan.anm");		
		pType.AddItemInHandsProfileIK("PortableGasStove", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/gas_cooker.anm");	
		pType.AddItemInHandsProfileIK("PortableGasLamp", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/GasLight.anm");
		pType.AddItemInHandsProfileIK("Candle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/candle.anm");
		pType.AddItemInHandsProfileIK("PetrolLighter", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/petrol_lighter.anm");	
		pType.AddItemInHandsProfileIK("FireplaceIndoor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/FireplaceIndoor.anm");
			
		//! Cooking two handed	
		pType.AddItemInHandsProfileIK("Pot", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 									"dz/anims/anm/player/ik/two_handed/CookingPot.anm");
		pType.AddItemInHandsProfileIK("FireplaceBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/two_handed/Fireplace.anm"); 
		
	}	
	
	//! Crafting	
	{	
		pType.AddItemInHandsProfileIK("Rope", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("ImprovisedRope", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("MetalWire", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/metalwire.anm");
		pType.AddItemInHandsProfileIK("Torch", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/torch.anm");
		pType.AddItemInHandsProfileIK("ChickenFeather", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("GorkaHelmetVisor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/Maska_glass_g.anm");
		pType.AddItemInHandsProfileIK("HandDrillKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/hand_drill_kit.anm");
		pType.AddItemInHandsProfileIK("WoodenStick", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/WoodenStick.anm"); 
		pType.AddItemInHandsProfileIK("LongWoodenStick", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/LongWoodenStick.anm");
		pType.AddItemInHandsProfileIK("BurlapSack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/bp_burlap_sack.anm");
	
		//! Crafting 2hd	
		pType.AddItemInHandsProfileIK("Spear", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	
	}	
	
	//! Cultivation	
	{	
		pType.AddItemInHandsProfileIK("TomatoSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/Seeds_Tomato.anm");
		pType.AddItemInHandsProfileIK("SeedBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Seeds_Tomato.anm");
		pType.AddItemInHandsProfileIK("PepperSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("PumpkinSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("ZucchiniSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("CannabisSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("AntiPestsSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("ParticleTest", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
			
		//! Cultivation two handed	
		pType.AddItemInHandsProfileIK("PlantMaterial", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	
	}	
		
	//! Drinks	
	{	
		pType.AddItemInHandsProfileIK("Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/water_bottle.anm");
		pType.AddItemInHandsProfileIK("Canteen", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/canteen.anm");	
		pType.AddItemInHandsProfileIK("WaterBottle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/water_bottle.anm");	
		pType.AddItemInHandsProfileIK("WaterPouch_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/waterpouch.anm");	
		pType.AddItemInHandsProfileIK("SodaCan_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/soda_can.anm");	
	}	
		
	//! Food	
	{	
		//! Food animal corpses	
		pType.AddItemInHandsProfileIK("Carp", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/carp_live.anm");
		pType.AddItemInHandsProfileIK("Sardines", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/sardines_live.anm");
		pType.AddItemInHandsProfileIK("Mackerel", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/mackerel_fillet.anm");	
		pType.AddItemInHandsProfileIK("Worm", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/bait_worm.anm");
		
		//! Food cans	
		pType.AddItemInHandsProfileIK("BakedBeansCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("BakedBeansCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("PeachesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("PeachesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("TacticalBaconCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("TacticalBaconCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("SpaghettiCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("SpaghettiCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("SardinesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/sardines.anm");
		pType.AddItemInHandsProfileIK("SardinesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/sardines.anm");
		pType.AddItemInHandsProfileIK("TunaCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("TunaCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/tuna.anm");
			
		//! Food fruits		
		pType.AddItemInHandsProfileIK("Apple", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/apple.anm");
		pType.AddItemInHandsProfileIK("Banana", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Orange", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/orange.anm");	
		pType.AddItemInHandsProfileIK("Tomato", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/tomato_fresh.anm");
		pType.AddItemInHandsProfileIK("GreenBellPepper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/pepper_green_fresh.anm");
		pType.AddItemInHandsProfileIK("Zucchini", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/zucchini_fresh.anm");
		pType.AddItemInHandsProfileIK("Pumpkin", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/pumpkin_fresh.anm");
		pType.AddItemInHandsProfileIK("SlicedPumpkin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Potato", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/potato_fresh.anm");
		pType.AddItemInHandsProfileIK("Kiwi", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/kiwi.anm");
		pType.AddItemInHandsProfileIK("SambucusBerry", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/berries.anm");
		pType.AddItemInHandsProfileIK("CaninaBerry", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/poisonousberries.anm");
		pType.AddItemInHandsProfileIK("Cannabis", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/berries.anm");
		pType.AddItemInHandsProfileIK("Plum", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Pear", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
				
		//! Food meats			
		pType.AddItemInHandsProfileIK("HumanSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_human.anm");
		pType.AddItemInHandsProfileIK("GoatSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("MouflonSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_steak.anm");	
		pType.AddItemInHandsProfileIK("BoarSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("PigSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("DeerSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("WolfSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("BearSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");	
		pType.AddItemInHandsProfileIK("CowSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("SheepSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("FoxSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("ChickenBreastMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_breast.anm");
		pType.AddItemInHandsProfileIK("RabbitLegMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_leg.anm");
		pType.AddItemInHandsProfileIK("CarpFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/carp_fillet_raw.anm");
		pType.AddItemInHandsProfileIK("MackerelFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/mackerel_fillet.anm"); 
		pType.AddItemInHandsProfileIK("Lard", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/lard.anm");
		
		//! Food mushrooms	
		pType.AddItemInHandsProfileIK("AgaricusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("AmanitaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("MacrolepiotaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("LactariusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PsilocybeMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("AuriculariaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		
		//! Food basic	
		pType.AddItemInHandsProfileIK("Edible_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BoxCerealCrunchin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/box_cereal.anm");
		pType.AddItemInHandsProfileIK("PowderedMilk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/powdered_milk.anm");
		pType.AddItemInHandsProfileIK("SmallGuts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Guts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Rice", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/rice.anm");
		pType.AddItemInHandsProfileIK("Marmalade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/marmalade.anm");
	}
	
	//! Medical 
	{
		pType.AddItemInHandsProfileIK("Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BandageDressing", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/bandage_dressing.anm");
		pType.AddItemInHandsProfileIK("DisinfectantSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/DesinfectantSpray.anm");
		pType.AddItemInHandsProfileIK("DisinfectantAlcohol", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PurificationTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/Water_purification_tablets.anm");
		pType.AddItemInHandsProfileIK("CharcoalTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/charcoal_tablets.anm");
		pType.AddItemInHandsProfileIK("PainkillerTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/painkillers2.anm");
		pType.AddItemInHandsProfileIK("VitaminBottle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/VitaminBottle.anm");
		pType.AddItemInHandsProfileIK("TetracyclineAntibiotics", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/tetracycline.anm");
		pType.AddItemInHandsProfileIK("Epinephrine", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Morphine", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Syringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Syringe_Empty.anm");
		pType.AddItemInHandsProfileIK("ClearSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Syringe_Full.anm");
		pType.AddItemInHandsProfileIK("BloodSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Syringe_Full.anm");
		pType.AddItemInHandsProfileIK("ClearSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BloodSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("InjectionVial", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/InjectionVial.anm");
		pType.AddItemInHandsProfileIK("SalineBag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/SalineBag.anm");
		pType.AddItemInHandsProfileIK("StartKitIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/StartKit.anm");
		pType.AddItemInHandsProfileIK("SalineBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/TransfusionKit.anm");
		pType.AddItemInHandsProfileIK("BloodBagEmpty", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Sealed.anm");
		pType.AddItemInHandsProfileIK("BloodBagFull", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Full.anm");
		pType.AddItemInHandsProfileIK("BloodBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Full_IV.anm");
		pType.AddItemInHandsProfileIK("BloodTestKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodTest_Kit.anm");
		pType.AddItemInHandsProfileIK("Splint", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/improvised_fixture.anm");
		pType.AddItemInHandsProfileIK("Thermometer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/thermometer.anm");
		pType.AddItemInHandsProfileIK("CigarettePack_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		
		//! Medical two handed
		pType.AddItemInHandsProfileIK("Defibrillator", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/two_handed/defibrillator.anm");
	}

	//! Navigation
	{
		pType.AddItemInHandsProfileIK("ChernarusMap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Map_chernarus.anm");
		pType.AddItemInHandsProfileIK("Compass", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/compass.anm");
		pType.AddItemInHandsProfileIK("OrienteeringCompass", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/compass_modern.anm");
	}
	
	//! Optic
	{
		pType.AddItemInHandsProfileIK("Binoculars", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/binoculars.anm");
		pType.AddItemInHandsProfileIK("Rangefinder", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/rangefinder.anm");
	}

	//! Radio
	{
		pType.AddItemInHandsProfileIK("Transmitter_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/TransfusionKit.anm");
		pType.AddItemInHandsProfileIK("StaticRadio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Megaphone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/megaphone.anm");
		pType.AddItemInHandsProfileIK("PersonalRadio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/PersonalRadio.anm");
		pType.AddItemInHandsProfileIK("Radio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 				"dz/anims/anm/player/ik/gear/Radio.anm");  
       
	   //! Radio heavy
		pType.AddItemInHandsProfileIK("CarBattery", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	}

	//! Traps
	{
		pType.AddItemInHandsProfileIK("LandMineTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/landmine.anm");
		pType.AddItemInHandsProfileIK("SmallFishTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/improvised_fish_trap.anm");
		pType.AddItemInHandsProfileIK("FishNetTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("TripwireTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/tripwire_g.anm");
		pType.AddItemInHandsProfileIK("RabbitSnareTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/rabbit_snare_folded.anm");
    
		//! Traps two handed
		pType.AddItemInHandsProfileIK("BearTrap", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/beartrap_g.anm");
	}
	
	//! Clothes
	
	//! Headgear
	{
		pType.AddItemInHandsProfileIK("GorkaHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/gorka_helmet.anm");
		pType.AddItemInHandsProfileIK("GorkaHelmetComplete", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/gorka_helmet_complete.anm");
		pType.AddItemInHandsProfileIK("FirefightersHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/firehelmet.anm");
		pType.AddItemInHandsProfileIK("BallisticHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/ballistic_helmet.anm");
		pType.AddItemInHandsProfileIK("ZSh3PilotHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/ZSh3_helmet.anm");
		pType.AddItemInHandsProfileIK("ConstructionHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/construction_helmet.anm");
		pType.AddItemInHandsProfileIK("BurlapSackCover", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/burlap_cover.anm");
		pType.AddItemInHandsProfileIK("Ushanka_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/ushanka_hat.anm");
		pType.AddItemInHandsProfileIK("MotoHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/moto_helmet.anm");
		pType.AddItemInHandsProfileIK("MilitaryBeret_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/military_beret.anm");
		pType.AddItemInHandsProfileIK("CowboyHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/cowboy_hat.anm");
		pType.AddItemInHandsProfileIK("BoonieHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/boonie_hat.anm");
		pType.AddItemInHandsProfileIK("PilotkaCap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/pilotka_cap.anm");
		pType.AddItemInHandsProfileIK("TankerHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/tanker_helmet.anm");
		pType.AddItemInHandsProfileIK("SkateHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/skate_helmet.anm");
		pType.AddItemInHandsProfileIK("PrisonerCap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/prisoner_cap.anm");
		pType.AddItemInHandsProfileIK("GreatHelm", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/greathelm.anm");
		pType.AddItemInHandsProfileIK("Ssh68Helmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/ssh68_helmet.anm");
		pType.AddItemInHandsProfileIK("DirtBikeHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/dirtbike_helmet.anm");
		pType.AddItemInHandsProfileIK("NioshFaceMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/nioshface_mask.anm");
		pType.AddItemInHandsProfileIK("DallasMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("HoxtonMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("WolfMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("AviatorGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("ThinFramesGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("SportGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("DesignerGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");	
	}
	
	//! Shoe
	{
		
		
		
	}
	
	//! Vest
	{
		pType.AddItemInHandsProfileIK("BallisticVest", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/vest_ballistic.anm");
		pType.AddItemInHandsProfileIK("HighCapacityVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/vest_highcapacity.anm");
		pType.AddItemInHandsProfileIK("PoliceVest", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/vest_police.anm");
		pType.AddItemInHandsProfileIK("PressVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/vest_press.anm");
		pType.AddItemInHandsProfileIK("UKAssVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/vest_ukassault.anm");


	}
	
	//! Clothing attachments (holsters, plates)
	{
		pType.AddItemInHandsProfileIK("CarrierPouches", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/plate_carrier_pouches_g.anm");
		pType.AddItemInHandsProfileIK("CarrierHolsterSolo", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/plate_carrier_holster.anm");
		pType.AddItemInHandsProfileIK("ChestHolster", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/chest_holster.anm");
	}
	
	//! Others
	{
		pType.AddItemInHandsProfileIK("Book_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/book.anm");
	}

	//!===========================================================
	// bone Remaps

	{
		// mapping "weaponBone" -> "humanBone" 

		array<string> rifleBoneRemap = { "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "trigger", "Weapon_Trigger", "charging", "Weapon_Bone_01", "bullet", "Weapon_Bullet", "mag_release", "Weapon_Bone_02", "boltrelease", "Weapon_Bone_03"};
		pType.AddItemBoneRemap("Rifle_Base", rifleBoneRemap);

		array<string> pistolBoneRemap ={ "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "bullet","Weapon_Bullet", "trigger", "Weapon_Trigger", "hammer", "Weapon_Bone_01", };
		pType.AddItemBoneRemap("Pistol_Base", pistolBoneRemap);
		
		array<string> izh18BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet"};
		pType.AddItemBoneRemap("Izh18_Base", izh18BoneRemap);
		
		array<string> Mosin9130BoneRemap ={ "bolthandle", "Weapon_Bolt", "boltbody", "Weapon_Bone_04","boltbodyS", "Weapon_Bone_05", "trigger", "Weapon_Trigger","bullet", "Weapon_Bullet"};
		pType.AddItemBoneRemap("Mosin9130_Base", Mosin9130BoneRemap);
		
		//array<string> M4A1BoneRemap ={ "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "trigger", "Weapon_Trigger","charginghandle", "Weapon_Bone_01","bullet","Weapon_Bullet", };
		//pType.AddItemBoneRemap("M4A1_Base", M4A1BoneRemap);
	}

	//!===========================================================
	// bone Remaps

	//
	pType.DebugPreloadAnimationInstances();
	
	GetGame().ProfilerStop("DayZPlayerTypeRegisterItems");

}

