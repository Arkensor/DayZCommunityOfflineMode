// This script is going through rewrites. Contact Boris Vacula if you want to consult something.
// TO DO: Uncomment SetObjectTexture and SetObjectMaterial functions and call them as least time as necesarry

class GardenBase extends Building
{
	// Paths to slot textures. Slots can have multiple states, so multiple textures must be generated
	static const string SLOT_TEXTURE_DIGGED_WET_LIME		= "dz\\gear\\cultivation\\data\\soil_digged_wet_lime_CO.paa";
	static const string SLOT_TEXTURE_DIGGED_WET_PLANT		= "dz\\gear\\cultivation\\data\\soil_digged_wet_plant_CO.paa";
	
	// Wet/dry material
	static const string SLOT_MATERIAL_WET					= "dz\\gear\\cultivation\\data\\soil_cultivated_wet.rvmat";
	static const string SLOT_MATERIAL_DRY					= "dz\\gear\\cultivation\\data\\soil_cultivated.rvmat";
	
	static const string SLOT_MATERIAL_LIMED_WET				= "dz\\gear\\cultivation\\data\\soil_cultivated_limed_wet.rvmat";
	static const string SLOT_MATERIAL_LIMED_DRY				= "dz\\gear\\cultivation\\data\\soil_cultivated_limed.rvmat";
	static const string SLOT_MATERIAL_COMPOST_WET			= "dz\\gear\\cultivation\\data\\soil_cultivated_compost_wet.rvmat";
	static const string SLOT_MATERIAL_COMPOST_DRY			= "dz\\gear\\cultivation\\data\\soil_cultivated_compost.rvmat";
	
	// slot names
	private static const string SLOT_SELECTION_DIGGED_PREFIX 	= "slotDigged_";
	private static const string SLOT_SELECTION_COVERED_PREFIX 	= "slotCovered_";
	private static const string SLOT_COMPONENT_PREFIX 			= "Component";
	private static const string SLOT_MEMORY_POINT_PREFIX 		= "slot_";
	
	
	private static const int 	CHECK_RAIN_INTERVAL 			= 30;
	int 						FIRST_SLOT_COMPONENT_INDEX; // Number of 'component##' selection in the p3d file which represents the first slot (View Geometry LOD). Needed for user actions to work on targeted slots.
	
	protected ref array<ref Slot> m_Slots;
	protected float m_BaseFertility;
	ref Timer m_DeleteWithDelayTimer; // Hack: This timer deletes attachments with a small delay. Attachments can't be deleted without it.
	ref Timer m_CheckRainTimer; // When rain starts, all slots must become watered.
	
	private static ref map<string,string> m_map_slots; // For the 'attachment slot -> plant slot' conversion. It is possible that this will be removed later.
	
	void GardenBase()
	{
		FIRST_SLOT_COMPONENT_INDEX = 2;
		
		m_map_slots = new map<string,string>;
		m_DeleteWithDelayTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		// Prepare m_map_slots
		for (int i = 1;  i <= GetGardenSlotsCount() + 1;  ++i)
		{
			// m_map_slots is supposed to be: <input, output>
			string input = "seedbase_" + i.ToString();
			string output = SLOT_COMPONENT_PREFIX;
			
			if (i < 10)
				output = output + "0"; // Example: '1' changes to '01'
			
			int i_add = i + 1; // + 1 because input like 'seedbase_5' must output 'component06' due to how p3d files work.
			output = output + i_add.ToString();
			
			m_map_slots.Set(input, output);
		}
		
		// m_CheckRainTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		if (GetGame().IsServer())
		{
			//m_CheckRainTimer.Run( CHECK_RAIN_INTERVAL, this, "CheckRainTick", NULL, true ); // Temporarily removed. It might have been causing server-side stuttering. Good solution could b provided by upcoming (?) staging system.
		}
	}

	void InitializeSlots()
	{
		m_Slots = new array<ref Slot>;
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = new Slot(m_BaseFertility);
			slot.SetSlotIndex(i);
			int i1 = i + 1;
			string name = "SeedBase_" + i1;
			int slot_id = InventorySlots.GetSlotIdFromString(name);
			slot.SetSlotId(slot_id);
			slot.SetGarden(this);
			m_Slots.Insert( slot );
		}
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		ctx.Read( m_BaseFertility );
		
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = m_Slots.Get( i );
			
