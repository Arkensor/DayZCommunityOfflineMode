class ActionTurnOffSpotlight: ActionInteractBase
{	
	void ActionTurnOffSpotlight()
	{
		m_MessageStart = "";
		m_MessageSuccess = "I've flipped the switch off";
	}
	
	override int GetType()
	{
		return AT_TURN_OFF_SPOTLIGHT;
	}

	override string GetText()
	{
		return "switch off spotlight";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
		ItemBase target_IB = ItemBase.Cast( target.GetObject() );
		
		if ( selection == Spotlight.SEL_REFLECTOR_COMP_U  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOff() )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject )
		{
			EntityAI target_EAI = EntityAI.Cast( targetObject );
			target_EAI.GetCompEM().SwitchOff();
		}
	}
};