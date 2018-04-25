/** @file 

  this file is interface to Human 

*/





// *************************************************************************************
// ! HumanInputController - what we know about the input - human.GetInputController()
// *************************************************************************************
class HumanInputController
{
	//--------------------------------------------------------------

	//! returns main heading angle (in radians)  -PI .. PI (this is world Y angle player is actualy pointing the camera)
	proto native float 			GetHeadingAngle();

	//! returns per tick aim change (in radians) 
	proto native vector 		GetAimChange();

	//! 1st/3rd person camera view
	proto native bool			CameraViewChanged();

	//! returns per tickaim change (in radians) 
	proto native bool			CameraIsFreeLook();
	
	//! returns true if camera is on right/ false-left shoulder
	proto native bool			Camera3rdIsRightShoulder();

	//--------------------------------------------------------------

	//! jump/climb action was pressed
	proto native bool			IsJumpClimb();


	//--------------------------------------------------------------

	//! returns pressed SHIFT (melee evade)
	proto native bool			IsMeleeEvade();

	//! returns SHIFT down (melee fast/heavy) attack modifier
	proto native bool			IsMeleeFastAttackModifier();

	//! returns 0,1,2 = none,left,right
	proto native int 			IsMeleeLREvade();

	//--------------------------------------------------------------

	//! returns true if weapon is raised
	proto native bool			IsWeaponRaised();

	//--------------------------------------------------------------
	
	//! returns true if Use/Attack button is pressed (== true for multiple ticks)
	//! LButton now
	proto native bool			IsUseButton();

	//! returns true if Use/Attack button has just been pressed (== true for 1 tick only)
	//! LButton 
	proto native bool			IsUseButtonDown();

	//! single use (== true for 1 tick only)
	//! Lbutton + not raised !
	proto native bool			IsSingleUse();

	//! Long click use (== true for multiple ticks)
	//! long Lbutton + not raised !
	proto native bool			IsContinuousUse();

	//! is start of cont use (== true for 1 tick only)
	//! long Lbutton + not raised !
	proto native bool			IsContinuousUseStart();

	//! is end of cont use (== true for 1 tick only)
	//! long Lbutton + not raised !
	proto native bool			IsContinuousUseEnd();

	//! is immediate action triggered - 1 tick only
	//! Middle Button + not raised !
	proto native bool			IsImmediateAction();


	//! weapon handling 
	
	//! R - reloading / bolting  (== true for 1 tick only)
	proto native bool			IsReloadOrMechanismSingleUse();

	//! R - reloading / bolting - long press (== true for multiple ticks)
	proto native bool			IsReloadOrMechanismContinuousUse();

	//! R - reloading / bolting  (== true for 1 tick only)
	proto native bool			IsReloadOrMechanismContinuousUseStart();

	//! R - reloading / bolting - long press (== true for multiple ticks)
	proto native bool			IsReloadOrMechanismContinuousUseEnd();

	//! zooming 
	proto native bool			IsZoom();

	//! sight has been changed (in/out ironsights)
	proto native bool			IsSightChange();

	//!
	proto native bool			IsZoomIn();

	//!
	proto native bool			IsZoomOut();

  	//! fire mode has changed
	proto native bool			IsFireModeChange();

  	//! zeroing up
	proto native bool			IsZeroingUp();

  	//! zeroing down
	proto native bool			IsZeroingDown();

  	//! holding breath
	proto native bool			IsHoldBreath();

	//--------------------------------------------------------------

	//! returns 1..12 if some gesture slot is used, 0 otherwise
	proto native int 			IsGestureSlot();


	//--------------------------------------------------------------

	//! returns 1..10 if some quickbar slot is used, 0 otherwise
	proto native int 			IsQuickBarSlot();

	//! single use (== true for 1 tick only)
	//! quickbar + not raised !
	proto native bool			IsQuickBarSingleUse();

	//! Long click use (== true for multiple ticks)
	//! long quickbar + not raised !
	proto native bool			IsQuickBarContinuousUse();

	//! is start of cont use (== true for 1 tick only)
	//! long quickbar + not raised !
	proto native bool			IsQuickBarContinuousUseStart();

	//! is end of cont use (== true for 1 tick only)
	//! long quickbar + not raised !	
	proto native bool			IsQuickBarContinuousUseEnd();



	//--------------------------------------------------------------
	// LIMITS 

	//! this disables sprint
	proto native void 			LimitsDisableSprint(bool pDisable);

