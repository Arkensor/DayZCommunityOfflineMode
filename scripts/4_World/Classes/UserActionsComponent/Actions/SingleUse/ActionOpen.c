class ActionOpen: ActionSingleUseBase
{
	void ActionOpen()
	{
		m_MessageSuccess = "I have opened it.";
		//m_Animation = "open";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_OPEN;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item && !item.IsOpen() )
		{ 
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		item.Open();
	}
};