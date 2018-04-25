
class WeaponEjectCasing extends WeaponStateBase
{
	void WeaponEjectCasing (Weapon_Base w = NULL, WeaponStateBase parent = NULL) { }

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		wpnDebugPrint("[wpnfsm] ejected fired out casing");
		int mi = m_weapon.GetCurrentMuzzle();
		m_weapon.EjectCasing(mi);
		m_weapon.SelectionBulletHide();
	}
};

class WeaponEjectCasing_W4T extends WeaponEjectCasing
{
	override bool IsWaitingForActionFinish () { return true; }
};

// eject casing and load bullet from att mag (no anim)
class WeaponEjectCasingAndChamberFromAttMag extends WeaponEjectCasing
{
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

class WeaponEjectCasingAndChamberFromAttMag_W4T extends WeaponEjectCasingAndChamberFromAttMag
{
	override bool IsWaitingForActionFinish () { return true; }
};

