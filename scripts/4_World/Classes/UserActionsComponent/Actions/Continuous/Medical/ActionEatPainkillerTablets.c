class ActionEatPainkillerTablets: ActionConsume
{
	void ActionEatPainkillerTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EATTABLETS;
	}
	
	override int GetType()
	{
		return AT_EAT_PAINKILLER_TABLETS;
	}

	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
	}
};