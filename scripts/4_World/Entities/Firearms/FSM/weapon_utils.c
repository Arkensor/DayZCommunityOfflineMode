bool chamberFromAttachedMagazine (Weapon_Base weapon, int muzzleIndex)
{
	Magazine mag = weapon.GetMagazine(muzzleIndex);
	if (mag)
	{
		wpnDebugPrint("[wpnfsm] chamberFromAttachedMagazine, using attached magazine mag=" + mag.ToString());
		float damage;
		string type;
		if (mag && mag.LocalAcquireCartridge(damage, type))
		{
			weapon.SelectionBulletShow();
			wpnDebugPrint("[wpnfsm] chamberFromAttachedMagazine, ok - cartridge acquired: dmg=" + damage + " type=" + type);
		}
		else
			Error("[wpnfsm] chamberFromAttachedMagazine, error - cannot take cartridge from magazine");

		if (weapon.LoadChamber(muzzleIndex, damage, type))
		{
			wpnDebugPrint("[wpnfsm] chamberFromAttachedMagazine, ok - loaded chamber");
			return true;
		}
		else
			Error("[wpnfsm] chamberFromAttachedMagazine, error - cannot load chamber!");
	}
	else
	{
		Error("[wpnfsm] chamberFromAttachedMagazine, error - no magazine attached");
	}
	return false;
}

void ejectBulletAndStoreInMagazine (Weapon_Base weapon, int muzzleIndex, Magazine mag, DayZPlayer p)
{
	float damage = 0;
	string type = string.Empty;
	string magazineTypeName = weapon.GetCartridgeMagazineTypeName(muzzleIndex);
	if (weapon.EjectCartridge(muzzleIndex, damage, type))
		wpnDebugPrint("[wpnfsm] ejectBulletAndStoreInMagazine, ejected chambered cartridge");
	else
		Error("[wpnfsm] ejectBulletAndStoreInMagazine, error - cannot eject chambered cartridge!");

	bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
	if (is_single_or_server)
	{
		if (mag == NULL)
		{
			// no magazine configured in parent state, looking in inventory
			if (DayZPlayerUtils.HandleStoreCartridge(p, weapon, muzzleIndex, damage, type, magazineTypeName))
				wpnDebugPrint("[wpnfsm] ejectBulletAndStoreInMagazine, ok - cartridge stored in magazine");
			else
				Error("[wpnfsm] ejectBulletAndStoreInMagazine, error - cannot store cartridge!");
		}
		else
		{
			if (mag.ServerStoreCartridge(damage, type))
			{
				mag.SetSynchDirty();
				wpnDebugPrint("[wpnfsm] ejectBulletAndStoreInMagazine, ok - cartridge stored in user defined magazine");
			}
			else
				Error("[wpnfsm] ejectBulletAndStoreInMagazine, error - cannot store cartridge in magazine");
		}
	}
}

bool magazinesHaveEqualSizes (notnull Magazine mag, notnull Magazine mag2)
{
	int w, h;
	GetGame().GetInventoryItemSize(mag, w, h);
	int w2, h2;
	GetGame().GetInventoryItemSize(mag2, w2, h2);
	if (w == w2 && h == h2)
	{
		wpnDebugPrint("[wpnfsm] guard - same inventory sizes");
		return true;
	}

	wpnDebugPrint("[wpnfsm] guard - different inventory sizes");
	return false;
}

