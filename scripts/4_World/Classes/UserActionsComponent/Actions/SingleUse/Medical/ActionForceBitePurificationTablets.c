class ActionForceBitePurificationTablets: ActionForceConsumeSingle
{
	override int GetType()
	{
		return AT_FORCE_BITE_PURIFICATION_TABLETS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		//target.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};