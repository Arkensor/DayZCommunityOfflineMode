/*
DayZPlayerImplement

this file is implemenation of dayzPlayer in script
- logic of movement
- camera switching logic

*/

class DayZPlayerImplement extends DayZPlayer
{
	// Timer 	m_UpdateTick;

	protected ref HumanMovementState 					m_MovementState = new HumanMovementState();	//!< cached ptr for movement state
	protected ref DayZPlayerImplementAiming				m_AimingModel;
	protected ref DayZPlayerImplementMeleeCombat		m_MeleeCombat;
	protected ref DayZPlayerMeleeFightLogic_LightHeavy	m_MeleeFightLogic;
	protected ref DayZPlayerImplementSwimming			m_Swimming;
	protected ref DayZPlayerImplementFallDamage			m_FallDamage;
	protected float 									m_FallYDiff;
	protected float										m_SprintedTime;
	protected bool										m_SprintFull;
	protected bool										m_IsFireWeaponRaised;
	protected bool										m_Camera3rdPerson;
	protected bool										m_CameraEyeZoom;
	protected bool										m_CameraIronsighs;
	protected bool										m_CameraOptics;
	protected float 									m_CameraIronsighsNotRaisedTime;
	protected bool										m_IsTryingHoldBreath;
	protected bool										m_IsShootingFromCamera;

	ref WeaponDebug										m_WeaponDebug;

	//! constructor 
	void 	DayZPlayerImplement()
	{
		//Print("DayZPlayerImplement: Init");
		m_IsFireWeaponRaised = false;
		m_SprintFull = false;
		m_SprintedTime = 0;
		m_AimingModel = new DayZPlayerImplementAiming(this);
		m_MeleeCombat = new DayZPlayerImplementMeleeCombat(this);
		m_MeleeFightLogic = new DayZPlayerMeleeFightLogic_LightHeavy(this);
		m_Swimming = new DayZPlayerImplementSwimming(this);
		m_FallDamage = new DayZPlayerImplementFallDamage(this);
		m_PostedEvent = null;
		m_PostedHandEvent = null;
		m_CameraEyeZoom = false;
		m_CameraOptics = false;
		m_IsShootingFromCamera = true;
		#ifdef PREVIEW_BUILD
		m_Camera3rdPerson = true;
		#endif
	}

	DayZPlayerImplementAiming GetAimingModel()
	{
		return m_AimingModel;
	}

	DayZPlayerImplementMeleeCombat GetMeleeCombat()
	{
		return m_MeleeCombat;
	}
	
	DayZPlayerMeleeFightLogic_LightHeavy GetMeleeFightLogic()
	{
		return m_MeleeFightLogic;
	}

	//! destructor 
	void 	~DayZPlayerImplement()
	{
	}

	override bool IsEyeZoom()
	{
		return m_CameraEyeZoom;
	}
	
	override bool IsShootingFromCamera()
	{
		return m_IsShootingFromCamera;
	}
	
	void OverrideShootFromCamera(bool pState)
	{
		m_IsShootingFromCamera = pState;
	}

	bool IsInIronsights()
	{
		return m_CameraIronsighs;
	}

	bool IsFireWeaponRaised()
	{
		return m_IsFireWeaponRaised;
	}

	bool IsTryingHoldBreath()
	{
		return m_IsTryingHoldBreath;
	}

	bool IsSprintFull()
	{
		return m_SprintFull;
	}

	void ShowWeaponDebug(bool show)
	{
		if (show)
		{
			m_WeaponDebug = new WeaponDebug;
		}
		else
		{
			m_WeaponDebug = null;
		}
	}

	//! Implementations only! - used on PlayerBase
	bool CanConsumeStamina(EStaminaConsumers consumer);
	void DepleteStamina(EStaminaModifiers modifier) {}


	//-------------------------------------------------------------
	//!
	//! HandleDeath
	//! 

