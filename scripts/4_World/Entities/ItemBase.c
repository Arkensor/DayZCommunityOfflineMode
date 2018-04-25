typedef ItemBase Inventory_Base;

class ItemBase extends InventoryItem
{
	static int		m_DebugActionsMask;
	
	// ============================================
	// Variable Manipulation System
	// ============================================
	//ref map<string,string> 	m_ItemVarsString;
	
	int		m_VariablesMask;//this holds information about which vars have been changed from their default values
	float 	m_VarQuantity;
	float 	m_VarTemperature;
	float 	m_VarWet;
	int 	m_VarLiquidType;
	int		m_Item_Stage;
	bool	m_IsHologram;
	// items color variables
	int 	m_ColorComponentR;
	int 	m_ColorComponentG;
	int 	m_ColorComponentB;
	int 	m_ColorComponentA;
	//-------------------------------------------------------
	//open / close actions
	bool m_Opened = true;
	
	ref TIntArray m_SingleUseActions;
	ref TIntArray m_ContinuousActions;
	ref TIntArray m_InteractActions;
	
	//==============================================
	// agent system
	private int	m_AttachedAgents;

	// Declarations
	void TransferModifiers( PlayerBase reciever );
	
	// Weapon muzzle flash particle effects
	ref static map<int, ref array<ref WeaponParticlesOnFire>> m_MuzzleFlashOnFire;
	ref static map<int, ref array<ref WeaponParticlesOnOverheating>> m_OverheatingEffect;
	ref static map<string, int> m_WeaponTypeToID;
	static int m_LastRegisteredWeaponID = 0;
	
	// -------------------------------------------------------------------------
	void ItemBase()
	{
		SetEventMask(EntityEvent.INIT); // Enable EOnInit event
		InitItemVariables();

		m_SingleUseActions = new TIntArray;
		m_ContinuousActions = new TIntArray;
		m_InteractActions = new TIntArray;
		SetUserActions();
		
		if (HasMuzzle())
		{
			LoadParticleConfigOnFire( GetMuzzleID() );
			LoadParticleConfigOnOverheating( GetMuzzleID() );
		}
	}
	
	void InitItemVariables()
	{
		m_VarTemperature = GetTemperatureInit();
		m_VarWet = GetWetInit();
		m_VarLiquidType = GetLiquidTypeInit();
		m_VarQuantity = GetQuantityInit();//should be by the CE, this is just a precaution
		m_IsHologram = false;
		
		//RegisterNetSyncVariableInt("m_VariablesMask");
		if ( HasQuantity() ) RegisterNetSyncVariableFloat("m_VarQuantity", GetQuantityMin(), GetQuantityMax() );
		RegisterNetSyncVariableFloat("m_VarTemperature", GetTemperatureMin(),GetTemperatureMax() );
		RegisterNetSyncVariableFloat("m_VarWet", GetWetMin(), GetWetMax() );
		RegisterNetSyncVariableInt("m_VarLiquidType");
		
		RegisterNetSyncVariableInt("m_ColorComponentR", 0, 255);
		RegisterNetSyncVariableInt("m_ColorComponentG", 0, 255);
		RegisterNetSyncVariableInt("m_ColorComponentB", 0, 255);
		RegisterNetSyncVariableInt("m_ColorComponentA", 0, 255);
		
		RegisterNetSyncVariableBool("m_IsHologram");
	
	}
	
	// Loads muzzle flash particle configuration from config and saves it to a map for faster access
	void LoadParticleConfigOnFire(int id)
	{
		if (!m_MuzzleFlashOnFire)
			m_MuzzleFlashOnFire = new map<int, ref array<ref WeaponParticlesOnFire>>;
		
		if ( !m_MuzzleFlashOnFire.Contains(id) )
		{
			string config_to_search = "CfgVehicles";
			
			if (IsInherited(Weapon))
				config_to_search = "CfgWeapons";	
			
			string muzzle_owner_config = config_to_search + " " + GetType() + " ";
			string config_OnFire_class = muzzle_owner_config + "Particles " + "OnFire ";
			
			int config_OnFire_subclass_count = GetGame().ConfigGetChildrenCount(config_OnFire_class);
			ref array<ref WeaponParticlesOnFire> WPOF_array = new array<ref WeaponParticlesOnFire>;
			
			for (int i = 0; i < config_OnFire_subclass_count; i++)
			{
				string particle_class = "";
				GetGame().ConfigGetChildName(config_OnFire_class, i, particle_class);
				string config_OnFire_entry = config_OnFire_class + particle_class;
				WeaponParticlesOnFire WPOF = new WeaponParticlesOnFire(this, config_OnFire_entry);
				WPOF_array.Insert(WPOF);
			}
			
			
			m_MuzzleFlashOnFire.Insert(id, WPOF_array);
		}
	}
	
	// Loads muzzle flash particle configuration from config and saves it to a map for faster access
	void LoadParticleConfigOnOverheating(int id)
	{
		if (!m_OverheatingEffect)
			m_OverheatingEffect = new map<int, ref array<ref WeaponParticlesOnOverheating>>;
		
		if ( !m_OverheatingEffect.Contains(id) )
		{
			string config_to_search = "CfgVehicles";
			
			if (IsInherited(Weapon))
				config_to_search = "CfgWeapons";	
			
			string muzzle_owner_config = config_to_search + " " + GetType() + " ";
			string config_OnOverheating_class = muzzle_owner_config + "Particles " + "OnOverheating ";
			
			int config_OnOverheating_subclass_count = GetGame().ConfigGetChildrenCount(config_OnOverheating_class);
			ref array<ref WeaponParticlesOnOverheating> WPOOH_array = new array<ref WeaponParticlesOnOverheating>;
			
			for (int i = 0; i < config_OnOverheating_subclass_count; i++)
			{
				string particle_class = "";
				GetGame().ConfigGetChildName(config_OnOverheating_class, i, particle_class);
				string config_OnOverheating_entry = config_OnOverheating_class + particle_class;
				WeaponParticlesOnOverheating WPOF = new WeaponParticlesOnOverheating(this, config_OnOverheating_entry);
				WPOOH_array.Insert(WPOF);
			}
			
			
			m_OverheatingEffect.Insert(id, WPOOH_array);
		}
	}
	
	//! Returns true if this item has a muzzle (weapons, suppressors)
	bool HasMuzzle()
	{
		if (IsInherited(Weapon)  ||  IsInherited(SuppressorBase))
			return true;
		
		return false;
	}
	
	//! Returns global muzzle ID. If not found, then it gets automatically registered.
	int GetMuzzleID()
	{
		if (!m_WeaponTypeToID)
			m_WeaponTypeToID = new map<string, int>;
		
		if ( m_WeaponTypeToID.Contains( GetType() ) )
		{
			return m_WeaponTypeToID.Get( GetType() );
		}
		else 
		{
			// Register new weapon ID
			m_WeaponTypeToID.Insert(GetType(), ++m_LastRegisteredWeaponID);
		}
		
		return m_LastRegisteredWeaponID;
	}
	
	// -------------------------------------------------------------------------
	void ~ItemBase()
	{
	}
	
	void SetUserActions()
	{
		string path;
		int stage;
		
		if (IsItemStaged())
		{
			GetItemStage(stage);
			path = "cfgVehicles " + GetType() + " Staging Stage_" + stage;
		}
		else
		{
			path = "cfgVehicles " + GetType();
		}
		g_Game.ConfigGetIntArray(path + " ContinuousActions", m_ContinuousActions);
		g_Game.ConfigGetIntArray(path + " SingleUseActions", m_SingleUseActions);
		g_Game.ConfigGetIntArray(path + " InteractActions", m_InteractActions);
	}
	
	override void GetSingleUseActions(out TIntArray actions)
	{	
		if ( m_SingleUseActions )
		{			
			for ( int i = 0; i < m_SingleUseActions.Count(); i++ )
			{
				actions.Insert(m_SingleUseActions.Get(i));
			}
		}
	}
	
	override void GetContinuousActions(out TIntArray actions)
	{
		if ( m_ContinuousActions )
		{
			for ( int i = 0; i < m_ContinuousActions.Count(); i++ )
			{
				actions.Insert(m_ContinuousActions.Get(i));
			}
		}
	}
	
	override void GetInteractActions(out TIntArray actions)
	{
		if ( m_InteractActions )
		{			
			for ( int i = 0; i < m_InteractActions.Count(); i++ )
			{
				actions.Insert(m_InteractActions.Get(i));
			}
		}
	}
	
	// -------------------------------------------------------------------------
	static int GetDebugActionsMask()
	{
		return ItemBase.m_DebugActionsMask;	
	}
	
	static void SetDebugActionsMask(int mask)
	{
		ItemBase.m_DebugActionsMask = mask;
	}
	
