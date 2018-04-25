class ActionForceDrinkDisinfectant: ActionForceDrink
{
	override int GetType()
	{
		return AT_FORCE_DRINK_DISINFECTANT;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
	}
};