	bool	HandleDeath(int pCurrentCommandID)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_DEATH)
		{
			return true;
		}


		if (!IsAlive() && g_Game.GetMissionState() == g_Game.MISSION_STATE_GAME)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ShowDeadScreen, DEAD_SCREEN_DELAY, false, true);
			
			StartCommand_Death();
			
			// disable voice communication
			GetGame().GetWorld().SetVoiceOn(false);
			return true;
		}

		return false;
	}
	
	static const int DEAD_SCREEN_DELAY = 1000;
	
	void ShowDeadScreen(bool show)
	{
	#ifndef NO_GUI	
		if (show)
		{
			GetGame().GetUIManager().ScreenFadeIn(0.5, "You are dead", FadeColors.BLACK, FadeColors.WHITE);
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
		}
	#endif
	}
	
	DayZPlayerInventory GetDayZPlayerInventory ()
	{
		DayZPlayerInventory inv = DayZPlayerInventory.Cast(GetInventory());
		return inv;
	}

	override void OnInputForRemote (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputForRemote - cannot read input type");
			return;
		}

		//Print("[inv] Input For Remote! type=" + userDataType);
		switch (userDataType)
		{
			case INPUT_UDT_WEAPON_REMOTE_SYNC:
				GetDayZPlayerInventory().OnSyncFromRemoteWeapon(ctx);
				break;
			case INPUT_UDT_WEAPON_REMOTE_EVENT:
				GetDayZPlayerInventory().OnEventFromRemoteWeapon(ctx);
				break;
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnInputUserDataFromRemote(ctx);
				break;
			default:
				Error("OnInputForRemote - unknown userDataType=" + userDataType);
				break;
		}
	}

	override void OnInputFromServer (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputFromServer - cannot read input type");
			return;
		}

		switch (userDataType)
		{
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnServerInventoryCommand(ctx);
				break;
			default:
				Error("OnInputFromServer - unknown userDataType=" + userDataType);
				break;
		}
	}

	ref HandEventBase m_PostedHandEvent; /// deferred hand event

	/**@fn	PostHandEvent
	 * @brief	deferred hands's fsm handling of events
	 * @NOTE: "post" stores the event for later use when ::CommandHandler is being run
	 **/
	override void PostHandEvent (HandEventBase e)
	{
		// @NOTE: synchronous event from server. this happens only on client(s) and is caused by CreateVehicle and DeleteObject network messages.
		bool synchronous = e.GetEventID() == HandEventID.DESTROYED || e.GetEventID() == HandEventID.CREATED;
		// @NOTE: there is no HandleInventory(dt) called on Remote(s). Immeadiate processing as workaroud.
		bool remote = GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_REMOTE || GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE;
		if (synchronous || IsDamageDestroyed() || remote)
		{
			hndDebugPrint("[hndfsm] SYNCHRONOUS hand event e=" + e.ToString());
			DayZPlayerInventory inv = GetDayZPlayerInventory();
			inv.ProcessHandEvent(e);
		}
		else
		{
			if (m_PostedHandEvent == NULL)
			{
				m_PostedHandEvent = e;
				hndDebugPrint("[hndfsm] Posted event m_PostedHandEvent=" + m_PostedHandEvent.ToString());
			}
			else
				Error("[hndfsm] warning - pending hand event already posted, curr_event=" + m_PostedHandEvent.ToString() + " new_event=" + e.ToString());
		}
	}
	void HandleInventory (float pDt)
	{
		if (!GetInventory())
			return;

		if (m_PostedHandEvent)
		{
			hndDebugSpam("[hndfsm] Hand event: deferred " + m_PostedHandEvent);
			DayZPlayerInventory inv = GetDayZPlayerInventory();

			HandEventBase hndEvent = m_PostedHandEvent; // make a copy for processing and release post
			hndDebugSpam("[hndfsm] Hand event: deferred event reset to null.");
			m_PostedHandEvent = NULL;

			inv.ProcessHandEvent(hndEvent); // process copy
		}
	}

	override void OnItemInHandsChanged () { GetItemAccessor().OnItemInHandsChanged(); }

	ref WeaponEventBase m_PostedEvent; /// deferred event
	/**@fn	PostWeaponEvent
	 * @brief	deferred weapon's fsm handling of events
	 * @NOTE: "post" stores the event for later use when ::CommandHandler is being run
	 **/
	void PostWeaponEvent(WeaponEventBase e)
	{
		if (m_PostedEvent == NULL)
		{
			m_PostedEvent = e;
			wpnDebugPrint("[wpnfsm] Posted event m_PostedEvent=" + m_PostedEvent.ToString());
		}
		else
			Error("[wpnfsm] warning - pending event already posted, curr_event=" + m_PostedHandEvent.ToString() + " new_event=" + e.ToString());
	}

	WeaponManager GetWeaponManager () { return null; }

	//-------------------------------------------------------------
	//!
	//! HandleWeapons
	//!
	void HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
		HumanCommandWeapons		hcw = GetCommandModifier_Weapons();
		Weapon_Base weapon;
		Class.CastTo(weapon, pInHands);

		if (hcw && weapon && weapon.CanProcessWeaponEvents())
		{
			weapon.GetCurrentState().OnUpdate(pDt);

			//wpnDebugPrint("[wpnfsm] HCW: playing A=" + typename.EnumToString(WeaponActions, hcw.GetRunningAction()) + " AT=" + WeaponActionTypeToString(hcw.GetRunningAction(), hcw.GetRunningActionType()) + " fini=" + hcw.IsActionFinished());

			if (!weapon.IsIdle())
			{

				while (true)
				{
					int weaponEventId = hcw.IsEvent();
					if (weaponEventId == -1)
					{
						break;
					}

					WeaponEventBase anim_event = WeaponAnimEventFactory(weaponEventId, this, NULL);
					wpnDebugPrint("[wpnfsm] HandleWeapons: event arrived " + typename.EnumToString(WeaponEvents, weaponEventId) + "(" + weaponEventId + ")  fsm_ev=" + anim_event.ToString());
					if (anim_event != NULL)
					{
						weapon.ProcessWeaponEvent(anim_event);
					}
				}

				if (hcw.IsActionFinished())
				{
					if (weapon.IsWaitingForActionFinish())
					{
						wpnDebugPrint("[wpnfsm] Weapon event: finished! notifying waiting state=" + weapon.GetCurrentState());
						weapon.ProcessWeaponEvent(new WeaponEventHumanCommandActionFinished(this));
					}
					else
					{
						wpnDebugPrint("[wpnfsm] Weapon event: ABORT! notifying running state=" + weapon.GetCurrentState());
						weapon.ProcessWeaponAbortEvent(new WeaponEventHumanCommandActionAborted(this));
					}
				}
			}

			if (m_PostedEvent)
			{
				wpnDebugPrint("[wpnfsm] Weapon event: deferred " + m_PostedEvent);
				weapon.ProcessWeaponEvent(m_PostedEvent);
				pExitIronSights = true;
				fsmDebugSpam("[wpnfsm] Weapon event: resetting deferred event" + m_PostedEvent);
				m_PostedEvent = NULL;
			}
		}

		// @TODO: refactor to action (rm from input, mk action)
		if (pInputs.IsReloadOrMechanismSingleUse() && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			if (weapon && weapon.CanProcessWeaponEvents())
			{
				if (GetWeaponManager().CanEjectBullet(weapon))
				{
					GetWeaponManager().EjectBullet();
					pExitIronSights = true;
				}
			}
		}

		GetMovementState(m_MovementState);

		// hold breath
		if (pInputs.IsHoldBreath() && m_MovementState.IsRaised() && IsInIronsights())
		{
			m_IsTryingHoldBreath = true;
		}
		else
		{
			m_IsTryingHoldBreath = false;
		}

		if (pInputs.IsFireModeChange())
		{
			weapon.SetNextMuzzleMode(weapon.GetCurrentMuzzle());
		}
		if (pInputs.IsZeroingUp())
		{
			weapon.StepZeroingUp();
		}
		if (pInputs.IsZeroingDown())
		{
			weapon.StepZeroingDown();
		}
		
		if (m_CameraIronsighs)
		{
			ItemOptics optic = weapon.GetAttachedOptics();
			if (optic && pInputs.IsZoomIn())
			{
				if (!m_CameraOptics)
				{
					m_CameraOptics = true;
					optic.EnterOptics();
				}
				else
				{
					optic.StepFOVUp();
				}
			}
			if (optic && pInputs.IsZoomOut())
			{
				if (m_CameraOptics)
				{
					if (optic.GetStepFOVIndex() > 0)
					{
						optic.StepFOVDown();
					}
					else
					{
						optic.ExitOptics();
						m_CameraOptics = false;
					}
				}
			}
		}

		if (weapon && weapon.CanProcessWeaponEvents())
		{
			if (pInputs.IsReloadOrMechanismContinuousUseStart())
			{
				weapon.ProcessWeaponEvent(new WeaponEventUnjam(this));
			}
		}

		if (!m_MovementState.IsRaised())
		{
			m_IsFireWeaponRaised = false;
			if (weapon && weapon.IsInOptics())
				weapon.ExitOptics();

			return; // if not raised => return
		}
		else
			m_IsFireWeaponRaised = true;

		if (pInputs.IsImmediateAction())
		{
			if (!weapon.IsInOptics())
				weapon.EnterOptics();
			else
				weapon.ExitOptics();
		}

		//! fire
		if (weapon && weapon.CanProcessWeaponEvents() && !GetGame().GetInput().GetActionDown(UAHeavyMeleeAttack, false))
		{
			bool autofire = weapon.GetCurrentModeAutoFire(weapon.GetCurrentMuzzle());
			int burst = weapon.GetCurrentModeBurstSize(weapon.GetCurrentMuzzle());
			if (!autofire)
			{
				if (pInputs.IsUseButtonDown())
				{
					weapon.ProcessWeaponEvent(new WeaponEventTrigger(this));
				}
			}
			else
			{
				if (pInputs.IsUseButton())
				{
					weapon.ProcessWeaponEvent(new WeaponEventTrigger(this));
				}
			}
		}
	}


	//-------------------------------------------------------------
	//!
	//! HandleDamageHit
	//! 

	bool m_DamageHitFullbody = false;
	int m_DamageHitAnimType = -1;
	float m_DamageHitDir = 0;

	bool HandleDamageHit(int pCurrentCommandID)
	{
		if (m_DamageHitAnimType != -1)
		{
			if (m_DamageHitFullbody)
			{
				StartCommand_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return true;
			}
			else
			{
				AddCommandModifier_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return false;
			}
		}

		return pCurrentCommandID == DayZPlayerConstants.COMMANDID_DAMAGE;
	}

	//! selects animation type and direction based on damage system data
	bool EvaluateDamageHitAnimation(TotalDamageResult pDamageResult, int pDamageType, EntityAI pSource, string pComponent, string pAmmoType, vector pModelPos, out int pAnimType, out float pAnimHitDir, out bool pAnimHitFullbody)
	{
		pAnimType = 0;
		pAnimHitFullbody = false; // additive anm
		
		switch(pDamageType)
		{
			case 0: // DT_CLOSE_COMBAT
				pAnimType = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " hitAnimation");
				if (pAnimType == 1 && !m_MeleeFightLogic.IsInBlock())
					pAnimHitFullbody = true;
			break;
			case 1: // DT_FIREARM
				return false; // skip evaluation of dmg hit anim (tmp)
				/*
				float fireDamage = pDamageResult.GetHighestDamage("Health");
				if (fireDamage > 80.0)
					pAnimHitFullbody = true;
				*/
			break;
			case 2: // DT_EXPLOSION
			break;
			case 3: // DT_CUSTOM (used by Fall Damage)
				return false; //! skip evaluation of dmg hit animation
			break;
		}

		//! direction
		vector targetDirection = GetDirection();
		vector toSourceDirection = (pSource.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		pAnimHitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		if (cross[1] < 0)
			pAnimHitDir = -pAnimHitDir;

		//Print("hitdir: " + pAnimHitDir.ToString());

		return true;
	}

	//! event from damage system
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, ammo, modelPos);

		int animType;
		float animHitDir;
		bool animHitFullbody;
		if (EvaluateDamageHitAnimation(damageResult, damageType, source, component, ammo, modelPos, animType, animHitDir, animHitFullbody))
		{
			DayZPlayerSyncJunctures.SendDamageHit(this, animType, animHitDir, animHitFullbody);
		}

		// interupt melee for non-blocked hit or heavy hit
		if( !m_MeleeFightLogic.IsInBlock() || animHitFullbody )
		{
			HumanCommandMelee hcm = GetCommand_Melee();
			if(hcm) hcm.Cancel();
		}
	}

	override void EEHitByRemote(int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, ammo, modelPos);
		
		Print("DayZPlayerImplement : EEHitByRemote");
	}

	//-------------------------------------------------------------
	//!
	//! This HeadingModel
	//! 

	float 	m_fLastHeadingDiff = 0;

	//!
	override bool	HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		//! on ladder - do nothing 
		if (GetCommand_Ladder())
		{
			m_fLastHeadingDiff = 0;
			return false;
		}		


