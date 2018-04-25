class ActionEatMeatCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL,UATimeSpent.DEFAULT);
	}
};

class ActionEatMeat: ActionEat
{
	void ActionEatMeat()
	{
		m_CallbackClass = ActionEatMeatCB;
	}

	override int GetType()
	{
		return AT_EAT_MEAT;
	}
			
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Edible_Base food_item = Edible_Base.Cast( item );
		if ( food_item )
		{			
			if ( food_item.IsMeat() && food_item.IsFoodRaw() )
			{
				PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
				if( module_lifespan )
				{
					module_lifespan.UpdateBloodyHandsVisibility( player, true );
				}
			}
		}
	}
}
