/// types of Inventory Location
enum InventoryLocationType
{
	UNKNOWN, ///< unknown, usually freshly created object
	GROUND, /// < ground
	ATTACHMENT, ///< attachment of another entity
	CARGO, ///< cargo of another entity
	HANDS, ///< hands of another entity
	PROXYCARGO, ///< cargo of a large object (building,...)
};

/// flags for searching locations in inventory
enum FindInventoryLocationType
{
	ATTACHMENT, 	///< 	ATT
	CARGO,			///< 	CGO
	HANDS, 			///< 	HND
	PROXYCARGO, 	///< 	PXY
	ANY_CARGO, 		///< 	CGO | PXY
	ANY, 			///< 	ATT | CGO | PXY | HND
};

//! InventoryLocation
class InventoryLocation
{
	/**
	 * @fn			IsValid
	 * @brief		verify current set inventory location
	 * @return	true if valid, false otherwise
	 **/
	proto native bool IsValid ();
	/**
	 * @fn		GetType
	 * @brief	returns type of InventoryLocation
	 *
	 * @see	InventoryLocationType for known types of inventory locations
	 **/
	proto native int GetType ();
	/**
	 * @fn		GetParent
	 * @brief	returns parent of current inventory location
	 *
	 * Parent entity can be null if item is on ground.
	 *
	 * @return parent entity, null otherwise
	 **/
	proto native EntityAI GetParent ();
	/**
	 * @fn		GetItem
	 * @brief	returns item of current inventory location
	 *
	 * Item can be null if and only if this is a query for location
	 * of item that is about to be created (new spawn).
	 *
	 * @return	item
	 **/
	proto native EntityAI GetItem ();
	/**
	 * @fn		GetSlot
	 * @brief	returns slot id if current type is Attachment
	 *
	 * @NOTE: slot id is related to order of entries in DZ/data/config.cpp secton CfgSlots
	 *
	 * @return	slot id if attachment, -1 otherwise
	 **/
	proto native int GetSlot ();
	/**
	 * @fn		GetIdx
	 * @brief	returns index of cargo if current type is Cargo / ProxyCargo
	 * @return	index or -1
	 **/
	proto native int GetIdx ();
	/**
	 * @fn		GetRow
	 * @brief	returns row of cargo if current type is Cargo / ProxyCargo
	 * @return	row or -1
	 **/
	proto native int GetRow ();
	/**
	 * @fn		GetCol
	 * @brief	returns column of cargo if current type is Cargo / ProxyCargo
	 * @return	index or -1
	 **/
	proto native int GetCol ();
	/**
	 * @fn		GetPos
	 * @brief	returns position of item in world if type is Ground
	 * @return	position
	 **/
	proto native vector GetPos ();
	/**
	 * @fn		GetDir
	 * @brief	returns direction of item in world if type is Ground
	 * @return	position
	 **/	
	proto native void GetDir (out float dir[4]);

	/**
	 * @fn		SetGround
	 * @brief	sets current inventory location type to Ground with transformation mat
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 * @param[in]	mat	transformation matrix
	 **/
	proto native void SetGround (EntityAI e, vector mat[4]);
	/**
	 * @fn		SetGroundEx
	 * @brief	sets current inventory location type to Ground with transformation mat
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 * @param[in]	pos		position vector
	 * @param[in]	dir		direction quat
	 **/
	proto native void SetGroundEx (EntityAI e, vector pos, float dir[4]);
	/**
	 * @fn		SetAttachment
	 * @brief	sets current inventory location type to Attachment with slot id set to <slotId>
	 * @param[in]	parent		parent entity the item <e> will be attached to
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 * @param[in]	slotId		slot id where the item will be attached
	 **/
	proto native void SetAttachment (notnull EntityAI parent, EntityAI e, int slotId);
	/**
	 * @fn		SetCargo
	 * @brief	sets current inventory location type to Cargo with coordinates (idx, row, col)
	 * @param[in]	parent		parent entity that will receive item <e> into cargo
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 * @param[in]	idx		index of cargo
	 * @param[in]	row		row of the cargo
	 * @param[in]	col		column of the cargo
	 **/
	proto native void SetCargo (notnull EntityAI parent, EntityAI e, int idx, int row, int col);
	/**
	 * @fn		SetProxyCargo
	 * @brief	sets current inventory location type to ProxyCargo with coordinates (idx, row, col)
	 * @NOTE: typical usage is in building-like dummy structures, where furniture with cargo is part
	 * 		of the model, and is not a physical entity (like person or bag).
	 * @param[in]	parent		parent entity that will receive item <e> into proxy cargo
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 * @param[in]	idx		index of cargo
	 * @param[in]	row		row of the cargo
	 * @param[in]	col		column of the cargo
	 *
	 **/
	proto native void SetProxyCargo (notnull EntityAI parent, EntityAI e, int idx, int row, int col);
	/**
	 * @fn		SetHands
	 * @brief	sets current inventory location type to Hands
	 * @param[in]	parent		parent entity the item <e> will be attached to. this'd better be inherited from Person
	 * @param[in]	e		entity with this inventory location (null for new spawns)
	 **/
	proto native void SetHands (notnull EntityAI parent, EntityAI e);
	/**
	 * @fn		Reset
	 * @brief	resets inventory location to UNKNOWN
	 * @NOTE	IsValid returns false afterwards
	 **/
	proto native void Reset ();

