class ActionPackTent: ActionInteractBase
{
	void ActionPackTent()
	{
		m_MessageSuccess = "I've packed tent.";
		m_MessageFail = "I cannot pack tent.";
		//m_Animation = "open";
	}

	override int GetType()
	{
		return AT_PACK_TENT;
	}

	override string GetText()
	{
		return "pack tent";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( player && targetObject )
		{
			float max_action_distance = 1; //m_MaximalActionDistance;
			if ( targetObject.IsInherited(CarTent) ) 
			{
				max_action_distance = 5.0;
			}
			else if ( targetObject.IsInherited(LargeTent) ) 
			{
				max_action_distance = 4.0;
			}
			else if ( targetObject.IsInherited(MediumTent) )
			{
				max_action_distance = 3.0;
			}
			float distance = Math.AbsInt(vector.Distance(targetObject.GetPosition(),player.GetPosition()));
			if (  distance <= max_action_distance )	
			{
				if ( targetObject.IsInherited(TentBase) ) 
				{
					TentBase tent = TentBase.Cast( targetObject );
					if ( tent.CanBePacked() )
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TentBase) ) 
		{
			TentBase tent = TentBase.Cast( targetObject );
			tent.Pack();
		}
	}
};