	//! is sprint disabled
	proto native bool			LimitsIsSprintDisabled();
	

	//--------------------------------------------------------------
	// overrides

	//!
	proto native void 			OverrideMovementSpeed(bool pOverride, float pVal);

	//!
	proto native void 			OverrideMovementAngle(bool pOverride, float pVal);
	
	//!
	proto native void 			OverrideAimChangeX(bool pOverride, float pVal);

	//!
	proto native void 			OverrideAimChangeY(bool pOverride, float pVal);

	//!
	proto native void 			OverrideMeleeEvade(bool pOverride, bool pValue);

	//!
	proto native void 			OverrideRaise(bool pOverride, bool pValue);

	//! 
	proto native void 			Override3rdIsRightShoulder(bool pOverride, bool pValue);


	//--------------------------------------------------------------

	//! never created by script
	private void HumanInputController()
	{
	}

	//! never created by script
	private void ~HumanInputController()
	{
	}	
}


// *************************************************************************************
// ! HumanCommandActionCallback - called as action callback 
// *************************************************************************************
class HumanCommandActionCallback
{
	//! cancels action 
	proto native 	void 	Cancel();	

	//! calls internal command 
	//! pInternalCommandId is one of CMD_ACTIONINT_ ... 
	proto native 	void 	InternalCommand(int pInternalCommandId);	

	//! enables calling cancel condition
	//! default is disabled
	//! when enabled - there must be a function "bool CancelCondition()" implemented
	proto native 	void 	EnableCancelCondition(bool pEnable);

	//! system implemented cancel condition (now raise or sprint cancels action)
	proto native 	bool	DefaultCancelCondition();

	//! registers animation event, when event registered and received - it sends OnAnimationEvent with registered id 
	proto native void 		RegisterAnimationEvent(string pAnimationEventStr, int pId);

	//! enables state change callback OnStateChange 
	proto native void 		EnableStateChangeCallback();

	//! example of implement of CancelCondition
	//! returns true it it should CANCEL the action
	//	bool				CancelCondition()	{ return false; }

	//! undef state - not running anymore
	static const int 	STATE_NONE 			= 0;

	//! looped state 
	static const int 	STATE_LOOP_IN 		= 1;
	static const int 	STATE_LOOP_LOOP 	= 2;
	static const int 	STATE_LOOP_END 		= 3;
	static const int 	STATE_LOOP_END2		= 4;
	static const int 	STATE_LOOP_LOOP2	= 5;
	static const int 	STATE_LOOP_ACTION	= 6;

	//! one time state 
	static const int 	STATE_NORMAL 		= 7;

	//! returns one of STATE_...
	proto native int 	GetState();


	//----------------------------------
	// debug helper

	//! returns debug string of current state 
	static string	GetStateString(int pState)
	{
		switch (pState)
		{
		case STATE_NONE:			return "NONE";

		case STATE_LOOP_IN:			return "LOOP_IN";
		case STATE_LOOP_LOOP: 		return "LOOP_LOOP";
		case STATE_LOOP_END:		return "LOOP_END";
		case STATE_LOOP_END2:		return "LOOP_END2";
		case STATE_LOOP_LOOP2:		return "LOOP_LOOP2";
		case STATE_LOOP_ACTION:		return "LOOP_ACTION";

			//! one time state 
		case STATE_NORMAL:			return "ONE TIME";
		}

		return "Undef";
	}


	//! returns debug string of current state 
	string	GetStateString()
	{
		return GetStateString(GetState());
	}



	//----------------------------------
	// callbacks

	//! calls animation event -> must be registered with RegisterAnimationEvent
	void 	OnAnimationEvent(int pEventID)	{};

	//! called ALWAYS! when action is finished (normal finish or cancel -> pCanceled)
	void 	OnFinish(bool pCanceled)	{};		

	//! called when enabled by EnableStateChangeCallback and state changes, call GetState() to get actual state 
	void 	OnStateChange(int pOldState, int pCurrentState)			{};
	
	
	// helps identify type of callback
	bool IsUserActionCallback()
	{
		return false;
	}
	
	bool IsGestureCallback()
	{
		return false;
	}
	
	bool IsSymptomCallback()
	{
		return false;
	}
}


// *************************************************************************************
// ! HumanCommandMove
// *************************************************************************************
class HumanCommandMove
{
	//! marks command to continue to combo 
	//! -180 -90 0 90 180 angles of current movement 
	proto native float 		GetCurrentMovementAngle();
		
