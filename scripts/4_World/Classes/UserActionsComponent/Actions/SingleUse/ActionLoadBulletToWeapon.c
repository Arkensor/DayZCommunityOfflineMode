/*
class ActionAttachCB
{
	override void CreateActionComponent()
	{
		
	}
}
*/
class ActionLoadBulletToWeapon: ActionSingleUseBase
{
	void ActionLoadBulletToWeapon()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "Start chambering.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override int GetType()
	{
		return AT_LOAD_BULLET_TO_WEAPON;
	}
		
	override string GetText()
	{
		return "Load bullet";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Weapon_Base wpn;
		Magazine mag;
		if( Class.CastTo(wpn, item) && Class.CastTo(mag, target.GetObject()) )
			return player.GetWeaponManager().CanLoadBullet(wpn, mag);
		return false;
	}
	
	override void Start( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Magazine mag;
		if ( Class.CastTo(mag, target.GetObject()) )
		{
			player.GetWeaponManager().LoadBullet(mag);
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