class BulletHide extends WeaponStateBase
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_weapon.SelectionBulletHide();
	}

	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};

class BulletHide_W4T extends BulletHide
{
	override bool IsWaitingForActionFinish () { return true; }
};

