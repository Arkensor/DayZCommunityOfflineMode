class RifleReChambering extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	Magazine m_dstMagazine; /// destination of the ejected cartridge
	Magazine m_srcMagazine; /// source of the loaded cartridge

	ref WeaponStateBase m_start;
	ref WeaponEjectBullet_Cartridge m_eject;
	ref WeaponChambering_Cartridge m_chamber;
	ref WeaponChambering_W4T m_w4t;

	void RifleReChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectBullet_Cartridge(m_weapon, this);
		m_chamber = new WeaponChambering_Cartridge(m_weapon, this);
		m_w4t = new WeaponChambering_W4T(m_weapon, this);
		// events
		WeaponEventAnimBulletEject __be_ = new WeaponEventAnimBulletEject;
		WeaponEventAnimBulletShow  __bs_ = new WeaponEventAnimBulletShow;
		WeaponEventAnimBulletInChamber __bc_ = new WeaponEventAnimBulletInChamber;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start  , __be_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(m_eject  , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bc_, m_w4t));
		m_fsm.AddTransition(new WeaponTransition(m_w4t    , _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] WeaponRechamber, mag=" + e.m_magazine.ToString());
		m_srcMagazine = e.m_magazine;
		m_chamber.m_srcMagazine = m_srcMagazine;

		// prepare magazine for ejected ammo
		int mi = m_weapon.GetCurrentMuzzle();
		string magazineTypeName = m_weapon.GetCartridgeMagazineTypeName(mi);
		float damage = 0.0;
		string type;
		if (m_weapon.GetCartridgeInfo(mi, damage, type))
		{
			bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			if (is_single_or_server)
			{
				m_dstMagazine = DayZPlayerUtils.SelectStoreCartridge(e.m_player, m_weapon, mi, m_srcMagazine, damage, magazineTypeName);
				if (!m_dstMagazine)
				{
					Error("[wpnfsm]   WeaponRechamber - error, cannot prepare mag for catridge, magType=" + magazineTypeName);
				}
			}
		}
		m_eject.m_dstMagazine = m_dstMagazine;
		super.OnEntry(e); // @NOTE: super after submachine init (prevent override from submachine start)

	}

	override void OnAbort (WeaponEventBase e)
	{
		m_srcMagazine = NULL;
		m_dstMagazine = NULL;
		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_srcMagazine = NULL;
		m_dstMagazine = NULL;
		super.OnExit(e);
	}
};

