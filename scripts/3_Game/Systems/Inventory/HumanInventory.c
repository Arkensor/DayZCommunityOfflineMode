/**@class		HumanInventory
 * @brief		inventory for plain man/human
 *
 * HumanInventory has simple synchronous operations only, i.e.
 * no animations are involved while adding/removing/swapping to/from hands.
 *
 * Animations are added on higher level of hierarchy (DayZPlayerInventory for example)
 **/
class HumanInventory : GameInventory
{
	/**@fn			GetEntityInHands
	 * @return		entity in hands
	 **/
	proto native EntityAI GetEntityInHands ();

	/**@fn			CanAddEntityInHands
	 * @brief		alternative for TestAddEntityInHands(e, true, true, true);
	 **/
	proto native bool CanAddEntityInHands (EntityAI e);

	/**@fn			TestAddEntityInHands
	 * @param[in]	e	entity to test for taking in hands
	 * @param[in]	do_item_check		deny if entity is not InventoryItem
	 * @param[in]	do_occupancy_test		deny if there is item in hands already
	 * @param[in]	do_script_check		deny if script conditions fail
	 * @return		true if item passed all tests
	 **/
	proto native bool TestAddEntityInHands (EntityAI e, bool do_item_check, bool do_occupancy_test, bool do_script_check);

	/**@fn			CanRemoveEntityInHands
	 * @return		true if entity can be removed from hands
	 **/
	proto native bool CanRemoveEntityInHands ();

	proto native bool CanOpenInventory();

	/**@fn			CreateInHands
	 * @brief		creates new entity in hands
	 * @param[in]	typeName type name of the entity to be created
	 * @return		new entity or null otherwise
	 **/
	proto native EntityAI CreateInHands (string typeName);

	/**
	 * @fn		CreateInInventory
	 * @brief	creates entity somewhere in inventory
	 *
	 * @param[in]	type	\p		item type to be placed in inventory
	 * @return	created entity
	 **/
	override EntityAI CreateInInventory (string type)
	{
		EntityAI newEntity = super.CreateInInventory(type);
		if (newEntity == null)
			newEntity = CreateInHands(type);
		return newEntity;
	}

	Man GetManOwner () { return Man.Cast(GetInventoryOwner()); }

	bool HasEntityInHands (EntityAI e) { return e == GetEntityInHands(); }
	
	void OnEntityInHandsCreated (EntityAI item)
	{
		GetManOwner().PostHandEvent(new HandEventCreated(GetManOwner(), item));
	}

	void OnEntityInHandsDestroyed (EntityAI item)
	{
		GetManOwner().PostHandEvent(new HandEventDestroyed(GetManOwner(), item));
	}

	bool HasLockedHands () { return false; }

	void LockHands ()
	{
		GetManOwner().PostHandEvent(new HandEventLock(GetManOwner(), null));
	}

	void UnlockHands ()
	{
		GetManOwner().PostHandEvent(new HandEventUnlock(GetManOwner(), null));
	}

	void HandEvent (InventoryMode mode, HandEventBase e)
	{
		Print("[inv] HumanInventory::HandEvent(" + typename.EnumToString(InventoryMode, mode) + ")");

		switch (mode)
		{
			case InventoryMode.PREDICTIVE:
				InventoryInputUserData.SendInputUserDataHandEvent(e);
				GetManOwner().PostHandEvent(e);
				break;

			case InventoryMode.JUNCTURE:
				InventoryInputUserData.SendInputUserDataHandEvent(e);
				break;

			case InventoryMode.LOCAL:
				GetManOwner().PostHandEvent(e);
				break;
			
			case InventoryMode.SERVER:
                InventoryInputUserData.SendServerHandEvent(GetManOwner(), e);
                GetManOwner().PostHandEvent(e);
                break;

			default:
				Error("HandEvent - Invalid mode");
		}
	}
	
