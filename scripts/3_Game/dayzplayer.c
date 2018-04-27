/** @file 

  this file is interface DayZPlayer classes 

*/


// *************************************************************************************
// ! DayZPlayerCameraResult - camera result -
// *************************************************************************************
class DayZPlayerCameraResult
{

	vector		m_CameraTM[4];			//!< transformation matrix - pos + orient of the camera
	float 		m_fFovMultiplier;		//!< fov multiplier - 1.0 default - used for modifying fov - 
	float 		m_fFovAbsolute;			//!< fov absolute value override - -1.0 default, if set - overrides absolute fov setting
	float 		m_fPositionModelSpace;  //!< 0.0 position is in heading space, 1.0 position is in model space
	float 		m_fDistance;			//!< camera distance (external cameras only)
	float 		m_fUseHeading;			//!< 0..1 (0 uses direct dir returned, 1 uses heading from player)
	
	int 		m_iDirectBone;			//!< -1 no bone, >= 0 - bone index camera is bound to, m_CameraTM is offset to the bone 
	int 		m_iDirectBoneMode;		//! 0 not used, 1 - pos, 2 - rot, 3 - pos+rot applied as a parent to m_CameraTM, 4 as 3 but cam aligned with Y
	float 		m_fInsideCamera;		//!< 0..1 (0 normal lod, 1 inside lod), >0.7 -> inside
	bool		m_bUpdateWhenBlendOut;	//!< true - camera is updated when blending to new camera (Ironsights == false)
	float 		m_fShootFromCamera;		//!< 1(default) - uses shoot from camera (+aiming sway), 0 pure weapon shoot (ironsights == 0)


	//! cannot be instanced from script (always created from C++)
	private void DayZPlayerCameraResult()
	{
	}

	//! cannot be instanced from script (always created from C++)
	private void ~DayZPlayerCameraResult()
	{
	}

}

// *************************************************************************************
// ! DayZPlayerCamera - main dayz player camera implement 
// *************************************************************************************
class DayZPlayerCamera 
{

	void DayZPlayerCamera(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_pPlayer		= pPlayer;
		m_pInput 		= pInput;
	}

	//! this overrides freelok for cameras
	bool		CanFreeLook()
	{
		return true;
	}

	//! virtual callback - called when camera is created
	void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
	}

	//!	virtual callback - called each frame
	void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
	}

	//!	helper to blend between cameras 
	//! ret[0] - up down angle
	//! ret[1] - left right angle
	//! ret[2] - roll
	vector 		GetBaseAngles()
	{
		return	"0 0 0";
	}
	
	//! data 
	protected 	DayZPlayer 				m_pPlayer;		//!< player camera is attached to
	protected 	HumanInputController	m_pInput;		//!< human input 
}


// *************************************************************************************
// ! DayZPlayerTypeStepSoundLookupTable - virtual 
// *************************************************************************************
class DayZPlayerTypeStepSoundLookupTable
{
	SoundObject GetSoundObject(int eventId, int pMovement, int pSurfaceHash, AnimBootsType pBoots)
	{
		return null;
	}
}

// *************************************************************************************
// ! DayZPlayerTypeAttachmentSoundLookupTable - virtual 
// *************************************************************************************
class DayZPlayerTypeAttachmentSoundLookupTable
{
	SoundObject GetSoundObject(int eventId, string slotName, int attachmentHash)
	{
		return null;
	}
}


// *************************************************************************************
// ! DayZPlayerType - DayZPlayer configuration 
// *************************************************************************************

//! class that configures static data for DayZPlayer
//! on each mission start DayZPlayerTypeCreate(DayZPlayerType pType) is called - can call methods

class DayZPlayerType
{
	//!----------------------------------------------------
	// item in hands config

	//! 
	proto native 	void SetDefaultItemInHandsProfile(string pAnimInstanceName, HumanItemBehaviorCfg pBehaviorCfg);


	//! reset profiles 
	proto native 	void ResetItemInHandsProfiles();

	//! adds setup profile for items of class pItemClass
	//! when item is in hands - pAnimInstanceName is used for animation sourcing 
	//! pCanRaise is whether weapon / item can be raised
	//! pRHandIK == false -> rhand ik is always turned off
	//! pLHandIK == false -> lhand ik is always turned off

	//! VKOSTIK: DO NOT FORGET !!! HumanItemBehaviorCfg.m_iPerItemCameraUD - per item camera user data - enables you setup different cameras for different items in hands

	//!
	proto native 	int AddItemInHandsProfile(string pItemClass, string pAnimInstanceName, HumanItemBehaviorCfg pBehaviorCfg);

