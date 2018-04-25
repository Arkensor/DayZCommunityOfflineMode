enum RBLAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
	OPENED 				= 1,
	JAMMED 				= 2,
};

enum RBLStableStateID
{
	UNKNOWN				=  0,
	RBL_CLO_BU0_MA0		=  1,
	RBL_CLO_BU1_MA0		=  2,
	RBL_CLO_BU1_MA1		=  3,
	RBL_CLO_BU0_MA1		=  4,
	RBL_OPN_BU0_MA1		=  5,
	RBL_OPN_BU0_MA0		=  6,
	RBL_JAM_BU1_MA0		=  7,
	RBL_JAM_BU1_MA1		=  8
}

class RBL_CLO_BU0_MA0 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { close nobull nomag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } close nobull nomag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_CLO_BU0_MA0; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }

};
class RBL_CLO_BU1_MA0 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { close bullet nomag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } close bullet nomag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_CLO_BU1_MA0; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class RBL_CLO_BU1_MA1 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { close bullet mag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } close bullet mag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_CLO_BU1_MA1; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return true; }
	override bool IsJammed () { return false; }
};
class RBL_CLO_BU0_MA1 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { close nobull mag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } close nobull mag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_CLO_BU0_MA1; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return true; }
	override bool IsJammed () { return false; }
};
class RBL_OPN_BU0_MA1 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { lock nobull mag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } open nobull mag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_OPN_BU0_MA1; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return true; }
	override bool IsJammed () { return false; }
};
class RBL_OPN_BU0_MA0 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { lock nobull nomag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } open nobull nomag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_OPN_BU0_MA0; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class RBL_JAM_BU1_MA0 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { jammed bullet nomag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } jammed bullet nomag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_JAM_BU1_MA0; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return true; }
};
class RBL_JAM_BU1_MA1 extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { jammed bullet mag"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } jammed bullet mag"); }
	override int GetCurrentStateID () { return RBLStableStateID.RBL_JAM_BU1_MA1; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return true; }
	override bool IsJammed () { return true; }
};


/**@class		Rifle_Base
 * @brief		base for rifles
 * @NOTE		name copies config base class
 **/
class RifleBoltLock_Base extends Rifle_Base
{
	ref WeaponStateBase C00;
	ref	WeaponStateBase C10;
	ref	WeaponStateBase C11;
	ref	WeaponStateBase C01;
	ref	WeaponStateBase L00;
	ref	WeaponStateBase L01;
	ref	WeaponStateBase J10;
	ref	WeaponStateBase J11;

	void RifleBoltLock_Base ()
	{
	}
	
