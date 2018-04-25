class ActionBiteCharcoalTablets: ActionConsumeSingle
{
	void ActionBiteCharcoalTablets()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
	}

	override int GetType()
	{
		return AT_BITE_CHARCOAL_TABLETS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
	}
};