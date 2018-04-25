class ActionFeedTetracyclineAntibiotics: ActionForceConsume
{
	override int GetType()
	{
		return AT_FEED_TETRACYCLINE_ANTIBIOTICS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		//ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};