#ifdef DEVELOPER
		int	actMenuValue = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_TURNSLIDE);
		if (actMenuValue != 0)
		{
			//! this clamps heading 
			return DayZPlayerImplementHeading.ClampHeading(pDt, pModel, m_fLastHeadingDiff);
		}
#endif

		GetMovementState(m_MovementState);
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{
			m_fLastHeadingDiff = 0;
			return false;
		}

		return DayZPlayerImplementHeading.RotateOrient(pDt, pModel, m_fLastHeadingDiff);	
	}


	//-------------------------------------------------------------
	//!
	//! This Aiming Model
	//! 

	override bool	AimingModel(float pDt, SDayZPlayerAimingModel pModel)
	{
		GetMovementState(m_MovementState);
		
		bool isInIronsights = IsInIronsights();
		bool isFireWeaponRaised = IsFireWeaponRaised();
		
		if (isInIronsights)
		{
			pModel.m_fAimSensitivity = 0.3;
		}
		else if (isFireWeaponRaised)
		{
			pModel.m_fAimSensitivity = 0.5;
		}
		else
		{
			pModel.m_fAimSensitivity = 1.0;
		}
		
		return m_AimingModel.ProcessAimFilters(pDt, pModel, m_MovementState.m_iStanceIdx);
	}

	//-------------------------------------------------------------
	//!
	//! This is main command logic
	//! 

	//! 
	override void  CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		HumanInputController hic = GetInputController();

		/*
		// Unconscious test 
		if (hic.IsUseButtonDown())
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				StartCommand_Unconscious();
			}
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				HumanCommandUnconscious	hcu = GetCommand_Unconscious();
				hcu.WakeUp();
			}
		}
		*/

		//! 3rd person camera
		if (hic.CameraViewChanged())
		{
			if (!GetGame().GetWorld().Is3rdPersonDisabled())
			{
				m_Camera3rdPerson = !m_Camera3rdPerson;
			}
			else
			{
				m_Camera3rdPerson = false;
			}
		}

		//! reset ironsights && optics
		bool	prevIronSights 	= m_CameraIronsighs;
		bool	prevOptics		= m_CameraOptics;
		m_CameraIronsighs		= false;
		m_CameraOptics			= false;

		//! top priority - handle death first
		if (HandleDeath(pCurrentCommandID))
		{
			return;
		}


		//! handle finished commands
		if (pCurrentCommandFinished)
		{
			// if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE)
			// start moving

			//! default behaviou after finish is to start move
			StartCommand_Move();

			return;
		};

		//! Sprint attack limiting - player has to be in full sprint for at least 0.5s
		//--------------------------------------------
		GetMovementState(m_MovementState);
		HumanCommandMove hcm = GetCommand_Move();
		if (hcm && hcm.GetCurrentMovementSpeed() > 2.99 && m_MovementState.m_iMovement == 3)
		{
			m_SprintedTime += pDt;
			if( m_SprintedTime > 0.5)
				m_SprintFull = true;
			else
				m_SprintFull = false;

			//Print(m_SprintedTime);
		}
		else
		{
			m_SprintedTime = 0.0;
			m_SprintFull = false;
		}

		//--------------------------------------------
		// swimming handling
		if (m_Swimming.HandleSwimming(pCurrentCommandID, hcm, m_MovementState))
		{
			return;			
		}
		
		//--------------------------------------------
		// ladder handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER)
		{
			return;
		}



		//--------------------------------------------
		// fall handling

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_FALL)
		{
			// vector	vel;
			// PhysicsGetVelocity(vel);
			// Print(vel);

			HumanCommandFall fall = GetCommand_Fall();

			if (fall.PhysicsLanded())
			{
				// land
				m_FallYDiff = m_FallYDiff - GetPosition()[1];
				Print(m_FallYDiff);
				if (m_FallYDiff < 0.5)
				{
					fall.Land(HumanCommandFall.LANDTYPE_NONE);
				}
				else if (m_FallYDiff < 1.0)
				{
					fall.Land(HumanCommandFall.LANDTYPE_LIGHT);
				}
				else if (m_FallYDiff < 2.0)
				{
					fall.Land(HumanCommandFall.LANDTYPE_MEDIUM);
				}
				else
				{
					fall.Land(HumanCommandFall.LANDTYPE_HEAVY);
				}

				m_FallDamage.HandleFallDamage(m_FallYDiff);
			}

			return;
		}

		// start falling ? 
		if (PhysicsIsFalling())
		{
			StartCommand_Fall(0);
			m_FallYDiff = GetPosition()[1];
			return;
		}

		//--------------------------------------------
		// damage hits
		if (HandleDamageHit(pCurrentCommandID))
		{
			return;
		}

		//--------------------------------------------
		// handle jumping