	/**
	 * @fn		Copy
	 * @brief	copies location data to another location
	 *
	 * @NOTE	it copies all data (except enforce internal data)
	 **/
	proto native InventoryLocation Copy (notnull InventoryLocation rhs);
	/**
	 * @fn		CopyLocationFrom
	 * @brief	copies location to another location without m_item member
	 *
	 * @NOTE	it does NOT do a copy of m_item member!
	 **/
	proto native InventoryLocation CopyLocationFrom (notnull InventoryLocation rhs);
	
	string DumpToString ()
	{
		string res = "{ type=" + typename.EnumToString(InventoryLocationType, GetType());
		switch (GetType())
		{
			case InventoryLocationType.UNKNOWN:
			{
				break;
			}
			case InventoryLocationType.GROUND:
			{
				res = res + " item=" + GetItem();
				res = res + " pos=" + GetPos();
				break;
			}
			case InventoryLocationType.ATTACHMENT:
			{
				res = res + " item=" + GetItem();
				res = res + " parent=" + GetParent();
				res = res + " slot=" + GetSlot();
				break;
			}
			case InventoryLocationType.CARGO:
			{
				res = res + " item=" + GetItem();
				res = res + " parent=" + GetParent();
				res = res + " idx=" + GetIdx() + " row=" + GetRow() + " col=" + GetCol();
				break;
			}
			case InventoryLocationType.HANDS:
			{
				res = res + " item=" + GetItem();
				res = res + " parent=" + GetParent();
				break;
			}
			case InventoryLocationType.PROXYCARGO:
			{
				res = res + " item=" + GetItem();
				res = res + " parent=" + GetParent();
				res = res + " idx=" + GetIdx() + " row=" + GetRow() + " col=" + GetCol();
				break;
			}
			default:
			{
				res = res + "??";
				break;
			}
		}
		res = res + " }";
		return res;
	}

	bool ReadFromContext (ParamsReadContext ctx)
	{
		EntityAI parent;
		EntityAI item;
		int type;
		int idx;
		int row;
		int col;
		if (!ctx.Read(type))
			return false;

		switch (type)
		{
			case InventoryLocationType.UNKNOWN:
			{
				break;
			}
			case InventoryLocationType.GROUND:
			{
				if (!ctx.Read(item))
					return false;
				vector pos;
				if (!ctx.Read(pos))
					return false;
				float dir[4];
				if (!ctx.Read(dir))
					return false;
				
				if (!item)
					break; // item is not in bubble

				SetGroundEx(item, pos, dir);
				break;
			}
			case InventoryLocationType.ATTACHMENT:
			{
				if (!ctx.Read(parent))
					return false;
				if (!ctx.Read(item))
					return false;
				int slot;
				if (!ctx.Read(slot))
					return false;
				
				if (!parent || !item)
					break; // parent or item is not in bubble

				SetAttachment(parent, item, slot);
				break;
			}
			case InventoryLocationType.CARGO:
			{
				if (!ctx.Read(parent))
					return false;
				if (!ctx.Read(item))
					return false;
				if (!ctx.Read(idx))
					return false;
				if (!ctx.Read(row))
					return false;
				if (!ctx.Read(col))
					return false;
				
				if (!parent || !item)
					break; // parent or item is not in bubble

				SetCargo(parent, item, idx, row, col);
				break;
			}
			case InventoryLocationType.HANDS:
			{
				if (!ctx.Read(parent))
					return false;
				if (!ctx.Read(item))
					return false;
				
				if (!parent || !item)
					break; // parent or item is not in bubble

				SetHands(parent, item);
				break;
			}
			case InventoryLocationType.PROXYCARGO:
			{
				if (!ctx.Read(parent))
					return false;
				if (!ctx.Read(item))
					return false;
				if (!ctx.Read(idx))
					return false;
				if (!ctx.Read(row))
					return false;
				if (!ctx.Read(col))
					return false;
				
				if (!parent || !item)
					break; // parent or item is not in bubble

				SetProxyCargo(parent, item, idx, row, col);
				break;
			}
			default:
			{
				Error("ReadFromContext - really unknown location type, this should not happen, type=" + type);
				return false;
			}
		}
		return true;
	}

	void WriteToContext (ParamsWriteContext ctx)
	{
		ctx.Write(GetType());
		switch (GetType())
		{
			case InventoryLocationType.UNKNOWN:
			{
				break;
			}
			case InventoryLocationType.GROUND:
			{
				ctx.Write(GetItem());
				vector pos = GetPos();
				ctx.Write(pos);
				float dir[4];
				GetDir(dir);
				ctx.Write(dir);
				break;
			}
			case InventoryLocationType.ATTACHMENT:
			{
				ctx.Write(GetParent());
				ctx.Write(GetItem());
				ctx.Write(GetSlot());
				break;
			}
			case InventoryLocationType.CARGO:
			{
				ctx.Write(GetParent());
				ctx.Write(GetItem());
				ctx.Write(GetIdx());
				ctx.Write(GetRow());
				ctx.Write(GetCol());
				break;
			}
			case InventoryLocationType.HANDS:
			{
				ctx.Write(GetParent());
				ctx.Write(GetItem());
				break;
			}
			case InventoryLocationType.PROXYCARGO:
			{
				ctx.Write(GetParent());
				ctx.Write(GetItem());
				ctx.Write(GetIdx());
				ctx.Write(GetRow());
				ctx.Write(GetCol());
				break;
			}
			default:
			{
				Error("WriteToContext - really unknown location type, this should not happen, type=" + GetType());
				break;
			}
		}
	}
};

