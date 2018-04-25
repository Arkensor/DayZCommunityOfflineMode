///@{ events

/**@enum	HandEventID
 * @brief	identifier for events mainly for rpc purposes
 **/
enum HandEventID
{
	UNKNOWN,
	TAKE,
	MOVETO,
	STASH,
	DROP,
	SWAP,
	FORCESWAP,
	DESTROY,
	CREATED,
	DESTROYED,
	REPLACE,
	REPLACED,
	LOCK,
	UNLOCK
};

/**@class	HandEventBase
 * @brief	represents event that triggers transition from state to state
 **/
class HandEventBase
{
	int m_EventID;
	Man m_Player;
	EntityAI m_Entity;

	void HandEventBase (Man p = null, EntityAI e = NULL) { m_EventID = 0; m_Player = p; m_Entity = e; }
	HandEventID GetEventID () { return m_EventID; }

	void ReadFromContext (ParamsReadContext ctx) { }
	void WriteToContext (ParamsWriteContext ctx)
	{
		ctx.Write(m_EventID);
		ctx.Write(m_Player);
		ctx.Write(m_Entity);
	}

	InventoryLocation GetDst () { return null; }
	bool IsSwapEvent () { return false; }

	static HandEventBase HandEventFactory (HandEventID id, Man p = null, EntityAI e = NULL)
	{
		switch (id)
		{
			case HandEventID.UNKNOWN: return null;
			case HandEventID.TAKE: return new HandEventTake(p, e);
			case HandEventID.MOVETO: return new HandEventMoveTo(p, e);
			case HandEventID.STASH: return new HandEventStash(p, e);
			case HandEventID.DROP: return new HandEventDrop(p, e);
			case HandEventID.SWAP: return new HandEventSwap(p, e);
			case HandEventID.FORCESWAP: return new HandEventForceSwap(p, e);
			case HandEventID.DESTROY: return new HandEventDestroy(p, e);
			case HandEventID.CREATED: return new HandEventCreated(p, e);
			case HandEventID.DESTROYED: return new HandEventDestroyed(p, e);
			case HandEventID.REPLACE: return new HandEventDestroyAndReplaceWithNew(p, e);
			case HandEventID.REPLACED: return new HandEventReplaced(p, e);
			case HandEventID.LOCK: return new HandEventLock(p, e);
			case HandEventID.UNLOCK: return new HandEventUnlock(p, e);
		}
		return null;
	}

	static HandEventBase CreateHandEventFromContext (ParamsReadContext ctx)
	{
		int eventID = -1;
		ctx.Read(eventID);
		Man player;
		ctx.Read(player);
		EntityAI entity;
		ctx.Read(entity);

		HandEventBase b = HandEventFactory(eventID, player, entity);
		b.ReadFromContext(ctx);
		return b;
	}
};

class HandEventTake extends HandEventBase
{
	void HandEventTake (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.TAKE; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		dst.SetHands(m_Player, m_Entity);
		return dst;
	}
};

class HandEventMoveTo extends HandEventBase
{
	ref InventoryLocation m_Dst; /// destination for item in hands

	void HandEventMoveTo (Man p = null, EntityAI e = NULL, InventoryLocation dst = NULL) { m_EventID = HandEventID.MOVETO; m_Dst = dst; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		m_Dst = new InventoryLocation;
		super.ReadFromContext(ctx);
		m_Dst.ReadFromContext(ctx);
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		m_Dst.WriteToContext(ctx);
	}

	override InventoryLocation GetDst () { return m_Dst; }
};

class HandEventStash extends HandEventMoveTo
{
	void HandEventStash (Man p = null, EntityAI e = NULL, InventoryLocation dst = NULL) { m_EventID = HandEventID.STASH; }
};

class HandEventDrop extends HandEventBase
{
	void HandEventDrop (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DROP; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		vector mat[4];
		Math3D.MatrixIdentity4(mat);
		mat[3] = m_Player.GetPosition();

		dst.SetGround(m_Entity, mat);
		return dst;
	}
};
class HandEventSwap extends HandEventBase
{
	void HandEventSwap (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.SWAP; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		dst.SetHands(m_Player, m_Entity);
		return dst;
	}
	
	override bool IsSwapEvent () { return true; }
};
class HandEventForceSwap extends HandEventBase
{
	ref InventoryLocation m_Dst; /// destination of item in hands

	void HandEventForceSwap (Man p = null, EntityAI e = NULL, InventoryLocation dst = NULL) { m_EventID = HandEventID.FORCESWAP; m_Dst = dst; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		m_Dst = new InventoryLocation;
		super.ReadFromContext(ctx);
		m_Dst.ReadFromContext(ctx);
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		m_Dst.WriteToContext(ctx);
	}

	override InventoryLocation GetDst () { return m_Dst; }
	override bool IsSwapEvent () { return true; }
};

class HandEventDestroy extends HandEventBase
{
	void HandEventDestroy (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DESTROY; }
};

class HandEventCreated extends HandEventBase
{
	void HandEventCreated (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.CREATED; }
};

class HandEventDestroyed extends HandEventBase
{
	void HandEventDestroyed (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DESTROYED; }
};

class HandEventDestroyAndReplaceWithNew extends HandEventBase
{
	string m_Type;
	ref ReplaceItemWithNewLambdaBase m_Lambda;

	void HandEventDestroyAndReplaceWithNew (Man p = null, EntityAI e = NULL, ReplaceItemWithNewLambdaBase lambda = NULL) { m_EventID = HandEventID.REPLACE; m_Lambda = lambda; }
	
	override void ReadFromContext (ParamsReadContext ctx)
	{
		super.ReadFromContext(ctx);
		ctx.Read(m_Type);
		// @TODO: lambda?
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		ctx.Write(m_Type);
		// @TODO: lambda?
	}
	override bool IsSwapEvent () { return true; }
};

class HandEventReplaced extends HandEventBase
{
	void HandEventReplaced (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.REPLACED; }
};

class HandEventLock extends HandEventBase
{
	void HandEventLock (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.LOCK; }
};

class HandEventUnlock extends HandEventBase
{
	void HandEventUnlock (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.UNLOCK; }
};

///@} events

