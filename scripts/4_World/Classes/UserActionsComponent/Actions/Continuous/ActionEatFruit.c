class ActionEatFruitCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL,UATimeSpent.DEFAULT);
	}
};

class ActionEatFruit: ActionEat
{
	void ActionEatFruit()
	{
		m_CallbackClass = ActionEatFruitCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATFRUIT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EATFRUIT;
	}

	override int GetType()
	{
		return AT_EAT_FRUIT;
	}
			
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Edible_Base food_item = Edible_Base.Cast( item );
		
		if( food_item.IsFruit() )
		{	
			return true;
		}
		
		return false;
	}
}
