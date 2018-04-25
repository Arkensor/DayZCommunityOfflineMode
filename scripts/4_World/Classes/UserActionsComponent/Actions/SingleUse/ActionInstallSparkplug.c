/*
	This is a generic user action for attaching sparkplug on any EntityAI object. The receiver must have a "sparkplug" selection in its View Geometry that the player can look at.
*/


class ActionInsertSparkplug: ActionSingleUseBase
{
	void ActionInsertSparkplug()
	{
		m_MessageSuccess = "I've inserted the sparkplug.";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_INSERT_SPARKPLUG;
	}
		
	override string GetText()
	{
		return "insert sparkplug";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() );
		string selection = target_EAI.GetActionComponentName(target.GetComponentIndex());
		
		if ( selection == "sparkplug"  &&  target_EAI.GetInventory().CanAddAttachment(item) )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() ); // cast to ItemBase
		target_EAI.LocalTakeEntityAsAttachment (item);
	}
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() ); // cast to ItemBase
		target_EAI.LocalTakeEntityAsAttachment (item);
	}
};