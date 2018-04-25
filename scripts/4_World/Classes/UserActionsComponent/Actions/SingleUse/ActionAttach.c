/*
class ActionAttachCB
{
	override void CreateActionComponent()
	{
		
	}
}
*/
class ActionAttach: ActionSingleUseBase
{
	void ActionAttach()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've attached the object.";
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
		return AT_ATTACH;
	}
		
	override string GetText()
	{
		return "attach";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( player && targetObject && item )
		{
			string type = item.GetType();
			ref TStringArray slots = new TStringArray;
			ref TStringArray attachments = new TStringArray;
			
			g_Game.ConfigGetTextArray("cfgVehicles " + item.GetType() + " inventorySlot", slots);
			
			if ( slots.Count() <= 0 )
			{
				string one_slot;
				g_Game.ConfigGetText("cfgVehicles " + item.GetType() + " inventorySlot", one_slot);
				slots.Insert(one_slot);
			}
			
			g_Game.ConfigGetTextArray("cfgVehicles " + targetObject.GetType() + " attachments", attachments);
			
			if ( attachments.Count() <= 0 || slots.Count() <= 0)
			{
				return false;
			}

			for ( int i = 0; i < slots.Count(); i++ )
			{
				string slot = slots.Get(i);
				for ( int y = 0; y < attachments.Count(); y++ )
				{
					string att = attachments.Get(y);

					if ( slot == att )
					{
						ItemBase item_in_slot;
						EntityAI target_entity = EntityAI.Cast( targetObject );
						EntityAI item_entity = (EntityAI) item;
						
						int slot_index = item_entity.GetInventory().GetSlotId();
						
						item_in_slot = ItemBase.Cast( target_entity.GetInventory().FindAttachment( slot_index ) );
						if ( !item_in_slot )
						{
							return true;
						}
					}
				}
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = EntityAI.Cast(target.GetObject());
		EntityAI item_entity = EntityAI.Cast(item);
		int slot_index = item_entity.GetInventory().GetSlotId();
		target_entity.LocalTakeEntityAsAttachmentEx(item, slot_index);
	}
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = EntityAI.Cast(target.GetObject());
		EntityAI item_entity = EntityAI.Cast(item);
		int slot_index = item_entity.GetInventory().GetSlotId();
		target_entity.LocalTakeEntityAsAttachmentEx(item, slot_index);
	}
};