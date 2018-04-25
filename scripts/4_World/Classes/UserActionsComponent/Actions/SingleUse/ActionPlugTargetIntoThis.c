class ActionPlugTargetIntoThis: ActionSingleUseBase
{
	void ActionPlugTargetIntoThis()
	{
		m_MessageStartFail = "m_MessageStartFail";
		m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "m_MessageSuccess";
		m_MessageFail = "m_MessageFail";
		m_MessageCancel = "m_MessageCancel";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_PLUG_TARGET_INTO_THIS;
	}
	
	override string GetText()
	{
		return "connect together";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() );
		
		if ( target_EAI.HasEnergyManager()  &&  item.HasEnergyManager() )
		{
			if ( !target_EAI.GetCompEM().IsPlugged()  &&  item.GetCompEM().CanReceivePlugFrom(target_EAI) )
			{
				// Check if the target_EAI is a device which is supposed to be plugged into something
				ref TIntArray cfg_actions = new TIntArray;
				g_Game.ConfigGetIntArray("cfgVehicles " +target_EAI.GetType() + " SingleUseActions", cfg_actions);	
				
				if ( cfg_actions )
				{
					for ( int i = 0;  i < cfg_actions.Count();  i++ )
					{
						int action = cfg_actions.Get(i);
						
						if ( action == AT_PLUG_THIS_INTO_TARGET )
						{
							return true;
						}
					}
				}
			}
			
			// Special case for vehicle batteries
			if ( item.IsInherited(VehicleBattery))
			{
				MetalWire metal_wire = MetalWire.Cast( item.GetCompEM().GetPluggedDevice() );
				
				if ( metal_wire  &&  metal_wire.GetCompEM().CanReceivePlugFrom(target_EAI) )
				{
					return true; // We can power the target from vehicle battery because it has metal wire attached.
				}
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
		ItemBase target_IB = ItemBase.Cast( target.GetObject() );
		
		if ( item.IsInherited(VehicleBattery) )
		{
			// Car/truck batteries can have a metal wire attached through which they can power common electric appliances
			
			MetalWire metal_wire = MetalWire.Cast( item.GetCompEM().GetPluggedDevice() );
			
			if (metal_wire)
			{
				target_IB.GetCompEM().PlugThisInto(metal_wire);
			}
		}
		else
		{
			// Everything else in general
			target_IB.GetCompEM().PlugThisInto(item);
		}
		
		// Special case for spotlights
		if ( target_IB.IsInherited(Spotlight) )
		{
			Spotlight spotlight = Spotlight.Cast( target_IB );
			spotlight.Unfold();
			vector player_ori = player.GetOrientation();
			vector s_pos = spotlight.GetPosition();
			spotlight.SetOrientation(player_ori);
			spotlight.SetPosition(s_pos);
			spotlight.PlaceOnSurface();
		}
		
		target_IB.GetInventory().TakeEntityAsAttachment( InventoryMode.LOCAL, item );
		player.LocalDropEntity( item );
	}
};