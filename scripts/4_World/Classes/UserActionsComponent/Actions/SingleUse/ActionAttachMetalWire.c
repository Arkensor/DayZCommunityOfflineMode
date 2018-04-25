class ActionAttachMetalWire: ActionSingleUseBase
{
	void ActionAttachMetalWire()
	{
		m_MessageSuccess = "Metal wire attached.";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_ATTACH_METAL_WIRE;
	}
		
	override string GetText()
	{
		return "Attach metal wire";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() );
		
		if ( target_EAI.GetInventory().CanAddAttachment(item) )
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