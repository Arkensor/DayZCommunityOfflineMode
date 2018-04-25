class InventoryItem extends EntityAI
{	
	proto native InventoryItemType GetInventoryItemType();

	//! Some inventoryItem devices can be switched on/off (radios, transmitters)
	proto native void SwitchOn(bool onOff);
	//! Some inventoryItem devices can be switched on/off (radios, transmitters)
	proto native bool	IsOn();	
	
	proto native MeleeCombatData GetMeleeCombatData();	
	
	override void EEUsed(Man owner)
	{
		if (GetGame().GetMission())
		{
			GetGame().GetMission().OnItemUsed(this, owner);
		}
	}
	
	void OnRightClick()
	{
	
	}

	event bool OnUseFromInventory(Man owner)
	{
		return false;
	}

	//! Get tooltip text
	string GetTooltip()
	{
		return ConfigGetString("descriptionShort");
	}

	override bool IsInventoryItem()
	{
		return true;
	}
	
	int GetMeleeMode()
	{
		return 0;
	}

	int GetMeleeHeavyMode()
	{
		return 1;
	}	
	
	int GetMeleeSprintMode()
	{
		return 2;
	}
	
	override bool IsMeleeWeapon()
	{
		if (ConfigIsExisting("isMeleeWeapon"))
			return ConfigGetBool("isMeleeWeapon");
		else
			return false; // TODO: allowed for everything that is not disabled in config (primarily for anim testing)
	}
};