class ActionForceABiteCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(UAQuantityConsumed.EAT_NORMAL);
	}
};

class ActionForceABite: ActionForceConsumeSingle
{
	void ActionForceABite()
	{		
		m_CallbackClass = ActionForceABiteCB;
		m_MessageStart = "Player started feeding you a bite.";
		m_MessageSuccess = "Player fed you a bite.";
		m_MessageFail = "Player moved and shoving something in your throat was canceled.";
		//m_Animation = "eat";
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}
	
	override int GetType()
	{
		return AT_FORCE_A_BITE;
	}

	override string GetText()
	{
		return "feed a bite";
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if ( item.GetQuantity() <= 0 )
		{
			item.SetQuantity(0);
		}
	}
};