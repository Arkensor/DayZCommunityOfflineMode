enum RifleAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
};

enum RifleStableStateID
{
	UNKNOWN				=  0,
	EmptyDischarged		=  1,
	EmptyCharged		=  2,
	LoadedCharged		=  3,
	LoadedJammed		=  4,
}

class RifleEmptyDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { EmptyDischarged E_D"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } EmptyDischarged E_D"); }
	override int GetCurrentStateID () { return RifleStableStateID.EmptyDischarged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class RifleEmptyCharged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { EmptyCharged E_C"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } EmptyCharged E_C"); }
	override int GetCurrentStateID () { return RifleStableStateID.EmptyCharged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class RifleLoadedCharged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedCharged L_C"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedCharged L_C"); }
	override int GetCurrentStateID () { return RifleStableStateID.LoadedCharged; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class RifleLoadedJammed extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedJammed L_J"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedJammed L_J"); }
	override int GetCurrentStateID () { return RifleStableStateID.LoadedJammed; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return true; }
};


/**@class		Rifle_Base
 * @brief		base for rifles
 * @NOTE		name copies config base class
 **/
class Rifle_Base extends Weapon_Base
{
	ref WeaponStateBase E_D;
	ref WeaponStateBase E_C;
	ref WeaponStateBase L_J;
	ref WeaponStateBase L_C;

	void Rifle_Base ()
	{
	}
	
	override void InitStateMachine()
	{
		// setup abilities
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));

		// basic weapon states
		E_D = new RifleEmptyDischarged(this);
		E_C = new RifleEmptyCharged(this);
		L_J = new RifleLoadedJammed(this);
		L_C = new RifleLoadedCharged(this);

		// unstable (intermediate) states
		WeaponStateBase		chg = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase	  chgEj = new RifleEjectCasing(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		df1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		WeaponStateBase		fir = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase		df2 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);
		//WeaponStateBase rch = new RifleChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED);
		WeaponStateBase		rrc = new RifleReChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED);
		WeaponStateBase		eje = new WeaponEjectBullet(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		uja = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);

		// events
		WeaponEventBase	__M__ = new WeaponEventMechanism;
		WeaponEventBase	__T__ = new WeaponEventTrigger;
		WeaponEventBase	__L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase __lE_ = new WeaponEventContinuousLoadBulletEnd;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		// charging
		m_fsm.AddTransition(new WeaponTransition(E_D  , __M__,	 chg, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(chg  , _fin_,	 E_C));

		m_fsm.AddTransition(new WeaponTransition(E_D  , __M__,	chgEj, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(chgEj, _fin_,	 E_C));

		// chamber not empty && not fired out
		/*WeaponGuardBase not_empty_and_not_fired_out =
			new GuardAnd(
					new GuardNot(new WeaponGuardChamberEmpty(this)),
					new GuardNot(new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(L_C, __L__, rrc, NULL, not_empty_and_not_fired_out));
		m_fsm.AddTransition(new WeaponTransition(rrc, _fin_, L_C));*/

		// fire
		m_fsm.AddTransition(new WeaponTransition(E_C, __T__, df1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(df1, _fin_, E_D));
		m_fsm.AddTransition(new WeaponTransition(df1, _dto_, E_D));
		m_fsm.AddTransition(new WeaponTransition(E_D, __T__, df2)); // fire.uncocked
		m_fsm.AddTransition(new WeaponTransition(df2, _fin_, E_D));
		m_fsm.AddTransition(new WeaponTransition(df2, _dto_, E_D));
		m_fsm.AddTransition(new WeaponTransition(L_C, __T__, fir, NULL, new GuardNot(new WeaponGuardJammed(this)))); // a) fire if not jammed
		m_fsm.AddTransition(new WeaponTransition(fir, _fin_, E_D));
		m_fsm.AddTransition(new WeaponTransition(fir, _rto_, E_D));
		m_fsm.AddTransition(new WeaponTransition(L_C, __T__, L_J, NULL, new WeaponGuardJammed(this))); // b) or switch to jam
		m_fsm.AddTransition(new WeaponTransition(E_C, __M__, chg)); // start charging again

		// unjam
		m_fsm.AddTransition(new WeaponTransition(L_J, __U__, uja));
		m_fsm.AddTransition(new WeaponTransition(uja, _fin_, L_J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(uja, _fin_, E_C, NULL, new GuardNot(new WeaponGuardJammed(this))));

		// eject good cartridge
		//m_fsm.AddTransition(new WeaponTransition(L_C, __M__, eje, NULL, not_empty_and_not_fired_out));
		//m_fsm.AddTransition(new WeaponTransition(eje, _fin_, E_C));

		m_fsm.SetInitialState(E_D);

		SelectionBulletHide();
		SelectionMagazineHide();

		m_fsm.Start();
	}
};


