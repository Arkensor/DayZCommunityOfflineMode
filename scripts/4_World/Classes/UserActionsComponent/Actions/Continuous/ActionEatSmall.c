class ActionEatSmallCB : ActionEatCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_SMALL,UATimeSpent.DEFAULT);
	}
};

class ActionEatSmall: ActionEat
{
	void ActionEatSmall()
	{
		m_CallbackClass = ActionEatSmallCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATFRUIT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EATFRUIT;
	}
	
	override int GetType()
	{
		return AT_EAT_SMALL;
	}
};