	//! direction held on keyboard/controller
	//! -180 -90 0 90 180 angles of input movement (no filtering, no postprocesses)
	//! return false in idle states
	proto bool				GetCurrentInputAngle(out float pAngle);

	//! 0,1,2..3 idle, walk, run, sprint 
	proto native float 		GetCurrentMovementSpeed();

	//! marks command to continue to combo 
	proto native void 		StartMeleeEvade();

	//! marks command to continue to combo, pDirAngle specifies angle
	proto native void 		StartMeleeEvadeA(float pDirAngle);

	//! this is true when in melee evade
	proto native bool		IsMeleeEvade();

	//! this enables/disables block 
	proto native void 		SetMeleeBlock(bool pBlock);

	//!	pStanceIds is one of STANCEIDX_ERECT,STANCEIDX_CROUCH,STANCEIDX_PRONE,STANCEIDX_RAISEDERECT,STANCEIDX_RAISEDCROUCH,STANCEIDX_RAISEDPRONE,
	//! or -1 when release the stance lock 
	proto native void 		ForceStance(int pStanceIdx);


	//!	pStanceIds is one of STANCEIDX_ERECT,STANCEIDX_CROUCH
	//! this forces to stand up to required stance
	proto native void 		ForceStanceUp(int pStanceIdx);
}



// *************************************************************************************
// ! HumanCommandMelee - actual command melee running on 
// *************************************************************************************
class HumanCommandMelee
{
	//! marks command to continue to combo 
	proto native void 		ContinueCombo();

	//! returns true if hit is in range, where person can continue to combo
	proto native bool		IsInComboRange();

	//! is true only once after hit event 
	proto native bool		WasHit();

	//! cancels command melee and goes to HumanCommandMove
	proto native void		Cancel();
}


// *************************************************************************************
// ! HumanCommandMelee2 - actual command melee running on - Heavy/Light version
// *************************************************************************************
class HumanCommandMelee2
{
	//! marks command to continue to combo 
	proto native void 		ContinueCombo(bool pHeavyHit);

	//! returns true if hit is in range, where person can continue to combo
	proto native bool		IsInComboRange();

	//! is true only once after hit event 
	proto native bool		WasHit();

	//! cancels command melee and goes to HumanCommandMove
	proto native void		Cancel();
}


// *************************************************************************************
// ! HumanCommandFall - actually falling 
// *************************************************************************************
class HumanCommandFall
{
	static const int LANDTYPE_NONE = 0;
	static const int LANDTYPE_LIGHT	= 1;
	static const int LANDTYPE_MEDIUM = 2;
	static const int LANDTYPE_HEAVY = 3;

	//! this is true when fall has physically landed - need to call Land after this is true
	proto native bool	PhysicsLanded();
	
	//! end fall with land
	proto native void 	Land(int pLandType);
	
	//! returns true if fall is in landing state 
	proto native bool	IsLanding();


}

// *************************************************************************************
// ! HumanCommandDeath - actually dying
// *************************************************************************************
class HumanCommandDeath
{	
	// nothing here for now 
}


// *************************************************************************************
// ! HumanCommandUnconscious - actually not dying
// *************************************************************************************
class HumanCommandUnconscious
{	
	proto native void 	WakeUp();
}


// *************************************************************************************
// ! HumanCommandDamage - additional damage 
// *************************************************************************************
class HumanCommandDamage
{	
	// nothing here for now 
}

// *************************************************************************************
// ! HumanCommandLadder ladder
// *************************************************************************************
class HumanCommandLadder
{	

	//! returns true if on exiting point
	proto native bool				CanExit();

	//!
	proto native void 				Exit();

	//! debug draws any ladder 
	proto native static bool		DebugDrawLadder(Building pBuilding, int pLadderIndex);
	
	//!
	proto native static int 		DebugGetLadderIndex(string pComponentName); 
}



// *************************************************************************************
// ! HumanCommandLadder ladder
// *************************************************************************************
class HumanCommandSwim
{	
	//!
	proto native void 				StopSwimming();

	//! returns water level from entity position 
	//! returns in format (totalWaterDepth, characterDepht, 0)
	proto native static vector 		WaterLevelCheck(Human pHuman, vector pPosition);
}



// *************************************************************************************
// ! HumanCommandFullBodyDamage - full body damages
// *************************************************************************************
class HumanCommandFullBodyDamage
{	
	// nothing here for now 
}