	//!
	proto native 	int AddItemInHandsProfileIK(string pItemClass, string pAnimInstanceName, HumanItemBehaviorCfg pBehaviorCfg, string pIkPoseAnim, string pWeaponStates = "");

	//! add bone remap for item class
	//! pBoneRemap has always 2x N members
	//! bone in item's P3D first, bone in Character skeleton second

	//! array<string> remap = { "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "trigger", "Weapon_Trigger" };
	//! AddItemBoneRemap("class", remap);

	proto native 	int AddItemBoneRemap(string pItemClass, array<string> pBoneRemap);


	//! debug / hack - loads all animation instances
	proto native 	void DebugPreloadAnimationInstances();



	//!----------------------------------------------------
	// camera creators

	//! resets camera creators
	proto native 	void ResetCameraCreators();

	//! registers camera creator - camera type => camera type (must be descendant of DayZPlayerCamera)
	proto native 	void RegisterCameraCreator(int pCameraID, typename pCameraType);

	//! registers camera on change function - static function 
	proto native 	void RegisterCameraOnChangeFnStatic(typename pClass, string pFnName);

	//! registers camera on change function - 
	proto native 	void RegisterCameraOnChangeFn(Class pInstance, string pFnName);


	//!----------------------------------------------------
	// global options

	//! sets look limits for a player
	proto native 	void SetLookLimits(float pDown, float pUp, float pLeft, float pRight);

	//! sets aim limits for a player
	proto native 	void SetAimLimits(float pDown, float pUp, float pLeft, float pRight);

	//! sets aim limits for a player
	proto native 	void SetCameraShootParams(float pRayDistance, float pRayRadius);



	//!----------------------------------------------------
	// event handling

	//! calls DayZPlayer.OnStepEvent();
	proto native	void RegisterStepEvent(string pEventType, float pFilterTimeout);

	//! calls DayZPlayer.OnSoundEvent();
	proto native	void RegisterSoundEvent(string pEventType, float pFilterTimeout);

	//! calls DayZPlayer.OnParticleEvent();
	proto native	void RegisterParticleEvent(string pEventType, float pFilterTimeout);


	void 				 RegisterStepSoundLookupTable(DayZPlayerTypeStepSoundLookupTable pSSLUT)
	{
		m_pStepSoundLookupTable = pSSLUT;
	}
	
	DayZPlayerTypeStepSoundLookupTable	GetStepSoundLookupTable()
	{
		return m_pStepSoundLookupTable;
	}
	
	void 				 RegisterAttachmentSoundLookupTable(DayZPlayerTypeAttachmentSoundLookupTable pASLUT)
	{
		m_pAttachmentSoundLookupTable = pASLUT;
	}
	
	DayZPlayerTypeAttachmentSoundLookupTable	GetAttachmentSoundLookupTable()
	{
		return m_pAttachmentSoundLookupTable;
	}
	
	NoiseParams GetNoiseParamsStand()
	{
		return m_pNoiseStepStand;
	}
	
	NoiseParams GetNoiseParamsCrouch()
	{
		return m_pNoiseStepCrouch;
	}
	
	NoiseParams GetNoiseParamsProne()
	{
		return m_pNoiseStepProne;
	}
	
	void LoadSoundWeaponEvent()
	{
		string cfgPath = "CfgVehicles SurvivorBase AnimEvents SoundWeapon ";
		
		m_animSoundEventsAttack = new array<ref AnimSoundEvent>;

		int soundCount = GetGame().ConfigGetChildrenCount(cfgPath);
		for(int i = 0; i < soundCount; i++)
		{
			string soundName;
			GetGame().ConfigGetChildName(cfgPath, i, soundName);
			string soundPath = cfgPath + soundName + " ";
			AnimSoundEvent soundEvent = new AnimSoundEvent(soundPath);
			m_animSoundEventsAttack.Insert(soundEvent);
		}
	}
	
	AnimSoundEvent GetSoundWeaponEvent(int event_id)
	{
		for(int i = 0; i < m_animSoundEventsAttack.Count(); i++)
		{
			AnimSoundEvent soundEvent = m_animSoundEventsAttack.Get(i);
			if(soundEvent.m_iID == event_id)
			{
				return soundEvent;
			}
		}

		return NULL;
	}
	
	private void DayZPlayerType()
	{
		string cfgPath = "CfgVehicles SurvivorBase ";
	
		m_pNoiseStepStand = new NoiseParams();
		m_pNoiseStepStand.LoadFromPath(cfgPath + "NoiseStepStand");
		
		m_pNoiseStepCrouch = new NoiseParams();
		m_pNoiseStepCrouch.LoadFromPath(cfgPath + "NoiseStepCrouch");
		
		m_pNoiseStepProne = new NoiseParams();
		m_pNoiseStepProne.LoadFromPath(cfgPath + "NoiseStepProne");
		
		
		LoadSoundWeaponEvent();
	}
	
