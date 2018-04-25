
// load bullet from att mag (no anim)
class WeaponChamberFromAttMag extends WeaponStateBase
{
	void WeaponChamberFromAttMag (Weapon_Base w = NULL, WeaponStateBase parent = NULL, int action = -1, int actionType = -1)
	{ }

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		int mi = m_weapon.GetCurrentMuzzle();
		chamberFromAttachedMagazine(m_weapon, mi);
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};

// load bullet from att mag (no anim)
class WeaponChamberFromAttMagOnExit extends WeaponStateBase
{
	void WeaponChamberFromAttMagOnExit (Weapon_Base w = NULL, WeaponStateBase parent = NULL, int action = -1, int actionType = -1)
	{ }

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		chamberFromAttachedMagazine(m_weapon, mi);

		super.OnExit(e);
	}
};