//! actions
enum WeaponActions
{
	NONE                = -1,
	RELOAD 				= 0,
	MECHANISM			= 1,
	CHAMBERING 			= 2,
	CHAMBERING_LOADER 	= 3,
	UNJAMMING			= 4,
	FIRE 				= 5,
};
//! action types
/// reload action types
enum WeaponActionReloadTypes
{
	//! reload action types - rifles
	RELOADRIFLE_MAGAZINE_BULLET 			= 0,   		// CMD_Reload_Magazine
	RELOADRIFLE_NOMAGAZINE_BULLET 			= 1,
	RELOADRIFLE_MAGAZINE_NOBULLET 			= 2,
	RELOADRIFLE_NOMAGAZINE_NOBULLET 		= 3,
	RELOADRIFLE_MAGAZINE_NOBULLET_OPEN		= 4,
	RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN	= 5,

	// reload action types - pistols
	RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_UNCOCKED	= 8, // CMD_Reload_Magazine	
	RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_UNCOCKED		= 9,
	RELOADPISTOL_MAGAZINE_BULLET_CLOSED					= 10,   		
	RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED 				= 11,
	RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_COCKED 		= 12,
	RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_COCKED 		= 13,
	RELOADPISTOL_NOMAGAZINE_NOBULLET_OPENED 			= 14,
	RELOADPISTOL_MAGAZINE_NOBULLET_OPENED 				= 15,

	RELOAD_MAGAZINE_DETACH 	= 16,
	RELOADRIFLE_MAGAZINE_DETACH = 17,                    // CMD_Reload_Magazine

	RELOADSRIFLE_MAGAZINE_BULLET 		= 20,   		// CMD_Reload_Magazine
	RELOADSRIFLE_NOMAGAZINE_BULLET 		= 21,
	RELOADSRIFLE_MAGAZINE_NOBULLET 		= 22,
	RELOADSRIFLE_NOMAGAZINE_NOBULLET 	= 23,
};

enum WeaponActionMechanismTypes
{
	//! mechanism action types
	MECHANISM_OPENED 	= 0,		// CMD_Reload_BoltAction
	MECHANISM_CLOSED 	= 1,		
	MECHANISM_SPECIAL 	= 2,	    // crossbow, magnum ... special mechanism action apropriate for custom weapons
    MECHANISM_CLOSED_UNCOCKED 	= 3,	
};

enum WeaponActionChamberingTypes
{
	//! chambering action types
	CHAMBERING_ONEBULLET_OPENED 					= 0,		// CMD_Reload_Chambering
	CHAMBERING_ONEBULLET_CLOSED  					= 1,		//
	CHAMBERING_ONEBULLET_CLOSED_UNCOCKED 	= 2,
	CHAMBERING_ONEBULLET_UNIQUE_OPENED 		= 3,		//
	CHAMBERING_ONEBULLET_UNIQUE_CLOSED		= 4,		//
	CHAMBERING_TWOBULLETS_START						= 6,		//  plays one bullet, then second, then ends, when CHAMBERING_TWOBULLETS_END arise, it's canceled
	CHAMBERING_TWOBULLETS_END							= 7,		//  - one bullet reload with closed mechanism		
	CHAMBERING_STARTLOOPABLE_OPENED				= 10,		// start loop chambering
	CHAMBERING_ENDLOOPABLE								= 11,		// end loop chambering
	CHAMBERING_STARTLOOPABLE_CLOSED				= 12,		// start loop chambering

	CHAMBERING_CROSSBOW_OPENED			= 20,		// chambering crossbow
	CHAMBERING_CROSSBOW_CLOSED			= 21,		// chambering crossbow
	CHAMBERING_CROSSBOW_FULL			= 22,		// chambering crossbow
};

enum WeaponActionChamberingLoaderTypes
{
	CHAMBERINGLOADER_OPENED		= 0,		// CMD_Reload_ChamberingFast - one bullet reload with open mechanism
	CHAMBERINGLOADER_CLOSED 	= 1,		//  - one bullet reload with closed mechanism
};

enum WeaponActionUnjammingTypes
{
	//! unjam action types
	UNJAMMING_START 					= 1,		// CMD_Weapon_Jam - 0
	UNJAMMING_END 						= 0,		// 1
};

