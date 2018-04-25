class ActionEatTetracyclineAntibiotics: ActionConsume
{
	void ActionEatTetracyclineAntibiotics()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EATPILLS;
	}

	override int GetType()
	{
		return AT_EAT_TETRACYCLINE_ANTIBIOTICS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};