class CableReel extends ItemBase
{
	bool m_ForceIntoHands;

	static const string SEL_CORD_FOLDED 			= "cord_folded";
	static const string SEL_CORD_PLUGGED 			= "cord_plugged";
	
	void CableReel () 
	{
		m_ForceIntoHands = false;
	}

	void ForceIntoHandsNow ( PlayerBase player ) 
	{
		m_ForceIntoHands = true;
		player.LocalTakeEntityToHands(this); // Local, because ForceIntoHandsNow is executed on both, Client and Server
		m_ForceIntoHands = false;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		EntityAI owner_EAI = GetHierarchyParent();
		if ( owner_EAI  &&  owner_EAI.IsKindOf("Fence"))
		{
			return true;
		}
		
		/*if ( GetCompEM().GetPluggedDevices()  &&  GetCompEM().GetPluggedDevicesCount() > 0 )
		{
			EntityAI powered_device = GetCompEM().GetPluggedDevices().Get(0).Ptr();
			
			if ( powered_device  &&  powered_device.IsKindOf ( "BarbedWire" ) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}*/
		
		bool allow_into_inv = !GetCompEM().IsPlugged();
		return allow_into_inv;
	}

	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		if ( m_ForceIntoHands )
		{
			return true;
		}
		else
		{
			EntityAI owner_EAI = GetHierarchyParent();
			if ( owner_EAI  &&  owner_EAI.IsKindOf("Fence"))
			{
				return true;
			}
			
			/*if ( GetCompEM().GetPluggedDevices()  &&  GetCompEM().GetPluggedDevicesCount() > 0 )
			{
				EntityAI powered_device = GetCompEM().GetPluggedDevices().Get(0).Ptr();
				
				if ( powered_device  &&  powered_device.IsKindOf ( "BarbedWire" ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}*/
		}
		
		bool allow_into_inv = !GetCompEM().IsPlugged();
		return allow_into_inv;
	}

	// Event called on item when it is placed in the player(Man) inventory, passes the owner as a parameter
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		GetCompEM().UnplugAllDevices();
		
		PlayerBase player_PB;
		Class.CastTo(player_PB, player);
		
		if ( !player_PB.IsPlacingLocal() )
		{
			GetCompEM().UnplugThis();
		}
	}
	
	override bool CanRemoveFromHands ( EntityAI player ) 
	{
		return true;
	}

	override void OnPlacementStarted( Man player )
	{
		super.OnPlacementStarted( player );
		
		ref array<string> array_of_selections = {SEL_CORD_PLUGGED, SEL_CORD_FOLDED};
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