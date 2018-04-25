class Man extends EntityAI
{
	proto native void DisableSimulation(bool disable);
	//! Returns player's identity
	proto native PlayerIdentity GetIdentity();
	//! Returns vehicle which this Man object is driving. If this Man object isn't as driver of any vehicle it will return NULL.
	proto native EntityAI GetDrivingVehicle();

	proto native string GetCurrentWeaponMode();
	proto native float GetCurrentZeroing();

	//! Set speech restriction
	proto native void SetSpeechRestricted(bool state);
	//! Check if player has resctricted speech
	proto native bool IsSpeechRestricted();

	//! Texture that is used on the Man's face and hands
	proto native void SetFaceTexture(string texture_name);
	//! Material that is used on the Man's face and hands
	proto native void SetFaceMaterial(string material_name);

	proto native float IsSoundInsideBuilding();

	void Man()
	{
		SetFlags(EntityFlags.TOUCHTRIGGERS, false);
	}

	override bool IsMan() { return true; }
	override bool IsHealthVisible() { return false; }

	int GetPlayerState ()
	{
		if( IsAlive() ) return EPlayerStates.ALIVE;
		else return EPlayerStates.DEAD;
	}

	///@{ inventory
	HumanInventory GetHumanInventory ()
	{
		HumanInventory i = HumanInventory.Cast(GetInventory());
		return i;
	}

	void EEItemIntoHands (EntityAI item) { }
	void EEItemOutOfHands (EntityAI item) { }

	///@{ drop juncture
	bool JunctureDropEntity (notnull EntityAI item)
	{
		return DropEntityImpl(InventoryMode.JUNCTURE, item);
	}

	override bool PredictiveDropEntity (notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveDropEntity input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			return JunctureDropEntity(item);
		else
			return DropEntityImpl(InventoryMode.PREDICTIVE, item);
	}

	override bool LocalDropEntity (notnull EntityAI item)
	{
		return DropEntityImpl(InventoryMode.LOCAL, item);
	}

	override bool ServerDropEntity (notnull EntityAI item)
	{
		return DropEntityImpl(InventoryMode.SERVER, item);
	}

	protected bool DropEntityImpl (InventoryMode mode, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::DropEntity(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = GetHumanInventory().DropEntity(mode, this, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} drop juncture

	override bool CanDropEntity (notnull EntityAI item) { return true; }

	void PostHandEvent (HandEventBase e) { }
	void OnItemInHandsChanged () { }

	bool NeedInventoryJunctureFromServer (notnull EntityAI item, EntityAI currParent, EntityAI newParent) { return false; }

	///@{ hand juncture
	void JunctureTakeEntityToHands (notnull EntityAI item)
	{
		TakeEntityToHandsImpl(InventoryMode.JUNCTURE, item);
	}

	void PredictiveTakeEntityToHands (EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToHands input data not sent yet, cannot allow another input action");
			return;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			JunctureTakeEntityToHands(item);
		else
			TakeEntityToHandsImpl(InventoryMode.PREDICTIVE, item);
	}

	void LocalTakeEntityToHands (EntityAI item)
	{
		TakeEntityToHandsImpl(InventoryMode.LOCAL, item);
	}

	protected void TakeEntityToHandsImpl (InventoryMode mode, EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2Hands(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		EntityAI itemInHands = GetHumanInventory().GetEntityInHands();
		if (itemInHands == null)
			GetHumanInventory().HandEvent(mode, new HandEventTake(this, item));
		else if (GetHumanInventory().CanSwapEntities(itemInHands, item))
			GetInventory().SwapEntities(mode, itemInHands, item);
		UpdateInventoryMenu();
	}
	///@} hand juncture

	void LocalDestroyEntityInHands ()
	{
		syncDebugPrint("[inv] (Man) Destroy IH=" + GetHumanInventory().GetEntityInHands());
		GetHumanInventory().LocalDestroyEntity(GetHumanInventory().GetEntityInHands());
		UpdateInventoryMenu();
	}

	void PredictiveMoveItemFromHandsToInventory ()
	{
		syncDebugPrint("[inv] (Man) Stash IH=" + GetHumanInventory().GetEntityInHands());
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveMoveItemFromHandsToInventory input data not sent yet, cannot allow another input action");
			return;
		}

		GetHumanInventory().TakeEntityToInventory(InventoryMode.PREDICTIVE, FindInventoryLocationType.ATTACHMENT | FindInventoryLocationType.CARGO, GetHumanInventory().GetEntityInHands());
		UpdateInventoryMenu();
	}

	bool LocalReplaceItemWithNew (ReplaceItemWithNewLambdaBase lambda)
	{
		syncDebugPrint("[inv] (Man) Replace lambda=" + lambda.DumpToString());
		bool code = GetHumanInventory().LocalReplaceItemWithNew(lambda);
		UpdateInventoryMenu();
		return code;
	}

	bool LocalReplaceItemInHandsWithNew (ReplaceItemWithNewLambdaBase lambda)
	{
		syncDebugPrint("[inv] (Man) Replace HND lambda=" + lambda.DumpToString());
		bool code = GetHumanInventory().LocalReplaceItemInHandsWithNew(lambda);
		UpdateInventoryMenu();
		return code;
	}

	///@{ to inv juncture
	bool JunctureTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		return TakeEntityToInventoryImpl(InventoryMode.JUNCTURE, flags, item);
	}

	override bool PredictiveTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToInventory input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			return JunctureTakeEntityToInventory(flags, item);
		else
			return TakeEntityToInventoryImpl(InventoryMode.PREDICTIVE, flags, item);
	}

	override bool LocalTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		return TakeEntityToInventoryImpl(InventoryMode.LOCAL, flags, item);
	}

	protected bool TakeEntityToInventoryImpl (InventoryMode mode, FindInventoryLocationType flags, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = GetHumanInventory().TakeEntityToInventory(mode, flags, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} to inv juncture

	///@{ to cgo juncture
	bool JunctureTakeEntityToCargo (notnull EntityAI item)
	{
		return TakeEntityToCargoImpl(InventoryMode.JUNCTURE, item);
	}

	override bool PredictiveTakeEntityToCargo (notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToCargo input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			return JunctureTakeEntityToCargo(item);
		else
			return TakeEntityToCargoImpl(InventoryMode.PREDICTIVE, item);
	}

	override bool LocalTakeEntityToCargo (notnull EntityAI item)
	{
		return TakeEntityToCargoImpl(InventoryMode.LOCAL, item);
	}

	protected bool TakeEntityToCargoImpl (InventoryMode mode, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = GetHumanInventory().TakeEntityToCargo(mode, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} to cgo juncture

	///@{ as att juncture
	bool JunctureTakeEntityAsAttachment (notnull EntityAI item)
	{
		return TakeEntityAsAttachmentImpl(InventoryMode.JUNCTURE, item);
	}

	override bool PredictiveTakeEntityAsAttachment (notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityAsAttachment input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			return JunctureTakeEntityAsAttachment(item);
		else
			return TakeEntityAsAttachmentImpl(InventoryMode.PREDICTIVE, item);
	}

	override bool LocalTakeEntityAsAttachment (notnull EntityAI item)
	{
		return TakeEntityAsAttachmentImpl(InventoryMode.LOCAL, item);
	}

	protected bool TakeEntityAsAttachmentImpl (InventoryMode mode, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2Att(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = GetHumanInventory().TakeEntityAsAttachment(mode, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} as att juncture

	///@{ as att ex juncture
	bool JunctureTakeEntityAsAttachmentEx (notnull EntityAI item, int slot)
	{
		return TakeEntityAsAttachmentExImpl(InventoryMode.JUNCTURE, item, slot);
	}

	override bool PredictiveTakeEntityAsAttachmentEx (notnull EntityAI item, int slot)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityAsAttachmentEx input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), this))
			return JunctureTakeEntityAsAttachmentEx(item, slot);
		else
			return TakeEntityAsAttachmentExImpl(InventoryMode.PREDICTIVE, item, slot);
	}

	override bool LocalTakeEntityAsAttachmentEx (notnull EntityAI item, int slot)
	{
		return TakeEntityAsAttachmentExImpl(InventoryMode.LOCAL, item, slot);
	}

	protected bool TakeEntityAsAttachmentExImpl (InventoryMode mode, notnull EntityAI item, int slot)
	{
		syncDebugPrint("[inv] Man::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = GetHumanInventory().TakeEntityAsAttachmentEx(mode, item, slot);
		UpdateInventoryMenu();
		return code;
	}
	///@} as att ex juncture

	///@{ swap juncture
	bool JunctureSwapEntities (notnull EntityAI item1, notnull EntityAI item2)
	{
		return SwapEntitiesImpl(InventoryMode.JUNCTURE, item1, item2);
	}

	bool PredictiveSwapEntities (notnull EntityAI item1, notnull EntityAI item2)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveSwapEntities input data not sent yet, cannot allow another input action");
			return false;
		}

		bool need_j1 = NeedInventoryJunctureFromServer(item1, item1.GetHierarchyParent(), item2.GetHierarchyParent());
		bool need_j2 = NeedInventoryJunctureFromServer(item2, item2.GetHierarchyParent(), item1.GetHierarchyParent());
		if (need_j1 || need_j2)
			return SwapEntitiesImpl(InventoryMode.JUNCTURE, item1, item2);
		else
			return SwapEntitiesImpl(InventoryMode.PREDICTIVE, item1, item2);
	}

	bool LocalSwapEntities (notnull EntityAI item1, notnull EntityAI item2)
	{
		return SwapEntitiesImpl(InventoryMode.LOCAL, item1, item2);
	}

	protected bool SwapEntitiesImpl (InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2)
	{
		bool code;
		syncDebugPrint("[inv] Man::SwapImpl(" + typename.EnumToString(InventoryMode, mode) + ") item1=" + item1 + " item2=" + item2);
		if (GameInventory.CanSwapEntities(item1, item2))
			code = GetHumanInventory().SwapEntities(mode, item1, item2);
		else
		{
			InventoryLocation item2_dst = new InventoryLocation;
			if (GameInventory.CanForceSwapEntities(item1, item2, item2_dst))
				code = GetHumanInventory().ForceSwapEntities(mode, item1, item2, item2_dst);
		}
		syncDebugPrint("[inv] (Man) SwapEntitiesImpl - cannot swap or forceswap");
		UpdateInventoryMenu();
		return code;
	}
	///@} swap juncture

	///@{ to target inv juncture
	bool JunctureTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		return TakeEntityToTargetInventoryImpl(InventoryMode.JUNCTURE, target, flags, item);
	}

	override bool PredictiveTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToTargetInventory input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), target))
			return JunctureTakeEntityToTargetInventory(target, flags, item);
		else
			return TakeEntityToTargetInventoryImpl(InventoryMode.PREDICTIVE, target, flags, item);
	}

	override bool LocalTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		return TakeEntityToTargetInventoryImpl(InventoryMode.LOCAL, target, flags, item);
	}

	protected bool TakeEntityToTargetInventoryImpl (InventoryMode mode, notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2TargetInv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = target.GetInventory().TakeEntityToInventory(mode, flags, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} to target inv juncture

	///@{ to target cgo ex juncture
	bool JunctureTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return TakeEntityToTargetCargoExImpl(InventoryMode.JUNCTURE, target, item, idx, row, col);
	}

	override bool PredictiveTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToTargetCargoEx input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), target))
			return JunctureTakeEntityToTargetCargoEx(target, item, idx, row, col);
		else
			return TakeEntityToTargetCargoExImpl(InventoryMode.PREDICTIVE, target, item, idx, row, col);
	}

	override bool LocalTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return TakeEntityToTargetCargoExImpl(InventoryMode.LOCAL, target, item, idx, row, col);
	}
	
	override bool ServerTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return TakeEntityToTargetCargoExImpl(InventoryMode.SERVER, target, item, idx, row, col);
	}

	protected bool TakeEntityToTargetCargoExImpl (InventoryMode mode, notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		syncDebugPrint("[inv] Man::Take2TargetCgoEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = target.GetInventory().TakeEntityToTargetCargoEx(mode, target, item, idx, row, col);
		UpdateInventoryMenu();
		return code;
	}
	///@} to target cgo ex juncture

	///@{ to target cgo juncture
	bool JunctureTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetCargoImpl(InventoryMode.JUNCTURE, target, item);
	}

	override bool PredictiveTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToTargetCargo input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), target))
			return JunctureTakeEntityToTargetCargo(target, item);
		else
			return TakeEntityToTargetCargoImpl(InventoryMode.PREDICTIVE, target, item);
	}

	override bool LocalTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetCargoImpl(InventoryMode.LOCAL, target, item);
	}
	
	override bool ServerTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetCargoImpl(InventoryMode.SERVER, target, item);
	}

	protected bool TakeEntityToTargetCargoImpl (InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2TargetCgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = target.GetInventory().TakeEntityToTargetCargo(mode, target, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} to target cgo juncture

	///@{ to target att ex juncture
	bool JunctureTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return TakeEntityToTargetAttachmentExImpl(InventoryMode.JUNCTURE, target, item, slot);
	}

	override bool PredictiveTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToTargetAttachmentEx input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), target))
			return JunctureTakeEntityToTargetAttachmentEx(target, item, slot);
		else
			return TakeEntityToTargetAttachmentExImpl(InventoryMode.PREDICTIVE, target, item, slot);
	}

	override bool LocalTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return TakeEntityToTargetAttachmentExImpl(InventoryMode.LOCAL, target, item, slot);
	}

	override bool ServerTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return TakeEntityToTargetAttachmentExImpl(InventoryMode.SERVER, target, item, slot);
	}

	protected bool TakeEntityToTargetAttachmentExImpl (InventoryMode mode, notnull EntityAI target, notnull EntityAI item, int slot)
	{
		syncDebugPrint("[inv] Man::Take2TargetAtt(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = target.GetInventory().TakeEntityAsAttachmentEx(mode, item, slot);
		UpdateInventoryMenu();
		return code;
	}
	///@} to target att ex juncture

	///@{ to target att juncture
	bool JunctureTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetAttachmentImpl(InventoryMode.JUNCTURE, target, item);
	}

	override bool PredictiveTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeEntityToTargetAttachment input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), target))
			return JunctureTakeEntityToTargetAttachment(target, item);
		else
			return TakeEntityToTargetAttachmentImpl(InventoryMode.PREDICTIVE, target, item);
	}

	override bool LocalTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetAttachmentImpl(InventoryMode.LOCAL, target, item);
	}

	override bool ServerTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return TakeEntityToTargetAttachmentImpl(InventoryMode.SERVER, target, item);
	}

	protected bool TakeEntityToTargetAttachmentImpl (InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
	{
		syncDebugPrint("[inv] Man::Take2TargetAtt(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		bool code = target.GetInventory().TakeEntityAsAttachment(mode, item);
		UpdateInventoryMenu();
		return code;
	}
	///@} to target att juncture

	///@{ to dst juncture
	bool JunctureTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		return TakeToDstImpl(InventoryMode.JUNCTURE, src, dst);
	}

	override bool PredictiveTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		if (!ScriptInputUserData.CanStoreInputUserData())
		{
			Print("[inv] PredictiveTakeToDst input data not sent yet, cannot allow another input action");
			return false;
		}

		if (NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
			return JunctureTakeToDst(src, dst);
		else
			return TakeToDstImpl(InventoryMode.PREDICTIVE, src, dst);
	}

	override bool LocalTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		return TakeToDstImpl(InventoryMode.LOCAL, src, dst);
	}

	protected bool TakeToDstImpl (InventoryMode mode, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		syncDebugPrint("[inv] Man::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + ") src=" + src.DumpToString() + " dst=" + dst.DumpToString());
		bool code = GetHumanInventory().TakeToDst(mode, src, dst);
		UpdateInventoryMenu();
		return code;
	}
	///@} to dst juncture
	///@} inventory

	override bool CanBeSkinned()
	{
		return true;
	}
	
	void UpdateInventoryMenu()
	{
		if( GetGame().IsClient() )
		{
			UIScriptedMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if( menu )
			{
				//menu.Refresh();
			}
		}
	}
	
	///@{ Stats
	//! Registers new stat type for this player.
	/*!
		\param[in] name The name of the stat type.
	*/
	proto native bool StatRegister( string name );
	//! Gets counter value of the specified stat type.
	/*!
		\param[in] name The name of the stat type.
	*/
	proto native float StatGet( string name );
	//! Gets counter value as string of the specified stat type.
	/*!
		\param[in]	name  The name of the stat type.
		\param[out] value A formatted string containing stat value.
	*/
	proto void StatGetCounter( string name, out string value );
	//! Gets counter value as formatted time string of the specified stat type.
	/*!
		\param[in]	name  The name of the stat type.
		\param[out] value A formatted string containing stat value.
	*/
	proto void StatGetAsTime( string name, out string value );
	//! Updates stat counter with given value.
	/*!
		\param[in] name  The name of the stat type.
		\param[in] value The specified value.
	*/
	proto native void StatUpdate( string name, float value );
	//! Updates stat counter by time.
	/*!
		Use this to measure play time.
		\param[in] name The name of the stat type.
	*/
	proto native void StatUpdateByTime( string name );
	//! Updates stat counter by player's actual position.
	/*!
		Use this to measure player's travelling distance.
		\param[in] name The name of the stat type.
	*/
	proto native void StatUpdateByPosition( string name );
	//! Updates stat counter by given position.
	/*!
		\param[in] name The name of the stat type.
		\param[in] pos	The specified position.
	*/
	proto native void StatUpdateByGivenPos( string name, vector pos );
	/*! Invokes read stats from server to client.
		\note Call this on client when you need update stats.
	*/
	proto native void StatInvokeUpdate();
	/*! Invokes save stats from server to client.
		\note Call this on server when you need update stats.
	*/
	proto native void StatSyncToClient();
	///@} Stats
};