#ifndef NO_GUI
#ifdef DEVELOPER
		//! enable this later for everything

		if (DiagMenu.GetValue(DayZPlayerConstants.DEBUG_ENABLEJUMP) && hic.IsJumpClimb())
		{
			m_FallYDiff = GetPosition()[1];
			StartCommand_Fall(3.0);
			return;
		}

#endif
#endif 




		////////////////////////////////////////////////
		// Eye Zoom logic
		
		if (hic.IsZoom() && !m_CameraEyeZoom)
		{
			m_CameraEyeZoom = true;
			//Print("To EyeZoom " +  m_CameraEyeZoom.ToString());
		}
		else if (!hic.IsZoom() && m_CameraEyeZoom )
		{
			//Print("From EyeZoom " +  m_CameraEyeZoom.ToString());
			m_CameraEyeZoom = false;
		}

		////////////////////////////////////////////////
		// ironsights logic

		// now 
		m_CameraIronsighs = prevIronSights;
		m_CameraOptics = prevOptics;

		//! check for raise double click
		bool	sightChange = false;
		
		if (hic.IsWeaponRaised())
		{	
			if (m_CameraIronsighsNotRaisedTime > 0 && m_CameraIronsighsNotRaisedTime < 0.3)
			{
				sightChange 	= true;
			}
			m_CameraIronsighsNotRaisedTime = 0.0;
		}
		else 
		{
			m_CameraIronsighsNotRaisedTime += pDt;
		}
	
		if (hic.IsSightChange() || sightChange)
		{
			HumanItemAccessor 	hia = GetItemAccessor();
			if (hia.IsItemInHandsWeapon())
			{
				m_CameraIronsighs = !m_CameraIronsighs;
				Print("To ironsights " +  m_CameraIronsighs.ToString());

				if (m_CameraIronsighs)
				{
					// go to ironsights - disable ironsights when
					//! if !raised
					//! if sprinting

					GetMovementState(m_MovementState);
					if (!hic.IsWeaponRaised() || m_MovementState.m_iMovement == 3)
					{
						Print("To ironsights or optics: failed -> sprint or no aim");
						ExitSights();
					}
				}
				else
				{
					ExitSights();
				}
			}
		}
		else if (m_CameraIronsighs)
		{
			// clear ironsights when lowered
			GetMovementState(m_MovementState);
			if (!m_MovementState.IsRaised())
			{
				Print("From ironsights and optics");
				ExitSights();
			}
		}

		//--------------------------------------------
		// anything whats handled by InputController

		//! 

		if (hic)
		{
			//! get weapon 
			EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
			bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);
			if (isWeapon)
			{
				bool exitIronSights = false;
				HandleWeapons(pDt, entityInHands, hic, exitIronSights);
				if (exitIronSights)
					return;
			}
			else
			{
				GetMovementState(m_MovementState);
				if(m_MeleeFightLogic.Process(pCurrentCommandID, hic, entityInHands, m_MovementState))
				{
					return;
				}
			}
		}
	}

	void ExitSights ()
	{
		m_CameraIronsighs = false;
		m_CameraOptics = false;

		ItemOptics optics = null;
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		Weapon weapon = Weapon.Cast(entityInHands);
		if (weapon)
		{
			optics = weapon.GetAttachedOptics();
		}
		else
		{
			optics = ItemOptics.Cast(entityInHands);
		}

		if (optics)
			optics.ExitOptics();
	}

	//-------------------------------------------------------------
	//!
	//! This is Debug functionality 
	//! 

	float 	m_TestDamageCounter 	= -1;	//! VKOSTIK - tady dat 0 misto -1
	float 	m_DebugDirectionVal 	= -180;
	bool	m_DebugTypeVal 			= false;

	//! 
	void  CommandHandlerDebug(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{

		//! 
		/*
		// debug - print ironsights pos / dir

		HumanItemAccessor hia = GetItemAccessor();

		vector pos, dir;

		if (hia.WeaponGetCameraPoint(pos, dir))
		{
			string 	a = "ITACC: ";
			a += pos.ToString();
			a += " ";
			a += dir.ToString();

		//Print("DayZPlayerImplement: Init");

			Print(a);
		}
		*/

		/* {
			HumanCommandMove mc = GetCommand_Move();		
			if (mc)
			{
		
				float gcms = mc.GetCurrentMovementSpeed();
				Print(gcms);
			}
		} */


		//! debug handling of adding commands 
		if (GetPluginManager())
		{
			PluginDayzPlayerDebug playerDebug = PluginDayzPlayerDebug.Cast(GetPluginManager().GetPluginByType(PluginDayzPlayerDebug));
			if (playerDebug)
			{
				playerDebug.CommandHandler();
			}
		}


		//! damage test
		if (m_TestDamageCounter >= 0)
		{
			// HumanCommandDamage damage = GetCommandModifier_Damage();
			if (m_TestDamageCounter > 3)
			{
				m_DebugDirectionVal = m_DebugDirectionVal + 90;
				m_TestDamageCounter = 0;

				if (m_DebugDirectionVal > 90)
				{
					m_DebugDirectionVal = -180;
					m_DebugTypeVal		= !m_DebugTypeVal;
				}

				if (m_DebugTypeVal)
				{
					AddCommandModifier_Damage(0, m_DebugDirectionVal); // starts light
				}
				else
				{
					StartCommand_Damage(0, m_DebugDirectionVal); // starts heavy (full body)
				}
				// 0.. types
				// from: -180 back, -90 left, 0 front, 90 right, 180 back 
				//AddCommandModifier_Damage(0, directionVal); // starts light
				//StartCommand_Damage(0, 0); // starts heavy (full body)
			}

			m_TestDamageCounter += pDt;
		}


#ifndef NO_GUI
#ifdef DEVELOPER

		// injury / exhaustion
		{
			HumanCommandAdditives ad = GetCommandModifier_Additives();

			//! diag menu - injured
			float v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWINJURY);
			if (v > 0)
			{
				v = (v - 1.0) * 0.1;
				ad.SetInjured(v, true);
			}

			//! diag menu - injured
			v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWEXHAUSTION);
			if (v > 0)
			{
				v = (v - 1) * 0.1;
				ad.SetExhaustion(v, true);
			}
		}