enum WeaponActionFireTypes
{
	//!	fire action types
	FIRE_NORMAL 						= 0,		//!
	FIRE_LAST 							= 1,		//!
	FIRE_COCKED 						= 2,		//!
	FIRE_UNCOCKED 						= 3,		//!
	FIRE_DRY							= 4,
	FIRE_JAM							= 5
};

string WeaponActionTypeToString (int A, int AT)
{
	switch (A)
	{
		case WeaponActions.NONE: return "---";
		case WeaponActions.RELOAD: return typename.EnumToString(WeaponActionReloadTypes, AT);
		case WeaponActions.MECHANISM: return typename.EnumToString(WeaponActionMechanismTypes, AT);
		case WeaponActions.CHAMBERING: return typename.EnumToString(WeaponActionChamberingTypes, AT);
		case WeaponActions.CHAMBERING_LOADER: return typename.EnumToString(WeaponActionChamberingLoaderTypes, AT);
		case WeaponActions.UNJAMMING: return typename.EnumToString(WeaponActionUnjammingTypes, AT);
		case WeaponActions.FIRE: return typename.EnumToString(WeaponActionFireTypes, AT);
	}
	return "---";
}

//! events
enum WeaponEvents
{
	ATTACHMENT_HIDE,
	ATTACHMENT_SHOW,
	BULLET_EJECT,
	BULLET_HIDE,
	BULLET_HIDE2,
	BULLET_IN_CHAMBER,
	BULLET_IN_MAGAZINE,
	BULLET_SHOW,
	BULLET_SHOW2,
	CANUNJAM_END,
	CANUNJAM_START,
	COCKED,
	MAGAZINE_ATTACHED,
	MAGAZINE_DETACHED,
	MAGAZINE_HIDE,
	MAGAZINE_SHOW,
	SLIDER_OPEN,
	UNJAMMED,
	HAMMER_UNCOCKED,
	HAMMER_COCKED
};


// *************************************************************************************
// ! HumanCommandWeapons - weapon handling
// *************************************************************************************
class HumanCommandWeapons
{	
	//!
    proto native 	bool		IsActionFinished();
	
	//! returns -1 when no action is running or RELOAD,MECHANISM, .... 
    proto native 	int 		GetRunningAction();

	//! returns -1 when no action is running or appropriate action type
    proto native 	int 		GetRunningActionType();

	//! start reload,mechanism,chambering,unjam ... 
	proto native 	bool		StartAction(WeaponActions pAction, int pActionType);

	//! register events 
	proto native    void    	RegisterEvent(string pName, int pId);

	//! return -1 when there is no event, otherwise it returns pId of event from animation 
	proto native 	int 		IsEvent();

	//!
	void 	RegisterDefaultEvents()
	{
		RegisterEvent("Weapon_AttachmentHide", WeaponEvents.ATTACHMENT_HIDE);
		RegisterEvent("Weapon_AttachmentShow", WeaponEvents.ATTACHMENT_SHOW);
		RegisterEvent("Weapon_BulletEject", WeaponEvents.BULLET_EJECT);
		RegisterEvent("Weapon_BulletHide", WeaponEvents.BULLET_HIDE);
		RegisterEvent("Weapon_BulletHide2", WeaponEvents.BULLET_HIDE2);
		RegisterEvent("Weapon_BulletInChamber", WeaponEvents.BULLET_IN_CHAMBER);
		RegisterEvent("Weapon_BulletInMagazine", WeaponEvents.BULLET_IN_MAGAZINE);
		RegisterEvent("Weapon_BulletShow", WeaponEvents.BULLET_SHOW);
		RegisterEvent("Weapon_BulletShow2", WeaponEvents.BULLET_SHOW2);
		RegisterEvent("Weapon_CanUnjam_End", WeaponEvents.CANUNJAM_END);
		RegisterEvent("Weapon_CanUnjam_Start", WeaponEvents.CANUNJAM_START);
		RegisterEvent("Weapon_Cocked", WeaponEvents.COCKED);
		RegisterEvent("Weapon_MagazineAttached", WeaponEvents.MAGAZINE_ATTACHED);
		RegisterEvent("Weapon_MagazineDetached", WeaponEvents.MAGAZINE_DETACHED);
		RegisterEvent("Weapon_MagazineHide", WeaponEvents.MAGAZINE_HIDE);
		RegisterEvent("Weapon_MagazineShow", WeaponEvents.MAGAZINE_SHOW);
		RegisterEvent("Weapon_SliderOpen", WeaponEvents.SLIDER_OPEN);
		RegisterEvent("Weapon_Unjammed", WeaponEvents.UNJAMMED);
		RegisterEvent("Weapon_Hammer_Uncocked", WeaponEvents.HAMMER_UNCOCKED);
		RegisterEvent("Weapon_Hammer_Cocked", WeaponEvents.HAMMER_COCKED);
	}