	override void InitStateMachine()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_BULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_BULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET_OPEN)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH)));

		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_CLOSED)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED)));//????
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)));
		
		//m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));

		// setup state machine
		// basic weapon states
		// close-locked-jammed | nobullet-bullet | nomag-mag
		// regexp: [CLJ][01][01]
		C00 = new RBL_CLO_BU0_MA0(this, NULL, RBLAnimState.DEFAULT);
		C10 = new RBL_CLO_BU1_MA0(this, NULL, RBLAnimState.DEFAULT);
		C11 = new RBL_CLO_BU1_MA1(this, NULL, RBLAnimState.DEFAULT);
		C01 = new RBL_CLO_BU0_MA1(this, NULL, RBLAnimState.DEFAULT);
		L00 = new RBL_OPN_BU0_MA0(this, NULL, RBLAnimState.OPENED);
		L01 = new RBL_OPN_BU0_MA1(this, NULL, RBLAnimState.OPENED);
		J10 = new RBL_JAM_BU1_MA0(this, NULL, RBLAnimState.JAMMED);
		J11 = new RBL_JAM_BU1_MA1(this, NULL, RBLAnimState.JAMMED);

		// unstable (intermediate) states
		WeaponCharging Mech_C00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponCharging Mech_C01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponCharging Mech_L00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED));
		WeaponCharging Mech_L01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED));
	
		WeaponCharging Mech_C10 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponCharging Mech_C11 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		//WeaponCharging Mech_J10 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED);
		//WeaponCharging Mech_J11 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED);
		
		WeaponStateBase Trigger_C00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)); // cock without clip
		WeaponStateBase Trigger_C10 = new WeaponFireLast(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL)); // cock with clip
		WeaponFireAndChamberNext Trigger_C11 = new WeaponFireAndChamberNext(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		WeaponStateBase Trigger_C11L = new WeaponFireLast(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST));
		WeaponStateBase Trigger_C01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_L00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_L01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_J10 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_J11 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
	
		//Fire and jam
		WeaponStateBase Trigger_C10J = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM)); // cock with clip
		WeaponStateBase Trigger_C11J = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM));

		WeaponStateBase Unjam_J10 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		WeaponStateBase Unjam_J11 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));

		WeaponChambering Chamber_C00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		WeaponChambering Chamber_C01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		WeaponChambering Chamber_L00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_CLOSED));
		WeaponChambering Chamber_L01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_CLOSED));

		WeaponAttachMagazine Attach_C00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET));
		WeaponAttachMagazine Attach_C10 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_BULLET));
		WeaponAttachMagazine Attach_L00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN));//TODO Need add open animation
		WeaponAttachMagazine Attach_L00E = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN));//TODO Need add open animation
		WeaponAttachMagazine Attach_J10 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_BULLET));
		
		WeaponReplacingMagAndChamberNext Reload_C11 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_BULLET));
		WeaponReplacingMagAndChamberNext Reload_C01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET));
		WeaponReplacingMagAndChamberNext Reload_L01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET_OPEN));//TODO Need add open animation
		WeaponReplacingMagAndChamberNext Reload_J11 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_BULLET));
		
		WeaponDetachingMag Detach_C11 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_C01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_L01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_J11 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		
		//WeaponStateBase chmMag = new WeaponChamberFromAttMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __A__ = new WeaponEventAttachMagazine;
		WeaponEventBase __D__ = new WeaponEventDetachMagazine;
		WeaponEventBase __S__ = new WeaponEventSwapMagazine;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();

		// order: Discharged-Charged | nobullet-bullet | nomag-mag
		// charging
		m_fsm.AddTransition(new WeaponTransition(   C00, __M__,    Mech_C00)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_C00, _fin_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C00, _abt_,    C00));

		m_fsm.AddTransition(new WeaponTransition(   C10, __M__, Mech_C10)); // charge from bullet nomag
		m_fsm.AddTransition(new WeaponTransition(	Mech_C10, _fin_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C10, _abt_ , C00, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C10, _abt_ , C10));
		
		m_fsm.AddTransition(new WeaponTransition(   C11, __M__,    Mech_C11)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_C11, _fin_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C11, _fin_ , C11));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C11, _abt_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C11, _abt_ , C11));
		
		
		m_fsm.AddTransition(new WeaponTransition(   C01, __M__,    Mech_C01)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_C01, _fin_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C01, _fin_, C11));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C01, _abt_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_C01, _abt_, C11));
		
		m_fsm.AddTransition(new WeaponTransition(   L00, __M__,    Mech_L00)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_L00, _fin_, C00));
			Mech_L00.AddTransition(new WeaponTransition(  Mech_L00.m_start, _abt_,   L00));
			Mech_L00.AddTransition(new WeaponTransition(  Mech_L00.m_onCK, _abt_,   C00));
		
		m_fsm.AddTransition(new WeaponTransition(   L01, __M__,    Mech_L01)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_L01, _fin_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(   Mech_L01, _fin_, C11)); 
			Mech_L01.AddTransition(new WeaponTransition(  Mech_L01.m_start, _abt_,   L01));
			Mech_L01.AddTransition(new WeaponTransition(  Mech_L01.m_onCK, _abt_,   C01));
			Mech_L01.AddTransition(new WeaponTransition(  Mech_L01.m_chamber, _abt_,   C11));

