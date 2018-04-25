class ActionDrinkAlcohol: ActionDrink
{
	override int GetType()
	{
		return AT_DRINK_ALCOHOL;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
	}
};