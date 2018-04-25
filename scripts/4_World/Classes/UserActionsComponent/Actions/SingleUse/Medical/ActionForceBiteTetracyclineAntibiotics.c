class ActionForceBiteTetracyclineAntibiotics: ActionForceConsumeSingle
{
	override int GetType()
	{
		return AT_FORCE_BITE_TETRACYCLINE_ANTIBIOTICS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		//target.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};