	private void ~DayZPlayerType();

	//!----------------------------------------------------
	// global settings

	//! returns Human global settings for DayZPlayer
	proto native 	SHumanGlobalSettings 		GetGlobalSettingsW();


	//!----------------------------------------------------
	// command configs

	// returns command move setting for write - in init phase
	proto native 	SHumanCommandMoveSettings	CommandMoveSettingsW();

	// returns command move setting for write - in init phase
	proto native 	SHumanCommandSwimSettings	CommandSwimSettingsW();


	//!< this is used for getting appropriate sound effect to play step event on various surfaces && boots
	//!< it's downcasted to StepSoundLookupTable
	ref DayZPlayerTypeStepSoundLookupTable m_pStepSoundLookupTable;
	ref DayZPlayerTypeAttachmentSoundLookupTable m_pAttachmentSoundLookupTable;
	ref NoiseParams m_pNoiseStepStand;
	ref NoiseParams m_pNoiseStepCrouch;
	ref NoiseParams m_pNoiseStepProne;
	
	ref array<ref AnimSoundEvent> m_animSoundEventsAttack;
}


// *************************************************************************************
// ! DayZPlayerConstants - dayz player constants 
// *************************************************************************************
//! defined in C++
enum DayZPlayerConstants
{
    //! ---------------------------------------------------------
    //! ---------------------- movement stances -----------------
    //! ---------------------------------------------------------
	DEBUG_MENU,				//!< dayz player debug menu in enfusion 
	DEBUG_TURNSLIDE,		//!< over turn slides / clamps
	DEBUG_SHOWDEBUGPLUGIN,  //!< menu for showing debug plugin
	DEBUG_SHOWINJURY,		//!< menu for showing injuries
	DEBUG_SHOWEXHAUSTION,	//!< menu for showing exhaustion
	DEBUG_ENABLEJUMP,		//!< menu for showing exhaustion
	

    //! ---------------------------------------------------------
    //! ---------------------- movement stances -----------------
    //! ---------------------------------------------------------

	STANCEIDX_ERECT,
	STANCEIDX_CROUCH,
	STANCEIDX_PRONE,
	STANCEIDX_RAISEDERECT,
	STANCEIDX_RAISEDCROUCH,
	STANCEIDX_RAISEDPRONE,


	//! stance masks 
	STANCEMASK_ERECT,
	STANCEMASK_CROUCH,
	STANCEMASK_PRONE,
	STANCEMASK_RAISEDERECT,
	STANCEMASK_RAISEDCROUCH,
	STANCEMASK_RAISEDPRONE,
	STANCEMASK_ALL,		// STANCEMASK_ERECT | STANCEMASK_CROUCH | STANCEMASK_PRONE | STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE
	STANCEMASK_NOTRAISED 	= STANCEMASK_ERECT | STANCEMASK_CROUCH | STANCEMASK_PRONE,
	STANCEMASK_RAISED 		= STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE,


	//! movements - masks 
	MOVEMENT_IDLE,  			//! 0x1
	MOVEMENT_WALK,				//! 0x2
	MOVEMENT_RUN,				//! 0x4
	MOVEMENT_SPRINT,			//! 0x8 

	//! movement idx
	MOVEMENTIDX_IDLE	= 0,
	MOVEMENTIDX_WALK	= 1,
	MOVEMENTIDX_RUN		= 2,
	MOVEMENTIDX_SPRINT  = 3,


    //! ---------------------------------------------------------
    //! ---------------------- COMMAND IDs ----------------------
    //! ---------------------------------------------------------
	
	//! command ids - main movement commands 
	COMMANDID_NONE,    	// type is int - no command - invalid state 
	COMMANDID_MOVE,     // type is int (overridden from C++) - normal movement (idle, walk, run, sprint, ... )
	COMMANDID_ACTION,	// type is int (overridden from C++) - full body action
	COMMANDID_MELEE,	// type is int (overridden from C++) - melee attacks
	COMMANDID_MELEE2,	// type is int (overridden from C++) - melee attacks
	COMMANDID_FALL,		// type is int (overridden from C++) - falling
	COMMANDID_DEATH,	// type is int (overridden from C++) - dead 
	COMMANDID_DAMAGE,	// type is int (overridden from C++) - fullbody damage
	COMMANDID_LADDER,	// type is int (overridden from C++) - ladder
	COMMANDID_UNCONSCIOUS,	// type is int (overridden from C++) - unconscious
	COMMANDID_SWIM,			// type is int (overridden from C++) - swimming


