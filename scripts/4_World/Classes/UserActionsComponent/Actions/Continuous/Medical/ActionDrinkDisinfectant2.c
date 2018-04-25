class ActionDrinkDisinfectant2: ActionDrink
{
	override int GetType()
	{
		return AT_DRINK_DISINFECTANT2;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
	}
};