	void SetCEBasedQuantity()
	{
		if( GetEconomyProfile() )
		{
			float q_min = GetEconomyProfile().GetQuantityMin();
			float q_max = GetEconomyProfile().GetQuantityMax();
			if( q_max > 0 )
			{
				float quantity_randomized = Math.RandomFloatInclusive(q_min, q_max);
				//PrintString("<==> Normalized quantity for item: "+ GetType()+", qmin:"+q_min.ToString()+"; qmax:"+q_max.ToString()+";quantity:" +quantity_randomized.ToString());
					SetQuantityNormalized(quantity_randomized, false);
				}
		}
	}
	
	void CombineItemsClient(ItemBase entity2, bool use_stack_max = false )
	{
		/*
		ref Param1<EntityAI> item = new Param1<EntityAI>(entity2);
		RPCSingleParam( ERPCs.RPC_ITEM_COMBINE, item, GetGame().GetPlayer() );
		*/
		
		if( GetGame().IsClient() )
		{
			if (ScriptInputUserData.CanStoreInputUserData())
			{
				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write(INPUT_UDT_ITEM_MANIPULATION);
				ctx.Write(-1);
				ItemBase i1 = this; // @NOTE: workaround for correct serialization
				ctx.Write(i1);
				ctx.Write(entity2);
				ctx.Write(use_stack_max);
				ctx.Write(-1);
				ctx.Send();
			}
		}
		else if( !GetGame().IsMultiplayer() )
		{
			CombineItems(entity2, use_stack_max);
		}
	}
	
	bool IsLiquidPresent()
	{
		
		if(GetLiquidType() != 0 && HasQuantity() ) return true;
		else return false;
	}
	
	bool IsLiquidContainer()
	{
		if( ConfigGetFloat("liquidContainerType")!=0 ) return true;
		else return false;
	}
	
	bool IsBloodContainer()
	{
		return false;
	}
	
	bool IsHologram()
	{
		return m_IsHologram;
	}
	
	void SetIsHologram( bool is_hologram )
	{
		m_IsHologram = is_hologram;
	}

	float GetNutritionalEnergy()
	{
		if( !IsLiquidPresent() )
		{
			Edible_Base edible = Edible_Base.Cast( this );
			return edible.GetFoodEnergy();
		}
		else
		{
			return Liquid.GetEnergy( GetLiquidType() );
		}
	}
	
	float GetNutritionalWaterContent()
	{
		if( !IsLiquidPresent() )
		{
			Edible_Base edible = Edible_Base.Cast( this );
			return edible.GetFoodWater();
		}
		else
		{
			return Liquid.GetWaterContent( GetLiquidType() );
		}
	}
	
	float GetNutritionalIndex()
	{
		if( !IsLiquidPresent() )
		{
			Edible_Base edible = Edible_Base.Cast( this );
			return edible.GetFoodNutritionalIndex();
		}
		else
		{
			return Liquid.GetNutritionalIndex( GetLiquidType() );
		}
	}
	
	float GetNutritionalFullnessIndex()
	{
		if( !IsLiquidPresent() )
		{
			Edible_Base edible = Edible_Base.Cast( this );
			return edible.GetFoodTotalVolume();
		}
		else
		{
			return Liquid.GetVolume( GetLiquidType() );
		}
	}
	
	float GetNutritionalToxicity()
	{
		if( !IsLiquidPresent() )
		{
			Edible_Base edible = Edible_Base.Cast( this );
			return edible.GetFoodToxicity();
		}
		else
		{
			return Liquid.GetToxicity(GetLiquidType());
		}
	}

	NutritionalProfile GetNutritionalProfile()
	{
		return new NutritionalProfile(GetNutritionalEnergy(),GetNutritionalWaterContent(),GetNutritionalIndex(),GetNutritionalFullnessIndex(), GetNutritionalToxicity());
		
	}
	
		
	//TODO move and improve
	// -------------------------------------------------------------------------
	override void EECargoIn(EntityAI item)
	{
		super.EECargoIn(item);

		if ( item.IsKindOf("BandanaMask_ColorBase") || item.IsKindOf("BandanaUntieable_ColorBase") )
		{
			Man owner = item.GetHierarchyRootPlayer();
			PlayerBase player;
			Class.CastTo(player, owner);
		
			if (player && player.ItemToInventory)
			{
				player.ItemToInventory = false;
				player.SwitchBandana(item);
			}
		}
	}
	// -------------------------------------------------------------------------
	override void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		
		Man owner_player_old = NULL;
		Man owner_player_new = NULL;
	
