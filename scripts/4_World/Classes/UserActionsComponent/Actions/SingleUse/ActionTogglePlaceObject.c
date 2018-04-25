class ActionTogglePlaceObject: ActionSingleUseBase
{

	void ActionTogglePlaceObject()
	{
		/*
		m_CommandUID        	= DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask       		= DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail 		= "";
		m_MessageStart 			= "";
		m_MessageSuccess 		= "";
		m_MessageFail 			= "";
		*/
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
		
	override int GetType()
	{
		return AT_TOGGLE_PLACE_OBJECT;
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool IsLocal()
	{
		return true;
	}
	
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}

	override string GetText()
	{
		return "Toggle placing";
	}
	
	
	override void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		player.TogglePlacingLocal();
	}
};