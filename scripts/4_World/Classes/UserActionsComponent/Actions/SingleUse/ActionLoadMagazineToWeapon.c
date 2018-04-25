class ActionLoadMagazineToWeapon: ActionSingleUseBase
{
	void ActionLoadMagazineToWeapon()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "Start reloading.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
		//m_CallbackClass = ActionLoadMagazineToWeaponCB;
		//m_CommandUID = -1;
		
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override int GetType()
	{
		return AT_LOAD_MAGAZINE_TO_WEAPON;
	}
		
	override string GetText()
	{
		return "Load weapon";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Weapon_Base wpn;
		Magazine mag;	
		if (Class.CastTo(wpn, item) && Class.CastTo(mag, target.GetObject()))
		{
			return player.GetWeaponManager().CanAttachMagazine(wpn, mag) || player.GetWeaponManager().CanSwapMagazine(wpn, mag);
		}
		return false;
	}
	
	override void Start( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Weapon_Base wpn;
		Magazine mag;
		if ( Class.CastTo(wpn, item) && Class.CastTo(mag, target.GetObject()) )
		{
			if ( player.GetWeaponManager().CanAttachMagazine(wpn, mag))
				player.GetWeaponManager().AttachMagazine(mag);
			else
				player.GetWeaponManager().SwapMagazine(mag);
		}
	}	
	
	override bool IsLocal()
	{
		return true;
	}
	
	override bool IsInstant()
	{
		return true;
	}
};