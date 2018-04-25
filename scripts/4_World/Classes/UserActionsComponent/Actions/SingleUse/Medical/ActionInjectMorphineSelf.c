class ActionInjectMorphineSelf: ActionInjectSelf
{
	void ActionInjectMorphineSelf()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MORPHINE;
	}
	
	override int GetType()
	{
		return AT_INJECT_MORPHINE_S;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};