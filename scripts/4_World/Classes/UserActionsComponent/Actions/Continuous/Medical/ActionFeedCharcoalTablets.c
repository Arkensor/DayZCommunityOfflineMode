class ActionFeedCharcoalTablets: ActionForceConsume
{
	override int GetType()
	{
		return AT_FEED_CHARCOAL_TABLETS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
	}
};