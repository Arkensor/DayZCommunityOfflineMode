class ActionInjectEpinephrineSelf: ActionInjectSelf
{
	void ActionInjectEpinephrineSelf()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MORPHINE;
	}
	
	override int GetType()
	{
		return AT_INJECT_EPINEPHRINE_S;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_HEART_ATTACK);
		player.GetStatShock().Set(0);
	}
};