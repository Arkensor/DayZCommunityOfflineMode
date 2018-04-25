class MagazineHide extends WeaponStateBase
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_weapon.SelectionMagazineHide();
	}

	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};

class MagazineHide_W4T extends MagazineHide
{
	override bool IsWaitingForActionFinish () { return true; }
};