	//----------------------------------------------------
	// weapon init states


	//! this sets to a weapon init state 
	//! each weapon has a different weapon state 
	//! those states are defined by animation set by AddItemInHandsProfileIK - in DayZPlayerCfgBase.c
	//! each frame of animation is one init state 
	proto native 	void 		SetInitState(int pFrameIndex);

	//! just static version of SetInitState
	proto native static void 	StaticSetInitState(Human pHuman, int pFrameIdx);


	//----------------------------------------------------
	// debug copy 

	//! return -1 when there is no event, otherwise it returns pId of event from animation 
	proto native 	int 		DebugIsEvent();

	//! 
	proto native 	void 		DebugResetEvents();
}


// *************************************************************************************
// ! HumanCommandAdditives - additional damage 
// *************************************************************************************
class HumanCommandAdditives
{	

	//! sets injury level 0..1, interpolate == false -> resets the value, otherwise it's interpolating towards the new value
	proto native void 	SetInjured(float pValue, bool pInterpolate);

	//! sets exhaustion level 0..1, interpolate == false -> resets the value, otherwise it's interpolating towards the new value
	proto native void 	SetExhaustion(float pValue, bool pInterpolate);
}


// *************************************************************************************
// ! HumanMovementState - movement state, command, stance, movement, human.GetMovementState(movementState)
// *************************************************************************************
class HumanMovementState
{
	int 		m_CommandTypeId;	//! current command's id 
	int 		m_iStanceIdx;		//! current stance (DayZPlayer.STANCEIDX_ERECT, ...), only if the command has a stance
	int 		m_iMovement;		//! current movement (0 idle, 1 walk, 2-run, 3-sprint), only if the command has a movement 

	//! 
	bool		IsRaised()
	{
		return m_iStanceIdx >= DayZPlayerConstants.STANCEIDX_RAISEDERECT;
	}
}



// *************************************************************************************
// ! Human - human script interface 
// *************************************************************************************
class Human extends Man
{

	//! gets human transform in World Space
	proto native	void 		GetTransformWS(out vector pTm[4]);


	//---------------------------------------------------------
	// bone transforms 

	//! returns bone index for a name (-1 if pBoneName doesn't exist)
	proto native 	int 		GetBoneIndexByName(string pBoneName);

	//! returns local space, model space, world space position of the bone 
	proto native	vector		GetBonePositionLS(int pBoneIndex);
	proto native 	vector		GetBonePositionMS(int pBoneIndex);
	proto native 	vector		GetBonePositionWS(int pBoneIndex);

	//! returns local space, model space, world space orientation (quaternion) of a bone 
	proto native	void 		GetBoneRotationLS(int pBoneIndex, out float pQuat[4]);
	proto native 	void 		GetBoneRotationMS(int pBoneIndex, out float pQuat[4]);
	proto native 	void 		GetBoneRotationWS(int pBoneIndex, out float pQuat[4]);

	//! returns local space, model space, world space orientation (quaternion) of a bone 
	proto native	void 		GetBoneTransformLS(int pBoneIndex, out vector pTm[4]);
	proto native 	void 		GetBoneTransformMS(int pBoneIndex, out vector pTm[4]);
	proto native 	void 		GetBoneTransformWS(int pBoneIndex, out vector pTm[4]);


	//---------------------------------------------------------
	// physic props
	
	//! returns true if physics controller is falling 
	proto native 	bool		PhysicsIsFalling();

	//! outs pVelocity - linear velocity of PHYSICS CONTROLLER 
	proto native 	void 		PhysicsGetVelocity(out vector pVelocity);

	//---------------------------------------------------------
	// controller 
			
	//! returns human input controller
	proto native 	HumanInputController	GetInputController();

	//---------------------------------------------------------
	// item accessor

	proto native 	HumanItemAccessor		GetItemAccessor();


	//---------------------------------------------------------
	// commands 

	//! returns movement state (current command id, )
	proto native 	void 	GetMovementState(HumanMovementState pState);

	//! returns current command ID
	// proto native 	int 	GetCurrentCommandID();