	override bool DropEntity (InventoryMode mode, EntityAI owner, notnull EntityAI item)
	{
		InventoryLocation src = new InventoryLocation;
		if (item && item.GetInventory() && item.GetInventory().GetCurrentInventoryLocation(src))
		{
			switch (src.GetType())
			{
				case InventoryLocationType.HANDS:
					Print("[inv] HumanInventory::DropEntity(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
					HandEvent(mode, new HandEventDrop(GetManOwner(), item));
					return true;

				default: return super.DropEntity(mode, owner, item);
			}
		}
		Error("No inventory location");
		return false;
	}

	override bool TakeEntityToInventory (InventoryMode mode, FindInventoryLocationType flags, notnull EntityAI item)
	{
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			if (FindFreeLocationFor(item, flags, dst))
			{
				if (dst.GetType() == InventoryLocationType.HANDS)
				{
					Print("[inv] HumanInventory::Take2H(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
					HandEvent(mode, new HandEventTake(GetManOwner(), item));
					return true;
				}
				else
				{
					switch (src.GetType())
					{
						case InventoryLocationType.HANDS:
						{
							if (GetInventoryOwner().IsAlive())
							{
								Print("[inv] HumanInventory::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") from H (alive) item=" + item);

								HandEvent(mode, new HandEventStash(GetManOwner(), item, dst));
								return true;
							}

							Print("[inv] HumanInventory::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") from H (dead) item=" + item);
							return super.TakeEntityToInventory(mode, flags, item);
						}
						default:
						{
							Print("[inv] HumanInventory::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
							return super.TakeEntityToInventory(mode, flags, item);
						}
					}
				}
			}
			else
			{
				// no room
				return false;
			}
		}
		Error("TakeEntityToInventory: No inventory location");
		return false;
	}

	/**
	\brief Put item into into cargo on specific cargo location
	*/
	override bool TakeEntityToCargoEx (InventoryMode mode, notnull EntityAI item, int idx, int row, int col)
	{
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			switch (src.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					if (GetInventoryOwner().IsAlive())
					{
						Print("[inv] HumanInventory::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " row=" + row + " col=" + col);
						InventoryLocation dst = new InventoryLocation;
						dst.SetCargo(GetInventoryOwner(), item, idx, row, col);

						HandEvent(mode, new HandEventStash(GetManOwner(), item, dst));
						return true;
					}
					return super.TakeEntityToCargoEx(mode, item, idx, row, col);
				}
				default: return super.TakeEntityToCargoEx(mode, item, idx, row, col);
			}
		}
		Error("TakeEntityToCargoEx: No inventory location");
		return false;
	}

	override bool TakeEntityAsAttachmentEx (InventoryMode mode, notnull EntityAI item, int slot)
	{
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			switch (src.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					if (GetInventoryOwner().IsAlive())
					{
						Print("[inv] HumanInventory::Take2Att(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " slot=" + slot);
						InventoryLocation dst = new InventoryLocation;
						EntityAI src_entity = GetInventoryOwner();
						dst.SetAttachment(src_entity, item, slot);

						HandEvent(mode, new HandEventStash(GetManOwner(), item, dst));
						return true;
					}
					return super.TakeEntityAsAttachmentEx(mode, item, slot);
				}
				default: return super.TakeEntityAsAttachmentEx(mode, item, slot);
			}
		}
		Error("TakeEntityAsAttachmentEx: No inventory location");
		return false;
	}