			slot.OnStoreLoadCustom( ctx );
			UpdateSlotTexture( i );
			
			string loadString = "";
			ctx.Read( loadString );
			
			if ( loadString != "" )
			{
				PlantBase plant = PlantBase.Cast( GetGame().CreateObject( loadString, GetSlotPosition(i) ) );
				slot.SetPlant( plant );
				plant.OnStoreLoadCustom( ctx, this );
			}
		}
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		ctx.Write( m_BaseFertility );
		
		int slots_count = GetGardenSlotsCount();
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = m_Slots.Get( i );
			
			slot.OnStoreSaveCustom( ctx );
			
			string saveString = "";
			
			if ( slot.GetPlant() )
			{
				saveString = slot.GetPlant().GetType();
				ctx.Write( saveString );
				PlantBase plant = slot.GetPlant();
				plant.OnStoreSaveCustom( ctx );
			}
			else
			{
				ctx.Write( saveString );
			}
		}
	}
	
	// On server -> client synchronization
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if ( GetGame().IsClient() )
		{
			switch(rpc_type)
			{
				case ERPCs.RPC_GARDEN_SLOTS:
				
					ref Param1<array<ref Slot>> p_slots = new Param1<array<ref Slot>>(NULL);
					
					if (ctx.Read(p_slots))
					{
						m_Slots = p_slots.param1;
						PrintSlots();
					}
					
				break;
			}
		}
	}
	
	// Synchronizes energy value
	protected void SynchSlots()
	{
		if ( GetGame().IsServer() )
		{
			Param1< array<ref Slot> > p = new Param1<array<ref Slot>>( m_Slots );
			GetGame().RPCSingleParam( this, ERPCs.RPC_GARDEN_SLOTS, p, true );
		}
	}
	
	void PrintSlots()
	{
		Print("PRINT ALL SLOTS FROM...");
		Print(this);
		int slots = GetInventory().GetSlotsCount();
		Print(slots);
		
		for ( int i = 0; i < slots ; i++ )
		{
			Slot slot = m_Slots.Get(i);
			Print(i);
			Print(slot);

			float slot_fertility = slot.GetFertility();
			float slot_fertility_usage = slot.GetFertilityMax();
			string slot_fertility_type = slot.GetFertilityType();
			float slot_water = slot.GetWater();
			float slot_water_usage = slot.GetWaterUsage();
			ItemBase slot_seed = slot.GetSeed();
			ItemBase slot_plant = slot.GetPlant();
			float slot_state= slot.GetState();
			float slot_slot_Index = slot.GetSlotIndex();
			float slot_slot_ID = slot.GetSlotId();
			
			Print(slot_fertility);
			Print(slot_fertility_usage);
			Print(slot_fertility_type);
			Print(slot_water);
			Print(slot_water_usage);
			Print(slot_seed);
			Print(slot_plant);
			Print(slot_state);
			Print(slot_slot_Index);
			Print(slot_slot_ID);
			Print("///////////////////////////");
		}
		
		Print("END OF ALL SLOTS FOR...");
		Print(this);
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return false;
	}

	override bool CanPutIntoHands( EntityAI player )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return false;
	}

	override bool CanRemoveFromHands( EntityAI player )
	{
		return false;
	}

	int GetGardenSlotsCount()
	{
		return 0;
	}
	
	// Digs the given slot. Parameters player and item are optional. Returns result message.
	string DigSlot( PlayerBase player, ItemBase item, int slot_index )
	{
		if (item) // Item is not always provided
			item.DecreaseHealth( "", "", 1 );
		
		Slot slot = m_Slots.Get( slot_index );
		slot.m_State = Slot.STATE_DIGGED;
		
		UpdateSlotTexture( slot_index );
		
		if (player)
		{
			PluginHorticulture module_horticulture = PluginHorticulture.Cast( GetPlugin( PluginHorticulture ) );
			
			if ( module_horticulture.GiveWormsToPlayer(player, 0.1) )
			{
				return "I've prepared the slot and found a worm in the ground.";
			}
			else
			{
				return "I've prepared the slot.";
			}
		}
		
		return "";
	}

	bool CanPlantSeed( string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL && slot.m_State == Slot.STATE_DIGGED )
		{
			return  true;
		}
		else
		{
			return false;
		}
	}
	
	// Converts attachment slot name into plant slot name. Example: 'seedbase_1' -> 'component02'
	string ConvertAttSlotToPlantSlot(string attach_slot)
	{
		// Give result
		if ( m_map_slots.Contains(attach_slot) )
		{
			string return_value = m_map_slots.Get(attach_slot);
			return return_value;
		}
		
		return "";
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		string path = "CfgVehicles " + item.GetType() + " Horticulture" + " PlantType";
		bool IsItemSeed = GetGame().ConfigIsExisting(path); // Is this item a seed?
		int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
		
		if ( IsItemSeed ) 
		{
			string converted_slot_name;
			Print(slot_name); // TO DO: (BLOKER) Until new system for attachment slots is implemented, slot_name is incorrect!
			vector pos = GetPosition();
			//int index = GetNearestSlotIDByState( pos , Slot.STATE_DIGGED ) + FIRST_SLOT_COMPONENT_INDEX; // +2 because selections are named differently than 'garden slot' IDs and 'attachment slots' IDs.
			int index = GetSlotIndexByAttachmentSlot( slot_name );
			index = index + FIRST_SLOT_COMPONENT_INDEX - 1; // Offset index because of COMPONENT## in p3d
			
			if (index < 10)
			{
				converted_slot_name = SLOT_COMPONENT_PREFIX + "0" + index.ToString();
			}
			else
			{
				converted_slot_name = SLOT_COMPONENT_PREFIX + index.ToString();
			}
			
			Print(index);
			Print(converted_slot_name);
			
			PlantSeed( NULL, ItemBase.Cast( item ), converted_slot_name);
			SynchSlots();
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		//slot_name = "SeedBase_9"; // TO DO: Remove this!

		string path = "CfgVehicles " + item.GetType() + " Horticulture " + "PlantType";
		bool IsItemSeed = GetGame().ConfigIsExisting(path); // Is this item a seed?
		
		if ( IsItemSeed ) 
		{
			//Print(item);
			//Print(slot_name); // TO DO: (BLOKER) Until new system for attachment slots is implemented, slot_name is incorrect!
			string converted_slot_name = ConvertAttSlotToPlantSlot(slot_name);
			//Print(converted_slot_name);
			Slot slot = GetSlotBySelection(converted_slot_name);
			//Print(slot);
			
			if (slot)
			{
				// TO DO: Manipuluje sa tu so zlym slotom! Ma sa resetnut slot 4, ale v skutocnosti sa resetne slot 0!
				//Print(slot.m_State);
				slot.m_State = Slot.STATE_DIGGED;
				//Print(slot.m_State);
				slot.SetSeed(NULL);
			}
			
			SynchSlots();
		}
	}
	
	// Sets all slots to be digged and ready for planting
	void DigAllSlots()
	{
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			DigSlot( NULL, NULL, i );
		}
	}
	
	// Plants the seed (item) into slot (selection_component). Returns message string depending on the result.
	string PlantSeed( PlayerBase player, ItemBase item, string selection_component )
	{
		int slot_index = GetSlotIndexBySelection( selection_component );
		string message = "ERROR! PLANTING OF THE SEED FAILED!";
		
		if ( slot_index >= 0 )
		{
			PluginHorticulture module_horticulture = PluginHorticulture.Cast( GetPlugin( PluginHorticulture ) );
			string plant_type = module_horticulture.GetPlantType( item );
			
			Print(slot_index);
			Slot slot = m_Slots.Get( slot_index );
			slot.m_State = Slot.STATE_PLANTED;
			slot.m_PlantType = plant_type;
			slot.SetSeed(item);
			
			slot.SetSlotComponent(selection_component);
			
			if ( NeedsFertilization(selection_component) == false )
			{
				message = "I've planted a seed into the fertilized soil.";
			}
			else
			{
				message = "I've planted a seed.";
			}
			
			float rain_intensity = GetGame().GetWeather().GetRain().GetActual();
			if ( rain_intensity < 0.5 )
			{
				message += " However the soil is too dry for the plant to grow.";
			}
		}
		
		return message;
	}
	
	// Creates a plant
	void CreatePlant(Slot slot )
	{
		int slot_index = GetSlotBySelection(slot.GetSlotComponent()).GetSlotIndex();
		vector pos = GetSlotPosition(slot_index);
		PlantBase plant = PlantBase.Cast( GetGame().CreateObject( slot.m_PlantType, pos ) );
		plant.SetPosition(pos); // Hackfix for plants having random offset. It doesn't work very well though since it causes problems elsewhere. For now there is no better solution.
		slot.SetPlant(plant);
		slot.m_State = Slot.STATE_COVERED;
		plant.Init( this, slot.m_Fertility, slot.m_HarvestingEfficiency, slot.GetWater() );
		ShowSelection(SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2));
		
		ItemBase seed = slot.GetSeed();
		GetGame().ObjectDelete(seed);
		//this.TakeEntityAsAttachment(plant); // TO DO: Uncomment this when plant proxies are fixed
		
		Param1<ItemBase> param_seed = new Param1<ItemBase>(seed);
		m_DeleteWithDelayTimer.Run(0.1, this, "DeleteWithDelay", param_seed, false);
	}
	
	void DeleteWithDelay(ItemBase param_seed)
	{
		if (this)
		{
			ItemBase seed = param_seed;
			if (seed)
			{
				GetGame().ObjectDelete( seed );
			}
		}
	}
		
	string Fertilize( PlayerBase player, ItemBase item, float consumed_quantity, string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL )
		{
			string item_type = item.GetType();
			
			if ( slot.m_FertilizerType == ""  ||  slot.m_FertilizerType == item_type )
			{
				slot.m_FertilizerType = item_type;
				
				float add_energy_to_slot = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture AddEnergyToSlot" );
				slot.m_FertilizerUsage = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture ConsumedQuantity" );
				
				float coef = Math.Clamp( consumed_quantity / slot.m_FertilizerUsage, 0.0, 1.0 );
				add_energy_to_slot = coef * add_energy_to_slot;
				
				slot.m_FertilizerQuantity += consumed_quantity;
				slot.m_Fertility += add_energy_to_slot;
				
				item.SetQuantity( item.GetQuantity() );
				
				if ( NeedsFertilization(selection_component) == false )
				{
					string selection = selection_component;
					selection.Replace( SLOT_COMPONENT_PREFIX, SLOT_SELECTION_DIGGED_PREFIX );
					int slot_index = GetSlotIndexBySelection(selection);
					
					UpdateSlotTexture( slot_index );
					
					//TODO Boris: Add soft skill 2.0
					//PluginExperience module_exp = GetPlugin(PluginExperience);
					//slot.m_HarvestingEfficiency = module_exp.GetExpParamNumber(player, PluginExperience.EXP_FARMER_FERTILIZATION, "efficiency");
						
					return "I've fertilized the ground. Now it have enough of fertilizer.";
				}
				else
				{
					return "I've fertilized the ground a bit.";
				}
			}
		}
		
		return "This slot is fertilized with another fertilizer already.";
	}

	bool IsCorrectFertilizer( ItemBase item, string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL )
		{
			string item_type = item.GetType();
			
			if ( slot.m_FertilizerType == "" || slot.m_FertilizerType == item_type )
			{
				return true;
			}
		}
		
		return false;
	}

	bool NeedsFertilization( string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot  &&  !slot.IsCovered() )
		{
			if ( slot.m_FertilizerType == ""  ||  slot.m_FertilizerQuantity < slot.m_FertilizerUsage )
			{
				return true;
			}
		}
		
		return false;
	}

	TStringArray GetHiddenSelectionsTextures()
	{
		string garden_type = this.GetType();
		TStringArray textures = new TStringArray;
		GetGame().ConfigGetTextArray( "CfgVehicles " + garden_type + " hiddenSelectionsTextures", textures );
		return textures;
	}

	void UpdateSlotTexture( int slot_index )
	{
		// 23.3.2018: Temporarily commented out as a quick fix for DAYZ-30633. This will be fixed properly when new hierarchy is implemented.
		
		/*
		Slot slot = m_Slots.Get( slot_index );
		
		// Show / Hide selections according to DIGGED or COVERED states.
		
		if ( slot.IsDigged()  ||  slot.IsPlanted() )
		{
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2) );
			ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		}
		else if ( slot.IsCovered() )
		{
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2) );
			ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		}
		if ( slot.m_State != Slot.STATE_NOT_DIGGED ) 
		{
			if ( slot.m_FertilizerType != "" )
			{
				SetSlotTextureFertilized( slot_index, slot.m_FertilizerType );
			}
			else 
			{
				SetSlotTextureDigged( slot_index );
			}
		}
		*/
	}

	void SetSlotTextureDigged( int slot_index )
	{
		
		TStringArray textures = GetHiddenSelectionsTextures();
		ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		string texture = textures.Get(0);
		//SetObjectTexture( slot_index, texture );
		
		Slot slot = m_Slots.Get( slot_index );
		
		if ( slot.NeedsWater() )
		{
			// Set dry material
			//this.SetObjectMaterial ( slot_index, SLOT_MATERIAL_DRY );
		}
		else
		{
			// Set wet material
			//this.SetObjectMaterial ( slot_index, SLOT_MATERIAL_WET );
		}
		
	}

	void SetSlotTextureFertilized( int slot_index, string item_type )
	{
		TStringArray textures = GetHiddenSelectionsTextures();
		
		int tex_id = GetGame().ConfigGetInt( "cfgVehicles " + item_type + " Horticulture TexId" );
		ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		//SetObjectTexture( slot_index, textures.Get(tex_id) );
		
		Slot slot = m_Slots.Get( slot_index );
		
		int slot_index_offset = 0;
		
		if ( slot.IsCovered() )
		{
			slot_index_offset = slot_index_offset + GetGardenSlotsCount();
		}
		
		// Set material according to dry / wet states
		if ( slot.NeedsWater() )
		{
			// Set dry material for garden lime
			if ( slot.m_FertilizerType == "GardenLime" )
			{
				//this.SetObjectMaterial ( slot_index + slot_index_offset, SLOT_MATERIAL_LIMED_DRY ); // TO DO: Save into constant
			}
			else if ( slot.m_FertilizerType == "PlantMaterial" )
			{
				//this.SetObjectMaterial ( slot_index + slot_index_offset, SLOT_MATERIAL_COMPOST_DRY ); // TO DO: Save into constant
			}
		}
		else
		{
			// Set dry material for compost
			if ( slot.m_FertilizerType == "GardenLime" )
			{
				//this.SetObjectMaterial ( slot_index + slot_index_offset, SLOT_MATERIAL_LIMED_WET ); // TO DO: Save into constant
			}
			else if ( slot.m_FertilizerType == "PlantMaterial" )
			{
				//this.SetObjectMaterial ( slot_index + slot_index_offset, SLOT_MATERIAL_COMPOST_WET ); // TO DO: Save into constant
			}
		}
	}
	
	void RemoveSlot( int index )
	{
		if ( m_Slots != NULL )
		{	
			Slot slot = m_Slots.Get( index );
			
			if ( slot.GetPlant() )
			{
				GetGame().ObjectDelete( slot.GetPlant() );
			}
			
			slot.Init( m_BaseFertility );
			slot.GiveWater( NULL, -9999 );
			
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (index + 1).ToStringLen(2) );
			//this.HideSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2), 1 );
		}
	}

	void RemoveSlotPlant( Object plant )
	{
		int index = GetSlotIndexByPlant( plant );
		if ( index >= 0 )
		{
			RemoveSlot( index );
		}
	}

	Slot GetSlotBySelection( string selection_component )
	{
		int slot_index = GetSlotIndexBySelection( selection_component );
		if ( slot_index >= 0 )
		{
			return m_Slots.Get( slot_index ); // If script crashes here, it's probably because member variable FIRST_SLOT_COMPONENT_INDEX is not set to correct value.
		}
		else
		{
			return NULL;
		}
	}

	int GetSlotIndexBySelection( string selection_component )
	{
		int slot_index = -1;
		
		if ( m_Slots != NULL )
		{
			string selection_component_lower = selection_component; 
			selection_component_lower.ToLower();
			
			string slot_component_prefix_lower = SLOT_COMPONENT_PREFIX;
			slot_component_prefix_lower.ToLower();
			
			int start = selection_component_lower.IndexOf( slot_component_prefix_lower );
			if ( start > -1 )
			{
				start += SLOT_COMPONENT_PREFIX.Length();
				int end = start + 2;
				int length = selection_component.Length();
				
				if ( length >= end )
				{
					int length_add = length - end; // Hack-fix for inconsistent component names in p3d
					int length_from_end = 2 + length_add;
					string num_str = selection_component.Substring( start, length_from_end );
					slot_index = num_str.ToInt();
				}
			}
		}
		
		if ( slot_index >= FIRST_SLOT_COMPONENT_INDEX )
		{
			slot_index -= FIRST_SLOT_COMPONENT_INDEX;
		}
		else
		{
			slot_index = -1;
		}
		
		return slot_index;
	}
	
	int GetSlotIndexByAttachmentSlot( string att_slot )
	{
		int slot_index = -1;
		
		int start = "SeedBase_".Length();
		int end = start + 2;
		
		string num_str = att_slot.Substring( start, 1 );
		slot_index = num_str.ToInt();
		
		return slot_index;
	}

	int GetSlotIndexByPlant( Object plant )
	{
		if ( m_Slots != NULL )
		{
			for ( int i = 0; i < m_Slots.Count(); i++ )
			{
				if ( m_Slots.Get(i).GetPlant() == plant )
				{
					return i;
				}
			}
		}
		
		return -1;
	}

	
	// TO DO: Remove! Player shouldn't dig slots anymore!
	int GetFreeSlotIndex( PlayerBase player )
	{
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + (GetGame().GetCurrentCameraDirection() * 5);
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component) )   
		{
			int slot_index = GetNearestSlotIDByState( contact_pos , Slot.STATE_NOT_DIGGED );
			
			if ( slot_index > -1 )
			{
				return slot_index;
			}
		}
		
		return -1;
	}
	
	int GetNearestSlotIDByState( vector position, int slot_state)
	{
		float nearest_distance = 1000.0;
		int nearest_slot_index = -1;
		Print(slot_state);
		int slots_count = GetGardenSlotsCount();
		for ( int i = 0; i < slots_count; i++ )
		{
			Print(i);
			Slot slot = m_Slots.Get(i); // Move this line by a scope higher in this function after debugging
			
			if (slot)
			{
				Print(slot.m_State);
			}
			
			vector slot_pos = GetSlotPosition( i );
			float current_distance = vector.Distance( position, slot_pos );
			
			if ( current_distance < nearest_distance )
			{
				
				if ( slot != NULL && slot.m_State == slot_state )
				{
					nearest_distance = current_distance;
					nearest_slot_index = i;
				}
			}
		}
		
		return nearest_slot_index;
	}

	vector GetSlotPosition( int index )
	{
		string memory_point = SLOT_MEMORY_POINT_PREFIX + (index + 1).ToStringLen(2);
		vector pos = this.GetSelectionPosition( memory_point );
		
		return this.ModelToWorld( pos );
	}
	
	void CheckRainTick()
	{
		if (this)
		{
			float rain_intensity = GetGame().GetWeather().GetRain().GetActual();
			
			if ( rain_intensity > 0 )
			{
				int slots_count = GetGardenSlotsCount();
				
				for ( int i = 0; i < slots_count; i++ )
				{
					if (m_Slots)
					{
						Slot slot = m_Slots.Get( i );
						
						if (slot)
						{
							slot.GiveWater( NULL, slot.GetWaterUsage() );
						}
					}
				}
			}
		}
	}
}