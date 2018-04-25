class ActionTurnOnSpotlight: ActionInteractBase
{	
	void ActionTurnOnSpotlight()
	{
		m_MessageSuccess = "I've flipped the switch on.";
		m_MessageStart = "";
	}
	
	override int GetType()
	{
		return AT_TURN_ON_SPOTLIGHT;
	}

	override string GetText()
	{
		return "switch on spotlight";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());

		if ( selection == Spotlight.SEL_REFLECTOR_COMP_U  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOn() )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if (targetObject)
		{
			EntityAI target_EAI = EntityAI.Cast( targetObject );
			target_EAI.GetCompEM().SwitchOn();
		}
	}
};