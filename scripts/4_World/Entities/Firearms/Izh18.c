/**@class		Izh18_Base
 * @brief		base for Izh18
 * @NOTE		name copies config base class
 **/
class Izh18_Base extends Weapon_Base
{
	void Izh18_Base ()
	{
	}
	
	override void InitStateMachine()
	{
		// setup abilities
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED));

		// setup state machine
		// basic weapon states
		WeaponStateBase FO = new RifleEmptyDischarged(this, NULL, RifleAnimState.DEFAULT);
		WeaponStateBase EC = new RifleEmptyCharged(this, NULL, RifleAnimState.DEFAULT);
		WeaponStateBase JM = new RifleLoadedJammed(this, NULL, RifleAnimState.DEFAULT);
		WeaponStateBase LC = new RifleLoadedCharged(this, NULL, RifleAnimState.DEFAULT);
		// unstable (intermediate) states
		WeaponStateBase Mech_FO = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase Mech_LC = new WeaponEjectBullet(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		
		WeaponStateBase Chamber_EC = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		WeaponStateBase Chamber_FO = new RifleChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		WeaponStateBase Chamber_LC = new RifleReChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		
		WeaponStateBase Trigger_EC = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);
		WeaponStateBase Trigger_LC = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase Trigger_FO = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);

		
		WeaponStateBase Unjam_JM = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		// charging
		m_fsm.AddTransition(new WeaponTransition(FO  , __M__,   Mech_FO));
		m_fsm.AddTransition(new WeaponTransition(Mech_FO  , _fin_,   EC));
		m_fsm.AddTransition(new WeaponTransition(Mech_FO  , _abt_,   FO, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Mech_FO  , _abt_,   EC));
		
		// eject good cartridge
		m_fsm.AddTransition(new WeaponTransition(LC  , __M__,   Mech_LC));
		m_fsm.AddTransition(new WeaponTransition(Mech_LC  , _fin_,   EC));
		m_fsm.AddTransition(new WeaponTransition(Mech_LC  , _abt_,   EC, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Mech_LC  , _abt_,   LC));

		// load cartridge
		// option 1: talking to submachine
		m_fsm.AddTransition(new WeaponTransition(EC  , __L__,   Chamber_EC));
		m_fsm.AddTransition(new WeaponTransition(Chamber_EC  , _fin_,   LC));
		m_fsm.AddTransition(new WeaponTransition(Chamber_EC  , _abt_,   EC, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Chamber_EC  , _abt_,   LC));

		
		m_fsm.AddTransition(new WeaponTransition(FO  , __L__,   Chamber_FO));
		m_fsm.AddTransition(new WeaponTransition(Chamber_FO  , _fin_,   LC));
		m_fsm.AddTransition(new WeaponTransition(Chamber_FO  , _abt_,   FO, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Chamber_FO  , _abt_,   EC, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Chamber_FO  , _abt_,   LC));


		m_fsm.AddTransition(new WeaponTransition(LC  , __L__,   Chamber_LC));
		m_fsm.AddTransition(new WeaponTransition(Chamber_LC  , _fin_,   LC));
		m_fsm.AddTransition(new WeaponTransition(Chamber_LC  , _abt_,   EC, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Chamber_LC  , _abt_,   LC));

		// option 2: talking to substates (across hierarchy, not yet fully implemented)
		//m_fsm.AddTransition(new WeaponTransition(E_D, __L__, chm.m_lo1)); // start loading 1 bullet in discharged
		//m_fsm.AddTransition(new WeaponTransition(chm.m_lo2, _fin_, LC));

		// fire
		m_fsm.AddTransition(new WeaponTransition(EC  		, __T__,   Trigger_EC)); // fire cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_EC , _fin_,   EC));
		m_fsm.AddTransition(new WeaponTransition(Trigger_EC , _dto_,   EC));
		m_fsm.AddTransition(new WeaponTransition(Trigger_EC , _abt_,   EC));
		
		m_fsm.AddTransition(new WeaponTransition(FO  , __T__,   Trigger_FO)); // fire uncocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_FO  , _fin_,   FO));
		m_fsm.AddTransition(new WeaponTransition(Trigger_FO  , _dto_,   FO));
		m_fsm.AddTransition(new WeaponTransition(Trigger_FO  , _abt_,   FO));
		
		m_fsm.AddTransition(new WeaponTransition(LC  , __T__,   Trigger_LC, NULL, new GuardNot(new WeaponGuardJammed(this)))); // a) fire if not jammed
		m_fsm.AddTransition(new WeaponTransition(Trigger_LC  , _fin_,   FO));
		m_fsm.AddTransition(new WeaponTransition(Trigger_LC  , _abt_,   FO));
		
		m_fsm.AddTransition(new WeaponTransition(LC  , __T__,   JM, NULL, new WeaponGuardJammed(this))); // b) or switch to jam
		
		m_fsm.AddTransition(new WeaponTransition(EC  , __M__,   Mech_FO)); // start charging again

		// unjam
		m_fsm.AddTransition(new WeaponTransition(JM  , __U__,   Unjam_JM));
		m_fsm.AddTransition(new WeaponTransition(Unjam_JM  , _fin_,   JM, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_JM  , _fin_,   EC));

		// initial state setup
		bool empty = true;
		bool discharged = false; // @TODO:
		WeaponStateBase init_state = EC;
		if (empty)
		{
			if (!discharged)
				init_state = EC;
		}
		else
		{
			init_state = LC; // can init state == load/jammed?
		}
		m_fsm.SetInitialState(init_state);

		SelectionBulletHide();

		m_fsm.Start();
	}
	
	override bool CanChamberBullet (int muzzleIndex, Magazine mag)
	{
		return CanChamberFromMag(muzzleIndex, mag);
	}
};
