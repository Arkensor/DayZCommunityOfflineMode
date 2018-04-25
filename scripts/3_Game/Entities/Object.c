class Object extends IEntity
{
	private void ~Object();
	private void Object();
	
	bool CanBeSkinned()
	{
		return false;
	}
	
	proto native void AddProxyPhysics(string proxySelectionName);	
	
	proto native void RemoveProxyPhysics(string proxySelectionName);	

	//! Retrieve all LODS	
	proto native bool GetLODS(notnull out array<LOD> lods);
	
	//! Retrieve LOD name
	proto native string GetLODName(LOD lod);
	
	//! Retrieve LOD by given name
	LOD GetLODByName( string name )
	{
		array<LOD> lods = new array<LOD>;
		GetLODS( lods );
		
		for ( int i = 0; i < lods.Count(); ++i )
		{
			string lod_name = GetLODName( lods.Get( i ) );
			lod_name.ToLower();
			name.ToLower();
			if ( lod_name == name ) 
			{
				return lods.Get( i );
			}
		}
		
		return NULL;
	}
	
	//! returns action component name by given component index
	proto native string GetActionComponentName(int componentIndex);

	//! returns action component name list by given component index
	proto native void GetActionComponentNameList(int componentIndex, TStringArray nameList);
	
	//! return true if selection containts action component
	proto native bool IsActionComponentPartOfSelection(int componentIndex, string selectionName);
	
	//! Flag to determine this object is marked to be deleted soon
	proto native bool ToDelete();
	
	//! Retrieve position
	proto native vector GetPosition();

	//! Returns world position.
	//! This function takes proxy transformation into account.
	proto native vector GetWorldPosition();
	
	//! Set position
	proto native void SetPosition(vector vPos);
	
	/**
	\brief Place object on surface
	*/
	proto native void PlaceOnSurface();

	/**
	\brief Retrieve orientation (yaw, pitch, roll) in <b>degrees</b>
	\return vector of yaw, pitch and roll angles (x = yaw, y = pitch, z = roll)
	*/
	proto native vector GetOrientation();

	/**
	\brief Set orientation (yaw, pitch, roll) in <b>degrees</b>
	@param vOrientation, vector of yaw, pitch and roll angles (x = yaw, y = pitch, z = roll)
	*/
	proto native void SetOrientation(vector vOrientation);
	
	//! Retrieve direction vector
	proto native vector GetDirection();
	
	/**
	\brief Set direction of object
	@param vDirection, direction vector
	\note object up vector is always y-axis
	*/
	proto native void SetDirection(vector vPos);
	
	//! Calculate local position of other entity to this entity
	proto native vector GetLocalPos(vector vPos);
	//! Calculate global position to this entity from local position
	proto native vector GetGlobalPos(vector vPos);
	
	//! Retrieve object's speed in global space
	proto native vector GetSpeed();
	
	//! Retrieve object's speed in local space
	proto native vector GetModelSpeed();
	
	//! Returns true if object's geometry can affect pathgraph generation
	proto native bool CanAffectPathgraph();
	
	//! Sets if object's geometry affects pathgraph generation. If first parameter is true, it ignores second parameter and sets affecting pathgraph by parameter set in object's config
	proto native void SetAffectPathgraph(bool fromConfig, bool affect);
	

	/**
	\brief Get Clipping info
	@param minMax array[2] of vectors, into minMax[0] is returned min, into minMax[1] is returned max
	\return radius of bounding box
	@code
	vector minMax[2];
	float radius = GetGame().GetPlayer().ClippingInfo(minMax);
	Print(minMax);
	Print(radius);
	@endcode
	*/
	proto float ClippingInfo(out vector minMax[2]);
	
	/** 
	\brief Gets collision bounding box
	@param minMax array[2] of vectors, into minMax[0] is returned min, into minMax[1] is returned max
	\return true if collision box exists, false otherwise
	@code
	vector minMax[2];
	if(GetGame().GetPlayer().GetCollisionBox(minMax))
		Print("has box");
	@endcode
	*/
	proto native bool GetCollisionBox(out vector minMax[2]);
	
	//! Get total damage (clipped to range 0..1)
	proto native float GetDamage();

	
	proto native void GetSelectionList(out TStringArray selectionList);
	
	//! Returns noise multiplier of surface under object
	float GetSurfaceNoise()
	{
		vector position = GetPosition();
		return GetGame().SurfaceGetNoiseMultiplier(position[0], position[2]);
	}
	
	//! Returns type of surface under object
	string GetSurfaceType()
	{
		string surface_type;
		GetGame().SurfaceGetTypeUnderObject(this, surface_type);
		return surface_type;
	}

	//! Returns true if the given selection was found in the p3d file. False if otherwise.
	bool HasSelection( string selection )
	{
		TStringArray selections = new TStringArray;
		GetSelectionList( selections );
		
		for ( local int i = 0; i < selections.Count(); ++i )
		{
			if ( selections.Get( i ) == selection )
			{
				return true;
			}
		}
		
		return false;
	}
	
	//! Searches object's config for the given animation name. Returns true if it was found, or false if not.
	bool HasAnimation( string anim_name )
	{
		string cfg_path_vehicles = "CfgVehicles " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_vehicles)  &&  GetGame().ConfigIsExisting (cfg_path_vehicles + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		string cfg_path_weapons = "CfgWeapons " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_weapons)  &&  GetGame().ConfigIsExisting (cfg_path_weapons + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		string cfg_path_magazines = "CfgMagazines " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_magazines)  &&  GetGame().ConfigIsExisting (cfg_path_magazines + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		return false;
	}
	
	//! If Returns true if this item has EnergyManager in its config. Otherwise returns false.
	bool HasEnergyManager()
	{
		return false;
	}
	
	proto native int GetMemoryPointIndex(string memoryPointName);
	proto native vector GetMemoryPointPos(string memoryPointName);

	//! Get config class of object
	string GetType()
	{
		string ret;
		GetGame().ObjectGetType(this, ret);
	
		return ret;
	}
	
	//! Get display name of entity
	string GetDisplayName()
//	string GetName()
	{
		string tmp;
		GetGame().ObjectGetDisplayName(this, tmp);
		return tmp;
	}
	
	//! Returns name of the model of this object without the '.p3d' suffix
	string GetModelName()
	{
		return GetGame().GetModelName(GetType());
	}


	int Release()
	{
		return GetGame().ObjectRelease(this);
	}
	
	//! Check config class name of the object
	bool IsKindOf(string type)
	{
		return GetGame().ObjectIsKindOf(this, type);
	}
		
	// Check alive state
	bool IsAlive()
	{
		if ( IsDamageDestroyed() ) 
		{
			return false; // Is dead
		}
		
		return true; // Is alive
	}

		
	//! Returns if this entity is Man
	bool IsMan()
	{
		return false;
	}
	
	//! Returns if this entity is EntityAI
	bool IsEntityAI()
	{
		return false;
	}
	
	//! Returns if this entity is Static Transmitter
	bool IsStaticTransmitter()
	{
		return false;
	}
	
	//! Returns if this entity is a non-static Transmitter
	bool IsTransmitter()
	{
		return false;
	}
	
	//! Returns if this entity is ItemBase
	bool IsItemBase()
	{
		return false;
	}
	
	bool IsContainer()
	{
		return false;
	}
	
	//! Returns if this entity is Magazine
	bool IsMagazine()
	{
		return false;
	}
	
	//! Returns if this entity is InventoryItem
	bool IsInventoryItem()
	{
		return false;
	}

	//! Returns if this entity is Clothing
	bool IsClothing()
	{
		return false;
	}

	//! Returns if this entity is fireplacebase
	bool IsFireplace()
	{	
		return false;
	}
	
	//! Returns if this entity is tree
	bool IsTree()
	{	
		return false;
	}
	
	//! Returns if this entity is rock
	bool IsRock()
	{	
		return false;
	}
	
	//! Returns whether object is PlanBase
	bool IsWoodBase()
	{
		return false;
	}

	//! Returns if this entity is bush
	bool IsBush()
	{
		return false;
	}
	
	//! Returns if this entity is Pelt_Base
	bool IsPeltBase()
	{
		return false;
	}

	//! Returns if this entity is Weapon
	bool IsWeapon()
	{
		return false;
	}
	
	//! returns if this entity is Melee Weapon
	bool IsMeleeWeapon()
	{
		return false;
	}
	
	//! Returns if this entity is Building
	bool IsBuilding()
	{
		return false;
	}
	
	//! Returns if this entity is Well (extends Building)
	bool IsWell()
	{
		return false;
	}
	
	//! Returns if this entity is transport
	bool IsTransport()
	{
		return false;
	}
	
	//! Returns if this entity can be constucted,deconstructed (e.g. fence,watchtower)
	bool CanUseConstruction()
	{
		return false;
	}

	//! Returns if the health of this entity should be displayed (widgets, inventory)
	bool IsHealthVisible()
	{
		return true;
	}

	//! Returns low and high bits of networkID.
	//! This id is shared between client and server for whole server-client session.
	proto void GetNetworkID( out int lowBits, out int highBits );
	
	//! Remote procedure call shortcut, see CGame.RPC / CGame.RPCSingleParam
	void RPC(int rpc_type, array<ref Param> params, bool guaranteed, PlayerIdentity recipient = NULL)
	{
		GetGame().RPC(this, rpc_type, params, guaranteed, recipient);
	}
	
	//! Remote procedure call shortcut, see CGame.RPCSingleParam / CGame.RPC
	void RPCSingleParam(int rpc_type, Param param, bool guaranteed, PlayerIdentity recipient = NULL)
	{
		GetGame().RPCSingleParam(this, rpc_type, param, guaranteed, recipient);
	}
	
	/**
  \brief Called after remote procedure call is recieved for this object
		@param rpc_type user defined identification of RPC
		@param ctx read context for params
	*/
	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx);

	vector GetSelectionPosition(string name)
	{
		return GetGame().ObjectGetSelectionPosition(this, name);
	}
	
	vector ModelToWorld(vector modelPos)
	{
		return GetGame().ObjectModelToWorld(this, modelPos);
	}
	
	vector WorldToModel(vector worldPos)
	{
		return GetGame().ObjectWorldToModel(this, worldPos);
	}

	// config class API	
	
	proto string ConfigGetString(string entryName);
	proto int ConfigGetInt(string entryName);
	bool ConfigGetBool(string entryName)
	{
		if ( ConfigGetInt(entryName) == 1 )
		{
			return true;
		}
		
		return false;
	}

	proto float ConfigGetFloat(string entryName);
	proto vector ConfigGetVector(string entryName);
	//proto ParamEntry ConfigGetEntry(string entryName);

	/**
  \brief Get array of strings from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetTextArray(string entryName, out TStringArray values);

	/**
  \brief Get array of floats from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetFloatArray(string entryName, out TFloatArray values);
	
	/**
  \brief Get array of integers from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetIntArray(string entryName, out TIntArray values);

	/**
  \brief Checks if given entry exists.
	@param entryName 
	*/
	proto native bool		ConfigIsExisting(string entryName);
	
	//! Compares config class name to given string
	bool KindOf( string tag )
	{
		bool found = false;
		string item_name = this.GetType();	
		TStringArray item_tag_array = new TStringArray;
		ConfigGetTextArray("cfgVehicles " + item_name + " itemInfo", item_tag_array);	
		
		int array_size = item_tag_array.Count();
		for (int i = 0; i < array_size; i++)
		{
			if ( item_tag_array.Get(i) == tag )
			{
				found = true;
				break;
			}
		}
		return found;
	}
	
	bool IsAnyInherited( array<typename> typenames )
	{
		bool ret;
		for( int i = 0; i < typenames.Count(); i++ )
		{
			ret = ret || this.IsInherited( typenames.Get(i) );
		}
		return ret;
	}
	
	// Damage system
	/**
  \brief Checks if object is destroyed.
	*/
	proto native bool   IsDamageDestroyed();
	
	/**
	\brief Returns current state of health.
	@param zoneName if empty string, returns state of global health
	@param healthType if empty string, returns state of main health
	*/
	proto native float  GetHealth(string zoneName, string healthType);
	
	/**
	\brief Returns current state of health in range <0, 1>.
	@param zoneName if empty string, returns state of global health
	@param healthType if empty string, returns state of main health
	*/
	proto native float  GetHealth01(string zoneName, string healthType);
	
	/**
  \brief Returns maximum value of health.
	@param zoneName if empty string, returns state of global health
	@param healthType if empty string, returns state of main health
	*/
	proto native float  GetMaxHealth(string zoneName, string healthType);
	
	/**
  \brief Sets current state of health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   SetHealth(string zoneName, string healthType, float value);
	
	/**
  \brief Adds health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   AddHealth(string zoneName, string healthType, float value);

	/**
  \brief Decreases health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   DecreaseHealth(string zoneName, string healthType, float value);
	
	/**
  \brief Decreases health with the option of auto-deleting the object if its ruined.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	void   DecreaseHealth(string zoneName, string healthType, float value, bool auto_delete)
	{
		DecreaseHealth( zoneName, healthType, value);
		
		float result_health = GetHealth(zoneName, healthType);
		
		if ( auto_delete  &&  result_health <= 0 )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, this); 
		}
	}
	
	//! Equivalent of GetHealth("", "");
	float GetHealth()
	{
		return GetHealth("", "");
	}
	//! Equivalent of GetHealth01("", "");
	float GetHealth01()
	{
		return GetHealth01("", "");
	}
	//! Equivalent of GetMaxHealth("", "");
	float GetMaxHealth()
	{
		return GetMaxHealth("", "");
	}
	//! Equivalent of SetHealth("", "", float value);
	void SetHealth(float health)
	{
		SetHealth("", "", health);
	}
	//! Equivalent of AddHealth("", "", float value);
	void AddHealth( float add_health)
	{
		AddHealth("", "", add_health);
	}
	//! Equivalent of DecreaseHealth("", "", float value, bool auto_delete);
	void DecreaseHealth(float dec_health, bool auto_delete = true)
	{
		DecreaseHealth("", "", dec_health, auto_delete);
	}
	//! Decreases health by the given coeficient.
	void DecreaseHealthCoef(float dec_health_coef, bool auto_delete = true)
	{
		float current_health = GetHealth();
		float final_health_coef = GetHealth01() - dec_health_coef;
		float final_health = GetMaxHealth() * final_health_coef;		
		DecreaseHealth("", "", current_health - final_health);
	}

	/**
  \brief Applies damage on object.
	@param damageType DT_CLOSE_COMBAT/DT_FIRE_ARM/DT_EXPLOSION/DT_CUSTOM
	@param source source of damage
	@param compomentName which component was hit
	@param ammoName ammoType, which defines how much damage should be applied
	@param directHitModelPos local position of hit
	@param damageCoef multiplier of applied damage
	*/
	proto native void   ProcessDirectDamage(int damageType, EntityAI source, string componentName, string ammoName, vector modelPos, float damageCoef = 1.0);
	
	
	proto native string GetDamageZoneNameByComponentIndex(int componentIndex);
	
	/**
  \brief Is this just static object without config? So we can avoid calling unnecessary methods
	*/
	proto native bool IsPlainObject();
	
	/**
  \brief Returns damage level (range 0..4, 0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined).
	*/
	proto native int GetHealthLabel();
	
	/**
  \brief Enable or disable object to receive damage
	@param enable or disable
	*/
	proto native void SetAllowDamage(bool val);
	
	/**
	\brief Returns whether the object is ruined (0 Health).
	\return \p bool true = item has 0 Health or less
	@code
		if ( item.IsRuined() )
		{
			Print("Object is ruined!");
		}
	@endcode
	*/
	bool IsRuined()
	{
		if ( IsDamageDestroyed() )
		{
			return true;
		}
		
		return false;
	}
	
	void GetSingleUseActions(out TIntArray actions)
	{	
	}
	
	void GetContinuousActions(out TIntArray actions)
	{
	}
	
	void GetInteractActions(out TIntArray actions)
	{
	}
	
	//! Plays the given sound once on this object's instance. Range is in meters. Created sound is only local, unless create_local is set to false. Returns the sound itself.
	SoundOnVehicle PlaySound(string sound_name, float range, bool create_local = true)
	{
		return GetGame().CreateSoundOnObject(this, sound_name, range, false, create_local);
	}
	
	//! Plays the given sound in loop on this object's instance. Range is in meters. Created sound is only local, unless create_local is set to false. Returns the sound itself.
	SoundOnVehicle PlaySoundLoop(string sound_name, float range, bool create_local = true)
	{
		return GetGame().CreateSoundOnObject(this, sound_name, range, true, create_local);
	}
	
	void PostAreaDamageActions() {}
	void PreAreaDamageActions() {}
};