	//!----- MOVE -----

	//! starts command - Move
	proto native 	HumanCommandMove			StartCommand_Move();

	proto native 	HumanCommandMove			GetCommand_Move();

	//!----- MELEE -----

	//! starts command - melee
	proto native 	HumanCommandMelee			StartCommand_Melee(EntityAI pTarget);

	proto native 	HumanCommandMelee			GetCommand_Melee();

	//! starts command - melee
	proto native 	HumanCommandMelee2			StartCommand_Melee2(EntityAI pTarget, bool pHeavyHit);

	proto native 	HumanCommandMelee2			GetCommand_Melee2();


	//!----- FALL -----

	//! starts command - fall
	//! pYVelocity <= 0 -> fall
	//! pYVelocity > 0 -> jump
	proto native 	HumanCommandFall			StartCommand_Fall(float pYVelocity);

	//! 
	proto native 	HumanCommandFall			GetCommand_Fall();


	//!----- LADDER -----

	//! starts command - fall
	proto native 	HumanCommandLadder 			StartCommand_Ladder(Building pBuilding, int pLadderIndex);

	proto native 	HumanCommandLadder 			GetCommand_Ladder();


	//!----- LADDER -----

	//! starts command - unconscious
	proto native 	HumanCommandSwim			StartCommand_Swim();

	proto native 	HumanCommandSwim 			GetCommand_Swim();


	//!----- Death -----

	//! starts command - death
	proto native 	HumanCommandDeath			StartCommand_Death();

	proto native 	HumanCommandDeath			GetCommand_Death();


	//! starts command - unconscious
	proto native 	HumanCommandUnconscious 	StartCommand_Unconscious();

	proto native 	HumanCommandUnconscious		GetCommand_Unconscious();


	//!----- FullBody Damages -----

	//! starts fullbody damage 
	// -> of type 0,...
	// -> from direction (lback -180,left -90,0,righ 90,rightback 180)
	proto native 	HumanCommandFullBodyDamage	StartCommand_Damage(int pType, float pDirection);

	proto native 	HumanCommandFullBodyDamage	GetCommand_Damage();



	//!----- ACTIONS ----- 

	//! starts command - Action 
	//! pStanceMask is mix of flags STANCEMASK_ERECT, ... 
	proto native 	HumanCommandActionCallback 	StartCommand_Action(int pActionID, typename pCallbackClass, int pStanceMask);

	//! is human is in command action - returns its callback, if current command is action 
	proto native	HumanCommandActionCallback 	GetCommand_Action();

	
	//---------------------------------------------------------
	// command modifiers

	//!
	proto native 	int 		GetCommandModifierCount();

	//! returns COMMANDID_ .. type id of command modifier on index pIndex
	proto native 	int 		GetCommandModifierID(int pIndex);	


	//! default (always-on modifiers)

	//! returns modifier for additives / small behaviours on human 
	proto native 	HumanCommandAdditives		GetCommandModifier_Additives();

	//! returns interface for handling weapons 
	proto native 	HumanCommandWeapons			GetCommandModifier_Weapons();




	// --- modifier/additive actions - played on COMMANDID_MOVE command

	//! adds action command modifier, creates callback instance for you 
	proto native 	HumanCommandActionCallback	AddCommandModifier_Action(int pActionID, typename pCallbackClass);

	//! force remove - normally if action is ended or interrupted - this is not needed to call 
	proto native 	void 						DeleteCommandModifier_Action(HumanCommandActionCallback pCallback);

	//! returns callback for action if any is active, null if nothing runs 
	proto native 	HumanCommandActionCallback	GetCommandModifier_Action();



	//!--- modifier for light Damages 

	//! starts additive damage 
	// -> of type 0,...
	// -> from direction (lback -180,left -90,0,righ 90,rightback 180)
	proto native 	HumanCommandDamage			AddCommandModifier_Damage(int pType, float pDirection);

	proto native 	void 						DeleteCommandModifier_Damage(HumanCommandDamage pDamage);

	proto native 	HumanCommandDamage			GetCommandModifier_Damage();






	//---------------------------------------------------------
	// debug info for Item Accessor


	//! returns current item's class name
	proto native   string  						DebugGetItemClass();

	//! returns current item's class that is found in config
	proto native   string  						DebugGetItemSuperClass();

	//! returns current item's animation instance 
	proto native   string  						DebugGetItemAnimInstance();


}