		if (old_owner)   
		{      
			if (old_owner.IsMan())
			{         
				owner_player_old = Man.Cast( old_owner );
			}      
			else
			{
				owner_player_old = Man.Cast( old_owner.GetHierarchyRootPlayer() );
			}   
		}   
		if (new_owner)
		{     
			if ( new_owner.IsMan() )
			{
				owner_player_new = Man.Cast( new_owner );
			}      
			else      
			{
				owner_player_new = Man.Cast( new_owner.GetHierarchyRootPlayer() );
			}   
		}   
		if ( owner_player_old != owner_player_new )   
		{
			if ( owner_player_old )
				OnInventoryExit(owner_player_old);
				// Debug.Log("Item dropped from inventory");

			if ( owner_player_new )
				OnInventoryEnter(owner_player_new);
				// Debug.Log("Item taken to inventory");
		}
	//will not work, item location not yet specified
	/*	if ( owner_player_new && owner_player_old && (owner_player_new == owner_player_old) )
		{
			PlayerBase playerOPN;
			Class.CastTo(playerOPN, owner_player_new);
			if (playerOPN && playerOPN.ItemToInventory)
			{
				playerOPN.ItemToInventory = false;
				playerOPN.SwitchBandana(this);
			}
		}*/
	}

	// -------------------------------------------------------------------------------
	override void EOnInit(IEntity other, int extra)
	{
		
	}
	// -------------------------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
		if(player)
		{
			player.RemoveQuickBarEntityShortcut(this);
			OnInventoryExit(player);
		}
	}
	// -------------------------------------------------------------------------------
	override void EEKilled( Object killer)
	{
		super.EEKilled( killer );
		
		//if item is able to explode in fire
		if ( CanExplodeInFire() )
		{
			float min_temp_to_explode	= 100;		//min temperature for item to explode
			
			if ( GetTemperature() >= min_temp_to_explode )		//TODO ? add check for parent -> fireplace
			{
				if ( IsMagazine() )
				{
					Magazine magazine = Magazine.Cast( this );					
					
					//explode ammo
					if ( magazine.GetAmmoCount() > 0 )
					{
						ExplodeAmmo();
					}
				}
				else
				{
					//explode item
					Explode();
				}
			}
		}
	}
	override void OnWasAttached( EntityAI parent, string slot_name )
	{
		//PlayerBase player;
		//if(PlayerBase.CastTo(player, GetHierarchyRootPlayer()))
		//{
		//	player.UpdateQuickBarEntityVisibility(this);
		//}
	}
	
	void ExplodeAmmo()
	{
		//timer
		ref Timer explode_timer = new Timer( CALL_CATEGORY_SYSTEM );
		
		//min/max time
		float min_time = 1;
		float max_time = 3;
		float delay = Math.RandomFloat( min_time, max_time );
		
		explode_timer.Run( delay, this, "DoAmmoExplosion" );
	}
	
	void DoAmmoExplosion()
	{
		Magazine magazine = Magazine.Cast( this );
		int pop_sounds_count = 6;
		string pop_sounds[ 6 ] = { "ammopops_1","ammopops_2","ammopops_3","ammopops_4","ammopops_5","ammopops_6" };
		
		//play sound
		int sound_idx = Math.RandomInt( 0, pop_sounds_count - 1 );
		string sound_name = pop_sounds[ sound_idx ];
		GetGame().CreateSoundOnObject( this, sound_name, 20, false );
		
		//remove ammo count
		magazine.ServerAddAmmoCount( -1 );
		
		//if condition then repeat -> ExplodeAmmo
		float min_temp_to_explode	= 100;		//min temperature for item to explode
				
		if ( magazine.GetAmmoCount() > 0 && GetTemperature() >= min_temp_to_explode )	//TODO ? add check for parent -> fireplace
		{
			ExplodeAmmo();
		}
	}

	// -------------------------------------------------------------------------------
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		if ( IsClothing() || IsContainer() )
		{
			DamageItemsInCargo(damageResult);
		}
	}

	void DamageItemsInCargo(TotalDamageResult damageResult)
	{	
		if ( GetInventory().GetCargo() )
		{
			int item_count = GetInventory().GetCargo().GetItemCount();
			if ( item_count > 0 )
			{
				int random_pick = Math.RandomInt(0, item_count);
				ItemBase item = ItemBase.Cast( GetInventory().GetCargo().GetItem(random_pick) );
				float dmg = damageResult.GetDamage("","Health") / -2;
				item.AddHealth("","",dmg);
			}
		}
	}

	bool CanBeSplit()
	{
		if( ConfigGetBool("canBeSplit") )
		{
			if( GetQuantity() > 1 )
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		
		return false;
	}
	
	void SplitIntoStackMaxClient( EntityAI destination_entity, int slot_id  )
	{
		if( GetGame().IsClient() )
		{
			if (ScriptInputUserData.CanStoreInputUserData())
			{
				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write(INPUT_UDT_ITEM_MANIPULATION);
				ctx.Write(1);
				ItemBase i1 = this; // @NOTE: workaround for correct serialization
				ctx.Write(i1);
				ctx.Write(destination_entity);
				ctx.Write(true);
				ctx.Write(slot_id);
				ctx.Send();
			}
		}
		else if( !GetGame().IsMultiplayer() )
		{
			SplitIntoStackMax( destination_entity, slot_id );
		}
	}

	void SplitIntoStackMax( EntityAI destination_entity, int slot_id )
	{
		int stack_max = InventorySlots.GetStackMaxForSlotId( slot_id );
		float quantity = GetQuantity();
		float split_quantity_new = stack_max;
		
		if ( stack_max == 0 || stack_max >= quantity || !CanBeSplit() )
		{
			return;
		}
		
		InventoryLocation source_location = new InventoryLocation;
		InventoryLocation destination_location = new InventoryLocation;
		ItemBase new_item;
		new_item = ItemBase.Cast( destination_entity.GetInventory().CreateAttachmentEx( this.GetType(), slot_id ) );
		
		if( new_item )
		{			
			MiscGameplayFunctions.TransferItemProperties(this,new_item);
			AddQuantity(-split_quantity_new);
			new_item.SetQuantity( split_quantity_new );
		}
	}

	void SplitItem(PlayerBase player)
	{
		if ( !CanBeSplit() )
		{
			return;
		}
		
		float quantity = GetQuantity();
		float split_quantity_new = Math.Floor( quantity / 2 );
		
		ItemBase new_item;
		new_item = player.CopyInventoryItem( this );

		if( !new_item )//when we are not working within the player's inventory OR there is no space in player inventory, spawn new item on ground
		{
			//new_item = GetGame().CreateObject(this.GetType(), this.GetPosition() + "1 0 0" );
			new_item = ItemBase.Cast( SpawnEntityOnGroundPos( this.GetType(), player.GetPosition() ) );
			new_item.PlaceOnSurface();
		}

		if( new_item )
		{
			MiscGameplayFunctions.TransferItemProperties(this,new_item);
			AddQuantity(-split_quantity_new);
			new_item.SetQuantity( split_quantity_new );
		}
	}
	
	override void OnRightClick()
	{
		super.OnRightClick();
		
		if( CanBeSplit() && !GetDayZGame().IsLeftCtrlDown() )
		{
			if( GetGame().IsClient() )
			{
				if (ScriptInputUserData.CanStoreInputUserData())
				{
					ScriptInputUserData ctx = new ScriptInputUserData;
					ctx.Write(INPUT_UDT_ITEM_MANIPULATION);
					ctx.Write(1);
					ItemBase i1 = this; // @NOTE: workaround for correct serialization
					ctx.Write(i1);
					ItemBase i2 = null; // @NOTE: workaround for correct serialization
					ctx.Write(i2);
					ctx.Write(false);
					ctx.Write(-1);
					ctx.Send();
				}
			}
			else if( !GetGame().IsMultiplayer() )
			{
				SplitItem( PlayerBase.Cast( GetGame().GetPlayer() ) );
			}
		}
	}
	
	bool CanBeCombined(ItemBase other_item, PlayerBase player = NULL)
	{
		bool can_this_be_combined =  ConfigGetBool("canBeSplit");
		
		if(!can_this_be_combined || !other_item) 
		{
			return false;
		}
		
		if( IsFullQuantity() )
		{
			return false;			
		}
		
		if( player && (player.GetHumanInventory().GetEntityInHands() == this || player.GetHumanInventory().GetEntityInHands() == other_item )) return false;
		
		return ( this.GetType() == other_item.GetType() );
	}
	
	void CombineItems( ItemBase other_item, bool use_stack_max = false )
	{
		if(!CanBeCombined(other_item)) return;
		if( !IsMagazine() )
		{
			float other_item_quantity = other_item.GetQuantity();
			float this_free_space;
			
			int max_quantity;
			if( use_stack_max )
			{
				int stack_max = InventorySlots.GetStackMaxForSlotId( GetInventory().GetSlotId() );
				max_quantity = stack_max;
			}
			else
			{
				max_quantity = GetQuantityMax();
			}
			this_free_space = max_quantity - GetQuantity();
			
			float quantity_used = 0;
			if( other_item_quantity > this_free_space )
			{
				quantity_used = this_free_space;
			}
			else
			{
				quantity_used = other_item_quantity;
			}
			if( quantity_used!= 0 )
			{
				this.AddQuantity(quantity_used);
				other_item.AddQuantity(-quantity_used);
			}
		}		
	}
	// -------------------------------------------------------------------------
	// Mirek: whole user action system moved to script
	// -------------------------------------------------------------------------	
	void GetRecipesActions(Man player, out TSelectableActionInfoArray outputList)
	{
		PlayerBase p = PlayerBase.Cast( player );
			
		array<int> recipes_ids = p.m_Recipes;
		PluginRecipesManager module_recipes_manager = PluginRecipesManager.Cast( GetPlugin(PluginRecipesManager) );
		if( module_recipes_manager )
		{
			EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
			module_recipes_manager.GetValidRecipes(ItemBase.Cast( this ),ItemBase.Cast( item_in_hands ),recipes_ids, PlayerBase.Cast( player ) );
		}
		for(int i = 0;i < recipes_ids.Count();i++)
		{
			int key = recipes_ids.Get(i);
			string recipe_name = module_recipes_manager.GetRecipeName(key);
			outputList.Insert(new TSelectableActionInfo(SAT_CRAFTING, key, recipe_name));
		}
	}
	
	// -------------------------------------------------------------------------	
	void GetDebugActions(out TSelectableActionInfoArray outputList)
	{
		//weight
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.GET_TOTAL_WEIGHT, "Print Weight"));
		
		//quantity
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.REMOVE_QUANTITY, "Quantity -20%"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.ADD_QUANTITY, "Quantity +20%"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.SET_QUANTITY_0, "Set Quantity 0"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.SET_MAX_QUANTITY, "Set Quantity Max"));
		
		//health
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.ADD_HEALTH, "Health +20"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.REMOVE_HEALTH, "Health -20"));

		//temperature
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.ADD_TEMPERATURE, "Temperature +20"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.REMOVE_TEMPERATURE, "Temperature -20"));
		
		//wet
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.ADD_WETNESS, "Wetness +20"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.REMOVE_WETNESS, "Wetness -20"));

		//liquidtype
		if( IsLiquidContainer() )
		{
			outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.LIQUIDTYPE_UP, "LiquidType Next"));
			outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.LIQUIDTYPE_DOWN, "LiquidType Previous"));
		}
		
		//strings
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.INJECT_STRING_TIGER, "Inject String Tiger"));
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.INJECT_STRING_RABBIT, "Inject String Rabbit"));
		
		// watch
		outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.WATCH_ITEM, "Watch"));

		// print bullets
		//outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION, EActions.PRINT_BULLETS, "Print Bullets"));
		
		//ctx.AddAction("ShowID",USE_ENUM_HERE,NULL,2000,false,false);
		//ctx.AddAction("Predend Consume",USE_ENUM_HERE,NULL, 1011,false,false);
		//ctx.AddAction("IsEmpty",USE_ENUM_HERE,NULL, 1012,false,false);
		//ctx.AddAction("HasAnyCargo",USE_ENUM_HERE,NULL, 1013,false,false);
		//ctx.AddAction("LightOn",USE_ENUM_HERE,NULL, 1013,false,false);
		//ctx.AddAction("Set Health 200", USE_ENUM_HERE, NULL, 1003, false, false);
		//ctx.AddAction("PrintQuantityMax", USE_ENUM_HERE, NULL, 1000, false, false);
		//ctx.AddAction("Print Classname", USE_ENUM_HERE, NULL, 1000, false, false);
		//ctx.AddAction("Has In Cargo", USE_ENUM_HERE, NULL, 1000, false, false);
		//ctx.AddAction("Print Health(new)", USE_ENUM_HERE, NULL, 1000, false, false);
		//ctx.AddAction("Quantity -1", USE_ENUM_HERE, NULL, 1004, false, false);
		//ctx.AddAction("IsMagazine", USE_ENUM_HERE, NULL, 1005, false, false);
		//ctx.AddAction("Ammo +1", USE_ENUM_HERE, NULL, 1006, false, false);
		//ctx.AddAction("Set Health 1", USE_ENUM_HERE, NULL, 1002, false, false);
		//ctx.AddAction("Ammo -1", USE_ENUM_HERE, NULL, 1007, false, false);
		//ctx.AddAction("Ammo SetMax", USE_ENUM_HERE, NULL, 1008, false, false);
		//ctx.AddAction("LightOff",USE_ENUM_HERE,NULL, 1022,false,false);
	}
	
	// -------------------------------------------------------------------------	
	// -------------------------------------------------------------------------		
	// -------------------------------------------------------------------------	
	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
		if(action_id >= EActions.RECIPES_RANGE_START && action_id < EActions.RECIPES_RANGE_END)
		{
			PluginRecipesManager plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin(PluginRecipesManager) );
			int idWithoutOffset = action_id - EActions.RECIPES_RANGE_START;
			PlayerBase p = PlayerBase.Cast( player );
			if( EActions.RECIPES_RANGE_START  < 1000 )
			{
				float anim_length = plugin_recipes_manager.GetRecipeLengthInSecs(idWithoutOffset);
				float specialty_weight = plugin_recipes_manager.GetRecipeSpecialty(idWithoutOffset);
				//p.SetUpCrafting( idWithoutOffset, this, player.GetHierarchyRootPlayer().GetHumanInventory().GetEntityInHands(),anim_length, specialty_weight);
			}
			else//this part is for the [DEBUG] actions
			{
				//plugin_recipes_manager.PerformRecipeClientRequest( idWithoutOffset, this, player.GetHierarchyRootPlayer().GetHumanInventory().GetEntityInHands() );
			}
		}
		
		if( GetGame().IsServer() )
		{
			if(action_id >= EActions.DEBUG_AGENTS_RANGE_INJECT_START && action_id < EActions.DEBUG_AGENTS_RANGE_INJECT_END)
			{
				int agent_id = action_id - EActions.DEBUG_AGENTS_RANGE_INJECT_START;
				InsertAgent(agent_id,100);
			}
	
			if(action_id >= EActions.DEBUG_AGENTS_RANGE_REMOVE_START && action_id < EActions.DEBUG_AGENTS_RANGE_REMOVE_END)
			{
				int agent_id2 = action_id - EActions.DEBUG_AGENTS_RANGE_REMOVE_START;
				RemoveAgent(agent_id2);
			}
			
			if( action_id == EActions.SET_MAX_QUANTITY ) //SetMaxQuantity
			{
				SetQuantityMax();
			}
			
			
			if( action_id == EActions.REMOVE_QUANTITY ) //Quantity -20%
			{
				AddQuantity(GetQuantityMax()/-5);
				//PrintVariables();
			}
			
			if( action_id == EActions.GET_TOTAL_WEIGHT ) //Prints total weight of item + its contents
			{
				Print(GetItemWeight());
			}
	
			/*
			if( action_id == EActions.INJECT_STRING_TIGER ) 
			{
				SetItemVariableString("varNote", "The tiger (Panthera tigris) is the largest cat species, most recognisable for their pattern of dark vertical stripes on reddish-orange fur with a lighter underside. The largest tigers have reached a total body length of up to 3.38 m (11.1 ft) over curves and have weighed up to 388.7 kg (857 lb) in the wild. The species is classified in the genus Panthera with the lion, leopard, jaguar and snow leopard. Tigers are apex predators, primarily preying on ungulates such as deer and bovids. They are territorial and generally solitary but social animals, often requiring large contiguous areas of habitat that support their prey requirements. This, coupled with the fact that they are indigenous to some of the more densely populated places on Earth, has caused significant conflicts with humans.");
			}
			if( action_id == EActions.INJECT_STRING_RABBIT ) 
			{
				SetItemVariableString("varNote", "Rabbits are small mammals in the family Leporidae of the order Lagomorpha, found in several parts of the world. There are eight different genera in the family classified as rabbits, including the European rabbit (Oryctolagus cuniculus), cottontail rabbits (genus Sylvilagus; 13 species), and the Amami rabbit (Pentalagus furnessi, an endangered species on Amami ?shima, Japan). There are many other species of rabbit, and these, along with pikas and hares, make up the order Lagomorpha. The male is called a buck and the female is a doe; a young rabbit is a kitten or kit.");
			}
			*/
	
			if( action_id == EActions.ADD_HEALTH ) 
			{
				this.AddHealth("","",20);
			}
			if( action_id == EActions.REMOVE_HEALTH ) 
			{
				this.AddHealth("","",-20);
			}
			
			if( action_id == EActions.SET_QUANTITY_0 ) //SetMaxQuantity
			{
				SetQuantity(0);
			}
			
			if( action_id == EActions.WATCH_ITEM ) //SetMaxQuantity
			{
				PluginItemDiagnostic mid = PluginItemDiagnostic.Cast( GetPlugin(PluginItemDiagnostic) );
				mid.RegisterDebugItem( ItemBase.Cast( this ), PlayerBase.Cast( player ));
			}
			
			if( action_id == EActions.ADD_QUANTITY )
			{
				AddQuantity(GetQuantityMax()/5);
				//PrintVariables();
			}
	
			if( action_id == EActions.ADD_TEMPERATURE )
			{
				AddTemperature(1);
				//PrintVariables();
			}
			
			if( action_id == EActions.REMOVE_TEMPERATURE )
			{
				AddTemperature(-1);
				//PrintVariables();
			}
			
			if( action_id == EActions.ADD_WETNESS )
			{
				AddWet(GetWetMax()/5);
				//PrintVariables();
			}
			
			if( action_id == EActions.REMOVE_WETNESS )
			{
				AddWet(-GetWetMax()/5);
				//PrintVariables();
			}
	
			if( action_id == EActions.LIQUIDTYPE_UP )
			{
				int curr_type = GetLiquidType();
				SetLiquidType(curr_type * 2);
				//AddWet(1);
				//PrintVariables();
			}
			
			if( action_id == EActions.LIQUIDTYPE_DOWN )
			{
				int curr_type2 = GetLiquidType();
				SetLiquidType(curr_type2 / 2);
			}

			if( action_id == EActions.PRINT_BULLETS )
			{
				if( IsMagazine() )
				{
					Magazine this_mag;
					Class.CastTo(this_mag, this);
					for(int i = 0; i < this_mag.GetAmmoCount(); i++)
					{
						float damage;
						string class_name;
						this_mag.GetCartridgeAtIndex(i, damage, class_name);
						PrintString("Bullet: " + class_name +", " + "Damage: "+ damage.ToString() );
					}					
				}
			}
		}
		/*
		if( action_id == USE_ENUM_HERE )
		{
			int id_1;
			int id_2;
			GetPersistentID(id_1,id_2);
			Debug.Log("id1: "+id_1.ToString());
			Debug.Log("id2: "+id_2.ToString());
		}
		

		if( action_id == USE_ENUM_HERE )//PrintQuantityMax
		{
			Debug.Log("max quantity for item " + this.ToString() + " is: " + GetQuantityMax().ToString(),"ItemVars");
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			Debug.Log("classname:"+this.GetType(),"recipes");
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			if( player.HasEntityInCargo(this) )
			{
				Debug.Log("+this item is in player's cargo space","recipes");
			}
			else
			{
				Debug.Log("-this item is NOT player's cargo space","recipes");
			}
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			Debug.Log(this.GetHealth("","").ToString());
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			this.AddQuantity(-1,false,true);
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			Debug.Log("IsMagazine:"+this.IsMagazine().ToString(),"recipes");
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			if( IsMagazine() )
			{
				Magazine mag = this;
				mag.SetAmmoCount(mag.GetAmmoCount() + 1);
			}
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			this.SetHealth("","",1);
		}
		
		if( action_id == USE_ENUM_HERE ) 
		{
			if( IsMagazine() )
			{
				Magazine mgz = this;
				mgz.AddAmmoCount(-1);
			}
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			if( IsMagazine() )
			{
				Magazine mgz2 = this;
				mgz2.SetAmmoCount(mgz2.GetAmmoMax());
			}
		}

		if( action_id == USE_ENUM_HERE ) 
		{
			PluginTransmissionAgents m_mta = GetPlugin(PluginTransmissionAgents);
			m_mta.TransmitAgents(this, player, AGT_UACTION_CONSUME);
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			Debug.Log(this.IsEmpty().ToString() );
		}
		if( action_id == USE_ENUM_HERE ) 
		{
			Debug.Log( "Has any cargo = "+this.HasAnyCargo().ToString() );
		}

		if( action_id == USE_ENUM_HERE ) 
		{	
			if (HasEnergyManager())
			{
				GetCompEM().SwitchOn();
			}
		}
		
		if( action_id == USE_ENUM_HERE ) 
		{
			this.SetHealth("","",200);
		}
		
		if( action_id == USE_ENUM_HERE ) 
		{
			if (HasEnergyManager())
			{
				GetCompEM().SwitchOff();
			}
		}
		
		if( action_id == USE_ENUM_HERE ) //Ineract
		{
			PlayerBase nplayer = player;
			nplayer.OnInteractMenu();
		}
		*/
		
		return false;
	}
	// -------------------------------------------------------------------------
	/**
	\brief Returns \p bool by whether that item can be put together with other item
		\param other_item \p ItemBase for check to combine
		\return \p bool Can combined with \p other_item
		@code
			item_stone.CanQuantityCombine(item_rags);		// returns false
			item_stone.CanQuantityCombine(item_stone);		// returns true
		@endcode
	*/
	/*
	bool CanQuantityCombine( ItemBase other_item )
	{
		if ( other_item == NULL || m_QuantityCanCombine == false )
		{
			return false;
		}
		
		string a = this.GetType();
		string b = other_item.GetType();

		if ( a != b )
		{
			return false;
		}
		
		return true;
	}*/

	// -------------------------------------------------------------------------
	/**
	\brief Quantity of this item will be moved to \p other_item, but only to maximum quantity of \p other_item
		\return \p bool from CanQuantityCombine()
		@code
			item_shovel.CanQuantitySplit();					// returns false, becouse shovel has quantity 1
			item_stone.CanQuantityCombine(item_stone);		// returns true, becouse stone has quantity 5
		@endcode
	*/
	/*
	bool QuantityCombine( ItemBase other_item )
	{
		if ( other_item == NULL )
		{
			return false;
		}
		
		if ( !CanQuantityCombine(other_item) )
		{
			MessageToOwnerAction("You can not combine these two items.");
			
			return false;
		}
		
		float can_move_quantity = other_item.GetQuantityMax() - other_item.GetQuantity();
		
		if ( can_move_quantity <= 0 )
		{
			MessageToOwnerAction("You can not combine, stack is maximum.");
			
			return false;
		}
		
		float keep_quantity = this.GetQuantity() - can_move_quantity;
		
		if ( keep_quantity > 0 ) 
		{
			other_item.AddQuantity(can_move_quantity, true);
		}
		else
		{
			other_item.AddQuantity(this.GetQuantity(), true);
		}
		
		SetQuantity( keep_quantity, true );
		
		return true;
	}
*/
	// -------------------------------------------------------------------------
	/**
	\brief Returns \p bool by whether that item can be split to two items by quantity. If quantity is 1 or 0, item can not be split.
		\return \p bool ItemBase can be split to two items
		@code
			item_shovel.CanQuantitySplit();		// returns false, becouse shovel has quantity 1
			item_stone.CanQuantitySplit();		// returns true, becouse stone has quantity 5
		@endcode
	*/
