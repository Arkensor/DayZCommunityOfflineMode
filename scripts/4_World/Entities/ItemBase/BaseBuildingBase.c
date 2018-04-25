//BASE BUILDING BASE
class BaseBuildingBase extends ItemBase
{
	//==============================================================================
	// CONSTRUCTION
	
	static const string ANIMATION_BARBED_WIRE 				= "BarbWire";
	static const string ANIMATION_CAMO_NET 					= "Camonet";
	static const string ANIMATION_LIGHTS 					= "xlights";
	
	static const string MESSAGE_NOT_ENOUGH_MATERIALS		= "There are not enough materials to build this part.";
	static const string MESSAGE_CANNOT_BE_DECONSTRUCTED		= "This part cannot be deconstructed because of some other part.";
	
	ref Construction 	m_Construction;
	ref array<string> 	m_BaseParts;
	// construction ^
	//==============================================================================
	
	//==============================================================================
	// ELECTRIC FENCES
	
	static const string 				BARBEDWIRE_SLOT_LEFT				= "barbedwireleft";
	static const string 				BARBEDWIRE_SLOT_RIGHT				= "barbedwireright";
	static const string 				BARBEDWIRE_SLOT_FRONT				= "barbedwirefront";
	static const string 				BARBEDWIRE_SLOTS[] = {BARBEDWIRE_SLOT_FRONT, BARBEDWIRE_SLOT_LEFT, BARBEDWIRE_SLOT_RIGHT};
	
	// Positions of damage triggers are local to the model. "0 0 0" means undefined and it will be ignored, therefore overwrite these in your item's constructor if you want to use them!
	const int 							DMG_TRIGGERS_COUNT						= 3;
	vector 								m_DmgTrgLocalPos[DMG_TRIGGERS_COUNT]	= {"0 0 0", "0 0 0", "0 0 0"}; // Order must be same with BARBEDWIRE_SLOTS!
	int									m_DmgTrgLocalDir[DMG_TRIGGERS_COUNT]	= {0,0,0}; // Direction of each trigger local to the model in degrees
	
	ref map<string,BarbedWire>		m_BarbedWires;
	ref array<BarbedWireTrigger> 		m_DmgTriggers;
	
	// electric fences ^
	//==============================================================================

	//==============================================================================
	// CONSTRUCTION	
	
	// Constructor
	void BaseBuildingBase() 
	{
		SetFlags(EntityFlags.TRIGGER, false);
		
		m_DmgTriggers = new array<BarbedWireTrigger>;
		m_BarbedWires = new map<string,BarbedWire>;
		
		m_Construction = new Construction( this );
	}
	
	// Destructor
	void ~BaseBuildingBase() 
	{
		//cleanup
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		//Construction init
		ConstructionInit();
	}
	
	//add to entity ai
	override bool CanUseConstruction()
	{
		return true;
	}