#endif
#endif 

	}



	//-------------------------------------------------------------
	//!
	//! This is main camera selection logic
	//! 
	override int 	CameraHandler(int pCameraMode)
	{
		// Print("DayZPlayerCfgBase: Camera Handler called");

		/* {
			int perItemCamUD = GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + ToString(perItemCamUD);
			Print(a);
		} */

		//! ironsights
		if (m_CameraIronsighs)
		{
			if (m_CameraOptics)
				return DayZPlayerCameras.DAYZCAMERA_OPTICS;
			else
				return DayZPlayerCameras.DAYZCAMERA_IRONSIGHTS;
		}
		
		if (!m_Camera3rdPerson)
		{
			return DayZPlayerCameras.DAYZCAMERA_1ST;
		}
		else // if (pCameraMode == DayZPlayerConstants.CAMERAMODE_EXTERNAL)
		{
			//! get movement state 
			GetMovementState(m_MovementState);
			
			//! VKOSTIK: v ostatnich commandech mimo COMMANDID_MOVE je 
			//! m_MovementState.m_iStanceIdx - DayZPlayerConstants.STANCEIDX_ERECT
			//! m_MovementState.m_iMovement = 0 (idle)

			//! COMMANDID_ACTION returns stance right - ERECT/CROUCH

			//! melee camera - not melee - stays in stance camera

			/* if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MELEE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
			}*/

			//		
			// normal movement cameras			
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO;
			}

			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO;
			}
			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
			{
				EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
				bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);

				if (isWeapon) 
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
				}
				else
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED_MELEE;
				}
			}

			else if (m_MovementState.m_iMovement == 3)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_SPR;
			}

			// DayZPlayerConstants.STANCEIDX_ERECT
			return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;

			//
		}

		return DayZPlayerCameras.DAYZCAMERA_1ST;
	}


	//-------------------------------------------------------------
	//!
	//! Input userdata
	//! 

	void OnInputUserDataReceived(ParamsReadContext ctx)
	{
		int userDataTypeParam = 0;
		if (!ctx.Read(userDataTypeParam))
		{
			Error("DayZPlayerImplement: cannot read input type");
			return;
		}

		OnInputUserDataProcess(userDataTypeParam, ctx);
	}

	bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if (userDataType == INPUT_UDT_MELEE_TARGET)
		{
			EntityAI target = null;
			if (ctx.Read(target))
			{
				m_MeleeCombat.SetTargetObject(target);
			}

			int hitZoneIdx = -1;
			if (ctx.Read(hitZoneIdx))
			{
				m_MeleeCombat.SetHitZoneIdx(hitZoneIdx);
			}
			return true;
		}

		return false;
	}

	//-------------------------------------------------------------
	//!
	//! SyncJunctures
	//! 

	void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		switch (pJunctureID)
		{
		case DayZPlayerSyncJunctures.SJ_DAMAGE_HIT:
			DayZPlayerSyncJunctures.ReadDamageHitParams(pCtx, m_DamageHitAnimType, m_DamageHitDir, m_DamageHitFullbody);
			break;
		}
	}

	AnimBootsType GetBootsType()
	{
		Entity boots = GetInventory().FindAttachment(InventorySlots.FEET);
		if (boots == NULL)
			return AnimBootsType.None;

		string bootsName = boots.GetType();

		if (bootsName.Contains("Sneakers") || bootsName.Contains("AthleticShoes") || bootsName.Contains("JoggingShoes"))
		{
			return AnimBootsType.Sneakers;
		}
		else if (bootsName.Contains("Boots") || bootsName.Contains("Wellies") || bootsName.Contains("Shoes"))
		{
			return AnimBootsType.Boots;
		}
		else
		{
			Print("Unrecognized boots type: \"" + bootsName + "\"");
			return AnimBootsType.None;
		}
	}
	
	//Get top body clothes
	AnimUpperBodyType GetBodyAttachmentType()
	{
		Entity attachment = GetInventory().FindAttachment(InventorySlots.BODY);
		if(attachment == NULL)
			return AnimUpperBodyType.None;
		
		string name = attachment.GetType();
		
		if (name.Contains("BomberJacket") || name.Contains("HikingJacket") || name.Contains("HuntingJacket") || name.Contains("QuiltedJacket") || name.Contains("Raincoat") || name.Contains("TacticalShirt") || name.Contains("TrackSuitJacket"))
		{
			return AnimUpperBodyType.NylonJacket;
		}
		else if (name.Contains("TShirt") || name.Contains("MedicalScrubsShirt") || name.Contains("NurseDress"))
		{
			return AnimUpperBodyType.TShirt;
		}
		else if (name.Contains("Shirt") || name.Contains("WoolDress") || name.Contains("Sweater"))
		{
			return AnimUpperBodyType.WoolShirt;
		}
		else if (name.Contains("ParamedicJacket") || name.Contains("FirefighterJacket") || name.Contains("GorkaEJacket") || name.Contains("TTSKOJacket") || name.Contains("M65Jacket"))
		{
			return AnimUpperBodyType.HeavyJacket;
		}
		else if (name.Contains("LeatherJacket") || name.Contains("RidersJacket"))
		{
			return AnimUpperBodyType.LeatherJacket;
		}
		else if (name.Contains("WoolCoat"))
		{
			return AnimUpperBodyType.Coat;
		}
		else if (name.Contains("PrisonersJacket") || name.Contains("PoliceJacket") || name.Contains("LabCoat") || name.Contains("USMCJacket") || name.Contains("PoliceJacketOrel"))
		{
			return AnimUpperBodyType.ChemlonDress;
		}
		else if (name.Contains("Ghillie"))
		{
			return AnimUpperBodyType.Ghillie;
		}
		else
		{
			Print("Unrecognized upper body type: \"" + name + "\"");
			return AnimUpperBodyType.None;
		}
	}
	
	//Get weapon on back
	AnimRangedWeaponType GetShoulderAttachmentType()
	{
		Entity shoulderAttachment = GetInventory().FindAttachment(InventorySlots.SHOULDER);
		if(shoulderAttachment == NULL)
			return AnimRangedWeaponType.None;
		
		string shoulderAttachmentName = shoulderAttachment.GetType();
		
		if (shoulderAttachmentName.Contains("Shotgun"))
		{
			return AnimRangedWeaponType.Shotgun;
		}
		else
		{
			return AnimRangedWeaponType.Rifle;
		}
	}

	//-------------------------------------------------------------
	//!
	//! Sounds
	//! 

	AbstractWave PlaySound(SoundObject so)
	{
		if (so == NULL)
		{
			//Print("PlaySound: NULL argument");
			return NULL;
		}

		so.SetPosition(GetPosition());
		AbstractWave wave = so.Play3D();
		return wave;
	}

	void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		if(noisePar != NULL)
			GetGame().GetNoiseSystem().AddNoise(this, noisePar, noiseMultiplier);
	}

	//-------------------------------------------------------------
	//!
	//! Event Processing
	//! 

	void OnStepEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		ref HumanMovementState	state = new HumanMovementState();
		GetMovementState(state);

		if(pUserString == "walk")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_WALK;
		else if(pUserString == "run")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_RUN;
		else if(pUserString == "sprint")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_SPRINT;
		
		if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			state.m_iMovement--;
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			DayZPlayerTypeStepSoundLookupTable table = type.GetStepSoundLookupTable();
			SoundObject soundObject = table.GetSoundObject(pUserInt, state.m_iMovement, GetSurfaceType().Hash(), GetBootsType());

			if (soundObject != NULL)
			{
				PlaySound(soundObject);
			}
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			float noiseMultiplier = GetSurfaceNoise();
			NoiseParams noiseParams;
			if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
				noiseParams = type.GetNoiseParamsStand();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
				noiseParams = type.GetNoiseParamsCrouch();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
				noiseParams = type.GetNoiseParamsProne();
			else
			{
				Print("OnStepEvent: wrong stance, id: " + state.m_iStanceIdx + "using backup with stand stance (id: 0)");
				noiseParams = type.GetNoiseParamsStand();
			}
			AddNoise(noiseParams, noiseMultiplier);
		}
	}

	void OnSoundEvent(string pEventType, string pUserString, int pUserInt)
	{
		if (pEventType == "Sound")
		{
			//Print("Sound event");
		}
		else if (pEventType == "SoundWeapon")
		{
			ProcessWeaponEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundAttachment")
		{
			ProcessAttachmentEvent(pEventType, pUserString, pUserInt);
		}
		else
		{
			Print("OnSoundEvent: Unknown sound event \"" + pEventType + "\"");
		}
	}

	void OnParticleEvent(string pEventType, string pUserString, int pUserInt)
	{
		//if(pEventType != "Sound" && pEventType != "RFootDown")
		Print("OnParticleEvent: Not implemented yet");
	}

	
	void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		AnimSoundEvent sound_event = NULL;

		float quantity = 0;
		
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		if (entityInHands != NULL && entityInHands.IsInherited(ItemBase))
		{
			ItemBase item;
			Class.CastTo(item, entityInHands);
			if(item.HasQuantity())
				quantity = (float)item.GetQuantity() / (item.GetQuantityMax() - item.GetQuantityMin());
			InventoryItemType invItemType = item.GetInventoryItemType();
			sound_event = invItemType.GetSoundEvent(pUserInt);
		}

		if (sound_event == NULL)
		{
			sound_event = type.GetSoundWeaponEvent(pUserInt);
		}

		if (sound_event != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder builder = sound_event.GetSoundBuilder();
				builder.SetVariable("quantity", quantity);
				builder.SetVariable("interior", IsSoundInsideBuilding());
				PlaySound(builder.BuildSoundObject());
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				AddNoise(sound_event.m_NoiseParams);
			}
		}
		else
		{
			//commented out, some events in animation should be processed only when some items are in hand
			//string className;
			//if (entityInHands != NULL)
			//	className = entityInHands.GetType();
			//else
			//	className = "SurvivorBase";
			//Print("OnSoundEvent: eventType: \"SoundWeapon\", Not defined event id: " + pUserInt + ", in class \"" + className + "\"");
		}
	}

	void ProcessAttachmentEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAttachmentSoundLookupTable table = type.GetAttachmentSoundLookupTable();
			
		array<string> attachments = new array<string>();
		pUserString.Split(",", attachments);
		for(int i = 0; i < attachments.Count(); i++)
		{
			int attachmentHash = -1;
			if(attachments[i] == "shoulder")
				attachmentHash = GetShoulderAttachmentType();
			else if(attachments[i] == "body")
				attachmentHash = GetBodyAttachmentType();
			SoundObject soundObject = table.GetSoundObject(pUserInt, attachments[i], attachmentHash);
			if (soundObject != NULL)
			{
				PlaySound(soundObject);
			}
		}
	}
	
	//-------------------------------------------------------------
	//!
	//! Voice over network conditions
	//! 
	bool CheckForVoNUse(EntityAI item)
	{
		if (!IsAlive() /*|| IsUnconscious()*/)
			return false;
		return true;
	}
	
	//-------------------------------------------------------------
	//!
	//! anti-cheat conditions
	//! 

	bool CheckForTakeItem(EntityAI item)
	{
		bool restrained = false; // IsRestrained()
		if (!IsAlive() || restrained)
			return true;
		return false;
	}
	bool CheckForDropItem(EntityAI item)
	{
		bool restrained = false; // IsRestrained()
		if (!IsAlive() || restrained)
			return true;
		return false;
	}

#ifdef DEVELOPER

	// -------------------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK)
		{
			Print("ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK");

			Param1<bool> rp = new Param1<bool>(false);

			ctx.Read(rp);

			GetInputController().OverrideMovementSpeed(rp.param1, 1);
			GetInputController().OverrideAimChangeX(rp.param1, 0.01);

		}
	}

#endif

}
