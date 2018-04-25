class ActionTurnOnPowerGenerator: ActionInteractBase
{	
	void ActionTurnOnPowerGenerator()
	{
		m_MessageSuccess = "I've started the generator.";
		m_MessageStart = "";
	}	
	
	override int GetType()
	{
		return AT_TURN_ON_POWER_GENERATOR;
	}

	override string GetText()
	{
		return "switch on";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		PowerGenerator target_PG = PowerGenerator.Cast( targetObject );

		float c_index = target.GetComponentIndex();		
		string selection = targetObject.GetActionComponentName(c_index);
		
		if ( target_PG.HasEnergyManager()  &&  target_PG.GetCompEM().CanSwitchOn()  &&  selection == "switch"  &&  target_PG.HasSparkplug() )
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
			ItemBase target_IB = ItemBase.Cast( targetObject );
			target_IB.GetCompEM().SwitchOn();
		}
	}
};