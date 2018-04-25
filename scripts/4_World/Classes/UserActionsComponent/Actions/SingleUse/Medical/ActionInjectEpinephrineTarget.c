class ActionInjectEpinephrineTarget: ActionInjectTarget
{
	void ActionInjectEpinephrineTarget()
	{
		//m_Animation = "INJECTEPINENT";
		m_MessageStart = "Player started injecting you epinephrine.";
		m_MessageSuccess = "Player finished injecting you epinephrine.";
	}

	override int GetType()
	{
		return AT_INJECT_EPINEPHRINE_T;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_HEART_ATTACK);
		ntarget.GetStatShock().Set(0);
	}
};