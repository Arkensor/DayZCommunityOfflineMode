class UndergroundStash extends ItemBase
{
	int 		m_StashIDLow;
	int 		m_StashIDHigh;
	
	ItemBase 	m_StashedItem;
	
	void SetStashedItem(ItemBase item)
	{
		m_StashedItem = item;
	}
	
	// Stores IDs of the stash instance.
	void StoreStashIDs(int ID_low, int ID_high)
	{
		m_StashIDLow = ID_low;
		m_StashIDHigh = ID_high;
	}
	
	// Returns persistent ID (low) of the stash instance
	int GetStashIDLow()
	{
		return m_StashIDLow;
	}

	// Returns persistent ID (high) of the stash instance
	int GetStashIDHigh()
	{
		return m_StashIDHigh;
	}
	
	ItemBase GetStashedItem()
	{
		return m_StashedItem;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return false;
	}
	
	override bool CanReceiveItemIntoHands (EntityAI item_to_hands)
	{
		return false;
	}
	
	override bool CanPutIntoHands (EntityAI parent)
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return false;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		// It is necesarry for the UndergroundStash object to remember link to its stashed item
		// STASHED ITEM ID
		ItemBase stashed_item = GetStashedItem();
		int ID_low = 0;
		int ID_high = 0;
		
		if (stashed_item)
		{
			stashed_item.GetPersistentID(ID_low, ID_high);
		
			// Save stashed item ID low
			ctx.Write( ID_low );
			
			// Save stashed item ID high
			ctx.Write( ID_high );
		}
	}
	
	override void OnStoreLoad (ParamsReadContext ctx)
	{
		super.OnStoreLoad(ctx);
		
		// It is necesarry for the UndergroundStash object to remember link to its stashed item
		
		// Load stashed item ID low
		int i_stashed_item_ID_low = 0;
		ctx.Read( i_stashed_item_ID_low );
		
		// Load stashed item ID high
		int i_stashed_item_ID_high = 0;
		ctx.Read( i_stashed_item_ID_high );
		
		Print(i_stashed_item_ID_low);
		Print(i_stashed_item_ID_high);
		
		// Because function GetEntityByPersitentID() cannot be called here, ID values must be stored and used later.
		if (i_stashed_item_ID_low  &&  i_stashed_item_ID_high)
		{
			StoreStashIDs(i_stashed_item_ID_low, i_stashed_item_ID_high );
		}
	}
	
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();
		
		// Restore the link between UndergroundStash and the item it has stashed underneath
		int low = GetStashIDLow();
		int high = GetStashIDHigh();
		
		// get pointer to EntityAI based on this ID
		EntityAI potential_stash = GetGame().GetEntityByPersitentID(low, high); // This function is available only in this event!
		
		if ( potential_stash )
		{
			SetStashedItem( ItemBase.Cast( potential_stash ) );
		}
	}
}