
//-----------------------------------------------------------------------------
class ItemSuppressor extends InventoryItemSuper
{
};

typedef ItemSuppressor SuppressorBase;

//-----------------------------------------------------------------------------
class ItemWatch extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemTransmitter extends InventoryItemSuper
{
	proto native void SetNextChannel();
	proto native void SetPrevChannel();
	proto native float GetTunedFrequency();
	proto native void EnableBroadcast(bool state);
	proto native void EnableReceive(bool state);
	proto native bool IsBroadcasting();
	proto native bool IsReceiving();
};

//-----------------------------------------------------------------------------
class ItemMegaphone extends InventoryItemSuper
{
}

//-----------------------------------------------------------------------------
class ItemRadio extends InventoryItemSuper
{
	proto native void TuneNext();
	proto native void TunePrev();
};

//-----------------------------------------------------------------------------
class ItemBarrel extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class UnderSlugLauncher extends Weapon
{
};

//-----------------------------------------------------------------------------
class ItemGPS extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemCompass extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class CarWheel extends InventoryItemSuper
{
	//! Returns current wheel radius.
	proto native float GetRadius();

	//! Returns wheel width.
	proto native float GetWidth();
};

class CarDoor extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class Clothing extends ItemBase
{
	override bool IsClothing()
	{
		return true;
	}

	// Conditions	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( ( GetNumberOfItems() == 0 || !parent || parent.IsMan() ) && super.CanPutInCargo( parent ) )
		{
			return true;
		}
		
		return false;
	}
};

typedef Clothing ClothingBase;

//-----------------------------------------------------------------------------
class ItemBook extends InventoryItemSuper
{
	override void EEUsed(Man owner)
	{
		if ( GetGame().IsServer() )
		{
			 RPCSingleParam(ERPCs.RPC_READ_A_BOOK, NULL, true, owner.GetIdentity());
		}
	}

	override event bool OnUseFromInventory(Man owner)
	{
		return false;
	}
	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext  ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == ERPCs.RPC_READ_A_BOOK)
		{
			super.EEUsed(NULL);
		}
	}
};

//-----------------------------------------------------------------------------
class ItemGrenade extends InventoryItemSuper
{
	/**@fn		SetPinned
	 * @brief	sets grenade to state GR_PINNED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetPinned ();
	/**@fn		SetUnpinned
	 * @brief	sets grenade to state GR_UNPINNED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetUnpinned ();
	/**@fn		SetIgnited
	 * @brief	sets grenade to state GR_TRIGGERED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetIgnited ();
	/**@fn		IsPinned
	 * @return	true if grenade is pinned
	 **/
	proto native bool IsPinned ();
	/**@fn		IsActivated
	 * @return	true if grenade is GR_TRIGGERED or GR_EXPLODED
	 **/
	proto native bool IsActivated ();
};

typedef ItemGrenade GrenadeBase;

//-----------------------------------------------------------------------------
class ItemMap extends InventoryItemSuper
{
};
