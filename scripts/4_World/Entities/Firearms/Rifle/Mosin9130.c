enum MosinAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
	OPENED 				= 1,
	JAMMED 				= 2,
};

enum MosinStableStateID
{
	UNKNOWN				=  0,
	EmptyDischarged		=  1,
	LoadedCharged		=  2,
	LoadedDischarged	=  3,
	LoadedJammed		=  4,
}


class MosinEmptyDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { EmptyDischarged C0"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } EmptyDischarged C0"); }
	override int GetCurrentStateID () { return MosinStableStateID.EmptyDischarged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class MosinLoadedCharged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedCharged C1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedCharged C1"); }
	override int GetCurrentStateID () { return MosinStableStateID.LoadedCharged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class MosinLoadedDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedDischarged D1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedDischarged D1"); }
	override int GetCurrentStateID () { return MosinStableStateID.LoadedDischarged; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class MosinLoadedJammed extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedJammed J1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedJammed J1"); }
	override int GetCurrentStateID () { return MosinStableStateID.LoadedJammed; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return true; }
};


/**@class		Mosin9130
 * @brief		base for Mosin
 * @NOTE		name copies config base class
 **/
class Mosin9130_Base extends Weapon_Base
{
	ref WeaponStateBase C0;
	ref	WeaponStateBase C1;
	ref	WeaponStateBase D1;
	ref	WeaponStateBase J1;
	
	void Mosin9130_Base ()
	{
	}

	override void InitStateMachine ()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		
		
		C0 = new MosinEmptyDischarged(this, NULL, MosinAnimState.DEFAULT);
		C1 = new MosinLoadedCharged(this, NULL, MosinAnimState.DEFAULT);
		J1 = new MosinLoadedJammed(this, NULL, MosinAnimState.JAMMED);
		D1 = new MosinLoadedDischarged(this, NULL, MosinAnimState.DEFAULT);
		
	
		WeaponStateBase		Mech_C1 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		Mech_D1 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		LoopedChambering 	SChamber_C0 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,false);
		LoopedChambering 	SChamber_C1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,false);
		LoopedChambering 	SChamber_D1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,false);
		
		LoopedChambering 	LChamber_C0 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		LoopedChambering 	LChamber_C1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		LoopedChambering 	LChamber_D1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		
		WeaponStateBase		Unjam_J1 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);
		
		WeaponStateBase		Trigger_C0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_C1 = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase		Trigger_D1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_J1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		// extend Rifle_Base fsm
		/*LoopedChambering lch = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering psh = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering lch2 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		LoopedChambering psh2 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		*/
		
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase	__M__ = new WeaponEventMechanism;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		
		// Mechanism
		/*m_fsm.AddTransition(new WeaponTransition( D1	, __M__,	Mech));
		m_fsm.AddTransition(new WeaponTransition( C1	, __M__,	Mech));
		m_fsm.AddTransition(new WeaponTransition( Mech  , _fin_,	C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech  , _fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	C1));*/
//----------------------------------------	
			
		m_fsm.AddTransition(new WeaponTransition( C1,		__M__,  Mech_C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_fin_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,		__M__,  Mech_D1));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  C1));
		
				
//----------------------------------------		
		
		m_fsm.AddTransition(new WeaponTransition( C0,			__L__,  SChamber_C0));
		m_fsm.AddTransition(new WeaponTransition( SChamber_C0,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( SChamber_C0,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( SChamber_C0,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__L__,  SChamber_C1, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( SChamber_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( SChamber_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,			__L__,  SChamber_D1));
		m_fsm.AddTransition(new WeaponTransition( SChamber_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( SChamber_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( SChamber_D1,	_abt_,  C1));
		
//----------------------------------------		
		
/*		m_fsm.AddTransition(new WeaponTransition( C0,			__lS_,  LChamber_C0));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__lS_,  LChamber_C1, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,			__lS_,  LChamber_D1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_abt_,  C1));*/
		
//------------------------------------------
		
			
		m_fsm.AddTransition(new WeaponTransition(J1, 		__U__, Unjam_J1));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, C1));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, C1));
		
		
//-----------------------------------------
		
		// fire
		m_fsm.AddTransition(new WeaponTransition(C0,			__T__, Trigger_C0)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_fin_, C0));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_dto_, C0));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_abt_, C0));
		
		m_fsm.AddTransition(new WeaponTransition(C1,			__T__, Trigger_C1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_fin_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, C1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, C1));
		
		m_fsm.AddTransition(new WeaponTransition(D1,			__T__, Trigger_D1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_fin_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_dto_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_abt_, D1));
		
		m_fsm.AddTransition(new WeaponTransition(J1,			__T__, Trigger_J1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_fin_, J1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_dto_, J1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_abt_, J1));
		
//-----------------------------------------	

		// load cartridge
/*		m_fsm.AddTransition(new WeaponTransition( E_C, __lS_,  lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( lch, _fin_,  L_C));
		
		m_fsm.AddTransition(new WeaponTransition( E_C, __L__,  lch2));
		m_fsm.AddTransition(new WeaponTransition( lch2, _fin_,  L_C));

		m_fsm.AddTransition(new WeaponTransition( E_D, __lS_,  lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( lch, _fin_,  L_C));
		
		m_fsm.AddTransition(new WeaponTransition( E_D, __L__,  lch2));
		m_fsm.AddTransition(new WeaponTransition( lch2, _fin_,  L_C));

		m_fsm.AddTransition(new WeaponTransition( L_C, __lS_,  psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( psh, _fin_,  L_C));
		
	
		m_fsm.AddTransition(new WeaponTransition( L_C, __L__,  psh2, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( psh2, _fin_,  L_C));

		// @TODO: chci load z loaded_fired_out - nefunguje mi nejak ten m_eject
		m_fsm.AddTransition(new WeaponTransition( E_D, __lS_, psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh, _fin_,  L_C));
	*/	
		
		m_fsm.SetInitialState(C0);

		SelectionBulletHide();
		SelectionMagazineHide();

		m_fsm.Start();
	}
};


class Mosin9130 extends Mosin9130_Base
{
};