	//! modifier commands - additive behaviour 
	COMMANDID_MOD_LOOKAT,    	// look at - always on 
	COMMANDID_MOD_WEAPONS,   	// weapons - always on 
    COMMANDID_MOD_ACTION,		// action - additive action 
	COMMANDID_MOD_DAMAGE,		// damage - additive damage 




    //! ---------------------------------------------------------
    //! ---------------------- ACTIONS --------------------------
    //! ---------------------------------------------------------

	//! internal action commands used in HumanCommandActionCallback.InternalCommand()
	CMD_ACTIONINT_END2			= -1,	//!< secondary ending
	CMD_ACTIONINT_END			= 0, 	//!< end action
	CMD_ACTIONINT_ACTION		= 1, 	//!< one time secondary action within an action
	CMD_ACTIONINT_ACTIONLOOP	= 2, 	//!< loop secondary action within an action


	//! animations constants 

	//! note: (erc - standing, cro - crouching, pne - prone)    	anim pose it can start from
	//! note: (end, end2, action, loop2)							internal command which can be send in loop state 

	//! looping 	
		
    //! --------------------------
    //! modifier (differential animation)
    CMD_ACTIONMOD_DRINK = 0,					// erc,cro    		[end, end2]
	CMD_ACTIONMOD_EAT = 1,						// erc,cro    		[end, end2]
	CMD_ACTIONMOD_EMPTYCANISTER = 2,			// erc,cro          [end]
	CMD_ACTIONMOD_CATCHRAIN = 3,				// erc,cro			[end]
	CMD_ACTIONMOD_EATPILLS = 4,					// erc,cro			[end]
	CMD_ACTIONMOD_EATTABLETS = 5,				// erc,cro   	    [end]
	CMD_ACTIONMOD_EMPTYBOTTLE = 6,				// erc,cro          [end]
	CMD_ACTIONMOD_OPENCOMPASS = 7,				// erc              [end]
	CMD_ACTIONMOD_DRINKCAN = 8,					// erc,cro			[end]
	CMD_ACTIONMOD_WALKIETALKIETUNE = 9,			// erc,cro			[end]
	CMD_ACTIONMOD_GIVEL = 10,					// erc,cro			[end]
	CMD_ACTIONMOD_GIVER = 11,					// erc,cro			[end]
	CMD_ACTIONMOD_SHAVE = 12,					// erc,cro			[end]
	CMD_ACTIONMOD_FILLMAG = 13,					// erc,cro			[end]
	CMD_ACTIONMOD_EMPTYMAG = 14,				// erc,cro			[end]
	CMD_ACTIONMOD_DRINKPOT = 15,				// erc,cro			[end]
	CMD_ACTIONMOD_EMPTYPOT = 16,				// erc,cro			[end]
	CMD_ACTIONMOD_EATFRUIT = 17,				// erc,cro			[end]
	
	// onetime 
	CMD_ACTIONMOD_LICKBATTERY = 100,    		// erc,cro,pne
	CMD_ACTIONMOD_PICKUP = 101,         		// erc,cro,pne
	CMD_ACTIONMOD_LIGHTFLARE = 102,     		// erc,cro,pne
	CMD_ACTIONMOD_LITCHEMLIGHT = 103,   		// erc,cro,pne
	CMD_ACTIONMOD_UNPINGRENAGE = 104,   		// erc,cro,pne
	CMD_ACTIONMOD_OPENDOORFW = 105,     		// erc,cro
	CMD_ACTIONMOD_OPENLID = 106,        		// erc,cro 
	CMD_ACTIONMOD_CLOSELID = 107,       		// erc,cro 	
	CMD_ACTIONMOD_WALKIETALKIEON = 108, 		// erc,cro,pne
	CMD_ACTIONMOD_WALKIETALKIEOFF = 109,		// erc,cro,pne
	CMD_ACTIONMOD_BATONEXTEND = 110,    		// erc,cro
	CMD_ACTIONMOD_BATONRETRACT = 111,   		// erc,cro
	CMD_ACTIONMOD_UNLOCKHANDCUFFTARGET = 112,   // erc,cro
	CMD_ACTIONMOD_FISHINGRODEXTEND = 113,   	// erc,cro
	CMD_ACTIONMOD_FISHINGRODRETRACT = 114,   	// erc,cro
	CMD_ACTIONMOD_TAKEITEM = 115,   			// erc,cro,pne
	CMD_ACTIONMOD_HIDEITEM = 116,   			// erc,cro,pne

