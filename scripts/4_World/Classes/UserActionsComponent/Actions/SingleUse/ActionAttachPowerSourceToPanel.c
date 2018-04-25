class ActionAttachPowerSourceToPanel: ActionSingleUseBase
{
	void ActionAttachPowerSourceToPanel()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've attached the power source to the panel.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override int GetType()
	{
		return AT_ATTACH_POWER_SOURCE_TO_PANEL;
	}
		
	override string GetText()
	{
		return "attach power source";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		if ( player && target_entity && item )
		{			
			if ( target_entity.IsStaticTransmitter() && target_entity.GetInventory().AttachmentCount() == 0 )	//has any power source attachment attached
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		target_entity.LocalTakeEntityAsAttachment( item );
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		target_entity.LocalTakeEntityAsAttachment( item );
	}
}