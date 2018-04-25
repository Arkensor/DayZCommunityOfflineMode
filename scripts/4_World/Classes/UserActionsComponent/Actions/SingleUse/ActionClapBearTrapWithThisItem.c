class ActionClapBearTrapWithThisItem: ActionSingleUseBase
{
	void ActionClapBearTrapWithThisItem()
	{
		m_MessageSuccess = "I've poked the bear trap";
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_CLAP_BEARTRAP_WITH_THIS_ITEM;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "prod bear trap";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		TrapBase target_TB;

		if ( Class.CastTo(target_TB,  target.GetObject() ) &&  item )
		{		
			if (target_TB.IsActive())
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		TrapBase target_TB;
		Class.CastTo(target_TB,  target.GetObject() );
		
		target_TB.SnapOnObject(item);
	}
};