class ActionInjectMorphineTarget: ActionInjectTarget
{
	void ActionInjectMorphineTarget()
	{
		m_MessageStart = "Player started injecting you morphine.";
		m_MessageSuccess = "Player finished injecting you morphine.";
	//	m_Animation = "INJECTEPIPENT";
	}

	override int GetType()
	{
		return AT_INJECT_MORPHINE_T;
	}
		
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		//target.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};