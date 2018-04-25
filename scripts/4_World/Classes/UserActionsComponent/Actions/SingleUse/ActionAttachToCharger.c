class ActionAttachToCharger: ActionSingleUseBase
{
	void ActionAttachToCharger()
	{
		m_MessageSuccess = "I've attached the battery to the charger";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_ATTACH_TO_CHARGER;
	}
		
	override string GetText()
	{
		return "attach battery to charger";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI;
		if ( Class.CastTo(target_EAI, target.GetObject()) &&  item )
		{
			if ( target_EAI.HasEnergyManager()  &&  target_EAI.GetCompEM().CanReceivePlugFrom(item) )
			{
				ItemBase battery = ItemBase.Cast( target_EAI.GetCompEM().GetPluggedDevice() );
				
				if (battery)
				{
					return false; // Something is already being charged from battery charger
				}
				else
				{
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase target_IB;
		Class.CastTo(target_IB,  target.GetObject() ); // cast to ItemBase
		target_IB.LocalTakeEntityAsAttachment(item);
	}

	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase target_IB;
		Class.CastTo(target_IB,  target.GetObject() ); // cast to ItemBase
		target_IB.LocalTakeEntityAsAttachment(item);
	}
};