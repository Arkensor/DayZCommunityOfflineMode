class ActionBiteVitaminBottle: ActionConsumeSingle
{
	void ActionBiteVitaminBottle()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
	}

	override int GetType()
	{
		return AT_BITE_VITAMIN_BOTTLE;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};