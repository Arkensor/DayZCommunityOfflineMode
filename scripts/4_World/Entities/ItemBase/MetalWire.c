class MetalWire extends ItemBase
{
	static string SEL_WIRE_ROLLED	= "rolled";
	static string SEL_WIRE_PREFIX	= "Att_";
	static string SEL_WIRE_SUFIX	= "_plugged";
	static string SEL_PLUG_SUFIX	= "_plug";
	
	void MetalWire()
	{
		
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( parent.IsInherited(VehicleBattery) )
		{
			ItemBase battery_owner = ItemBase.Cast( parent.GetHierarchyParent() );
			
			if ( battery_owner  &&  battery_owner.IsInherited(BatteryCharger) ) // Battery Charger check here allows the Metal Wire to be attached on car battery directly from player's inventory (backpack)
			{
				return false;
			}
		}
		
		return true;
	}
	
	void UpdateAllSelections()
	{
		HideAllSelections();
		EntityAI energy_source = GetCompEM().GetEnergySource();
		
		if (energy_source)
		{
			string ES_model = energy_source.GetModelName();
			EntityAI powered_device = GetCompEM().GetPluggedDevice();
			
			if (powered_device)
			{
				// Show metal wire selection
				string selection_wire = SEL_WIRE_PREFIX + ES_model + SEL_WIRE_SUFIX;
				ShowSelection(selection_wire);
				
				// Show plug selection
				string selection_plug = SEL_WIRE_PREFIX + ES_model + SEL_PLUG_SUFIX;
				selection_plug.ToLower();
				ShowSelection(selection_plug);
				
				// Set plug's texture
				int selection_index = GetHiddenSelectionIndex(selection_plug);
				string texture_path = powered_device.GetCompEM().GetCordTextureFile();
				SetObjectTexture( selection_index, texture_path );
			}
			else
			{
				// Show metal wire selection
				string selection_wire2 = SEL_WIRE_PREFIX + ES_model;
				ShowSelection(selection_wire2);
			}
		}
		else
		{
			ShowSelection(SEL_WIRE_ROLLED);
		}
	}
	
	// Called when THIS is attached to battery
	override void OnIsPlugged(EntityAI source_device)
	{
		UpdateAllSelections();
	}
	
	// Called when THIS is detached from battery
	override void OnIsUnplugged(EntityAI last_energy_source)
	{
		UpdateAllSelections();
		GetCompEM().UnplugAllDevices();
	}
	
	// Called when some device is plugged into THIS
	override void OnOwnSocketTaken( EntityAI device )
	{
		UpdateAllSelections();
	}
	
	// Called when some device is unplugged from THIS
	override void OnOwnSocketReleased( EntityAI device )
	{
		UpdateAllSelections();
	}
}