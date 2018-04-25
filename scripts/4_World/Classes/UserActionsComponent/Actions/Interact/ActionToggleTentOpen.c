class ActionToggleTentOpen: ActionInteractBase
{
	void ActionToggleTentOpen()
	{
		m_MessageSuccess = "I've performed action.";
		m_MessageFail = "I cannot perform action.";
		//m_Animation = "open";
	}

	override int GetType()
	{
		return AT_TOGGLE_TENT_OPEN;
	}

	override string GetText()
	{
		return "toggle";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( player && targetObject )
		{
			float max_action_distance = 1; //m_MaximalActionDistance;
			
			if ( targetObject.IsInherited(CarTent) ) max_action_distance = 5.0;
			else if ( targetObject.IsInherited(LargeTent) ) max_action_distance = 4.0;
			else if ( targetObject.IsInherited(MediumTent) ) max_action_distance = 3.0;
			
			float distance = Math.AbsInt(vector.Distance(targetObject.GetPosition(),player.GetPosition()));
			
			if (  distance <= max_action_distance /*&& player.IsFacingTarget(targetObject) */ )	
			{
				if ( targetObject.IsInherited(TentBase) ) 
				{
					string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
					
					TentBase tent = TentBase.Cast( targetObject );
					
					if ( tent.CanToggleSelection(selection) )
					{
						return true;
					}
				}
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TentBase) ) 
		{
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			TentBase tent = TentBase.Cast( targetObject );
			tent.ToggleSelection( selection );
			
			//regenerate pathgraph
			tent.SetAffectPathgraph( true, false );
			
			if (item.CanAffectPathgraph())
			{
				//Start update
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, tent);
			}
		}
	}
};