    //! --------------------------
    //! fb (full body)
    CMD_ACTIONFB_DRINK = 0,						// pne				[end, end2]
	CMD_ACTIONFB_EAT = 1,						// pne				[end, end2]
	CMD_ACTIONFB_CATCHRAIN = 3,					// pne				[end]
	CMD_ACTIONFB_EATPILLS = 4,					// pne				[end]
	CMD_ACTIONFB_EATTABLETS = 5,				// pne				[end]
	CMD_ACTIONFB_DRINKCAN = 8,					// pne				[end]
	CMD_ACTIONFB_WALKIETALKIETUNE = 9,			// pne				[end]
	CMD_ACTIONFB_GIVEL = 10,					// pne				[end]
	CMD_ACTIONFB_GIVER = 11,					// pne				[end]
	CMD_ACTIONFB_FILLMAG = 13,					// pne				[end]
	CMD_ACTIONFB_EMPTYMAG = 14,					// pne				[end]
	CMD_ACTIONFB_DRINKPOND = 50,	        	// cro 	      	    [end]
	CMD_ACTIONFB_DRINKWELL = 51,         		// cro 	            [end]
	CMD_ACTIONFB_FILLBOTTLEWELL = 52,    		// cro	            [end]
	CMD_ACTIONFB_FIREESTINGUISHER = 53,  		// erc              [end]
	CMD_ACTIONFB_WRING = 54,   		    		// cro 		        [end]
	CMD_ACTIONFB_BERRIES = 55,           		// erc,cro		    [end]
	CMD_ACTIONFB_CHOPTREE = 56,          		// erc			    [end]
	CMD_ACTIONFB_DIGSHOVEL = 57,         		// erc		        [end]
	CMD_ACTIONFB_DIGHOE = 58,            		// erc			    [end]
	CMD_ACTIONFB_FISHING = 59,           		// cro			    [action, end(catch fish), end2(not catching anything) ]
	CMD_ACTIONFB_CPR = 60,              		// cro     		    [end]
	CMD_ACTIONFB_BANDAGE = 61,          		// cro		        [end]
	CMD_ACTIONFB_CRAFTING = 62,         		// cro		        [end]
	CMD_ACTIONFB_INTERRACTPLAYER = 63,  		// cro 	            [end]
	CMD_ACTIONFB_DRINKCANISTER = 64, 			// erc,cro          [end]
	CMD_ACTIONFB_FORCEFEED = 65, 	    		// erc,cro          [end]
	CMD_ACTIONFB_BANDAGETARGET = 66, 			// erc,cro          [end]
	CMD_ACTIONFB_DISINFECTTARGET = 67, 			// cro     		    [end]
	CMD_ACTIONFB_DEPLOY = 68,           		// cro			    [end]
	CMD_ACTIONFB_STARTFIREMATCH = 69,   		// cro			    [end]
	CMD_ACTIONFB_ANIMALSKINNING = 70,   		// cro				[end]
	CMD_ACTIONFB_WASHHANDSWELL = 71,			// cro				[end]
	CMD_ACTIONFB_WASHHANDSPOND = 72,    		// cro				[end]
	CMD_ACTIONFB_SALINEBLOODBAGTARGET = 73,  	// erc,cro			[end]
	CMD_ACTIONFB_SALINEBLOODBAG = 74,  			// erc,cro			[end]
	CMD_ACTIONFB_STITCHUPSELF = 75,   			// erc,cro			[end]
	CMD_ACTIONFB_VOMIT = 76,            		// cro				[end]
	CMD_ACTIONFB_CUTTIESTARGET = 77,    		// erc,cro			[end (finish cutting), end2 (cancel cutting)]
	CMD_ACTIONFB_TIEUPTARGET = 78,  			// erc,cro			[end (finish tying up), end2 (cancel tying up)]
	CMD_ACTIONFB_STARTFIRETORCH = 79,			// erc,cro			[end]
	CMD_ACTIONFB_CHECKPULSE = 80,				// cro				[end]
	CMD_ACTIONFB_EMPTYSEEDSPACK = 81,			// erc, cro			[end]
	CMD_ACTIONFB_CLEANWOUNDSTARGET = 82,		// erc, cro			[end]
	CMD_ACTIONFB_STARTFIREDRILL = 83,			// erc, cro			[end]
	CMD_ACTIONFB_STARTFIRELIGHTER = 84,			// erc, cro			[end]
	CMD_ACTIONFB_COLLECTBLOODSELF = 85,			// erc, cro			[end]
	CMD_ACTIONFB_POURBOTTLE = 86,				// erc, cro			[end]
	CMD_ACTIONFB_INTERACTITEM = 87,				// cro				[end]
	CMD_ACTIONFB_POURCAN = 88,					// erc, cro			[end]
	CMD_ACTIONFB_EATFRUIT = 89,					// pne				[end]
	
