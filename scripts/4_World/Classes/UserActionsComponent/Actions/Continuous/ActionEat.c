class ActionEatCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_SMALL,UATimeSpent.DEFAULT);
	}
};

class ActionEat: ActionConsume
{
	void ActionEat()
	{
		m_CallbackClass = ActionEatCB;
		m_MessageStartFail = "There's nothing to eat.";
		m_MessageStart = "I have started eating.";
		m_MessageSuccess = "I have finished eating.";
		m_MessageFail = "Player moved and eating was canceled.";
		m_MessageCancel = "I stopped eating.";
		//m_Sound = "EatingSoft_0";
		//m_Animation = "EAT";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_EAT;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "eat";
	}
};