/*
	bool CanQuantitySplit()
	{
		if ( GetQuantityMax() <= 1 || GetQuantity() <= 1 || m_QuantityCanSplit == false )
		{
			return false;
		}
		
		return true;
	}
*/
	// -------------------------------------------------------------------------
	/**
	\brief Half of quantity from this item will be moved to new item object. New item object will be copy of this. Extends damamge.
		\return \p ItemBase Instance of this object with half quantity
		@code
			ItemBase item_stone_new = item_stone.QuantitySplit();	// If this item has 2 or more quantity, will be splitted		
		@endcode
	*/
	/*
	ItemBase QuantitySplit()
	{
		if ( !CanQuantitySplit() )
		{
			return NULL;
		}
		
		float split_quantity = Math.Floor( GetQuantity() / 2 );
		
		AddQuantity(-split_quantity);
		PlayerBase player = GetGame().GetPlayer();
		ItemBase new_item = player.CopyInventoryItem( this );
		new_item.SetQuantity( split_quantity, true );
		
		return new_item;
	}
*/
	// -------------------------------------------------------------------------
	/**
	\brief Creates a explosion by ammoType in config of object. If object dont have this parameter ("ammoType" like grenade) explsotion is default "G_GrenadeHand"
		\return \p void
		@code
			ItemBase item = GetGame().GetPlayer().CreateInInventory("GrenadeRGD5");
			
			item.Explode();
		@endcode
	*/
	void Explode()
	{
		string explosion_name = this.ConfigGetString("ammoType");
		
		if ( explosion_name == "" )
		{
			explosion_name = "Dummy_Heavy";
		}
		
		if ( GetGame().IsServer() )
		{
			DamageSystem.ExplosionDamage(this, NULL, explosion_name, GetPosition());
		}
		
		Particle.Play(ParticleList.EXPLOSION_TEST, GetPosition());
		GetGame().ObjectDelete(this);
	}
	
	//! Called when this item is activated from a trip wire that was stepped on.
	void OnActivatedByTripWire()
	{
		// Override this method for Trip Wire functionality
	}

	//----------------------------------------------------------------
	//returns true if item is able to explode when put in fire
	bool CanExplodeInFire()
	{
		return false;
	}
	
	//----------------------------------------------------------------
	bool CanEat()
	{
		return true;
	}
	
	//----------------------------------------------------------------
	//has FoodStages in config?
	bool CanBeCooked()
	{
		string config_path = "CfgVehicles" + " " + GetType() + " " + "Food" + " " + "FoodStages";
		return GetGame().ConfigIsExisting ( config_path );
	}
	
	//----------------------------------------------------------------
	bool CanRepair(ItemBase item_repair_kit)
	{
		PluginRepairing module_repairing = PluginRepairing.Cast( GetPlugin(PluginRepairing) );
		return module_repairing.CanRepair(this, item_repair_kit);
	}

	//----------------------------------------------------------------
	bool Repair(PlayerBase player, ItemBase item_repair_kit, float specialty_weight)
	{
		PluginRepairing module_repairing = PluginRepairing.Cast( GetPlugin(PluginRepairing) );
		return module_repairing.Repair(player, this, item_repair_kit, specialty_weight);
	}

	//----------------------------------------------------------------
	int GetItemSize()
	{
		/*
		vector v_size = this.ConfigGetVector("itemSize");
		int v_size_x = v_size[0];
		int v_size_y = v_size[1];
		int size = v_size_x * v_size_y;
		return size;
		*/
		
		return 1;
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in grey color
		\return \p void
		@code
			item_stone.MessageToOwnerStatus("Some Status Message");
		@endcode
	*/
	void MessageToOwnerStatus( string text )
	{
		PlayerBase player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
		
		if ( player )
		{
			player.MessageStatus( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in yellow color
		\return \p void
		@code
			item_stone.MessageToOwnerAction("Some Action Message");
		@endcode
	*/
	void MessageToOwnerAction( string text )
	{
		PlayerBase player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
		
		if ( player )
		{
			player.MessageAction( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in green color
		\return \p void
		@code
			item_stone.MessageToOwnerFriendly("Some Friendly Message");
		@endcode
	*/
	void MessageToOwnerFriendly( string text )
	{
		PlayerBase player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
		
		if ( player )
		{
			player.MessageFriendly( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in red color
		\return \p void
		@code
			item_stone.MessageToOwnerImportant("Some Important Message");
		@endcode
	*/
	void MessageToOwnerImportant( string text )
	{
		PlayerBase player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
		
		if ( player )
		{
			player.MessageImportant( text );
		}
	}

	override bool IsItemBase()
	{
		return true;
	}

	// Checks if item is of questioned kind
	override bool KindOf( string tag )
	{
		bool found = false;
		string item_name = this.GetType();
		ref TStringArray item_tag_array = new TStringArray;
		GetGame().ConfigGetTextArray("cfgVehicles " + item_name + " itemInfo", item_tag_array);	
		
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

	override void OnRPC(PlayerIdentity sender, int rpc_type,ParamsReadContext  ctx) 
	{
		//Debug.Log("OnRPC called");
		super.OnRPC(sender, rpc_type,ctx);
		//if( rpc_type == ERPCs.RPC_ITEM_SPLIT ) SplitItem();
		/*
		if( rpc_type == ERPCs.RPC_ITEM_COMBINE ) 
		{

			ref Param1<EntityAI> item = new Param1<EntityAI>(NULL);

			ctx.Read(item);
			ItemBase item_other = item.param1;
			
			CombineItems( item_other );
		}
		*/
		if( GetGame().IsDebug() )
		{
			if( rpc_type == ERPCs.RPC_ITEM_DIAG )
			{	
				PluginItemDiagnostic mid = PluginItemDiagnostic.Cast( GetPlugin(PluginItemDiagnostic) );
				mid.OnRPC(this,ctx);
			}
		}
	}

	//-----------------------------
	// VARIABLE MANIPULATION SYSTEM
	//-----------------------------

	void TransferVariablesFloat(array<float> float_vars)
	{
		DeSerializeNumericalVars(float_vars);
	}
		
	array<float> GetVariablesFloat()
	{
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		SerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
		return CashedObjectsArrays.ARRAY_FLOAT;
	}

	int NameToID(string name)
	{
		PluginVariables plugin = PluginVariables.Cast( GetPlugin(PluginVariables) );
		return plugin.GetID(name);
	}

	string IDToName(int id)
	{
		PluginVariables plugin = PluginVariables.Cast( GetPlugin(PluginVariables) );
		return plugin.GetName(id);
	}

	void OnSyncVariables(ParamsReadContext ctx)//with ID optimization
	{
		//Debug.Log("OnSyncVariables called for item:  "+ ToString(this.GetType()),"varSync");
		//read the flags
		//ref Param1<int> pflags = new Param1<int>(0);
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		
		int varFlags = CashedObjectsParams.PARAM1_INT.param1;
		//--------------
		
		
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			ReadVarsFromCTX(ctx);
		}
/*
		if( varFlags & ItemVariableFlags.STRING )
		{
			OnSyncStrings(ctx);
		}
		*/
		// ------- quantity refresh --------
		Mission mission;
		Hud hud;
		mission = GetGame().GetMission();
		if ( mission )
		{
			hud = mission.GetHud();
			if(hud) hud.RefreshQuantity( this );
		}
		//----------------------------------
		
	}	
		
	void SerializeNumericalVars(array<float> floats_out)
	{
		// the order of serialization must be the same as the order of de-serialization
		floats_out.Insert(m_VariablesMask);
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_QUANTITY) )
		{
			floats_out.Insert(m_VarQuantity);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_TEMPERATURE) )
		{
			floats_out.Insert(m_VarTemperature);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_WET) )
		{
			floats_out.Insert(m_VarWet);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_LIQUIDTYPE) )
		{
			floats_out.Insert(m_VarLiquidType);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_COLOR) )
		{
			floats_out.Insert(m_ColorComponentR);
			floats_out.Insert(m_ColorComponentG);
			floats_out.Insert(m_ColorComponentB);
			floats_out.Insert(m_ColorComponentA);
		}
		//--------------------------------------------
	}
	
	void DeSerializeNumericalVars(array<float> floats)
	{
		// the order of serialization must be the same as the order of de-serialization
		int index = 0;
		int mask = Math.Round(floats.Get(index));
		
		index++;
		//--------------------------------------------
		if( mask & VARIABLE_QUANTITY )
		{
			float quantity = floats.Get(index);
			SetQuantity(quantity, true, false, true );
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_TEMPERATURE )
		{
			float temperature = floats.Get(index);
			SetTemperature(temperature, true);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_WET )
		{
			float wet = floats.Get(index);
			SetWet(wet, true);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_LIQUIDTYPE )
		{
			int liquidtype = Math.Round(floats.Get(index));
			SetLiquidType(liquidtype, true);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_COLOR )
		{
			m_ColorComponentR = Math.Round(floats.Get(index));
			index++;
			m_ColorComponentG = Math.Round(floats.Get(index));
			index++;
			m_ColorComponentB = Math.Round(floats.Get(index));
			index++;
			m_ColorComponentA = Math.Round(floats.Get(index));
			index++;
		}
	}

	
	void WriteVarsToCTX(ParamsWriteContext ctx)
	{
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		SerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
		
		int array_size = CashedObjectsArrays.ARRAY_FLOAT.Count();
		
		CashedObjectsParams.PARAM1_INT.param1 = array_size;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		
		for(int i = 0; i < array_size; i++)
		{
			CashedObjectsParams.PARAM1_FLOAT.param1 = CashedObjectsArrays.ARRAY_FLOAT.Get(i);
			ctx.Write(CashedObjectsParams.PARAM1_FLOAT);
		}
	}
	
	void ReadVarsFromCTX(ParamsReadContext ctx)//with ID optimization
	{
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int numOfItems = CashedObjectsParams.PARAM1_INT.param1;
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		
		for(int i = 0; i < numOfItems; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_FLOAT);
			float value = CashedObjectsParams.PARAM1_FLOAT.param1;
			
			CashedObjectsArrays.ARRAY_FLOAT.Insert(value);
		}
		DeSerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
	}
	
	void SaveVariables(ParamsWriteContext ctx)
	{
		//Debug.Log("Saving Item Stage 0 "+ClassName(this)+" " + ToString(this));
		
		//first set the flags

		int varFlags = 0;
		
		if( m_VariablesMask )
		{
			varFlags = varFlags | ItemVariableFlags.FLOAT;
		}

		//ref Param1<int> pflags = new Param1<int>( varFlags );
		CashedObjectsParams.PARAM1_INT.param1 = varFlags;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		//-------------------
			
		//now serialize the variables
		
		//floats
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			WriteVarsToCTX(ctx);
		}

	}

		
	//----------------------------------------------------------------
	void LoadVariables(ParamsReadContext ctx)
	{
		//read the flags
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		
		int varFlags = CashedObjectsParams.PARAM1_INT.param1;
		//--------------
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			ReadVarsFromCTX(ctx);
		}
	}


	//----------------------------------------------------------------
	override void OnStoreLoad(ParamsReadContext ctx)
	{   
		super.OnStoreLoad(ctx);
		PlayerBase player;
		if( Class.CastTo(player, GetHierarchyRootPlayer()) )
		{
			//Load quickbar item bind
			int itemQBIndex;
			ctx.Read(itemQBIndex);
			if( itemQBIndex != -1 )
			{
				player.SetLoadedQuickBarItemBind(this,itemQBIndex);
			}
		}
		LoadVariables(ctx);// variable management system
		LoadAgents(ctx);//agent trasmission system
	}

	//----------------------------------------------------------------

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		PlayerBase player;
		if(PlayerBase.CastTo(player,GetHierarchyRootPlayer()))
		{
			//Save quickbar item bind
			int itemQBIndex = -1;
			itemQBIndex = player.FindQuickBarEntityIndex(this);
			ctx.Write(itemQBIndex);			
		}
		SaveVariables(ctx);// variable management system
		SaveAgents(ctx);//agent trasmission system
	}
	//----------------------------------------------------------------

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		RefreshHud();
	}
	
	void RefreshHud()
	{
		// ------- quantity refresh --------
		Mission mission;
		Hud hud;
		mission = GetGame().GetMission();
		if ( mission )
		{
			hud = mission.GetHud();
			if(hud) hud.RefreshQuantity( this );
		}
		//----------------------------------
	}


	//-------------------------	Quantity
	//----------------------------------------------------------------
	//! Set item quantity[related to varQuantity... config entry], destroy_config = true > if the quantity reaches varQuantityMin or lower and the item config contains the varQuantityDestroyOnMin = true entry, the item gets destroyed. destroy_forced = true means item gets destroyed when quantity reaches varQuantityMin or lower regardless of config setting, returns true if the item gets deleted
	bool SetQuantity(float value, bool destroy_config = true, bool destroy_forced = false, bool allow_client = false)
	{
		if( !IsServerCheck(allow_client) ) return false;
		if( !HasQuantity() ) return false;
		if( IsLiquidContainer() && GetLiquidType() == 0 )
		{
			Debug.LogError("No LiquidType specified, try setting 'varLiquidTypeInit' to a particular liquid type");
			return false;
		}
		
		bool on_min_value = value <= (GetQuantityMin() + 0.001); //workaround, items with "varQuantityDestroyOnMin = true;" get destroyed
		
		if( on_min_value )
		{
			if( destroy_config )
			{
				bool dstr = ConfigGetBool("varQuantityDestroyOnMin");
				if( dstr )
				{
					this.Delete();
					return true;
				}
			}
			else if( destroy_forced )
			{
				this.Delete();
				return true;
			}
			// we get here if destroy_config IS true AND dstr(config destroy param) IS false;
			RemoveAllAgents();//we remove all agents when we got to the min value, but the item is not getting deleted
		}
		
		float min = GetQuantityMin();
		float max = GetQuantityMax();
		
		m_VarQuantity = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_QUANTITY);
		return false;
	}

	//----------------------------------------------------------------
	//! add item quantity[related to varQuantity... config entry], destroy_config = true > if the quantity reaches varQuantityMin or lower and the item config contains the varQuantityDestroyOnMin = true entry, the item gets destroyed. destroy_forced = true means item gets destroyed when quantity reaches varQuantityMin or lower regardless of config setting, returns true if the item gets deleted
	bool AddQuantity(float value, bool destroy_config = true, bool destroy_forced = false)
	{	
		return SetQuantity(GetQuantity() + value, destroy_config, destroy_forced);
	}
	//----------------------------------------------------------------
	void SetQuantityMax()
	{
		float max = GetQuantityMax();
		SetQuantity(max);
	}
	//----------------------------------------------------------------
	//! Sets quantity in normalized 0..1 form between the item's Min a Max values as defined by item's config(for Min 0 and Max 5000, setting 0.5 will result in value 2500)
	void SetQuantityNormalized(float value, bool destroy_config = true, bool destroy_forced = false)
	{
		float value_clamped = Math.Clamp(value, 0, 1);//just to make sure
		int result = Math.Round(Math.Lerp(GetQuantityMin(), GetQuantityMax(), value_clamped));
		SetQuantity(result, destroy_config, destroy_forced);
	}

	/*void SetAmmoNormalized(float value)
	{
		float value_clamped = Math.Clamp(value, 0, 1);
		Magazine this_mag = Magazine.Cast( this );
		int max_rounds = this_mag.GetAmmoMax();
		int result = value * max_rounds;//can the rounded if higher precision is required
		this_mag.SetAmmoCount(result);
	}*/
	//----------------------------------------------------------------
	int GetQuantityMax()
	{
		return ConfigGetInt("varQuantityMax");
	}
	//----------------------------------------------------------------
	int GetQuantityMin()
	{
		return ConfigGetInt("varQuantityMin");
	}
	//----------------------------------------------------------------
	int GetQuantityInit()
	{
		return ConfigGetInt("varQuantityInit");
	}
	
	bool HasQuantity()
	{
		if( GetQuantityMax() - GetQuantityMin() == 0 )
		{
			return false;			
		}
		else
		{
			return true;			
		}
	}

	float GetQuantity()
	{
			//PrintString(m_VarQuantity.ToString());
			return m_VarQuantity;
		}
	
	bool IsFullQuantity()
	{
		if( GetQuantity() == GetQuantityMax() )
		{
			return true;			
		}
		else
		{
			return false;			
		}
	}
	
	//Calculates weight of single item without attachments
	float GetSingleInventoryItemWeight()
	{
		int ConfWeight = ConfigGetInt("weight");
		float item_wetness = GetWet();
		float itemQuantity = 0;
		float Weight = 0;
		if (GetQuantity() != 0)
		{
			itemQuantity = GetQuantity();
		}

		if (ConfigGetBool("canBeSplit")) //quantity determines size of the stack
		{
			Weight = ((item_wetness + 1) * ConfWeight * itemQuantity);
		}
		else if (ConfigGetString("stackedUnit") == "cm") //duct tape, at the moment
		{
			int MaxLength = ConfigGetInt("varQuantityMax");
			Weight = ((item_wetness + 1) * ConfWeight * itemQuantity/MaxLength);
		}
		else if (itemQuantity != 1) //quantity determines weight of item without container (i.e. sardines in a can)
		{
			Weight = ((item_wetness + 1) * (ConfWeight + itemQuantity));
		}
		else
		{
			Weight = ((item_wetness + 1) * ConfWeight);
		}
		return Math.Round(Weight);
	}

	//calculates total weight of item
	override int GetItemWeight()
	{
		int i = 0;
		float totalWeight;
		float item_wetness = this.GetWet();
		int ConfWeight;
		
		int AttachmentsCount = GetInventory().AttachmentCount();
		Cargo cargo = GetInventory().GetCargo();
		
		ConfWeight = this.ConfigGetInt("weight");
		
		//attachments?
		if (AttachmentsCount > 0)
		{
			for (i = 0; i < AttachmentsCount; i++)
			{
				totalWeight += GetInventory().GetAttachmentFromIndex(i).GetItemWeight();
			}
			totalWeight += (item_wetness + 1) * ConfWeight;
		}
		//cargo?
		else if (cargo != NULL )
		{
			for (i = 0; i < cargo.GetItemCount(); i++)
			{
				totalWeight += cargo.GetItem(i).GetItemWeight();
			}
			totalWeight += (item_wetness + 1) * ConfWeight;
			//return Math.Round(GetContainerWeight() + ((item_wetness + 1) * ConfWeight));
		}
		//other
		else
		{
			if (this.ConfigGetBool("canBeSplit")) //quantity determines size of the stack
			{
				totalWeight += Math.Round((item_wetness + 1) * this.GetQuantity() * ConfWeight);
			}
			else if (this.ConfigGetString("stackedUnit") == "cm") //duct tape, at the moment
			{
				int MaxLength = this.ConfigGetInt("varQuantityMax");
				totalWeight += Math.Round(((item_wetness + 1) * ConfWeight * (this.GetQuantity()/MaxLength)));
			}
			else //quantity determines weight of item without container (i.e. sardines in a can)
			{
				totalWeight += Math.Round((item_wetness + 1) * (this.GetQuantity() + ConfWeight));
			}
		}
		
		return Math.Round(totalWeight);
	}


	// Returns the number of items in cargo, otherwise returns 0(non-cargo objects)
	int GetNumberOfItems()
	{
		if ( GetInventory().GetCargo() != NULL )
		{
			int item_count = GetInventory().GetCargo().GetItemCount();
			return item_count;
		}
		return 0;
	}	
	
	void SetVariableMask(int variable)
	{
		m_VariablesMask = variable | m_VariablesMask; 
		if( GetGame().IsServer() ) SetSynchDirty();
	}
	
	//!Removes variable from variable mask, making it appear as though the variable has never been changed from default
	void RemoveItemVariable(int variable)
	{
		m_VariablesMask = ~variable & m_VariablesMask;
	}
	
	//!'true' if this variable has ever been changed from default
	bool IsVariableSet(int variable)
	{
		return (variable & m_VariablesMask);
	}
	
	//-------------------------	Energy

	//----------------------------------------------------------------
	float GetEnergy()
	{
		float energy = 0;
		if( this.HasEnergyManager() )
		{
			energy = this.GetCompEM().GetEnergy();
		}
		return energy;
	}

	void SetTemperature(float value, bool allow_client = false)
	{
		if( !IsServerCheck(allow_client) ) return;
		float min = ConfigGetFloat("varTemperatureMin");
		float max = ConfigGetFloat("varTemperatureMax");
		
		m_VarTemperature = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_TEMPERATURE);
	}
	//----------------------------------------------------------------
	void AddTemperature(float value)
	{
		SetTemperature( value + GetTemperature() );
	}
	//----------------------------------------------------------------
	void SetTemperatureMax()
	{
		float max = ConfigGetFloat("varTemperatureMax");
		SetTemperature(max);
	}
	//----------------------------------------------------------------
	float GetTemperature()
	{
		return m_VarTemperature;
	}
	
	float GetTemperatureInit()
		{
			return ConfigGetFloat("varTemperatureInit");
		}
	
	float GetTemperatureMin()
	{
		return ConfigGetFloat("varTemperatureMin");
	}

	float GetTemperatureMax()
	{
		return ConfigGetFloat("varTemperatureMax");
	}

	//----------------------------------------------------------------
	void SetWet(float value, bool allow_client = false)
	{
		if( !IsServerCheck(allow_client) ) return;
		float min = ConfigGetFloat("varWetMin");
		float max = ConfigGetFloat("varWetMax");
		
		m_VarWet = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_WET);
	}
	//----------------------------------------------------------------
	void AddWet(float value)
	{
		SetWet( GetWet() + value );
	}
	//----------------------------------------------------------------
	void SetWetMax()
	{
		float max = ConfigGetFloat("varWetMax");
		SetWet(max);
	}
	//----------------------------------------------------------------
	override float GetWet()
	{
		return m_VarWet;
	}
	//----------------------------------------------------------------
	float GetWetMax()
	{
		return ConfigGetFloat("varWetMax");
	}
	//----------------------------------------------------------------
	float GetWetMin()
	{
		return ConfigGetFloat("varWetMin");
	}
	//----------------------------------------------------------------
	float GetWetInit()
	{
		return ConfigGetFloat("varWetInit");
	}
	//----------------------------------------------------------------
	bool IsServerCheck(bool allow_client)
	{
		if(allow_client) return true;
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) 
		{
			Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
			return false;
		}
		else
		{
			return true;			
		}
	}

	//----------------------------------------------------------------
	//-------------------------	Color
	// sets items color variable given color components
	void SetColor(int r, int g, int b, int a)
	{
		m_ColorComponentR = r;
		m_ColorComponentG = g;
		m_ColorComponentB = b;
		m_ColorComponentA = a;
		SetVariableMask(VARIABLE_COLOR);
	}
	//! gets item's color variable as components
	void GetColor(out int r,out int g,out int b,out int a)
	{
		r = m_ColorComponentR;
		g = m_ColorComponentG;
		b = m_ColorComponentB;
		a = m_ColorComponentA;
	}
	
	bool IsColorSet()
	{
		if(!IsVariableSet(VARIABLE_COLOR))
		{
			return false;
		}
		return true;
	}
	
	//! Returns item's PROCEDURAL color as formated string, i.e. "#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
	string GetColorString()
	{
		int r,g,b,a;
		GetColor(r,g,b,a);
		r = r/255;
		g = g/255;
		b = b/255;
		a = a/255;
		string save_color = r.ToString()+","+g.ToString()+","+b.ToString()+","+a.ToString();
		string color = "#(argb,8,8,3)color("+save_color+",CO)";
		
		return color;
	}
	//----------------------------------------------------------------
	//-------------------------	LiquidType

	void SetLiquidType(int value, bool allow_client = false)
	{
		if( !IsServerCheck(allow_client) ) return;
		m_VarLiquidType = value;
		SetVariableMask(VARIABLE_LIQUIDTYPE);
	}
	
	int GetLiquidTypeInit()
	{
		return ConfigGetInt("varLiquidTypeInit");
	}
	
	int GetLiquidType()
	{
		return m_VarLiquidType;
	}

	// -------------------------------------------------------------------------
	//! Event called on item when it is placed in the player(Man) inventory, passes the owner as a parameter
	void OnInventoryEnter(Man player)
	{
		PlayerBase nplayer;
		if ( PlayerBase.CastTo(nplayer, player) )
		{
			nplayer.OnItemInventoryEnter(this);
			array<EntityAI> subItems = new array<EntityAI>;
			GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER,subItems);
			
			for(int i = 0; i < subItems.Count(); i++)
			{
				nplayer.SetEnableQuickBarEntityShortcut(subItems.Get(i),true);				
			}
			//nplayer.CalculatePlayerLoad();
			//nplayer.SetEnableQuickBarEntityShortcut(this,true);
			//nplayer.CalculatePlayerLoad();
			//nplayer.ChangePlayerLoad(this, false);
		}
	}

	// -------------------------------------------------------------------------
	//! Event called on item when it is removed from the player(Man) inventory, passes the old owner as a parameter
	void OnInventoryExit(Man player)
	{
		PlayerBase nplayer;
		if ( PlayerBase.CastTo(nplayer,player) )
		{		
			nplayer.OnItemInventoryExit(this);
			// Test item and attached items for quickbar shortcut to remove 
			array<EntityAI> subItems = new array<EntityAI>;
			GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER,subItems);
			
			for(int i = 0; i < subItems.Count(); i++)
			{
				nplayer.SetEnableQuickBarEntityShortcut(subItems.Get(i),false);				
			}
			//nplayer.CalculatePlayerLoad();
			//nplayer.SetEnableQuickBarEntityShortcut(this,false);
			//nplayer.ChangePlayerLoad(this, true);
			
			if (nplayer && nplayer.ItemToInventory && (nplayer.m_EntitySwapped == true || this.IsKindOf("BandanaUntieable_ColorBase") || this.IsKindOf("BandanaMask") ))
			{
				nplayer.ItemToInventory = false;
				nplayer.SwitchBandana(this);
			}
		}
		
		if ( HasEnergyManager() )
		{
			GetCompEM().UpdatePlugState(); // Unplug the el. device if it's necesarry.
		}
	}

	// ADVANCED PLACEMENT EVENTS
	override void OnPlacementStarted( Man player ) 
	{
		super.OnPlacementStarted( player );
		
		SetIsHologram( true );
	}
	
	//-----------------------------
	// AGENT SYSTEM
	//-----------------------------
	//--------------------------------------------------------------------------
	bool ContainsAgent( int agent_id )
	{
		if( agent_id & m_AttachedAgents )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//--------------------------------------------------------------------------
	override void RemoveAgent( int agent_id )
	{
		if( ContainsAgent(agent_id) )
		{
			m_AttachedAgents = ~agent_id & m_AttachedAgents;//invert the mask and apply AND to substitute missing XOR
		}
	}

	//--------------------------------------------------------------------------
	override void RemoveAllAgents()
	{
		m_AttachedAgents = 0;
	}

	// -------------------------------------------------------------------------
	override void InsertAgent(int agent, int count)
	{
		if( count<1 ) return;
		Debug.Log("Inserting Agent on item: " + agent.ToString() +" count: " + count.ToString());
		m_AttachedAgents = (agent | m_AttachedAgents);
	}
	
	//!transfer agents from another item
	void TransferAgents(int agents)
	{
		m_AttachedAgents = (m_AttachedAgents | agents);
	}
	// -------------------------------------------------------------------------
	override int GetAgents()
	{
		return m_AttachedAgents;
	}

	// -------------------------------------------------------------------------
	void LoadAgents(ParamsReadContext ctx)
	{
		ctx.Read(m_AttachedAgents);
	}
	// -------------------------------------------------------------------------
	void SaveAgents(ParamsWriteContext ctx)
	{
		
		ctx.Write(m_AttachedAgents);
	}
	// -------------------------------------------------------------------------

	
	//! Called when entity is being restored from storage folder
	override void EEOnStorageLoad()
	{
		//Print("EEOnStorageLoad");
	}

	//! Called when entity is being created as new by CE/ Debug
	override void EEOnCECreate()
	{
		//Print("EEOnCECreate");
		if( !IsMagazine() && HasQuantity() ) SetCEBasedQuantity();
		//SetCEBasedQuantity();
	}
	
	
	//-------------------------
	// OPEN/CLOSE USER ACTIONS
	//-------------------------
	void Open()
	{
		m_Opened = true;
	}

	void Close()
	{
		m_Opened = false;
	}

	bool IsOpen()
	{
		return m_Opened;
	}
	
	
	//-------------------------
	void Consume(PlayerBase player, float amount)
	{
		AddQuantity(-amount, false, false);
	}

	// ------------------------------------------------------------
	// CONDITIONS
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		
		if ( parent )
		{
			if ( parent.IsInherited(DayZInfected) )
				return true;

			if ( !parent.IsRuined() )
				return true;
		};
		
		return true;
	}	
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if ( !IsRuined() )
			return true;
		return false;
	}
	
	// Plays all muzzle flash particle effect
	static void PlayFireParticles(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		int id = muzzle_owner.GetMuzzleID();
		array<ref WeaponParticlesOnFire> WPOF_array = m_MuzzleFlashOnFire.Get(id);
		
		if (WPOF_array)
		{
			for (int i = 0; i < WPOF_array.Count(); i++)
			{
				WeaponParticlesOnFire WPOF = WPOF_array.Get(i);
				
				if (WPOF)
				{
					WPOF.OnActivate(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
				}
			}
		}
	}
	
	// Plays all muzzle flash particle effect
	static void PlayOverheatingParticles(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		int id = muzzle_owner.GetMuzzleID();
		array<ref WeaponParticlesOnOverheating> WPOOH_array = m_OverheatingEffect.Get(id);
		
		if (WPOOH_array)
		{
			for (int i = 0; i < WPOOH_array.Count(); i++)
			{
				WeaponParticlesOnOverheating WPOOH = WPOOH_array.Get(i);
				
				if (WPOOH)
				{
					WPOOH.OnActivate(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
				}
			}
		}
	}
	
	// Plays all muzzle flash particle effect
	static void StopOverheatingParticles(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		int id = muzzle_owner.GetMuzzleID();
		array<ref WeaponParticlesOnOverheating> WPOOH_array = m_OverheatingEffect.Get(id);
		
		if (WPOOH_array)
		{
			for (int i = 0; i < WPOOH_array.Count(); i++)
			{
				WeaponParticlesOnOverheating WPOOH = WPOOH_array.Get(i);
				
				if (WPOOH)
				{
					WPOOH.OnDeactivate(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
				}
			}
		}
	}
	
	//----------------------------------------------------------------
	//Has item stages in config?
	bool IsItemStaged()
	{
		string path;
		path = "cfgVehicles " + GetType() + " Staging";
		if (!GetGame().ConfigIsExisting(path))
		{
			return false;
		}
		return true;
	}
	
	//----------------------------------------------------------------
	//Gets current item stage (+ returns true if staging is relevant)
	bool GetItemStage(out int stage)
	{
		stage = m_Item_Stage;
		return IsItemStaged();
	}
	
	void SetItemStage(int stage)
	{
		string config_path;
		m_Item_Stage = stage;
		
		if (IsItemStaged())
		{
			ref array<string> config_selections	= new array<string>;
			
			config_path = "CfgVehicles " + GetType() + " proxySelections"/*hiddenSelections*/;
			GetGame().ConfigGetTextArray( config_path, config_selections );
			
			for ( int i = 0; i < config_selections.Count(); i++ )
			{
				if ( config_selections.Get( i ) != config_selections.Get( m_Item_Stage ) )
				{
					 SetAnimationPhase( config_selections.Get( i ), 1 ); //hides other selections on value 1 !!!
				}
			}
			
			SetAnimationPhase( config_selections.Get( m_Item_Stage ), 0 );
		}
		//TODO add event here!
		//ItemStageChanged();
		
		//re-initializes useractions for the current stage
		SetUserActions();
	}
	
	/*event ItemStageChanged()
	{
	}*/
}



EntityAI SpawnItemOnLocation (string object_name, notnull InventoryLocation loc, bool full_quantity)
{
	EntityAI entity = SpawnEntity(object_name, loc);
	if (entity)
	{
		bool is_item = entity.IsInherited( ItemBase );
		if ( is_item && full_quantity )
		{
			ItemBase item = ItemBase.Cast( entity );
			item.SetQuantity(item.GetQuantityMax());// Set full quantity
		}
	}
	else
	{
		Print("Cannot spawn entity: " + object_name);
		return NULL;
	}
	return entity;
}

void SetupSpawnedItem (ItemBase item, float health, float quantity)
{
	if (item)
	{
		if ( quantity == -1 )
		{
			if (item.HasQuantity())
				quantity = item.GetQuantityMax();
		}

		item.SetHealth("", "", health);

		if ( quantity > 0 )
		{
			item.SetQuantity(quantity);
		}
	}
}