	// onetime 
	CMD_ACTIONFB_LICKBATTERY = 100,    			// pne
	CMD_ACTIONFB_PICKUP = 101,         			// pne
	CMD_ACTIONFB_LIGHTFLARE = 102,     			// pne
	CMD_ACTIONFB_LITCHEMLIGHT = 103,   			// pne
	CMD_ACTIONFB_UNPINGRENAGE = 104,   			// pne
	CMD_ACTIONFB_WALKIETALKIEON = 108, 			// pne
	CMD_ACTIONFB_WALKIETALKIEOFF = 109,			// pne
	CMD_ACTIONFB_TAKEITEM = 115,   				// pne
	CMD_ACTIONFB_HIDEITEM = 116,   				// pne
	
	CMD_ACTIONFB_HANDCUFFTARGET = 150,    		// erc,cro
	CMD_ACTIONFB_MORPHINE = 151,    			// cro
	CMD_ACTIONFB_INJECTION = 152,    			// cro
	CMD_ACTIONFB_INJECTIONTARGET = 153,    		// erc,cro
	CMD_ACTIONFB_DRINKSIP = 154,    			// cro
	
	
	
	CMD_FUNNYSTUFF_VIKTOR 		= 3000,
	CMD_FUNNYSTUFF_VIKTOR_LOOP 	= 3001,
	CMD_FUNNYSTUFF_MAREK 		= 3002,
	CMD_FUNNYSTUFF_MAREK_LOOP 	= 3003,
	CMD_FUNNYSTUFF_KAKEE 		= 3004,
	CMD_FUNNYSTUFF_KAKEE_LOOP 	= 3005,
	CMD_FUNNYSTUFF_JONNY 		= 3006,
	CMD_FUNNYSTUFF_JONNY_LOOP 	= 3007,
	CMD_FUNNYSTUFF_MARTIN 		= 3008,
	CMD_FUNNYSTUFF_MARTIN_LOOP 	= 3009,
	CMD_FUNNYSTUFF_ADAM		 	= 3010,
	CMD_FUNNYSTUFF_ADAM_LOOP 	= 3011,
    
    //! ---------------------------------------------------------
    //! ---------------------- GESTURES -------------------------
    //! ---------------------------------------------------------
    //! modifier (differential animation)
    CMD_GESTUREMOD_GREETING = 1000,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_POINT = 1001,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_THUMB = 1002,				// erc,cro	 	    [end, CMD_ACTIONINT_ACTION to switch to THUMB DOWN]
    CMD_GESTUREMOD_THUMBDOWN = 1003,			// erc,cro	 	    [end, CMD_ACTIONINT_ACTION to switch to THUMB UP]
    CMD_GESTUREMOD_SILENCE = 1004,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_TAUNT = 1005,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_TIMEOUT = 1006,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_HEART = 1007,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_FACEPALM = 1008,				// erc,cro   	    [end]
    CMD_GESTUREMOD_WATCHING = 1009,				// erc,cro	 	    [end]
    CMD_GESTUREMOD_HOLD = 1010,					// erc,cro	 	    [end]
    CMD_GESTUREMOD_LISTENING = 1011,			// erc,cro	 	    [end]
    CMD_GESTUREMOD_POINTSELF = 1012,			// erc,cro	 	    [end]
    CMD_GESTUREMOD_LOOKATME = 1013,				// erc,cro	 	    [end]

    CMD_GESTUREMOD_RPS = 10000,		    		// erc 	            [SPECIAL, SEE BELOW]

    CMD_ACTIONINT_RPS_ROCK 	= 10,	    		// RPS Internal Gesture
    CMD_ACTIONINT_RPS_PAPER = 11,				// RPS Internal Gesture
    CMD_ACTIONINT_RPS_SCISSORS = 12,			// RPS Internal Gesture
	
	
    // onetime 		
    CMD_GESTUREMOD_THROAT = 1100,				// erc,cro,pne
    CMD_GESTUREMOD_CLAP = 1101,		    		// erc,cro
    CMD_GESTUREMOD_DABBING = 1102,				// erc,cro
    CMD_GESTUREMOD_MOVE = 1103,					// erc,cro,pne
    CMD_GESTUREMOD_DOWN = 1104,					// erc,cro,pne
    CMD_GESTUREMOD_COME = 1105,					// erc,cro,pne
    CMD_GESTUREMOD_TAUNTKISS = 1106,			// erc,cro,pne
    CMD_GESTUREMOD_TAUNTELBOW = 1107,			// erc,cro
    CMD_GESTUREMOD_TAUNTTHINK = 1108,			// erc,cro,pne
	
