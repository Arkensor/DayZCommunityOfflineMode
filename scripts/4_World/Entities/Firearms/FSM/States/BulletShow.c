class BulletShow extends WeaponStateBase
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_weapon.SelectionBulletShow();
	}

	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};

class BulletShow_W4T extends BulletShow
{
	override bool IsWaitingForActionFinish () { return true; }
};