	override bool SwapEntities (InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2)
	{
		InventoryLocation src1 = new InventoryLocation;
		InventoryLocation src2 = new InventoryLocation;
		if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
		{
			Print("[inv] HumanInventory::Swap(" + typename.EnumToString(InventoryMode, mode) + ") src1=" + src1.DumpToString() + "src2=" + src2.DumpToString());
			bool handled = false;
			switch (src1.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					Print("[inv] HumanInventory::Swap(" + typename.EnumToString(InventoryMode, mode) + ")IH=src1=" + src1.DumpToString() + " src2=" + src2.DumpToString());
					HandEvent(mode, new HandEventSwap(GetManOwner(), item2));
					handled = true;
					break;
				}
			}

			switch (src2.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					Print("[inv] HumanInventory::Swap(" + typename.EnumToString(InventoryMode, mode) + ")src1=" + src1.DumpToString() + " IH=src2=" + src2.DumpToString());
					HandEvent(mode, new HandEventSwap(GetManOwner(), item1));
					handled = true;
					break;
				}
			}

			if (!handled)
				return super.SwapEntities(mode, item1, item2);
			return true;
		}
		Error("SwapEntities: No inventory location");
		return false;
	}

	override bool ForceSwapEntities (InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation item2_dst)
	{
		InventoryLocation src1 = new InventoryLocation;
		InventoryLocation src2 = new InventoryLocation;
		if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
		{
			Print("[inv] HumanInventory::FSwap(" + typename.EnumToString(InventoryMode, mode) + ") src1=" + src1.DumpToString() + " src2=" + src2.DumpToString());
			bool handled = false;
			switch (src1.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					Print("[inv] HumanInventory::FSwap(" + typename.EnumToString(InventoryMode, mode) + ") IH=src1=" + src1.DumpToString() + " src2=" + src2.DumpToString());
					HandEvent(mode, new HandEventForceSwap(GetManOwner(), item2));
					handled = true;
					break;
				}
			}

			switch (src2.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					Print("[inv] HumanInventory::FSwap(" + typename.EnumToString(InventoryMode, mode) + ") src1=" + src1.DumpToString() + " IH=src2=" + src2.DumpToString());
					HandEvent(mode, new HandEventForceSwap(GetManOwner(), item1, item2_dst));
					handled = true;
					break;
				}
			}

			if (!handled)
				return super.SwapEntities(mode, item1, item2);
			return true;
		}
		Error("ForceSwapEntities: No inventory location");
		return false;
	}

	override bool LocalDestroyEntity (notnull EntityAI item)
	{
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			switch (src.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					if (GetInventoryOwner().IsAlive())
					{
						Print("[inv] HumanInventory::LocalDestroy inv item=" + item);
						HandEvent(InventoryMode.LOCAL, new HandEventDestroy(GetManOwner(), item));
						return true;
					}
					return super.LocalDestroyEntity(item);
				}
				default: return super.LocalDestroyEntity(item);
			}
		}
		Error("LocalDestroyEntity: No inventory location");
		return false;
	}

	override bool LocalReplaceItemWithNew (ReplaceItemWithNewLambdaBase lambda)
	{
		EntityAI itemInHands = GetEntityInHands();
		if (itemInHands == lambda.m_OldItem)
			return LocalReplaceItemInHandsWithNew(lambda);
		return super.LocalReplaceItemWithNew(lambda);
	}

	bool LocalReplaceItemInHandsWithNew (ReplaceItemWithNewLambdaBase lambda)
	{
		EntityAI itemInHands = GetEntityInHands();
		InventoryLocation src = new InventoryLocation;
		if (itemInHands && itemInHands.GetInventory().GetCurrentInventoryLocation(src))
		{
			switch (src.GetType())
			{
				case InventoryLocationType.HANDS:
				{
					if (GetInventoryOwner().IsAlive())
					{
						Print("[inv] HumanInventory::LocalReplace IH=" + lambda.DumpToString());
						HandEvent(InventoryMode.LOCAL, HandEventDestroyAndReplaceWithNew(GetManOwner(), null, lambda));
						return true;
					}

					Error("TODO"); // replace-with-new in corpse's hands, not implemented
					return false;
				}
				default:
					Error("[inv] src has to be hands");
					return false;
			}
		}
		Error("LocalReplaceItemInHandsWithNew: No");
		return false;
	}
};