	//on store save/load
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		GetConstruction().OnStoreSave( ctx );
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		GetConstruction().OnStoreLoad( ctx );
	}
	
	//construction init
	void ConstructionInit()
	{
		GetConstruction().Init( m_BaseParts );
	}
	Construction GetConstruction()
	{
		return m_Construction;
	}
	
	// --- EVENTS
	override void EEItemAttached ( EntityAI item, string slot_name )
	{
		super.EEItemAttached ( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		string item_type = item_IB.GetType();
		
		// Attachment control
		switch (item_type)
		{
			case "BarbedWire":
				BarbedWire attached_BW = BarbedWire.Cast( item_IB );
				OnBarbedWireAttached(attached_BW, slot_name); // Removed function
			break;
			
			case "CableReel":
				CableReel attached_cable_reel = CableReel.Cast( item_IB );
				// OnCableReelAttached(attached_cable_reel, slot_name); // Removed function
			break;
		
			case "CamoNet":
				this.SetAnimationPhase ( ANIMATION_CAMO_NET, 0 );
			break;
				
			case "XmasLights":
				this.SetAnimationPhase ( ANIMATION_LIGHTS, 0 );
				XmasLights fence_light = XmasLights.Cast( item_IB );
				fence_light.AttachToObject( this );
			break;
		}
		
		// this.OnItemAttached ( item ); // Removed function
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name )
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		string item_type = item_IB.GetType();
		
		// Attachment control
		switch (item_type)
		{
			case "BarbedWire":
				BarbedWire detached_BW = BarbedWire.Cast( item_IB );
				OnBarbedWireDetached(detached_BW, slot_name); // Removed function
			break;
		
			case "CableReel":
				CableReel detached_CR = CableReel.Cast( item_IB );
				// OnCableReelDetached(detached_CR, slot_name); // Removed function
			break;
			
			case "CamoNet":
				this.SetAnimationPhase ( ANIMATION_CAMO_NET, 1 );
			break;
		
			case "XmasLights":
				this.SetAnimationPhase ( ANIMATION_LIGHTS, 1 );
				XmasLights fence_light = XmasLights.Cast( item_IB );
				fence_light.DetachFromObject( this );
			break;
		}
	}
	

	// construction ^
	//==============================================================================
	
	
	
	/*==============================================
					ELECTRIC FENCES
	==============================================*/
	
	
	// When a barbed wire is attached to this fence/watchtower
	void OnBarbedWireAttached(BarbedWire attached_BW, string slot_name)
	{
		SetWire(slot_name, attached_BW);
		UpdateDamageTriggers();
		ShowSelection(ANIMATION_BARBED_WIRE);
	}
	
	// Creates all damage triggers this structure needs. They are tied to the barbed wire item.
	void UpdateDamageTriggers()
	{
		DestroyDamageTriggers();
		
		for ( local int i = 0; i < 3; ++i )
		{
			local string slot_name = BARBEDWIRE_SLOTS[i];
			local BarbedWire bw = BarbedWire.Cast( FindAttachmentBySlotName( slot_name ) );
			
			if (bw)
			{
				local vector local_pos = m_DmgTrgLocalPos[ i ];
				if ( local_pos != "0 0 0" ) // "0 0 0" is considered as undefined!
				{
					local vector global_pos	= ModelToWorld (local_pos);
					local BarbedWireTrigger dmg_trg = BarbedWireTrigger.Cast( GetGame().CreateObject( "BarbedWireTrigger", global_pos, false ) );
					vector mins = "-1.8 -0.4 -0.6";
					vector maxs = "1.8 0.4 0.6";
					local vector ori = GetOrientation();
					ori[0] = ori[0] + m_DmgTrgLocalDir[i]; // rotate the trigger
					dmg_trg.SetOrientation( ori );
					dmg_trg.SetExtents(mins, maxs);	
					dmg_trg.SetParentObject( bw );
					m_DmgTriggers.Insert(dmg_trg);
				}
			}
		}
	}
	
	// Removes all damage triggers
	void DestroyDamageTriggers()
	{
		local int count = m_DmgTriggers.Count();
		--count;
		for ( local int i = count; i >= 0; --i )
		{
			BarbedWireTrigger trg = m_DmgTriggers.Get(i);
			
			if ( trg  &&  GetGame() ) // It's necesarry to check if the game exists. Otherwise a crash occurs while quitting.
				GetGame().ObjectDelete( trg );
			
			m_DmgTriggers.Remove(i);
		}
	}
	
	void SetWire(string slot_name, BarbedWire wire)
	{
		if (wire)
		{
			m_BarbedWires.Set(slot_name, wire);
		}
		else
		{
			m_BarbedWires.Set(slot_name, NULL);
		}
	}
	
	
	// !Returns the some barbed wire attached on this fence
	BarbedWire GetWire()
	{
		BarbedWire return_wire;
		ref array<BarbedWire> wires = GetWires();
		
		if (wires.Count() > 0)
		{
			return_wire = wires.Get(0);
		}
		
		return return_wire;
	}
	
	void OnBarbedWireDetached(BarbedWire detached_BW, string slot_name)
	{
		detached_BW.GetCompEM().UnplugAllDevices();
		SetWire(slot_name, NULL);
		UpdateDamageTriggers();
		HideSelection(ANIMATION_BARBED_WIRE);
	}
	
	// !Returns array of barbed wires attached to this BBB object
	array<BarbedWire> GetWires()
	{
		array<BarbedWire> return_array = new array<BarbedWire>;
		
		for ( local int i = 0; i < 3; ++i )
		{
			BarbedWire bw = m_BarbedWires.Get( BARBEDWIRE_SLOTS[i] );
			
			if (bw)
			{
				return_array.Insert(bw);
			}
		}
		
		return return_array;
	}
	
	//! Returns the number of barbed wires attached to this structure
	int GetWiresCount()
	{
		int total_count = m_BarbedWires.Count();
		int counter = 0;
		
		for ( local int i = 0; i < total_count; ++i )
		{
			local BarbedWire bw = m_BarbedWires.Get( BARBEDWIRE_SLOTS[i] );
			
			if (bw)
			{
				counter++;
			}
		}
		
		return counter;
	}
	
	// electric fences ^
	//==============================================================================	
}