class ActionForceDrinkAlcohol: ActionForceDrink
{
	override int GetType()
	{
		return AT_FORCE_DRINK_ALCOHOL;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		//ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};