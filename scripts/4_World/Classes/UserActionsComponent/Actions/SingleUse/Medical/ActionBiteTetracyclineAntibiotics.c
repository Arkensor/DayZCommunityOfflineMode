class ActionBiteTetracyclineAntibiotics: ActionConsumeSingle
{
	void ActionBiteTetracyclineAntibiotics()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}

	override int GetType()
	{
		return AT_BITE_TETRACYCLINE_ANTIBIOTICS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};