    //! --------------------------
    //! fb (full body)
    CMD_GESTUREFB_GREETING = 1000,				// pne 	    [end]
    CMD_GESTUREFB_POINT = 1001,					// pne 	    [end]
    CMD_GESTUREFB_THUMB = 1002,					// pne 	    [end, CMD_ACTIONINT_ACTION to switch to THUMB DOWN]
    CMD_GESTUREFB_THUMBDOWN = 1003,				// pne 	    [end, CMD_ACTIONINT_ACTION to switch to THUMB UP]
    CMD_GESTUREFB_SILENCE = 1004,				// pne 	    [end]
    CMD_GESTUREFB_TAUNT = 1005,					// pne 	    [end]
    CMD_GESTUREFB_TIMEOUT = 1006,				// pne 	    [end]
    CMD_GESTUREFB_HEART = 1007,					// pne 	    [end]
    CMD_GESTUREFB_WATCHING = 1009,				// pne 	    [end]
    CMD_GESTUREFB_HOLD = 1010,					// pne 	    [end]
    CMD_GESTUREFB_LISTENING = 1011,				// pne 	    [end]
    CMD_GESTUREFB_POINTSELF = 1012,				// pne 	    [end]
    CMD_GESTUREFB_LOOKATME = 1013,				// pne 	    [end]
	
    CMD_GESTUREFB_SALUTE = 1050,	    		// erc              [end]
    CMD_GESTUREFB_CAMPFIRE = 1051,	    		// cro              [end]
    CMD_GESTUREFB_LYINGDOWN = 1052,	    		// cro              [end]
    CMD_GESTUREFB_SOS = 1053,	        		// erc              [end]
    CMD_GESTUREFB_SITA = 1054,	        		// cro              [end]
    CMD_GESTUREFB_SITB = 1055,	        		// cro              [end]
    CMD_GESTUREFB_DABBING = 1056,				// erc, cro         [end]

    // onetime 
    CMD_GESTUREFB_THROAT = 1100,				// pne
    CMD_GESTUREFB_MOVE = 1103,					// pne
    CMD_GESTUREFB_DOWN = 1104,					// pne
    CMD_GESTUREFB_COME = 1105,					// pne
    CMD_GESTUREFB_TAUNTKISS = 1106,				// pne
    CMD_GESTUREFB_TAUNTTHINK = 1108,			// pne
    CMD_GESTUREFB_DANCE = 1109,	        		// erc
    
    //! ---------------------------------------------------------
    //! ---------------------- SUICIDE --------------------------
    //! ---------------------------------------------------------
    //! fb (full body)
    CMD_SUICIDEFB_1HD = 2000,	        		// cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_FIREAXE = 2001,	    		// erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_PITCHFORK = 2002,	    		// erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_PISTOL = 2003,	    		// cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_RIFLE = 2004,	        		// cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_SWORD = 2005,	        		// erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_UNARMED = 2006,	    		// cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_WOODAXE = 2007,	    		// erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_SPEAR = 2008,	        		// erc              [end (cancel), end2 (commit)]

	// 32767 is totaly MAXIMAL index !!!!!

};

//! defined in C++
enum DayZPlayerInstanceType
{
	INSTANCETYPE_SERVER,		//! server instance 
	INSTANCETYPE_CLIENT,		//! client instance && controlled
	INSTANCETYPE_REMOTE,		//! client instance && other player 
	
	INSTANCETYPE_AI_SERVER,				//! temporary!
	INSTANCETYPE_AI_REMOTE,				//! temporary!
	INSTANCETYPE_AI_SINGLEPLAYER,	//! temporary!
}

// *************************************************************************************
// ! DayZPlayer - mainly for logic and camera 
// *************************************************************************************
class SDayZPlayerHeadingModel
{
	int 	m_iCamMode;				//[in]		- DayZPlayerConstants.CAMERAMODE_...
	int 	m_iCurrentCommandID;    //[in]		- Current Main Command ID 
	float 	m_fOrientationAngle;	//[in/out] 	- horizontal model orientation (where you face) - in rad
	float 	m_fHeadingAngle;		//[in/out] 	- horizontal aim angle (where you aim) - in rad

	//! cannot be created from script
	protected void SDayZPlayerHeadingModel()
	{
	}
}


// *************************************************************************************
// ! DayZPlayer - mainly for logic and camera 
// *************************************************************************************
class SDayZPlayerAimingModel
{
	int 	m_iCamMode;				//[in]		- DayZPlayerConstants.CAMERAMODE_... HEAD,EXTERNAL,WEAPON...
	int 	m_iCurrentCommandID;    //[in]		- Current Main Command ID 
	float	m_fCurrentAimX;			//[in]		- horizontal aim angle - in degrees
	float	m_fCurrentAimY;			//[in]		- vertical aim angle - in degrees