/*		
		m_fsm.AddTransition(new WeaponTransition(   J10, __M__,    Mech_J10)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_J10, _fin_, C00 )); 
		
		m_fsm.AddTransition(new WeaponTransition(   J11, __M__,    Mech_J11)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(   Mech_J11, _fin_, C11, NULL, new WeaponGuardHasAmmo(this))); 
		m_fsm.AddTransition(new WeaponTransition(   Mech_J11, _fin_ , C01, NULL, new GuardNot(new WeaponGuardHasAmmo(this))));
*/
		// fire
		m_fsm.AddTransition(new WeaponTransition(   C00, __T__,    Trigger_C00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C00, _fin_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C00, _dto_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C00, _abt_,    C00));
		
		m_fsm.AddTransition(new WeaponTransition(   C01, __T__,    Trigger_C01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C01, _fin_,    C01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C01, _dto_,    C01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C01, _abt_,    C01));
		
		m_fsm.AddTransition(new WeaponTransition(   C10, __T__,    Trigger_C10J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10J, _fin_,    J10));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10J, _rto_,    J10));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10J, _abt_,    J10));
		
		m_fsm.AddTransition(new WeaponTransition(   C10, __T__,    Trigger_C10));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10, _fin_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10, _rto_,    C00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10, _abt_,    C00));
		

		
		m_fsm.AddTransition(new WeaponTransition(   C11, __T__,    Trigger_C11J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11J, _fin_,    J11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11J, _rto_,    J11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C10J, _abt_,    J11));
		
		m_fsm.AddTransition(new WeaponTransition(   C11, __T__,    Trigger_C11, NULL, new WeaponGuardHasAmmo(this)));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11, _rto_,    C11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11, _abt_,    C11));
		
		m_fsm.AddTransition(new WeaponTransition(   C11, __T__,    Trigger_C11L));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11L, _fin_,    L01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11L, _rto_,    L01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_C11L, _abt_,    L01));
	
		m_fsm.AddTransition(new WeaponTransition(   L00, __T__,    Trigger_L00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L00, _fin_,    L00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L00, _dto_,    L00));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L00, _abt_,    L00));
		
		m_fsm.AddTransition(new WeaponTransition(   L01, __T__,    Trigger_L01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L01, _fin_,    L01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L01, _dto_,    L01));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_L01, _abt_,    L01));
		
		m_fsm.AddTransition(new WeaponTransition(  J10, __T__,    Trigger_J10)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J10, _fin_,   J10));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J10, _dto_,   J10));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J10, _abt_,   J10));
		
		m_fsm.AddTransition(new WeaponTransition(  J11, __T__,    Trigger_J11)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J11, _fin_,   J11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J11, _dto_,   J11));
		m_fsm.AddTransition(new WeaponTransition(   Trigger_J11, _abt_,   J11));
		
		


		// load cartridge
		m_fsm.AddTransition(new WeaponTransition(   C00, __L__, Chamber_C00)); // chamber from closed=1
		m_fsm.AddTransition(new WeaponTransition(Chamber_C00, _fin_,    C10));
		m_fsm.AddTransition(new WeaponTransition(Chamber_C00, _abt_ , C00, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(Chamber_C00, _abt_ , C10));


		m_fsm.AddTransition(new WeaponTransition(   C01, __L__, Chamber_C01)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(Chamber_C01, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(Chamber_C01, _abt_ , C01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(Chamber_C01, _abt_ , C11));
	
		m_fsm.AddTransition(new WeaponTransition(   L00, __L__, Chamber_L00)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(Chamber_L00, _fin_,    C10));
		m_fsm.AddTransition(new WeaponTransition(Chamber_L00, _abt_ , L00, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(Chamber_L00, _abt_ , C10));
	
		m_fsm.AddTransition(new WeaponTransition(   L01, __L__, Chamber_L01)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(Chamber_L01, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(Chamber_L01, _abt_ , L01, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(Chamber_L01, _abt_ , C11));

		// attach magazine (no mag in weapon)
		m_fsm.AddTransition(new WeaponTransition(   C00, __A__, Attach_C00, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b0/m0
		m_fsm.AddTransition(new WeaponTransition(Attach_C00, _fin_,    C01, NULL, new WeaponGuardChamberEmpty(this)) );
		m_fsm.AddTransition(new WeaponTransition(Attach_C00, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(Attach_C00, _abt_,    C11, NULL, new WeaponGuardChamberFull(this)) );
		m_fsm.AddTransition(new WeaponTransition(Attach_C00, _abt_,    C01, NULL, new WeaponGuardHasMag(this)) );
		m_fsm.AddTransition(new WeaponTransition(Attach_C00, _abt_,    C00));

	
		m_fsm.AddTransition(new WeaponTransition(   C10, __A__, Attach_C10, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b1/m0
		m_fsm.AddTransition(new WeaponTransition(Attach_C10, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(Attach_C10, _abt_,    C11, NULL, new WeaponGuardHasMag(this)) );
		m_fsm.AddTransition(new WeaponTransition(Attach_C10, _abt_,    C10));

	
		m_fsm.AddTransition(new WeaponTransition(   L00, __A__, Attach_L00, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b0/m0
		m_fsm.AddTransition(new WeaponTransition(Attach_L00, _fin_,    C01, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Attach_L00, _fin_,    C11));
			Attach_L00.AddTransition(new WeaponTransition(Attach_L00.m_start,   _abt_,   L00));
			Attach_L00.AddTransition(new WeaponTransition(Attach_L00.m_getNew,  _abt_,   L00));
			Attach_L00.AddTransition(new WeaponTransition(Attach_L00.m_attach,  _abt_,   L01));
			Attach_L00.AddTransition(new WeaponTransition(Attach_L00.m_onCK,  _abt_,   C01));
			Attach_L00.AddTransition(new WeaponTransition(Attach_L00.m_chamber, _abt_,   C11));
		
		m_fsm.AddTransition(new WeaponTransition(   J10, __A__, Attach_J10, NULL, new WeaponGuardCanAttachMag(this))); // attach from JAM/b1/m0
		m_fsm.AddTransition(new WeaponTransition(Attach_J10, _fin_,    J11));
		m_fsm.AddTransition(new WeaponTransition(Attach_J10, _abt_,    J11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(Attach_J10, _abt_,    J10));


		// replace magazine
		m_fsm.AddTransition(new WeaponTransition(   C11, __S__, Reload_C11)); // swap in Chg/b1/m1
		m_fsm.AddTransition(new WeaponTransition(Reload_C11, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(Reload_C11, _abt_,    C11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(Reload_C11, _abt_,    C10));

	
		m_fsm.AddTransition(new WeaponTransition(   C01, __S__, Reload_C01)); // swap in CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(Reload_C01, _fin_,   C01, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Reload_C01, _fin_,   C11));
		m_fsm.AddTransition(new WeaponTransition(Reload_C01, _abt_,    C11, NULL, new WeaponGuardChamberFull(this)) );
		m_fsm.AddTransition(new WeaponTransition(Reload_C01, _abt_,    C01, NULL, new WeaponGuardHasMag(this)) );
		m_fsm.AddTransition(new WeaponTransition(Reload_C01, _abt_,    C00));
		
	
		m_fsm.AddTransition(new WeaponTransition(   L01, __S__, Reload_L01)); // swap in CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(Reload_L01, _fin_,   C01, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Reload_L01, _fin_,   C11));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_start ,    _abt_,   L01));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_detach,    _abt_,   L00));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_getNew,    _abt_,   L00));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_storeOld,  _abt_,   L00));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_attach,    _abt_,   L01));
			Reload_C01.AddTransition(new WeaponTransition(Reload_C01.m_onCK,      _abt_,   C01));
			Reload_L01.AddTransition(new WeaponTransition(Reload_L01.m_chamber,   _abt_,   C11));

		m_fsm.AddTransition(new WeaponTransition(   J11, __S__, Reload_J11)); // swap in JAM/b1/m1
		m_fsm.AddTransition(new WeaponTransition(Reload_J11, _fin_,    J11));
		m_fsm.AddTransition(new WeaponTransition(Reload_J11, _abt_,    J11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(Reload_J11, _abt_,    J10));
			
      // @TODO: abort in jam

		// detach magazine
		m_fsm.AddTransition(new WeaponTransition(   C11, __D__, Detach_C11, NULL, new WeaponGuardCanDetachMag(this))); // detach from Chg/b1/m1
		m_fsm.AddTransition(new WeaponTransition(Detach_C11, _fin_,    C10));
			Detach_C11.AddTransition(new WeaponTransition(Detach_C11.m_start,  _abt_,   C11));
			Detach_C11.AddTransition(new WeaponTransition(Detach_C11.m_store,  _abt_,   C10));
			Detach_C11.AddTransition(new WeaponTransition(Detach_C11.m_hideM,  _abt_,   C10));
		m_fsm.AddTransition(new WeaponTransition(   C01, __D__, Detach_C01, NULL, new WeaponGuardCanDetachMag(this))); // detach from Chg/b0/m1
		m_fsm.AddTransition(new WeaponTransition(Detach_C01, _fin_,    C00));
			Detach_C01.AddTransition(new WeaponTransition(Detach_C01.m_start,  _abt_,   C01));
			Detach_C01.AddTransition(new WeaponTransition(Detach_C01.m_store,  _abt_,   C00));
			Detach_C01.AddTransition(new WeaponTransition(Detach_C01.m_hideM,  _abt_,   C00));
		m_fsm.AddTransition(new WeaponTransition(   L01, __D__, Detach_L01, NULL, new WeaponGuardCanDetachMag(this))); // detach from Dic/b0/m1
		m_fsm.AddTransition(new WeaponTransition(Detach_L01, _fin_,    L00));
			Detach_L01.AddTransition(new WeaponTransition(Detach_L01.m_start,  _abt_,   L01));
			Detach_L01.AddTransition(new WeaponTransition(Detach_L01.m_store,  _abt_,   L00));
			Detach_L01.AddTransition(new WeaponTransition(Detach_L01.m_hideM,  _abt_,   L00));
		m_fsm.AddTransition(new WeaponTransition(   J11, __D__, Detach_J11, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b1/m1 jammed
		m_fsm.AddTransition(new WeaponTransition(Detach_J11, _fin_,    J10));
			Detach_J11.AddTransition(new WeaponTransition(Detach_J11.m_start,  _abt_,   J11));
			Detach_J11.AddTransition(new WeaponTransition(Detach_J11.m_store,  _abt_,   J10));
			Detach_J11.AddTransition(new WeaponTransition(Detach_J11.m_hideM,  _abt_,   J10));



		// unjam
		m_fsm.AddTransition(new WeaponTransition(   J10, __U__, Unjam_J10)); // unjam nomag
		m_fsm.AddTransition(new WeaponTransition(	Unjam_J10, _fin_,    J10, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(	Unjam_J10, _fin_,    C00, NULL, new GuardNot(new WeaponGuardJammed(this))));
		m_fsm.AddTransition(new WeaponTransition(	Unjam_J10, _abt_,    J10));
		m_fsm.AddTransition(new WeaponTransition(   J11, __U__,    Unjam_J11)); // unjam with mag with ammo
		m_fsm.AddTransition(new WeaponTransition(   Unjam_J11, _fin_,    J11, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(   Unjam_J11, _fin_,    C01, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(   Unjam_J11, _fin_,    C11));
		m_fsm.AddTransition(new WeaponTransition(   Unjam_J11, _abt_,    J11));

		m_fsm.SetInitialState(C00);

		SelectionBulletHide();
		SelectionMagazineHide();

		m_fsm.Start();
	}
};


