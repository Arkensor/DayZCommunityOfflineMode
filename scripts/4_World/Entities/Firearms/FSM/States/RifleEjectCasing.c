class RifleEjectCasing extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;

	ref WeaponStartAction m_start;
	ref WeaponEjectCasing m_eject;
	ref BulletHide_W4T m_hideB;

	void RifleEjectCasing (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponStartAction(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectCasing(m_weapon, this);
		m_hideB = new BulletHide_W4T(m_weapon, this);

		// events
		WeaponEventBase __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase __bh_ = new WeaponEventAnimBulletHide;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		// transitions
		m_fsm.AddTransition(new WeaponTransition(m_start, __be_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(m_eject, __bh_, m_hideB));
		m_fsm.AddTransition(new WeaponTransition(m_hideB, _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}
};



