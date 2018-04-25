///@{ actions

/**@class	HandActionBase
 * @brief	represents action executed on transition just between OnExit from old state and OnEntry to new state
 **/
class HandActionBase
{
	/**@fn			Action
	 * @brief		executed when transition occurs
	 **/
	void Action (HandEventBase e) { }
};


class HandActionCreated extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=created");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			player.OnItemInHandsChanged();
		}
		else
			Error("[hndfsm] HandActionCreated - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
	}
};

class HandActionTake extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=take");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			dst.SetHands(player, item);

			GameInventory.LocationSyncMoveEntity(src, dst);

			player.OnItemInHandsChanged();
		}
		else
			Error("[hndfsm] HandActionTake - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
	}
};

class HandActionDrop extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=drop");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			GameInventory.SetGroundPosByOwner(player, item, dst);

			GameInventory.LocationSyncMoveEntity(src, dst);

			player.OnItemInHandsChanged();
		}
		else
			Error("[hndfsm] HandActionDrop - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
	}
};

class HandActionStash extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=stash");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		HandEventStash es = HandEventStash.Cast(e);
		if (es)
		{
			InventoryLocation src = new InventoryLocation;
			if (item.GetInventory().GetCurrentInventoryLocation(src))
			{
				GameInventory.LocationSyncMoveEntity(src, es.m_Dst);

				player.OnItemInHandsChanged();
			}
			else
				Error("[hndfsm] HandActionStash - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
		}
		else
			Error("[hndfsm] HandActionStash - this is no HandEventStash");
	}
};

class HandActionMoveTo extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=moveTo");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		HandEventMoveTo es = HandEventMoveTo.Cast(e);
		if (es)
		{
			InventoryLocation src = new InventoryLocation;
			if (item.GetInventory().GetCurrentInventoryLocation(src))
			{
				GameInventory.LocationSyncMoveEntity(src, es.m_Dst);

				player.OnItemInHandsChanged();
			}
			else
				Error("[hndfsm] HandActionMoveTo - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
		}
		else
			Error("[hndfsm] HandActionMoveTo - this is no HandEventMoveTo");
	}
};

class HandActionDestroy extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=destroy");
		Man player = e.m_Player;
		EntityAI item = e.m_Entity;

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			GetGame().ObjectDelete(src.GetItem());

			player.OnItemInHandsChanged();
		}
		else
			Error("[hndfsm] HandActionDestroy - item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
	}
};

class HandActionDestroyed extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=destroyed");
		Man player = e.m_Player;

		player.OnItemInHandsChanged();
	}
};

class HandActionDestroyAndReplaceWithNew extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		Man player = e.m_Player;
		EntityAI itemInHands = player.GetHumanInventory().GetEntityInHands();

		InventoryLocation src = new InventoryLocation;
		if (itemInHands.GetInventory().GetCurrentInventoryLocation(src))
		{
			HandEventDestroyAndReplaceWithNew edr = HandEventDestroyAndReplaceWithNew.Cast(e);
			if (edr)
			{
				hndDebugPrint("[hndfsm] action=replace with new");

				edr.m_Lambda.Execute();
				//player.GetItemAccessor().OnItemInHandsChanged();
				return;
			}
			else
				Error("[hndfsm] HandActionDestroyAndReplaceWithNew - not a HandEventDestroyAndReplaceWithNew event");
		}
		else
			Error("[hndfsm] HandActionDestroyAndReplaceWithNew - itemInHands has no InventoryLocation");
	}
};

class HandActionReplaced extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=replaced");
		Man player = e.m_Player;

		player.OnItemInHandsChanged();
	}
};

class HandActionSwap extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=swap");
		Man player = e.m_Player;
		EntityAI itemInHands = player.GetHumanInventory().GetEntityInHands();
		EntityAI itemToHands = e.m_Entity;

		hndDebugPrint("[hndfsm] action=swap + IH=" + itemInHands + "I2H=" + itemToHands);

		HandEventSwap es = HandEventSwap.Cast(e);
		if (es)
		{
			InventoryLocation src1, src2, dst1, dst2;
			if (GameInventory.MakeSrcAndDstForSwap(itemInHands, itemToHands, src1, src2, dst1, dst2))
			{
				hndDebugPrint("[hndfsm] Swap src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString() + "dst2=" + dst2.DumpToString());

				GameInventory.LocationSwap(src1, src2, dst1, dst2);

				player.OnItemInHandsChanged();
			}
			else
				Error("[hndfsm] HandActionSwap - cannot get inv location of items");
		}
		else
			Error("[hndfsm] HandActionSwap - this is no HandEventSwap");
	}
};

class HandActionForceSwap extends HandActionBase
{
	override void Action (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] action=forceswap");

		Man player = e.m_Player;
		EntityAI itemToHands = e.m_Entity;
		EntityAI itemInHands = player.GetHumanInventory().GetEntityInHands();
		//hndDebugPrint("[hndfsm] action=forceswap + I2H=" + itemToHands + "IH=" + itemInHands);

		HandEventForceSwap es = HandEventForceSwap.Cast(e);
		if (es)
		{
			InventoryLocation src1 = new InventoryLocation;
			InventoryLocation src2 = new InventoryLocation;
			InventoryLocation dst1 = new InventoryLocation;
			if (itemToHands.GetInventory().GetCurrentInventoryLocation(src1) && itemInHands.GetInventory().GetCurrentInventoryLocation(src2))
			{
				dst1.Copy(src1);
				dst1.CopyLocationFrom(src2);

				//hndDebugPrint("[hndfsm] FSwap e.m_dst2=" + es.m_Dst.DumpToString());
				//hndDebugPrint("[hndfsm] FSwap src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString());
				GameInventory.LocationSwap(src1, src2, dst1, es.m_Dst);
				player.OnItemInHandsChanged();
			}
		}
		else
			Error("[hndfsm] HandActionSwap - this is no HandEventSwap");
	}
};

///@} actions

