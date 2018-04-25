class ActionForceBiteCharcoalTablets: ActionForceConsumeSingle
{
	override int GetType()
	{
		return AT_FORCE_BITE_CHARCOAL_TABLETS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
	}
};