	float	m_fAimXCamOffset;		//!< [out] 	- camera offset modifier
	float	m_fAimYCamOffset;		//!< [out] 	- camera offset modifier
	float	m_fAimXHandsOffset;		//!< [out] 	- hands offset modifier
	float	m_fAimYHandsOffset;		//!< [out] 	- hands offset modifier
	float	m_fAimXMouseShift;		//[out]		- shift like mouse does
	float	m_fAimYMouseShift;		//[out]		- shift like mouse does
	float	m_fAimSensitivity;		//[out]		- aim sensitivity
	float 	m_fCamPosOffsetX;		//[out]		- currently not supported
	float 	m_fCamPosOffsetY;		//[out]		- currently not supported

	//! cannot be created from script
	protected void SDayZPlayerAimingModel()
	{
	}
}


// *************************************************************************************
// ! DayZPlayer - mainly for logic and camera 
// *************************************************************************************
class DayZPlayer extends Human
{

	//! returns appropriate DayZPlayerType
	proto native DayZPlayerType		GetDayZPlayerType();
	
	proto native DayZPlayerInstanceType 	GetInstanceType();

	//! ---------------- heading model -------------------------
	//! updated each tick
	//! this can limit / extend player's heading (orientation) behaviour in game 
	
	bool			HeadingModel(float pDt, SDayZPlayerHeadingModel pModel);	


	//! ---------------- aiming model -------------------------

	//! updated each tick - this takes care about aiming 
	
	bool			AimingModel(float pDt, SDayZPlayerAimingModel pModel);	

	
	
	//! ---------------- command handling -------------------------

	//! updated each tick 
	//! handles: starting new commands, canceling, interruption 
	//! pDt 						- delta time 
	//! pCurrentCommandID 			- actually running command's ID 
	//! pCurrentCommandFinished		- is current command finished 
	
	void 			CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished);	

	//! ---------------- camera handling -------------------------

	//! virtual 
	//! returns registered camera type
	//! pCameraMode - DayZPlayerConstants.CAMERAMODE_ ... 
	int 			CameraHandler(int pCameraMode);


	//! ---------------- camera additional functions -------------------------

	//! gets current camera 
	proto native 	DayZPlayerCamera		GetCurrentCamera();

	//! gets current camera 
	proto native 	int 					GetCurrentPerItemCameraUD();
	
	proto native	bool					IsCameraBlending();
	
	//! returns true if player is using EyeZoom, otherwise false
	bool									IsEyeZoom();
	//! return true if shots are fired from camere, otherwise false
	bool									IsShootingFromCamera();

	//! processes melee hit
	proto native	MeleeCombatData			GetMeleeCombatData();

	//! processes melee hit
	proto native	void					ProcessMeleeHit(InventoryItem pMeleeWeapon, int pMeleeModeIndex, Object pTarget, int pComponentIndex, vector pHitWorldPos);
	
	//! ---------------- sync stuff -------------------------
	
	proto native	void					SendSyncJuncture(int pJunctureID, ParamsWriteContext ctx);
	proto native	void					StoreInputForRemotes(ParamsWriteContext ctx);
	
	//! ---------------- sync shadow -------------------------


	//! test if entity is DayZPlayer 
	//static proto native bool				IsEntityDayZPlayer(IEntity pEntity);

	//! 
	proto native 	bool					DebugSyncShadowSetup(DayZPlayer pPlayer);



	//! ---------------- camera additiona functions -------------------------
	
	
	//! returns true if player is currently in one of the stances specified by stance mask 
	//! IsPlayerInStance(STANCEMASK_ERECT | STANCEMASK_CROUCH) returns true if player is standing or crouching and not raised (aimed)
	//! IsPlayerInStance(STANCEMASK_PRONE | STANCEIDX_RAISEDPRONE) returns true if player is in or in prone (both raised or nonraised)
	//! IsPlayerInStance(STANCEMASK_ALL) returns true always 
	//! IsPlayerInStance(STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE) returns true if player has raised hands
	
	// PSOVIS - move to native
	bool		IsPlayerInStance(int pStanceMask)		// STANCEMASK_ERECT | STANCEMASK_CROUCH 
	{
		HumanMovementState		state = new HumanMovementState;
		GetMovementState(state);
		bool ret = ((1 << state.m_iStanceIdx) & pStanceMask) != 0;
		delete state;
		return ret;
	}
	
	void OnInputForRemote (ParamsReadContext ctx) { }
	void OnInputFromServer (ParamsReadContext ctx) { }
}

