class ActionTakeASip: ActionTakeABite
{
	void ActionTakeASip()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINKCAN;
		m_MessageSuccess = "I took a sip.";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return false;
	}
	
	override int GetType()
	{
		return AT_TAKE_A_SIP;
	}

	override string GetText()
	{
		return "take a sip";
	}
};