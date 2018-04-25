
class WeaponFireLast extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	
	float m_dtAccumulator;
	ref WeaponFire m_fire;
	ref WeaponEjectCasing_W4T m_eject;

	void WeaponFireLast (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_fire = new WeaponFire(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectCasing_W4T(m_weapon, this);

		// events
		WeaponEventAnimBulletEject  __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventReloadTimeout __to_ = new WeaponEventReloadTimeout;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		// transitions
		m_fsm.AddTransition(new WeaponTransition( m_fire, __be_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(m_eject, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(m_eject, __to_, NULL));

		m_fsm.SetInitialState(m_fire);
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_dtAccumulator = 0;
	}
	
	override void OnUpdate (float dt)
	{
		m_dtAccumulator += dt;
		
		WeaponStateBase curstate = GetFSM().GetCurrentState();
		
		if (curstate == m_eject)
		{
			DayZPlayer p;
			Class.CastTo(p, m_weapon.GetHierarchyParent());
		
			HumanInputController hic = p.GetInputController();

			int muzzleIndex = m_weapon.GetCurrentMuzzle();
			float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
			if ( hic.IsUseButton() && m_dtAccumulator >= reloadTime)
				if (m_weapon.CanProcessWeaponEvents())
					m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
		}
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};


