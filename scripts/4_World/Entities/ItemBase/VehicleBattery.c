class VehicleBattery : ItemBase
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		string obj_type = parent.GetType();
		if ( parent.IsInherited(BatteryCharger) )
		{
			BatteryCharger charger = BatteryCharger.Cast( parent );
			return charger.CanReceiveAttachment(this);
		}
		
		return true;
	}
	
	
	override bool CanDetachAttachment ( EntityAI parent )
	{
		return true;
	}
	
	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		if ( HasEnergyManager() )
		{
			ItemBase powered_device = ItemBase.Cast( GetCompEM().GetPluggedDevice() ); // Should return metal wire or barbed wire attachment
			
			if ( powered_device  &&  powered_device.IsInherited( MetalWire ) )
			{
				return true;
			}
			else if (powered_device)
			{
				return false;
			}
		}

		return true;
	}
	
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		if ( HasEnergyManager() )
		{
			ItemBase powered_device = ItemBase.Cast( GetCompEM().GetPluggedDevice() ); // Should return metal wire or barbed wire attachment
			
			if ( powered_device )
			{
				if ( powered_device.IsInherited( MetalWire ) )
				{
					powered_device.GetCompEM().UnplugAllDevices();
				}
				else
				{
					this.GetCompEM().UnplugAllDevices();
				}
			}
		}
	}
}
