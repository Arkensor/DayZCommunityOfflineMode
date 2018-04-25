
class InventoryInputUserData
{
	///@{ move
	static void SerializeMove (ParamsWriteContext ctx, int type, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		ctx.Write(7); // le fu
		ctx.Write(type);
		src.WriteToContext(ctx);
		dst.WriteToContext(ctx);
	}

	static void SendInputUserDataMove (int type, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms sending cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeMove(ctx, type, src, dst);
			ctx.Send();
		}
	}

	static void SendServerMove (int type, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		if (GetGame().IsServer())
		{
			syncDebugPrint("[syncinv] server sending cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeMove(ctx, type, src, dst);
			GameInventory.ServerLocationSyncMoveEntity(src.GetItem(), ctx);
		}
	}
	///@} move

	///@{ swap
	static void SerializeSwap (ParamsWriteContext ctx, notnull EntityAI item1, notnull EntityAI item2)
	{
		ctx.Write(7);
		ctx.Write(InventoryCommandType.SWAP);
		ctx.Write(item1);
		ctx.Write(item2);
	}

	static void SendInputUserDataSwap (notnull EntityAI item1, notnull EntityAI item2)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms sending cmd=SWAP item1=" + item1 + " item2=" + item2);
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeSwap(ctx, item1, item2);
			ctx.Send();
		}
	}

	static void SendServerSwap (notnull EntityAI item1, notnull EntityAI item2)
	{
		if (GetGame().IsServer())
		{
			syncDebugPrint("[syncinv] server sending cmd=SWAP item1=" + item1 + " item2=" + item2);
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeSwap(ctx, item1, item2);
			GameInventory.ServerLocationSwap(item1, item2, ctx);
		}
	}
	///@} swap
	
	///@{ f-swap
	static void SerializeForceSwap (ParamsWriteContext ctx, notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation dst2)
	{
		ctx.Write(7);
		ctx.Write(InventoryCommandType.FORCESWAP);
		ctx.Write(item1);
		ctx.Write(item2);
		dst2.WriteToContext(ctx);
	}

	static void SendInputUserDataForceSwap (notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation dst2)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms SendInputUserDataForceSwap cmd=FORCESWAP item1=" + item1 + " item2=" + item2 + " dst2=" + dst2.DumpToString());
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeForceSwap(ctx, item1, item2, dst2);
			ctx.Send();
		}
	}

	static void SendServerForceSwap (notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation dst2)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] SendInputUserDataForceSwap cmd=FORCESWAP item1=" + item1 + " item2=" + item2 + " dst2=" + dst2.DumpToString());
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeForceSwap(ctx, item1, item2, dst2);
			GameInventory.ServerLocationSwap(item1, item2, ctx);
		}
	}
	///@} f-swap

	///@{ hand
	static void SerializeHandEvent (ParamsWriteContext ctx, HandEventBase e)
	{
		ctx.Write(7);
		ctx.Write(InventoryCommandType.HAND_EVENT);
		e.WriteToContext(ctx);
	}

	static void SendInputUserDataHandEvent (HandEventBase e)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms SendInputUserDataHandEvent e=" + e);
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeHandEvent(ctx, e);
			ctx.Send();
		}
	}

	static void SendServerHandEvent (notnull Man player, HandEventBase e)
	{
		if (GetGame().IsServer())
		{
			syncDebugPrint("[syncinv] SendInputUserDataHandEvent e=" + e);
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeHandEvent(ctx, e);
			GameInventory.ServerHandEvent(player, e.m_Entity, ctx);
		}
	}
	///@} hand

	static void SerializeDestroy (ParamsWriteContext ctx, notnull InventoryLocation src)
	{
		ctx.Write(7);
		ctx.Write(InventoryCommandType.DESTROY);
		src.WriteToContext(ctx);
	}

	static void SendInputUserDataDestroy (notnull InventoryLocation src)
	{
		if (GetGame().IsClient())
		{
			syncDebugPrint("[syncinv] SendInputUserDataDestroy src=" + src.DumpToString());
			ScriptInputUserData ctx = new ScriptInputUserData;
			SerializeDestroy(ctx, src);
			ctx.Send();
		}
	}
};

