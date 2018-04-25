class ActionClose: ActionSingleUseBase
{
	void ActionClose()
	{
		m_MessageSuccess = "I have closed it.";
		//m_Animation = "close";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_CLOSE;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "close";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item && item.IsOpen() ) 
		{
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		item.Close();
	}
};