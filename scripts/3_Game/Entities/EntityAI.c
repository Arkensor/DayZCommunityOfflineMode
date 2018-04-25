class EntityAI extends Entity
{
	void EntityAI ()
	{
		// Set up the Energy Manager
		string type = GetType();
		string param_access_energy_sys = "CfgVehicles " + type + " EnergyManager ";
		bool is_electic_device = GetGame().ConfigIsExisting(param_access_energy_sys);

		if (is_electic_device)
		{
			CreateComponent(COMP_TYPE_ENERGY_MANAGER);
			m_EM = ComponentEnergyManager.Cast(  CreateComponent(COMP_TYPE_ENERGY_MANAGER));
			RegisterNetSyncVariableBool("m_EM.m_IsSwichedOn");
			RegisterNetSyncVariableBool("m_EM.m_CanWork");
		}
	}

	void ~EntityAI()
	{
		if ( HasEnergyManager() )
			GetCompEM().OnDeviceDestroyed();
	}

	private ref ComponentsBank m_ComponentsBank;
	ComponentEnergyManager m_EM;

	//! CreateComponent
	Component CreateComponent(int comp_type, string extended_class_name="")
	{
		return GetComponent(comp_type, extended_class_name);
	}

	//! GetComponent
	Component GetComponent(int comp_type, string extended_class_name="")
	{
		if ( m_ComponentsBank == NULL )
			m_ComponentsBank = new ComponentsBank(this);
		
		return m_ComponentsBank.GetComponent(comp_type, extended_class_name);
	}

	//! DeleteComponent
	bool DeleteComponent(int comp_type)
	{
		return m_ComponentsBank.DeleteComponent(comp_type);
	}
	
	//! IsComponentExist
	bool HasComponent(int comp_type)
	{
		if ( m_ComponentsBank )
			return m_ComponentsBank.IsComponentAlreadyExist(comp_type);
		
		return false;
	}

	//! Log
	void Log(string msg, string fnc_name = "n/a")
	{
		Debug.Log(msg, "Object", "n/a", fnc_name, this.GetType());
	}

	//! LogWarning
	void LogWarning(string msg, string fnc_name = "n/a")
	{
		Debug.LogWarning(msg, "Object", "n/a", fnc_name, this.GetType());
	}

	//! LogError
	void LogError(string msg, string fnc_name = "n/a")
	{
		Debug.LogError(msg, "Object", "n/a", fnc_name, this.GetType());
	}

	private EntityEventHandler m_event_handler;

	///@{ Skinning
	bool IsSkinned()
	{
		if (GetCompBS())
			return GetCompBS().IsSkinned();
		
		return false;
	}

	void SetAsSkinned()
	{
		if (GetCompBS())
			GetCompBS().SetAsSkinned();
	}

	bool CanBeSkinnedWith(EntityAI tool)
	{
		if ( !IsSkinned()  &&  tool )
			if ( !IsAlive()  &&  tool.ConfigGetBool("canSkinBodies") )
				return true;

		return false;
	}
	///@} Skinning

	// ADVANCED PLACEMENT EVENTS
	void OnPlacementStarted( Man player ) { }
		
	void OnHologramBeingPlaced( Man player ) { }
	
	void OnPlacementComplete( Man player ) { }

	void OnPlacementCancelled( Man player )
	{
		if ( HasEnergyManager() )
		{
			Man attached_to = Man.Cast( GetHierarchyParent() );
			if (!attached_to  ||  attached_to == player )// Check for exception with attaching a cable reel to an electric fence
				GetCompEM().UnplugThis();
		}
	}
	
	bool CanBePlaced( Man player, vector position )
	{
		return true;
	}

	//! Method which returns message why object can't be placed at given position
	string CanBePlacedFailMessage( Man player, vector position )
	{
		return "";
	}	
	
	//! is this container empty or not, checks both cargo and attachments
	bool IsEmpty()
	{
		return (!HasAnyCargo() && GetInventory().AttachmentCount() == 0);
	}

	//! is this container empty or not, checks only cargo
	bool HasAnyCargo()
	{
		Cargo cargo = GetInventory().GetCargo();
		
		if(!cargo) return false;//this is not a cargo container
		
		if( cargo.GetItemCount() > 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	EntityEventHandler GetEventHandler() { return m_event_handler; }

	void SetEventHandler(EntityEventHandler event_handler)
	{
		m_event_handler = event_handler;
	}
	
	int GetAgents() { return 0; }
	void RemoveAgent(int agent_id);
	void RemoveAllAgents();
	void InsertAgent(int agent, int count);

	override bool IsEntityAI() { return true; }

	/**@brief Delete this object in next frame
	 * @return \p void
	 *
	 * @code
	 *			ItemBase item = GetGame().GetPlayer().CreateInInventory("GrenadeRGD5");
	 *			item.Delete();
	 * @endcode
	**/
	void Delete()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, this);
	}
	
	//! Returns root of current hierarchy (for example: if this entity is in Backpack on gnd, returns Backpack)
	proto native EntityAI GetHierarchyRoot();

	//! Returns root of current hierarchy cast-ed to Man
	proto native Man GetHierarchyRootPlayer();
	
	//! Returns direct parent of current entity
	proto native EntityAI GetHierarchyParent();

	//! Get economy item profile (if assigned, otherwise null)
	proto native CEItemProfile GetEconomyProfile();

	//! Called upon object creation
	void EEInit()
	{
		if (GetInventory())
			GetInventory().EEInit();
	}
	
	//! Called upon object deleting
	void EEDelete(EntityAI parent)
	{
		GetInventory().EEDelete(parent);
	}

	void EEAnimHook(int userType, string param) { }

	void EEAnimDone(string moveName)
	{
		if (m_event_handler) 
		{
			Param param = new Param1<string>(moveName);
			m_event_handler.OnEvent(this, EEAnimDone, param);
		}
	}

	void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner) { }

	void EEAnimStateChanged(string moveName) { }

	void EEFired(int muzzleType, int mode, string ammoType) { }

	void EEAmmoChanged()
	{
		Mission mission;
		Hud hud;
		mission = GetGame().GetMission();
		if ( mission )
		{
			hud = mission.GetHud();
			if(hud) hud.RefreshQuantity( this );
		}
	}

	void EELocal(bool isLocal) { }

	void EEDammaged(string hitpointName, float hit) { }

	void EEKilled(Object killer) { }
	
	void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos) { }
	
	void EEHitByRemote(int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		
	}
	
	// !Called on PARENT when a child is attached to it.
	void EEItemAttached(EntityAI item, string slot_name)
	{
		//Print (slot_name);
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnItemAttached(item, slot_name);
				}
			}
		}
		
		// Energy Manager
		if ( GetGame().IsServer() )
		{
			if ( this.HasEnergyManager()  &&  item.HasEnergyManager() )
			{
				GetCompEM().OnAttachmentAdded(item);
			}
		}				
		
		//SwitchItemSelectionTexture(item, slot_name);

		item.OnWasAttached(this, slot_name);
	}
	
	// !switches materials and/or textures of cloting items on player
	void SwitchItemSelectionTexture(EntityAI item, string slot_name)
	{
	}
	
	// !Called on PARENT when a child is detached from it.
	void EEItemDetached(EntityAI item, string slot_name)
	{
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnItemDetached(item, slot_name);
				}
			}
		}
		
		// Energy Manager
		if ( GetGame().IsServer() )
		{
			if ( this.HasEnergyManager()  &&  item.HasEnergyManager() )
			{
				GetCompEM().OnAttachmentRemoved(item);
			}
		}
		
		//SwitchItemSelectionTexture(item, slot_name);
		
		item.OnWasDetached(this, slot_name);
	}

	void EECargoIn(EntityAI item)
	{
		Mission mission;
		Hud hud;
		mission = GetGame().GetMission();
		if ( mission )
		{
			hud = mission.GetHud();
			if(hud) hud.RefreshItemPosition( item );
		}
	}

	void EECargoOut(EntityAI item)
	{
	}

	void EECargoMove(EntityAI item)
	{
	}
	
	void EEOnWaterEnter()
	{
	}
	
	void EEOnWaterExit()
	{
	}

	void EEUsed(Man owner)
	{
	}

	//! Called on client (in multiplayer) when sqf synchronize variable is synchronized
	void EEVariableSynchronized(string variable_name)
	{
	}

	//! Called when entity is part of "connected system" and being restored after load
	void EEOnAfterLoad()
	{
		// ENERGY MANAGER
		// Restore connections between devices which were connected before server restart
		if ( HasEnergyManager()  &&  GetCompEM().GetRestorePlugState() )
		{
			int low = GetCompEM().GetEnergySourceIDLow();
			int high = GetCompEM().GetEnergySourceIDHigh();
			
			// get pointer to EntityAI based on this ID
			EntityAI potential_energy_source = GetGame().GetEntityByPersitentID(low, high); // This function is available only in this event!
			
			// IMPORTANT!
			// Object IDs acquired here become INVALID when electric devices are transfered to another server while in plugged state (like Flashlight plugged into its attachment 9V battery)
			// To avoid issues, these items must be excluded from this system of restoring plug state so they don't unintentionally plug to incorrect devices through these invalid IDs.
			// Therefore their plug state is being restored withing the EEItemAttached() event while being excluded by the following 'if' conditions...
			
			bool IsAttachment = false;
			
			if (potential_energy_source)
				IsAttachment = GetInventory().HasAttachment(potential_energy_source);
			
			if ( !IsAttachment	&&	potential_energy_source )
				IsAttachment = potential_energy_source.GetInventory().HasAttachment(this);
			
			if ( potential_energy_source  &&  potential_energy_source.HasEnergyManager()  &&  !IsAttachment )
			{
				GetCompEM().PlugThisInto(potential_energy_source); // restore connection
			}
		}
	}
	
	//! Called when entity is being restored from storage folder
	void EEOnStorageLoad()
	{
	}

	//! Called when entity is being created as new by CE/ Debug
	void EEOnCECreate()
	{
	}

	//! Checks if this instance is of type DayZCreature
	bool IsDayZCreature()
	{
		return false;
	}
	
	//! Sets all animation values to 1, making them INVISIBLE if they are configured in models.cfg in such way. These selections must also be defined in the entity's config class in 'AnimationSources'. 
	void HideAllSelections()
	{
		string cfg_path = "cfgVehicles " + GetType() + " AnimationSources";
		
		if ( GetGame().ConfigIsExisting(cfg_path) )
		{
			int	selections = GetGame().ConfigGetChildrenCount(cfg_path);
			
			for (int i = 0; i < selections; i++)
			{
				string selection_name;
				GetGame().ConfigGetChildName(cfg_path, i, selection_name);
				HideSelection(selection_name);
			}
		}
	}
	
	//! Sets all animation values to 0, making them VISIBLE if they are configured in models.cfg in such way. These selections must also be defined in the entity's config class in 'AnimationSources'. 
	void ShowAllSelections()
	{
		string cfg_path = "cfgVehicles " + GetType() + " AnimationSources";
		
		if ( GetGame().ConfigIsExisting(cfg_path) )
		{
			int	selections = GetGame().ConfigGetChildrenCount(cfg_path);
			
			for (int i = 0; i < selections; i++)
			{
				string selection_name;
				GetGame().ConfigGetChildName(cfg_path, i, selection_name);
				ShowSelection(selection_name);
			}
		}
	}
	
	/**
	\brief This event is called when player is marked captive.
		\param captive \p bool true when player is marked captive, false when it is unmarked captive.
	*/
	void EEOnSetCaptive(bool captive) { }

	/**@fn		CanReceiveAttachment
	 * @brief calls this->CanReceiveAttachment(attachment)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, attachment, "CanReceiveAttachment");
	 **/
	bool CanReceiveAttachment (EntityAI attachment)
	{
		return true;
	}

	/**@fn		CanPutAsAttachment
	 * @brief	calls this->CanPutAsAttachment(parent)
	 * @param[in] child	\p	item to be put as attachment of a parent
	 * @return	true if action allowed
	 *
	 * @note: engine code is scriptConditionExecute(this, parent, "CanPutAsAttachment")
	 **/
	bool CanPutAsAttachment (EntityAI parent)
	{
		return true;
	}
	/**@fn		CanReleaseAttachment
	 * @brief calls this->CanReleaseAttachment(attachment)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, attachment, "CanReleaseAttachment");
	 **/
	bool CanReleaseAttachment (EntityAI attachment)
	{
		return true;
	}
	/**@fn		CanDetachAttachment
	 * @brief	calls this->CanDetachAttachment(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "CanDetachAttachment");
	 **/
	bool CanDetachAttachment (EntityAI parent)
	{
		return true;
	}

	/**@fn		CanReceiveItemIntoCargo
	 * @brief	calls this->CanReceiveItemIntoCargo(cargo)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, cargo, "CanReceiveItemIntoCargo");
	 **/
	bool CanReceiveItemIntoCargo (EntityAI cargo)
	{
		return true;
	}
	/**@fn		CanPutInCargo
	 * @brief	calls this->CanPutInCargo(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "CanPutInCargo");
	 **/
	bool CanPutInCargo (EntityAI parent)
	{
		return true;
	}

	/**@fn		CanRelaseCargo
	 * @brief	calls this->CanRelaseCargo(cargo)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, cargo, "CanRelaseCargo");
	 **/
	bool CanRelaseCargo (EntityAI cargo)
	{
		return true;
	}

	/**@fn		CanRemoveFromCargo
	 * @brief	calls this->CanRemoveFromCargo(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "CanRemoveFromCargo");
	 **/
	bool CanRemoveFromCargo (EntityAI parent)
	{
		return true;
	}
	
	/**@fn		CanReceiveItemIntoInventory
	 * @brief calls this->CanReceiveItemIntoInventory(item)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, , "CanReceiveItemIntoInventory");
	 **/
	/*bool CanReceiveItemIntoInventory (EntityAI entity_ai)
	{
		return true;
	}*/

	/**@fn		CanPutInInventory
	 * @brief calls this->CanPutInInventory(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "ConditionIntoInventory");
	 **/
	/*bool CanPutInInventory (EntityAI parent)
	{
		return true;
	}*/

	/**@fn		CanReceiveItemIntoHands
	 * @brief	calls this->CanReceiveItemIntoHands(item_to_hands)
	 * @return	true if action allowed
	 *
	 * @note: scriptConditionExecute(this, item_to_hands, "CanReceiveItemIntoHands");
	 **/
	bool CanReceiveItemIntoHands (EntityAI item_to_hands)
	{
		return true;
	}

	/**@fn		CanPutIntoHands
	 * @brief calls this->CanPutIntoHands(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "CanPutIntoHands");
	 **/
	bool CanPutIntoHands (EntityAI parent)
	{
		return true;
	}

	/**@fn		CanReleaseFromHands
	 * @brief calls this->CanReleaseFromHands(handheld)
	 * @return	true if action allowed
	 *
	 * @note: scriptConditionExecute(this, handheld, "CanReleaseFromHands");
	 **/
	bool CanReleaseFromHands (EntityAI handheld)
	{
		return true;
	}

	/**@fn		CanRemoveFromHands
	 * @brief	calls this->CanRemoveFromHands(parent)
	 * @return	true if action allowed
	 *
	 * @note: return scriptConditionExecute(this, parent, "CanRemoveFromHands");
	 **/
	bool CanRemoveFromHands (EntityAI parent)
	{
		return true;
	}


	// !Called on CHILD when it's attached to parent.
	void OnWasAttached( EntityAI parent, string slot_name ) { }
		
	// !Called on CHILD when it's detached from parent.
	void OnWasDetached( EntityAI parent, string slot_name ) { }
	
	proto native GameInventory GetInventory ();
	proto native void CreateAndInitInventory ();
	proto native void DestroyInventory ();
		
	int GetSlotsCountCorrect()
	{
		return GetInventory().GetSlotsCount();
	}

	EntityAI FindAttachmentBySlotName(string slot_name)
	{
		if ( GetGame() )
		{
			int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
			return GetInventory().FindAttachment(slot_id); 
		}
		return null;
	}

	/**
	\brief Put item anywhere into this entity (as attachment or into cargo, recursively)
	*/
	bool PredictiveTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		return GetInventory().TakeEntityToInventory(InventoryMode.PREDICTIVE, flags, item);
	}
	bool LocalTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		return GetInventory().TakeEntityToInventory(InventoryMode.LOCAL, flags, item);
	}
	bool PredictiveTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityToInventory(InventoryMode.PREDICTIVE, flags, item);
	}
	bool LocalTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityToInventory(InventoryMode.LOCAL, flags, item);
	}
	/**
	\brief Put item into into cargo
	*/
	bool PredictiveTakeEntityToCargo (notnull EntityAI item)
	{
		return GetInventory().TakeEntityToCargo(InventoryMode.PREDICTIVE, item);
	}
	bool LocalTakeEntityToCargo (notnull EntityAI item)
	{
		return GetInventory().TakeEntityToCargo(InventoryMode.LOCAL, item);
	}
	bool PredictiveTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityToCargo(InventoryMode.PREDICTIVE, item);
	}
	bool LocalTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityToCargo(InventoryMode.LOCAL, item);
	}
	bool ServerTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityToCargo(InventoryMode.SERVER, item);
	}
	/**
	\brief Put item into into cargo on specific cargo location
	*/
	bool PredictiveTakeEntityToCargoEx (notnull EntityAI item, int idx, int row, int col)
	{
		return GetInventory().TakeEntityToCargoEx(InventoryMode.PREDICTIVE, item, idx, row, col);
	}
	bool LocalTakeEntityToCargoEx (notnull EntityAI item, int idx, int row, int col)
	{
		return GetInventory().TakeEntityToCargoEx(InventoryMode.LOCAL, item, idx, row, col);
	}
	bool PredictiveTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return GetInventory().TakeEntityToTargetCargoEx(InventoryMode.PREDICTIVE, target, item, idx, row, col);
	}
	bool ServerTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return GetInventory().TakeEntityToTargetCargoEx(InventoryMode.SERVER, target, item, idx, row, col);
	}
	bool LocalTakeEntityToTargetCargoEx (notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		return GetInventory().TakeEntityToTargetCargoEx(InventoryMode.LOCAL, target, item, idx, row, col);
	}
	/**
	\brief Returns if item can be added as attachment on specific slot. Note that slot index IS NOT slot ID! Slot ID is defined in DZ/data/config.cpp
	*/
	bool PredictiveTakeEntityAsAttachmentEx (notnull EntityAI item, int slot)
	{
		return GetInventory().TakeEntityAsAttachmentEx(InventoryMode.PREDICTIVE, item, slot);
	}
	bool LocalTakeEntityAsAttachmentEx (notnull EntityAI item, int slot)
	{
		return GetInventory().TakeEntityAsAttachmentEx(InventoryMode.LOCAL, item, slot);
	}
	bool PredictiveTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return target.GetInventory().TakeEntityAsAttachmentEx(InventoryMode.PREDICTIVE, item, slot);
	}
	bool LocalTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return target.GetInventory().TakeEntityAsAttachmentEx(InventoryMode.LOCAL, item, slot);
	}
	bool ServerTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		return target.GetInventory().TakeEntityAsAttachmentEx(InventoryMode.SERVER, item, slot);
	}
	bool PredictiveTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityAsAttachment(InventoryMode.PREDICTIVE, item);
	}
	bool LocalTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityAsAttachment(InventoryMode.LOCAL, item);
	}
	bool ServerTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		return target.GetInventory().TakeEntityAsAttachment(InventoryMode.SERVER, item);
	}

	bool PredictiveTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		return GetInventory().TakeToDst(InventoryMode.PREDICTIVE, src, dst);
	}
	bool LocalTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		return GetInventory().TakeToDst(InventoryMode.LOCAL, src, dst);
	}

	/**
	\brief Put item into as attachment
	*/
	bool PredictiveTakeEntityAsAttachment (notnull EntityAI item)
	{
		return GetInventory().TakeEntityAsAttachment(InventoryMode.PREDICTIVE, item);
	}
	bool LocalTakeEntityAsAttachment (notnull EntityAI item)
	{
		return GetInventory().TakeEntityAsAttachment(InventoryMode.LOCAL, item);
	}

	bool PredictiveDropEntity (notnull EntityAI item) { return false; }
	bool LocalDropEntity (notnull EntityAI item) { return false; }
	bool ServerDropEntity (notnull EntityAI item) { return false; }

	/**
	\brief Get attached entity by type
	*/
	EntityAI GetAttachmentByType(typename type)
	{
		for ( int i = 0; i < GetInventory().AttachmentCount(); i++ )
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex ( i );
			if ( attachment && attachment.IsInherited ( type ) )
				return attachment;
			}
		return NULL;
	}

	/**
	\brief Get attached entity by config type name
	*/
	EntityAI GetAttachmentByConfigTypeName(string type)
	{
		for ( int i = 0; i < GetInventory().AttachmentCount(); i++ )
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex ( i );
			if ( attachment.IsKindOf ( type ) )
				return attachment;
			}
		return NULL;
	}
	/**
	\brief Returns if item can be dropped out from this entity
	*/
	bool CanDropEntity (notnull EntityAI item) { return true; }

	/**
	 **/
	EntityAI SpawnEntityOnGroundPos (string object_name, vector pos)
	{
		InventoryLocation il = new InventoryLocation;
		vector mat[4];
		Math3D.MatrixIdentity4(mat);
		mat[3] = pos;
		il.SetGround(NULL, mat);
		return SpawnEntity(object_name, il);
	}
	/**
	 **/
	EntityAI SpawnEntityOnGround (string object_name, vector mat[4])
	{
		InventoryLocation il = new InventoryLocation;
		il.SetGround(NULL, mat);
		return SpawnEntity(object_name, il);
	}

	// Returns wetness value. This is just a necesarry forward declaration for Energy Manager which works with EntityAI. This function itself works in ItemBase where its overwritten.
	float GetWet()
	{
		return 0; // Only ItemBase objects have wetness!
	}
	
	//! Returns index of the string found in cfg array 'hiddenSelections'. If it's not found then it returns -1.
	int GetHiddenSelectionIndex( string selection )
	{
		array<string> config_selections	= new array<string>;
		string cfg_hidden_selections = "CfgVehicles" + " " + GetType() + " " + "hiddenSelections";
		GetGame().ConfigGetTextArray ( cfg_hidden_selections, config_selections );
		
		for (int i = 0; i < config_selections.Count(); ++i)
		{
			if ( config_selections.Get ( i ) == selection )
			{
				return i;
			}
		}
		
		return -1;
	}

	/**
	 * @fn		RegisterNetSyncVariableBool
	 * @brief	registers bool variable synchronized over network
	 *
	 * @param[in]	variableName	\p		which variable should be synchronized
	 **/	
	proto native void RegisterNetSyncVariableBool(string variableName);
	
	/**
	 * @fn		RegisterNetSyncVariableInt
	 * @brief	registers int variable synchronized over network
	 *
	 * @param[in]	variableName	\p		which variable should be synchronized
	 * @param[in]	minValue		\p		minimal value used for quantization (when minValue == maxValue, no quatization is done)
	 * @param[in]	maxValue		\p		maximal value used for quantization (when minValue == maxValue, no quatization is done)
	 **/	
	proto native void RegisterNetSyncVariableInt(string variableName, int minValue = 0, int maxValue = 0);
	
	/**
	 * @fn		RegisterNetSyncVariableFloat
	 * @brief	registers float variable synchronized over network
	 *
	 * @param[in]	variableName	\p		which variable should be synchronized
	 * @param[in]	minValue		\p		minimal value used for quantization (when minValue == maxValue, no quatization is done)
	 * @param[in]	maxValue		\p		maximal value used for quantization (when minValue == maxValue, no quatization is done)
	 * @param[in]	precision		\p		precision in number of digits after decimal point
	 **/	
	proto native void RegisterNetSyncVariableFloat(string variableName, float minValue = 0, float maxValue = 0, int precision = 0);

	proto native void SwitchLight(bool isOn);
	
	//! Change texture in hiddenSelections
	proto native void SetObjectTexture(int index, string texture_name);
	//! Change material in hiddenSelections
	proto native void SetObjectMaterial(int index, string mat_name);
		
	proto native bool	IsPilotLight();
	proto native void SetPilotLight(bool isOn);
	
	proto native bool IsWaterContact();
	
	//! Returns the vehicle in which the given unit is mounted. If there is none, NULL is returned.
	proto native Transport GetTransport();
	
	bool IsRestrained()
	{
		return false;		
	}

	/**
	\brief Engine calls this function to collect data from entity to store to game database (on server side).
	@code
	void OnStoreSave(ParamsWriteContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnStoreSave(ctx);

		// write any data (using params) you want to store
		Param4<bool, int, float, string> p1 = new Param4<bool, int, float, string>(true, 56, 6.28, "Pepe");
		Param4<float, string, float, string> p2 = new Param4<float, string, float, string>(9.56, "ahoj", 6.28, "svet");
		p1.Write(ctx);
		p2.Write(ctx);
	}
	@endcode
	*/
	void OnStoreSave(ParamsWriteContext ctx)
	{
		// Saving of energy related states
		if ( HasEnergyManager() )
		{
			// Save energy amount
			ctx.Write( GetCompEM().GetEnergy() );
			
			// Save passive/active state
			ctx.Write( GetCompEM().IsPassive() );
			
			// Save ON/OFF state
			ctx.Write( GetCompEM().IsSwitchedOn() );
			
			// Save plugged/unplugged state
			ctx.Write( GetCompEM().IsPlugged() );
			
			// ENERGY SOURCE
			// Save energy source IDs
			EntityAI energy_source = GetCompEM().GetEnergySource();
			int ID_low = 0;
			int ID_high = 0;
			
			if (energy_source)
			{
				energy_source.GetPersistentID(ID_low, ID_high);
			}
			
			// Save energy source ID low
			ctx.Write( ID_low );
			
			// Save energy source ID high
			ctx.Write( ID_high );
		}
	}
	
	/**
	\brief Called when data is loaded from game database (on server side).
	@code
	void OnStoreLoad(ParamsReadContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnStoreLoad(ctx);

		// read data loaded from game database (format and order of reading must be the same as writing!)
		Param4<bool, int, float, string> p1 = new Param4<bool, int, float, string>(false, 0, 0, "");
		Param4<float, string, float, string> p2 = new Param4<float, string, float, string>(0, "", 0, "");
		if (p1.Read(ctx))
		{
		  Print(p1.param1);
		  Print(p1.param2);
		  Print(p1.param3);
		  Print(p1.param4);
		}
		else
		{
			Print("no data");
		}

		if (p2.Read(ctx))
		{
		  Print(p2.param1);
		  Print(p2.param2);
		  Print(p2.param3);
		  Print(p2.param4);
		}
		else
		{
			Print("no data");
		}
	}
	@endcode
	*/
	void OnStoreLoad (ParamsReadContext ctx)
	{
		// Restoring of energy related states
		if ( HasEnergyManager() )
		{
			// Load energy amount
			float f_energy = 0;
			ctx.Read( f_energy );
			GetCompEM().SetEnergy(f_energy);
			
			// Load passive/active state
			bool b_is_passive = false;
			ctx.Read( b_is_passive );
			GetCompEM().SetPassiveState(b_is_passive);
			
			// Load ON/OFF state
			bool b_is_on = false;
			ctx.Read( b_is_on );
			
			// Load plugged/unplugged state
			bool b_is_plugged = false;
			ctx.Read( b_is_plugged );
			
			// ENERGY SOURCE
			// Load energy source ID low
			int i_energy_source_ID_low = 0; // Even 0 can be valid ID!
			ctx.Read( i_energy_source_ID_low );
			
			// Load energy source ID high
			int i_energy_source_ID_high = 0; // Even 0 can be valid ID!
			ctx.Read( i_energy_source_ID_high );
			
			if ( b_is_plugged )
			{
				// Because function GetEntityByPersitentID() cannot be called here, ID values must be stored and used later.
				GetCompEM().StoreEnergySourceIDs( i_energy_source_ID_low, i_energy_source_ID_high );
				GetCompEM().RestorePlugState(true);
			}
			
			if (b_is_on)
			{
				GetCompEM().SwitchOn();
			}
		}
	}

	//! Sets object synchronization dirty flag, which signalize that object wants to be synchronized (take effect only in MP on server side)
	proto native void SetSynchDirty();

	/**
	\brief Called on clients after receiving synchronization data from server.
	*/
	void OnVariablesSynchronized()
	{
		if ( HasEnergyManager() )
		{
			GetCompEM().DeviceUpdate();
			GetCompEM().StartUpdates();
		}
	}

	proto native void SetAITargetCallbacks(AbstractAITargetCallbacks callbacks);

	override void EOnFrame(IEntity other, float timeSlice)
	{
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnFrame(other, timeSlice);
				}
			}
		}
	}
	
	Shape DebugBBoxDraw()
	{
		return GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxDraw();
	}

	void DebugBBoxSetColor(int color)
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxSetColor(color);
	}

	void DebugBBoxDelete()
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxDelete();
	}

	Shape DebugDirectionDraw(float distance = 1)
	{
		return GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionDraw(distance);
	}

	void DebugDirectionSetColor(int color)
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionSetColor(color);
	}

	void DebugDirectionDelete()
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionDelete();
	}

	//! Hides selection of the given name. Must be configed in config.hpp and models.cfg
	void HideSelection( string selection_name )
	{
		SetAnimationPhase ( selection_name, 1 ); // 1 = hide, 0 = unhide!
	}

	//! Shows selection of the given name. Must be configed in config.hpp and models.cfg
	void ShowSelection( string selection_name )
	{
		SetAnimationPhase ( selection_name, 0 ); // 1 = hide, 0 = unhide!
	}

	//! Returns low and high bits of persistence id of this entity.
	//! This id stays the same even after server restart.
	proto void GetPersistentID( out int low, out int high );

	//! Get remaining economy lifetime (seconds)
	proto native float GetLifetime();
	//! Reset economy lifetime to default (seconds)
	proto native void IncreaseLifetime();

	// BODY STAGING
	//! Use this to access Body Staging component on dead character. Returns NULL if the given object lacks such component.
	ComponentBodyStaging GetCompBS()
	{
		if ( HasComponent(COMP_TYPE_BODY_STAGING) )
			return ComponentBodyStaging.Cast( GetComponent(COMP_TYPE_BODY_STAGING) );
		return NULL;
	}

	///@{ energy manager
	//! ENERGY MANAGER:Documentation: Confluence >> Camping & Squatting >> Electricity >> Energy Manager functionalities
	//! Use this to access Energy Manager component on your device. Returns NULL if the given object lacks such component.
	ComponentEnergyManager GetCompEM()
	{
		if ( HasComponent(COMP_TYPE_ENERGY_MANAGER) )
			return ComponentEnergyManager.Cast( GetComponent(COMP_TYPE_ENERGY_MANAGER) );
		return NULL;
	}

	//! If this item has class EnergyManager in its config then it returns true. Otherwise returns false.
	override bool HasEnergyManager()
	{
		return HasComponent(COMP_TYPE_ENERGY_MANAGER);
	}

	// ------ Public Events for Energy manager component. Overwrite these and put your own functionality into them. ------

	//! Energy manager event: Called only once when this device starts doing its work
	void OnWorkStart() {}

	//! Energy manager event: Called every device update if its supposed to do some work. The update can be every second or at random, depending on its manipulation.
	void OnWork( float consumed_energy ) {}

	//! Energy manager event: Called when the device stops working (was switched OFF or ran out of energy)
	void OnWorkStop() {}

	//! Energy manager event: Called when the device is switched on
	void OnSwitchOn() {}

	//! Energy manager event: Called when the device is switched OFF
	void OnSwitchOff() {}

	//! Energy manager event: Called when this device is plugged into some energy source
	void OnIsPlugged(EntityAI source_device) {}

	//! Energy manager event: Called when this device is UNPLUGGED from the energy source
	void OnIsUnplugged( EntityAI last_energy_source ) {}

	//! Energy manager event: When something is plugged into this device
	void OnOwnSocketTaken( EntityAI device ) {}

	//! Energy manager event: When something is UNPLUGGED from this device
	void OnOwnSocketReleased( EntityAI device ) {}

	//! Energy manager event: Object's initialization. Energy Manager is fully initialized by this point.
	void OnInitEnergy() {}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if ( GetGame().IsClient() )
		{
			switch(rpc_type)
			{
				// ENERGY MANAGER - server => client synchronization of plug connections, switch states and energy values.
				case ERPCs.RPC_EM_PLUG_CHANGED:
				{
					ref Param1<EntityAI> p_plug = new Param1<EntityAI>(NULL);
					if (ctx.Read(p_plug))
					{
						EntityAI energy_source = p_plug.param1;
						
						if (energy_source)
							GetCompEM().PlugThisInto(energy_source);
						else
							GetCompEM().UnplugThis();
					}
					break;
				}

				// BODY STAGING - server => client synchronization of skinned state.
				case ERPCs.RPC_BS_SKINNED_STATE:
				{
					ref Param1<bool> p_skinned_state= new Param1<bool>(false);
					if (ctx.Read(p_skinned_state))
					{
						float state = p_skinned_state.param1;
						if (state && GetCompBS())
							GetCompBS().SetAsSkinnedClient();
					}
					break;
				}
			}
		}
	}
	///@} energy manager
	
	int GetItemWeight()
	{
	}
};
