class ActionUnplugThisByCord: ActionInteractBase
{
	void ActionUnplugThisByCord()
	{
		m_MessageSuccess = "I've unplugged and folded the cord.";
		m_MessageStart = "";
	}

	override int GetType()
	{
		return AT_UNPLUG_THIS_BY_CORD;
	}
		
	override string GetText()
	{
		return "unplug by cord";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		
		if ( target_IB  &&  target_IB.HasEnergyManager() )
		{
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());			
			if ( selection == ComponentEnergyManager.SEL_CORD_PLUGGED )
			{
				return true;
			}
			
			// Special case for unfolded spotlight
			if ( selection == Spotlight.SEL_CORD_PLUGGED_U )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		ItemBase target_IB = ItemBase.Cast( target.GetObject() ); // cast to ItemBase
		target_IB.GetCompEM().UnplugThis();
		
		if ( target_IB.IsInherited(Spotlight) )
		{
			target_IB.HideSelection ( Spotlight.SEL_CORD_PLUGGED_U );
			target_IB.ShowSelection ( Spotlight.SEL_CORD_FOLDED_U );
		}
	}
};