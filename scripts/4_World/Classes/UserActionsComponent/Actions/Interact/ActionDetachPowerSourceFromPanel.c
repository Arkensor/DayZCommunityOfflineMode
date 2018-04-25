
class ActionDetachPowerSourceFromPanel: ActionInteractBase
{
	void ActionDetachPowerSourceFromPanel()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've detached the power source from the panel.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
	}
	
	override int GetType()
	{
		return AT_DETACH_POWER_SOURCE_FROM_PANEL;
	}
		
	override string GetText()
	{
		return "detach power source";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		
		if ( player && target_object )
		{
			EntityAI target_entity = EntityAI.Cast( target.GetObject() );
			string selection = target_object.GetActionComponentName( target.GetComponentIndex() );
			
			
			if ( target_entity && target_entity.GetInventory() && target_entity.GetInventory().AttachmentCount() > 0 && selection == "power_source" )	//has any power source attachment attached
			{
				return true;
			}
		}
		
		return false;
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Process(player, target, item, acdata);
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Process(player, target, item, acdata);
	}

	void Process( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		EntityAI attachment;
		int attachments_count = target_entity.GetInventory().AttachmentCount();
		for ( int j = 0; j < attachments_count; j++ )				//find any attached power source
		{
			// @TODO: vezme prvni att?
			attachment = target_entity.GetInventory().GetAttachmentFromIndex( j );
			if ( attachment )
				break;
		}
		
		if ( attachment )
		{
			player.PredictiveTakeEntityToHands(attachment );
		}
	}
}