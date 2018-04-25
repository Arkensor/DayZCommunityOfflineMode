class Spotlight extends ItemBase
{
	private bool	m_IsFolded;
	
	// Spotlight can be extended and compressed
	static const string SEL_REFLECTOR_COMP_U 		= "reflector";
	static const string SEL_CORD_FOLDED_U 			= "cord_folded";
	static const string SEL_CORD_PLUGGED_U 			= "cord_plugged";
	static const string SEL_CORD_PLUGGED_F 			= "spotlight_folded_cord_plugged";
	static const string SEL_CORD_FOLDED_F 			= "spotlight_folded_cord_folded";
	
	static const string SEL_INVENTORY 				= "inventory";
	static const string SEL_PLACING 				= "placing";
	static const string SEL_GLASS_F 				= "glass_folded";
	static const string SEL_GLASS_U 				= "glass_unfolded";
	static const string SEL_REFLECTOR_F				= "reflector_folded";
	static const string SEL_REFLECTOR_FAR_F			= "reflector_far_folded";
	static const string SEL_REFLECTOR_U				= "reflector_unfolded";
	static const string SEL_REFLECTOR_FAR_U			= "reflector_far_unfolded";
	
	/*
	Spotlight, folded and unfolded.
	*/
	
	void Spotlight()
	{
		Fold();
		UpdateAllSelections();
	}

	override void OnWorkStart()
	{
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		SetPilotLight(false);
	}
	
	// Called when this device is picked up
	override void EEItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		
		// When the item is picked up by a player
		if (new_owner)
		{
			Fold();
			GetCompEM().SwitchOff();
		}
	}

	override void OnIsPlugged(EntityAI source_device)
	{
		GetCompEM().SwitchOn(); // Switch on for better convenience
		UpdateAllSelections();
	}
	
	void UpdateAllSelections()
	{
		PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
		bool is_plugged = GetCompEM().IsPlugged();
		HideAllSelections();
		
		if ( IsFolded() )
		{
			ShowSelection( SEL_INVENTORY );
			ShowSelection( SEL_GLASS_F );
			ShowSelection( SEL_REFLECTOR_F );
			ShowSelection( SEL_REFLECTOR_FAR_F );
			
			if (is_plugged)
			{
				ShowSelection( SEL_CORD_PLUGGED_F );
			}
			else
			{
				ShowSelection( SEL_CORD_FOLDED_F );
			}
		}
		else
		{
			ShowSelection( SEL_PLACING );
			ShowSelection( SEL_REFLECTOR_U );
			ShowSelection( SEL_REFLECTOR_FAR_U );
			ShowSelection( SEL_GLASS_U );
			ShowSelection( SEL_REFLECTOR_COMP_U );
			
			if (is_plugged)
			{
				ShowSelection( SEL_CORD_PLUGGED_U );
			}
			else
			{
				ShowSelection( SEL_CORD_FOLDED_U );
			}
		}
	}
	
	override void OnIsUnplugged( EntityAI last_energy_source )
	{
		UpdateAllSelections();
	}

	override void OnPlacementComplete( Man player )
	{
		Unfold();
	}

	void Fold()
	{
		m_IsFolded = true;
		GetCompEM().SwitchOff();
		GetCompEM().UnplugThis();
		
		UpdateAllSelections();
	}

	void Unfold()
	{
		m_IsFolded = false;
		GetCompEM().SwitchOn();
		
		UpdateAllSelections();
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{   
		super.OnStoreSave(ctx);
		
		// Save folded/unfolded state
		ctx.Write( m_IsFolded );
	}

	override void OnStoreLoad(ParamsReadContext ctx)
	{   
		super.OnStoreLoad(ctx);
		
		// Load folded/unfolded state
		bool b_is_folded = false;
		ctx.Read(b_is_folded );
		
		if (b_is_folded)
		{
			Fold();
		}
		else
		{
			Unfold();
		}
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		// Commented out so Reposition action is possible to execute
		return true; //GetCompEM().IsCordFolded(); 
	}

	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		// Commented out so Reposition action is possible to execute
		return true; //GetCompEM().IsCordFolded();
	}

	bool IsFolded()
	{
		return m_IsFolded;
	}
		
	override void OnPlacementStarted( Man player )
	{
		super.OnPlacementStarted( player );
		
		ref array<string> array_of_selections = {SEL_CORD_PLUGGED_U, SEL_CORD_FOLDED_U};
		PlayerBase player_PB = PlayerBase.Cast( player );
				
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			player_PB.GetHologramServer().SetSelectionToRefresh( array_of_selections );		
		}
		else
		{
			player_PB.GetHologramLocal().SetSelectionToRefresh( array_of_selections );
		}
	}
}