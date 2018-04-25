// eject
//TODO MW delete this file
class WeaponEjectBullet_Start extends WeaponStartAction
{ };
/*
class WeaponEjectBullet_Cartridge extends WeaponStateBase
{
	Magazine m_dstMagazine; /// destination of the cartridge

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		DayZPlayer p = e.m_player;
		int mi = m_weapon.GetCurrentMuzzle();

		ejectBulletAndStoreInMagazine(m_weapon, mi, m_dstMagazine, p); // MP-safe
	}

	override void OnAbort (WeaponEventBase e)
	{
		m_dstMagazine = NULL;
		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dstMagazine = NULL;
		super.OnExit(e);
	}
};
*/
/*
class WeaponEjectBullet_Cartridge_W4T extends WeaponEjectBullet_Cartridge
{
	override bool IsWaitingForActionFinish () { return true; }
};
*/
class WeaponEjectBullet extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	Magazine m_dstMagazine; /// destination of the cartridge

	ref WeaponEjectBullet_Start m_start;
	ref WeaponEjectBullet_Cartridge_W4T m_eject; // @TODO: workaround for missing BH event
	ref BulletHide_W4T m_hideB;
	ref WeaponChamberFromAttMag_W4T m_chamber;

	void WeaponEjectBullet (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponEjectBullet_Start(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectBullet_Cartridge_W4T(m_weapon, this); // @TODO: workaround for missing BH event (_W4T)
		m_hideB = new BulletHide_W4T(m_weapon, this);
		m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
		
		// events
		WeaponEventBase __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase __bh_ = new WeaponEventAnimBulletHide;
		WeaponEventBase __ck_ = new WeaponEventAnimCocked;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		// transitions
		m_fsm.AddTransition(new WeaponTransition(  m_start, __be_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(  m_eject, __bh_, m_hideB));

		m_fsm.AddTransition(new WeaponTransition(  m_eject, __ck_, m_chamber, NULL, new WeaponGuardHasAmmo(m_weapon)));
		m_fsm.AddTransition(new WeaponTransition(  m_hideB, __ck_, m_chamber, NULL, new WeaponGuardHasAmmo(m_weapon)));

		m_fsm.AddTransition(new WeaponTransition(  m_hideB, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(  m_eject, _fin_, NULL)); // @TODO: workaround for missing BH event
		m_fsm.AddTransition(new WeaponTransition(m_chamber, _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e.m_magazine)
			m_dstMagazine = e.m_magazine;
		m_eject.m_dstMagazine = m_dstMagazine;

		super.OnEntry(e); // @NOTE: super after submachine init (prevent override from submachine start)
		wpnDebugPrint("[wpnfsm] WeaponEjectBullet m_mag=" + m_dstMagazine.ToString() + ", e.mag=" + e.m_magazine.ToString());
	}

	override void OnAbort (WeaponEventBase e)
	{
		m_dstMagazine = NULL;
		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dstMagazine = NULL;
		super.OnExit(e);
	}
};


