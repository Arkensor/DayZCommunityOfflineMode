class ActionDrinkCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC2,UATimeSpent.DEFAULT);
	}
};

class ActionDrink: ActionConsume
{
	void ActionDrink()
	{
		m_CallbackClass = ActionDrinkCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_DRINK;
		m_MessageStartFail = "There's nothing to drink.";
		m_MessageStart = "I have started drinking.";
		m_MessageSuccess = "I have finished drinking.";
		m_MessageFail = "Player moved and drinking was canceled.";
		m_MessageCancel = "I stopped drinking.";
		//m_Sounds.Insert("DrinkBottle_0");
		//m_Sounds.Insert("DrinkBottle_1");
		//m_Animation = "DRINK";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_DRINK;
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override string GetText()
	{
		return "drink";
	}
};