class ActionPlugIn: ActionSingleUseBase
{
	void ActionPlugIn()
	{
		
		m_MessageStartFail = "m_MessageStartFail";
		m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "m_MessageSuccess";
		m_MessageFail = "m_MessageFail";
		m_MessageCancel = "m_MessageCancel";
		
		//m_TimeToCompleteAction = 4;
		////m_MaximalActionDistance = 1;
		//m_Animation = "eat";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	

	override int GetType()
	{
		return AT_PLUG_THIS_INTO_TARGET;
	}
		
	override string GetText()
	{
		return "plug in";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase target_IB = ItemBase.Cast( target.GetObject() );
		
		if ( target_IB  &&  item )
		{
			if ( item.HasEnergyManager()  &&  !item.GetCompEM().IsPlugged()  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanReceivePlugFrom(item) )
			{
				return true;
			}
			
			ItemBase attached_device = GetAttachedDevice(target_IB);
			
			if (attached_device)
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		
		if ( target_IB.HasEnergyManager() )
		{
			ItemBase attached_device = GetAttachedDevice(target_IB);
			
			if (attached_device)
			{
				target_IB = attached_device;
			}
			
			item.GetCompEM().PlugThisInto(target_IB);
		
			if ( !player.IsPlacingServer() )
			{
				Process(player, target, item, acdata);
			}
		}
	}
	
	void Process( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );

		target_IB.GetInventory().TakeEntityAsAttachment( InventoryMode.LOCAL, item );
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if ( !player.IsPlacingLocal() )
		{
			player.TogglePlacingLocal();
		}
		else
		{
			Process(player, target, item, acdata);
		}
	}
	
	ItemBase GetAttachedDevice(ItemBase parent)
	{
		string parent_type = parent.GetType();
		
		if ( parent.IsInherited(CarBattery)  ||  parent.IsInherited(TruckBattery) )
		{
			ItemBase parent_attachment = ItemBase.Cast( parent.GetAttachmentByType(MetalWire) );
			
			if (!parent_attachment)
			{
				parent_attachment = ItemBase.Cast( parent.GetAttachmentByType(BarbedWire) );
			}
			return parent_attachment;
		}
		
		return NULL;
	}
};