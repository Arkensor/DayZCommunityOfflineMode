class ActionActivateTrap: ActionInteractBase
{	
	void ActionActivateTrap()
	{
		m_MessageSuccess = "I've activated trap.";
		m_MessageFail = "I cannot activate trap.";
		//m_Animation = "open";
	}

	override int GetType()
	{
		return AT_ACTIVATE_TRAP;
	}

	override string GetText()
	{
		return "Activate the trap";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TrapBase) ) 
		{
			TrapBase trap = TrapBase.Cast( targetObject );
			
			if ( trap.IsActivable() )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TrapBase) ) 
		{
			TrapBase trap = TrapBase.Cast( targetObject );
			trap.StartActivate( player );
		}
	}
};