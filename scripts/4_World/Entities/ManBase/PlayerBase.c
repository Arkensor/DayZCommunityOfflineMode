class PlayerBase extends ManBase
{	
	private int						m_LifeSpanState;
	private int						m_LastShavedSeconds;
	private int						m_BloodType;
	private bool					m_HasBloodTypeVisible;
	private bool					m_LiquidTendencyDrain;
	private bool					m_HasBloodyHandsVisible;
	private int 					m_StoreLoadVersion;
	
	const int 						ACT_STORE_SAVE_VERSION = 2;

	private PluginPlayerStatus		m_ModulePlayerStatus;
	PluginConfigEmotesProfile 		m_ConfigEmotesProfile;
	private PluginLifespan 			m_ModuleLifespan;
	
	ref PlayerStats 				m_PlayerStats;
	PluginRecipesManager 			m_ModuleRecipesManager;
	ref BleedingSourcesManager 		m_BleedingManager;
	ref ModifiersManager			m_ModifiersManager;
	ref NotifiersManager 			m_NotifiersManager = NULL;
	ref protected ActionManagerBase	m_ActionManager;
	ref PlayerAgentPool				m_AgentPool;
	ref Environment					m_Environment;
	ref EmoteManager 				m_EmoteManager;
	ref VehicleManager 				m_VehicleManager;
	ref StateManager				m_StateManager;
	ref DisplayStatus 				m_DisplayStatus = NULL;
	ref StaminaHandler				m_StaminaHandler;
	ref InjuryHandler				m_InjuryHandler;
	ref SoftSkillsManager			m_SoftSkillsManager;
	ref StanceIndicator				m_StanceIndicator;
	ref TransferValues				m_TrasferValues;
	ref DebugMonitorValues			m_DebugMonitorValues;
	const int 						BLEEDING_TRESHOLD = 100;
	const int 						SHOCK_TRESHOLD = 1000;
	const int 						OVERLOAD_LIMIT = 30000;
	float 							m_CargoLoad;
	ref Param2<float,float>			m_UAProgressParam;
	ref Param2<int,int> 			m_UAParam;
	ref Param2<float,float>			m_StaminaParam;
	ref Param1<string> 				m_UAParamMessage;
	ref ItemsGenerator 				m_ItemsGenerator;
	ref BulletHitReaction			m_BulletHitReaction;
	//ref CraftingMeta 				m_CraftingMeta;
	ref WeaponManager				m_WeaponManager;
	ref CraftingManager 			m_CraftingManager;
	ref InventoryActionHandler 		m_InventoryActionHandler;
	ref protected QuickBarBase		m_QuickBarBase;
	bool 							m_QuickBarHold;
	Hud 							m_HUD;
	protected bool					m_CancelAction;
	protected float 				m_dT;
	protected int 					m_RecipePick;
	protected bool					m_IsHoldingBreath;
	bool							m_EntitySwapped; //TODO revise whole switching system, might not use "MiscGameplayFunctions.TurnItemIntoItem" correctly
	float							m_LastPostFrameTickTime;
	int m_Time;
	#ifdef BOT
	ref Bot							m_Bot;
	#endif
	
	// XBOX_TEMP - Call from xbox mission
	void StaroyeRespawn()
	{
#ifdef PLATFORM_XBOX
		StaroyeInfectedRespawner.InfectedRespawn();
#endif
	}
	
	float GetDeltaT()
	{
		return m_dT;
	}
	//Temp QuickBar load	
	ref array<ref Param2<EntityAI,int> >	m_aQuickBarLoad;
	
	//hologram
	ref Hologram m_HologramServer;
	ref Hologram m_HologramLocal;
	vector m_LocalProjectionPosition = "0 0 0 ";
	vector m_LocalProjectionOrientation = "0 0 0 "; 

	// Temporary functions
	// NTF_COUNT
	ref Param3<string, int, bool> m_MyNotifiers[NTF_COUNT];
	
	//Sheduler
	float m_LastTick = -1;
	
	//crafting start
	int m_RecipeID = -1;
	EntityAI m_Item1;
	EntityAI m_Item2;
	bool m_IsCraftingReady;
	float m_RecipeAnimLength;
	vector m_CraftingInitialPos;
	ref array<int> m_Recipes;
	//crafting end
	
	//writing notes start
	bool enterNoteMenuRead;
	bool enterNoteMenuWrite;
	EntityAI m_writingImplement;
	EntityAI m_paper;
	int m_Handwriting = 0; //to be extended into some index (list with multiple font types)
	//writing notes end
	
	bool m_IsClimbingLadder;
	bool m_IsSwimming;
	bool ItemToInventory;

	// debug monitor
	int m_DebugMonitorEnabled;
	
	void PlayerBase()
	{	
		Init();
	}
	

	void Init()
	{		
		//Print("PSOVIS: new player");
		PrintString( "isServer " + GetGame().IsServer().ToString() );
		PrintString( "isClient " + GetGame().IsClient().ToString() );
		PrintString( "isMultiplayer " + GetGame().IsMultiplayer().ToString() );
		SetEventMask(EntityEvent.INIT|EntityEvent.POSTFRAME);
		//m_Agents = new array<int>; //transmission agents
		m_StoreLoadVersion = 0;
		m_IsCraftingReady = false;
		m_Recipes = new array<int>;
		m_LastShavedSeconds = 0;
		m_BloodType = 0;
		m_HasBloodTypeVisible = false;
		m_LifeSpanState = 0;
		m_HasBloodyHandsVisible = false;
		m_PlayerSelected = false;
		m_LiquidTendencyDrain = false;
		m_UAParamMessage = new Param1<string>("");
		m_UAParam = new Param2<int,int>(0,0);
		m_UAProgressParam = new Param2<float,float>(0,0);
		m_QuickBarBase = new QuickBarBase(this);
		m_CargoLoad = 0;
		m_HUD = GetGame().GetMission().GetHud();
		m_CancelAction = false;
		m_RecipePick = 0;
		
		if(  GetGame().IsClient() || ( !GetGame().IsMultiplayer() && GetGame().IsServer() ) )
		{
			m_StanceIndicator = new StanceIndicator(this);
		}
		if( GetGame().IsServer() )
		{
			m_PlayerStats = new PlayerStats(this);//player stats
			m_NotifiersManager = new NotifiersManager(this); // player notifiers 
			m_AgentPool = new PlayerAgentPool(this); // agent pool manager
			m_BleedingManager = new BleedingSourcesManager(this);
			//m_Environment = new Environment(this);//environment effects on player
			m_ModifiersManager = new ModifiersManager(this); // player modifiers 
			//m_ItemsGenerator = new ItemsGenerator(this);// jtomasik - generaters stones around player, disabled due to possible performance cost on server 
		}
		m_StateManager = new StateManager(this); // state manager
		m_ModuleRecipesManager = PluginRecipesManager.Cast(GetPlugin(PluginRecipesManager));
		m_DisplayStatus = new DisplayStatus(this);
		m_TrasferValues = new TransferValues(this);
		m_EmoteManager = new EmoteManager(this);
		m_SoftSkillsManager = new SoftSkillsManager(this);//Soft Skills calculation
		m_StaminaHandler = new StaminaHandler(this);//current stamina calculation	
		m_InjuryHandler = new InjuryHandler(this);
		m_WeaponManager = new WeaponManager(this);
		m_DebugMonitorValues = new DebugMonitorValues(this);
		
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_CraftingManager = new CraftingManager(this,m_ModuleRecipesManager);
			m_InventoryActionHandler = new InventoryActionHandler(this);
		}

		m_ActionManager = NULL;
		//m_VehicleManager = new VehicleManager(this,m_ActionManager);	
		
		m_ConfigEmotesProfile = PluginConfigEmotesProfile.Cast( GetPlugin(PluginConfigEmotesProfile) );
		
		if ( GetDayZGame().IsDebug() )
		{
			PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed = PluginConfigDebugProfileFixed.Cast( GetPlugin(PluginConfigDebugProfileFixed) );
			PluginConfigDebugProfile m_ConfigDebugProfile = PluginConfigDebugProfile.Cast( GetPlugin(PluginConfigDebugProfile) );
			PluginDeveloper m_Developer = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
			
			PlayerBase player = this;
				
			string default_preset = m_ConfigDebugProfile.GetDefaultPreset();
			if ( player && default_preset != "" )
			{
				TStringArray preset_array = new TStringArray;
				m_ConfigDebugProfileFixed.GetPresetItems( default_preset, preset_array );
				
				bool is_preset_fixed = true;
				if ( preset_array.Count() == 0 )
				{
					m_ConfigDebugProfile.GetPresetItems( default_preset, preset_array );
					is_preset_fixed = false;
				}
				
				if ( preset_array.Count() > 0 )
				{
					m_Developer.ClearInventory(player);
					
					for (int i = 0; i < preset_array.Count(); i++)
					{
						float health = 100;
						int quantity = -1;
						if ( is_preset_fixed )
						{
							health = m_ConfigDebugProfileFixed.GetItemHealth( default_preset, i );
							quantity = m_ConfigDebugProfileFixed.GetItemQuantity( default_preset, i );
						}
						else
						{
							health = m_ConfigDebugProfile.GetItemHealth( default_preset, i );
							quantity = m_ConfigDebugProfile.GetItemQuantity( default_preset, i );
						}
						m_Developer.SpawnEntityInInventory(player, preset_array.Get(i), health, quantity );
					}
				}
			}
		}
		
		m_ModulePlayerStatus	= PluginPlayerStatus.Cast( GetPlugin(PluginPlayerStatus) );
		m_ModuleLifespan		= PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		
		RegisterNetSyncVariableInt("m_LifeSpanState", LifeSpanState.BEARD_NONE, LifeSpanState.COUNT);
		RegisterNetSyncVariableInt("m_BloodType", 0, 128);
		RegisterNetSyncVariableBool("m_HasBloodyHandsVisible");
		RegisterNetSyncVariableBool("m_HasBloodTypeVisible");
		RegisterNetSyncVariableBool("m_LiquidTendencyDrain");
		
		/*if (g_Game && g_Game.m_tilePublic && g_Game.m_isTileSet == true)
		{
			g_Game.m_isTileSet = false;
			g_Game.m_tilePublic.Show(false);		
		}*/
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.OnPlayerLoaded);
	}
	
	//--------------------------------------------------------------------------
	// PLAYER DAMAGE EVENT HANDLING
	//--------------------------------------------------------------------------
	override void EEOnSetCaptive(bool captive)
	{
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER && captive )
		{
			GetGame().RPCSingleParam(this, ERPCs.RPC_ON_SET_CAPTIVE, NULL, true, GetIdentity());
		}
	}
	
	
	BulletHitReaction GetBulletHitReaction()
	{
		return m_BulletHitReaction;
	}
	
	void SpawnBulletHitReaction()
	{
		m_BulletHitReaction = new BulletHitReaction();
	}
	
	override void EEKilled( Object killer )
	{
		Print("EEKilled, you have died");
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			// @NOTE: this branch does not happen, EEKilled is called only on server
			if( GetGame().GetPlayer() == this )
			{
				super.EEKilled( killer );
			}
			if (GetHumanInventory().GetEntityInHands())
				ServerDropEntity( GetHumanInventory().GetEntityInHands() );
		}
		else if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)//server
		{
			if( GetBleedingManager() ) delete GetBleedingManager();
			if( GetHumanInventory().GetEntityInHands() )
				ServerDropEntity( GetHumanInventory().GetEntityInHands() );
		}
		
		GetStateManager().OnPlayerKilled();
		
		// kill character in database
		if (GetHive())
		{
			GetHive().CharacterKill(this);
		}

	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, ammo, modelPos);
		
		//DamagePlayer(damageResult, source, modelPos, ammo);
		
		//new bleeding computation
		//---------------------------------------
		if ( GetBleedingManager() && damageResult != null )
		{
			float dmg = damageResult.GetDamage(component, "Blood");
			GetBleedingManager().ProcessHit(dmg, component, ammo, modelPos);
		}
		//Print(damageResult.GetDamage(component,"Health"));
		//---------------------------------------
		
		//if( GetBleedingManager() ) GetBleedingManager().ProcessHit(component, ammo, modelPos);
		#ifdef DEVELOPER
		if(DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE))
		{
			Print("EEHitBy() | Charater " + GetDisplayName() + " hit by " + source.GetDisplayName() + " to " + component);
		}
		
		PluginRemotePlayerDebugServer plugin_remote_server = PluginRemotePlayerDebugServer.Cast( GetPlugin(PluginRemotePlayerDebugServer) );
		if(plugin_remote_server)
		{
			plugin_remote_server.OnDamageEvent(this, damageResult);
		}
		#endif
		if (GetGame().IsDebugMonitor())
			m_DebugMonitorValues.SetLastDamage(source.GetDisplayName());
	}
	
	override void EEHitByRemote(int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, ammo, modelPos);
		
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			SpawnBulletHitReaction();
		}	
		
		Print("DayZPlayerImplement : EEHitByRemote");
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		SwitchItemSelectionTexture(item, slot_name);
		SwitchItemTypeAttach(item, slot_name);
		m_QuickBarBase.updateSlotsCount();
		//Print("item " + item.GetType() + " in slot" + slot_name);
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		SwitchItemSelectionTexture(item, slot_name);
		SwitchItemTypeDetach(item, slot_name);
		m_QuickBarBase.updateSlotsCount();
	}
	
	void SwitchItemTypeAttach(EntityAI item, string slot_name)
	{
		if( !GetGame().IsServer() )
			return;
		//super.SwitchItemSelectionTexture(item, slot_name);
		
		// Switches folded bandana to appropriate mask/scarf
		if ( (slot_name == "mask" || slot_name == "headgear") && ( item.IsKindOf("Bandana_ColorBase") || item.IsKindOf("BandanaMask_ColorBase") || item.IsKindOf("BandanaUntieable_ColorBase") ) )
		{
			int length = item.GetType().Length();
			int underscoreIndex = item.GetType().IndexOfFrom(1, "_");
			string pattern = item.GetType().Substring(underscoreIndex, (length - underscoreIndex));
			PlayerBase player = this;// = GetGame().GetPlayer();
			
			//player.RemoveAttachment(item);
			m_EntitySwapped = false;
			
			if (item.IsKindOf("Bandana_ColorBase"))
			{
				switch (slot_name)
				{
					case ("mask"):
						MiscGameplayFunctions.TurnItemIntoItem(ItemBase.Cast( item ), "BandanaMask" + pattern, player);
						m_EntitySwapped = true;
						//if (item)	item.SetHealth("", "", itemHealth);
					break;
					
					case ("headgear"):
						MiscGameplayFunctions.TurnItemIntoItem(ItemBase.Cast( item ), "BandanaUntieable" + pattern, player);
						m_EntitySwapped = true;
						//if (item)	item.SetHealth("", "", itemHealth);
					break;
				}
			}
		}
	}
	
	void SwitchItemTypeDetach(EntityAI item, string slot_name)
	{
		if ( ( slot_name == "mask" && item.IsKindOf("BandanaMask_ColorBase") ) || ( slot_name == "headgear" && item.IsKindOf("BandanaUntieable_ColorBase") ) )
		{
			ItemToInventory = true;
			/*
			int length = item.GetType().Length();
			int underscoreIndex = item.GetType().IndexOfFrom(5, "_"); // disregards the "" string, second '_'
			string pattern = item.GetType().Substring(underscoreIndex, (length - underscoreIndex));
			PlayerBase player = this;// = GetGame().GetPlayer();

			MiscGameplayFunctions.TurnItemIntoItem(item, "Bandana" + pattern, player);
			if (player.GetItemInHands() == item)
			{
				player.RemoveItemFromHands(item);
				if (item)
				{
					item.Delete();
				}
			}
			*/
		}
	}
	
	void SwitchBandana(EntityAI item)
	{
		if( GetGame().IsServer() )
		{
			int length = item.GetType().Length();
			int underscoreIndex = item.GetType().IndexOfFrom(1, "_");
			string pattern = item.GetType().Substring(underscoreIndex, (length - underscoreIndex));
			PlayerBase player = this;// = GetGame().GetPlayer();
		
			MiscGameplayFunctions.TurnItemIntoItem(ItemBase.Cast( item ), "Bandana" + pattern, player);
		}
	}
	
	//TODO cleanup
	override void SwitchItemSelectionTexture(EntityAI item, string slot_name)
	{
		if (GetGame().GetPlayer())
		{
			if((slot_name == "armband" && FindAttachmentBySlotName("armband")) || ((slot_name == "body" && FindAttachmentBySlotName("armband"))))
			{
				array<string> texture_array = new array<string>;
				GetGame().ConfigGetTextArray ("cfgVehicles " + FindAttachmentBySlotName("armband").GetType() + " hiddenSelectionsTextures", texture_array);
				int selection = 0;
				int selection_hide = 0;
				string texture = "";
				string show_this = "";
				string hide_this = "";
				
				//has top
				if(FindAttachmentBySlotName("body"))
				{
					if (GetGame().GetPlayer().ConfigGetBool("woman") != 1)
					{
						show_this = "camoMaleBig";
						hide_this = "camoMale";
					}
					else
					{
						show_this = "camoFemaleBig";
						hide_this = "camoFemale";
					}
				}
				//has no top
				else
				{
					if (GetGame().GetPlayer().ConfigGetBool("woman") != 1)
					{
						show_this = "camoMale";
						hide_this = "camoMaleBig";
					}
					else
					{
						show_this = "camoFemale";
						hide_this = "camoFemaleBig";
					}
				}
				
				selection = FindAttachmentBySlotName("armband").GetHiddenSelectionIndex(show_this);
				selection_hide = FindAttachmentBySlotName("armband").GetHiddenSelectionIndex(hide_this);
				
				ItemBase itemIB = ItemBase.Cast(FindAttachmentBySlotName("armband"));
				if(itemIB.ConfigGetString("color") == "Dyed")
				{
					texture = itemIB.GetColorString();
				}
				else
				{
					texture = texture_array.Get(selection);
				}
				
				FindAttachmentBySlotName("armband").SetObjectTexture ( selection, texture );
				FindAttachmentBySlotName("armband").SetObjectTexture ( selection_hide, "" );
		
			}
			// matches skin colour shown on item to player's
			if((slot_name == "body" || slot_name == "legs" || slot_name == "feet" || slot_name == "gloves") && (FindAttachmentBySlotName(slot_name) && FindAttachmentBySlotName(slot_name).GetHiddenSelectionIndex("personality") >= 0))
			{
				string slot1 = slot_name.Substring(0, 1);
				slot1.ToUpper();
				int length = slot_name.Length();
				string slot2 = slot_name.Substring(1, length - 1);
				string slot_upper = slot1 + slot2;
				string tmp = GetGame().GetPlayer().ConfigGetString ("empty"+slot_upper);
					
				array<string> tone_mat_array = new array<string>;
				array<string> tone_texture_array = new array<string>;
				int personality = FindAttachmentBySlotName(slot_name).GetHiddenSelectionIndex ("personality");
				GetGame().ConfigGetTextArray ("cfgVehicles " + tmp + " hiddenSelectionsMaterials", tone_mat_array);
				GetGame().ConfigGetTextArray ("cfgVehicles " + tmp + " hiddenSelectionsTextures", tone_texture_array);
				string tone_mat = tone_mat_array.Get(0);
				string tone_texture = tone_texture_array.Get(0);
				
				FindAttachmentBySlotName(slot_name).SetObjectMaterial ( personality, tone_mat );
				FindAttachmentBySlotName(slot_name).SetObjectTexture ( personality, tone_texture );
			}
		}
	}
	
	void RemoveAllItems()
	{
		array<EntityAI> itemsArray = new array<EntityAI>;
		ItemBase item;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			Class.CastTo(item, itemsArray.Get(i));
			if (item && !item.IsInherited(SurvivorBase))	GetInventory().LocalDestroyEntity(item);
		}
	}
	
	
	// --------------------------------------------------
	// User Actions
	//---------------------------------------------------
	
	override void GetSingleUseActions(out TIntArray actions)
	{
		//actions.Insert(AT_WORLD_CRAFT_CANCEL); 
		actions.Insert(AT_WORLD_CRAFT_SWITCH);
		actions.Insert(AT_WORLD_LIQUID_ACTION_SWITCH);
	}
	
	override void GetContinuousActions(out TIntArray actions)
	{
		//actions.Insert(AT_CPR_T);
		actions.Insert(AT_UNCOVER_HEAD_S);
		actions.Insert(AT_UNCOVER_HEAD_T);
		actions.Insert(AT_ATTACHED);
		actions.Insert(AT_WORLD_CRAFT);
		//actions.Insert(AT_DRINK_POND); //<-- moved to interact (will be reused for continuous in future)
		//actions.Insert(AT_DRINK_WELL); //<-- moved to interact (will be reused for continuous in future)
		actions.Insert(AT_IGNITE_FIREPLACE_BY_AIR);
		//actions.Insert(AT_WASH_HANDS_WELL); //<-- moved to interact (will be reused for continuous in future)
		//actions.Insert(AT_WASH_HANDS_WATER); //<-- moved to interact (will be reused for continuous in future)
		actions.Insert(AT_BUILD_OVEN);
		actions.Insert(AT_DISMANTLE_OVEN);
	}
	
	override void GetInteractActions(out TIntArray actions)
	{
		actions.Insert(AT_PICK_BERRY);
		actions.Insert(AT_OPEN_DOORS);
		actions.Insert(AT_CLOSE_DOORS);
		actions.Insert(AT_ENTER_LADDER);
		actions.Insert(AT_EXIT_LADDER);
		actions.Insert(AT_TAKE_ITEM);
		actions.Insert(AT_TAKE_ITEM_TO_HANDS);
		//actions.Insert(AT_SWAP_ITEM_TO_HANDS);
		actions.Insert(AT_LOCKED_DOORS);
		actions.Insert(AT_DRINK_POND_ONE);
		actions.Insert(AT_DRINK_WELL_ONE);
		actions.Insert(AT_WASH_HANDS_WELL_ONE);
		actions.Insert(AT_WASH_HANDS_WATER_ONE);
	}
	
	int GetCurrentRecipe()
	{
		return m_RecipePick;
	}
	
	void SetNextRecipe()
	{
		m_RecipePick++;
	}
	
	void SetFirstRecipe()
	{
		m_RecipePick = 0;
	}
	
	//---------------------------------------------------
	
	
	ActionManagerBase GetActionManager()
	{
		return m_ActionManager;
	}
	
	EmoteManager GetEmoteManager()
	{
		return m_EmoteManager;
	}

	VehicleManager GetVehicleManager()
	{
		return m_VehicleManager;
	}
	
	InventoryActionHandler GetInventoryActionHandler()
	{
		return m_InventoryActionHandler;
	}
	
	void SetStamina( int value, int range )
	{
		m_ModulePlayerStatus.SetStamina( value, range );
	}
	
	PlayerStats GetPlayerStats()
	{
		return m_PlayerStats;	
	}

	CraftingManager GetCraftingManager()
	{
		return m_CraftingManager;		
	}
	
	StaminaHandler GetStaminaHandler()
	{
		return m_StaminaHandler;		
	}
	
	override WeaponManager GetWeaponManager()
	{
		return m_WeaponManager;		
	}
	
	void OnWeaponActionEnd()
	{
		m_WeaponManager.OnWeaponActionEnd();
	}
	/*CraftingMeta GetCraftingMeta()
	{
		return m_CraftingMeta;		
	}

	void CreateCraftingMeta(int id, EntityAI item1,EntityAI item2, vector init_pos, float length, float specialty_weight)
	{
		m_CraftingMeta = new CraftingMeta(id,item1, item2, init_pos, length, specialty_weight);
	}*/
	
	int GetCraftingRecipeID()
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			return GetCraftingManager().GetRecipeID();
		}
		else
		{
			return m_RecipeID;			
		}		
	}
	
	void SetCraftingRecipeID(int recipeID)
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			GetCraftingManager().SetRecipeID(recipeID);
		}
		else
		{
			m_RecipeID = recipeID;			
		}		
	}
	// --------------------------------------------------
	// QuickBar
	//---------------------------------------------------
	
	void RemoveQuickBarEntityShortcut(EntityAI entity)
	{
		int index = m_QuickBarBase.FindEntityIndex(entity);
		if(index != -1)
			m_QuickBarBase.SetEntityShortcut(entity,-1);
	}
	//---------------------------------------------------
	void SetEnableQuickBarEntityShortcut(EntityAI entity, bool value)
	{
		int index = m_QuickBarBase.FindEntityIndex(entity);
		if(index != -1)
			m_QuickBarBase.SetShotcutEnable(index,value);

	}
	//---------------------------------------------------
	int FindQuickBarEntityIndex(EntityAI entity)
	{
		int index;
		index = m_QuickBarBase.FindEntityIndex(entity);

		if(m_QuickBarBase.GetEntity(index) == NULL )
			return -1;

		return index;
	}
	//---------------------------------------------------
	
	int GetQuickBarSize()
	{
		return m_QuickBarBase.GetSize();
	}
	//---------------------------------------------------	
	EntityAI GetQuickBarEntity(int index)
	{
		return m_QuickBarBase.GetEntity(index);
	}
	//---------------------------------------------------
	void UpdateQuickBarEntityVisibility(EntityAI entity)
	{
		int i = FindQuickBarEntityIndex(entity);
		if( i >= 0 )
			m_QuickBarBase.UpdateShotcutVisibility(i);
	}
	//---------------------------------------------------
	void SetQuickBarEntityShortcut(EntityAI entity, int index, bool force = false )
	{
		m_QuickBarBase.SetEntityShortcut(entity, index, force);
	}
	//---------------------------------------------------
	void OnQuickbarSetEntityRequest(ParamsReadContext ctx)
	{
		m_QuickBarBase.OnSetEntityRequest(ctx);
	}
	//---------------------------------------------------	

	// Applies splint on all limbs.
	void ApplySplint()
	{
		// The idea is to slightly increase health of broken limb so the player is still limping. Using more splints will help but each time less. 100% recovery can be achieved only through long term healing.
		
		float add_health_coef = 0.33;
		AddHealth("LeftArm", 	"Health",	( GetMaxHealth("LeftArm", "Health")  - GetHealth("LeftArm", "Health")  ) * add_health_coef 	);
		AddHealth("RightArm",	"Health",	( GetMaxHealth("RightArm", "Health") - GetHealth("RightArm", "Health") ) * add_health_coef 	);
		AddHealth("LeftLeg", 	"Health",	( GetMaxHealth("LeftLeg", "Health")  - GetHealth("LeftLeg", "Health")  ) * add_health_coef 	);
		AddHealth("RightLeg",	"Health",	( GetMaxHealth("RightLeg", "Health") - GetHealth("RightLeg", "Health") ) * add_health_coef 	);
	}
	
	void ProcessHoldBreath()
	{
		if(IsTryingHoldBreath() && CanConsumeStamina(EStaminaConsumers.HOLD_BREATH) )
		{
			DepleteStamina(EStaminaModifiers.HOLD_BREATH);
			m_IsHoldingBreath = true;
		}
		else
		{
			m_IsHoldingBreath = false;			
		}
	}
	
	bool IsHoldingBreath()
	{
		return m_IsHoldingBreath;
	}	
	
	
	void DamagePlayer(TotalDamageResult damageResult, EntityAI source, vector modelPos, string ammo)
	{

		if ( source.IsMan() )
		{
			ManBase man = ManBase.Cast( source );
			ItemBase item = ItemBase.Cast( man.GetHumanInventory().GetEntityInHands() );
			
			if ( item  &&  item.HasEnergyManager() )
			{
				// Prototyping of energy based weapons. 
				
				bool energy_consumed = item.GetCompEM().ConsumeEnergy( item.GetCompEM().GetEnergyUsage() );
				
				if (energy_consumed)
				{
					if ( item.KindOf("CattleProd") )
					{
						DecreaseHealth("GlobalHealth","Shock",2000);
					}
					if ( item.KindOf("StunBaton") )
					{
						DecreaseHealth("GlobalHealth","Shock",4000);
					}
					if ( item.KindOf("Chainsaw") )
					{
						DecreaseHealth("GlobalHealth","Shock",1000);
						DecreaseHealth("GlobalHealth","Blood",1000);
						DecreaseHealth("GlobalHealth","Health",500);
					}
				}
			}
		}
	}


	EntityAI FindCargoByBaseType(string searched_item)
	{
		EntityAI attachment;
		string item_name;
		int attcount = this.GetInventory().AttachmentCount();
		
		for (int att = 0; att < attcount; att++)
		{	
			attachment = GetInventory().GetAttachmentFromIndex(att);
			if ( attachment.IsItemBase() )
			{
				item_name = attachment.GetType();
				if ( GetGame().IsKindOf(item_name, searched_item) )
				{
					return attachment;
				}
			}
		}
		return NULL;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	void OnPlayerLoaded()
	{
		if ( GetGame().IsDebug() )
		{
			if ( !GetGame().IsMultiplayer()  &&  GetGame().GetPlayer()  &&  GetGame().GetPlayer().GetID() == this.GetID() )
			{
				PluginSceneManager scene_editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
				scene_editor.InitLoad();
			}
		}
	
		if ( m_HUD )
		{
			m_HUD.UpdateBloodName();
		}
	}

	// --------------------------------------------------
	// Advanced placement
	//---------------------------------------------------
	
	//get hologram
	Hologram GetHologramServer()
	{
		return m_HologramServer;
	}
	
	Hologram GetHologramLocal()
	{
		return m_HologramLocal;
	}
	
	void PlacingStartServer()
	{
		m_HologramServer = new Hologram( this );
		GetHologramServer().SetProjectionPosition( GetLocalProjectionPosition() );
		GetHologramServer().SetProjectionOrientation( GetLocalProjectionOrientation() );
		GetHologramServer().GetProjectionEntity().OnPlacementStarted( this );
		GetHologramServer().CheckPowerSource();
		GetHologramServer().RefreshVisual();	
	}
	
	void PlacingStartLocal()
	{
		m_HologramLocal = new Hologram( this );
		GetHologramLocal().GetProjectionEntity().OnPlacementStarted( this );
	}
	
	void PlacingCancelServer()
	{
		if( IsPlacingServer() )
		{
			GetHologramServer().GetParentEntity().OnPlacementCancelled( this );
			
			delete m_HologramServer;
		}
	}
	
	void PlacingCancelLocal()
	{
		delete m_HologramLocal;
	}
	
	void PlacingCompleteServer()
	{
		delete m_HologramServer;	
	}
	
	void PlacingCompleteLocal()
	{
		delete m_HologramLocal;
	}
	
	bool IsPlacingServer()
	{
		if ( m_HologramServer )
		{
			return true;
		}
		else
		{
			return false;			
		}
	}
	
	bool IsPlacingLocal()
	{
		if ( m_HologramLocal )
		{
			return true;
		}
		
		return false;			
	}
	
	bool TogglePlacingServer( int userDataType, ParamsReadContext ctx )
	{	
		if ( userDataType == INPUT_UDT_ADVANCED_PLACEMENT )			
		{			
			PlacingCancelServer();	
			return true;
		}
		
		return false;	
	}
		
	void TogglePlacingLocal()
	{		
		if ( IsPlacingLocal() )
		{
			if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT && GetGame().IsMultiplayer() )
			{
				if (ScriptInputUserData.CanStoreInputUserData())
				{
					ScriptInputUserData ctx = new ScriptInputUserData;
					ctx.Write(INPUT_UDT_ADVANCED_PLACEMENT);
					ctx.Send();

					PlacingCancelLocal();
				}
			}		
		}
		else
		{
			PlacingStartLocal();
		}
	}

	void SetLocalProjectionPosition( vector local_position )
	{
		m_LocalProjectionPosition = local_position;
	}
	
	void SetLocalProjectionOrientation( vector local_orientation )
	{
		m_LocalProjectionOrientation = local_orientation;
	}
	
	vector GetLocalProjectionPosition()
	{
		return m_LocalProjectionPosition;
	}
	
	vector GetLocalProjectionOrientation()
	{
		return m_LocalProjectionOrientation;
	}

	// ------------------------------------------------------------------------

	override void EEOnWaterEnter()
	{
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() && m_Environment )
		{
			m_Environment.ProcessItemsWetness();
		}
	}

	// -------------------------------------------------------------------------
	void ~PlayerBase()
	{
	}

	BleedingSourcesManager GetBleedingManager()
	{
		return m_BleedingManager;
	}

	StateManager GetStateManager()
	{
		return m_StateManager;
	}
	DisplayStatus GetDisplayStatus()
	{
		return m_DisplayStatus;
	}

	TransferValues GetTransferValues()
	{
		return m_TrasferValues;
	}

	DebugMonitorValues GetDebugMonitorValues()
	{
		return m_DebugMonitorValues;
	}

	// -------------------------------------------------------------------------
	/*bool  IsCraftingSetUp()
	{
		return m_IsCraftingReady;
	}*/
	// -------------------------------------------------------------------------
	/*void  PerformRecipe()
	{*/
		/*
		Debug.Log("Ingredient1: "+ToString(m_Item1.Ptr().GetType()),"recipes");
		Debug.Log("Ingredient2: "+ToString(m_Item2.Ptr().GetType()),"recipes");
		*/
		/*int id = GetCraftingMeta().GetRecipeID();
		ItemBase item1 = GetCraftingMeta().GetIngredient1();
		ItemBase item2 = GetCraftingMeta().GetIngredient2();
		
		Debug.Log("PerformRecipe called on player: "+ string.ToString(this),"recipes");
		
		if ( m_ModuleRecipesManager )
		{
			if( !item1 || !item2 )
			{
				Debug.Log("PerformRecipe: At least one of the object links is now null, not performing the recipe","recipes");
			}
			else
			{

				m_ModuleRecipesManager.PerformRecipeServer(id, item1,item2,this);
			}
			RequestCraftingDisable();
		}
	}*/
	// -------------------------------------------------------------------------
	/*float GetCraftingSetUpDistance()
	{
		return Math.AbsFloat( vector.Distance( GetCraftingMeta().GetInitPos(), GetPosition() ) );
	}*/
	// -------------------------------------------------------------------------

	/*void RequestCraftingSetup(int id, EntityAI item1, EntityAI item2, int craft_type)
	{
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
		{
			SetUpCraftingClient(id,item1,item2,craft_type);
			SendCraftingMeta();
		}

	}*/

	/*void RequestCraftingDisable()
	{
		if( GetGame().IsServer() ) 
		{
			SendDisableRequestToClient();
			DisableCrafting();
		}

	}*/
	
	/*private void SendDisableRequestToClient()
	{
		GetGame().RPCSingleParam(this, ERPCs.RPC_CRAFTING_DISABLE, NULL,this);
	}*/

	/*private void OnReceiveDisableRequest()
	{
		DisableCrafting();
	}*/
	
	/*private void SendCraftingMeta()
	{
		array<ref Param> params = new array<ref Param>;
		CraftingMeta cm = GetCraftingMeta();
		cm.SerializeValues(params);
		GetGame().RPC(this, ERPCs.RPC_CRAFTING_SETUP, params,this);
	}*/

	/*private void OnReceiveCraftingMeta(ParamsReadContext ctx)
	{
		CreateCraftingMeta(0,NULL,NULL,"0 0 0", 0,0);
		GetCraftingMeta().DeserializeValuesCtx(ctx);
		SetUpCraftingServer();
		
	}*/
	// -------------------------------------------------------------------------
	/*private void SetUpCraftingServer()
	{
		if( GetGame().IsMultiplayer() && GetGame().IsServer() ) 
		{
			m_IsCraftingReady = true;
			m_ModuleRecipesManager.OnCraftingSetUpServer( GetCraftingMeta(), this );
		}
	}*/
	
	/*private void SetUpCraftingClient( int id, EntityAI item1, EntityAI item2, int craft_type)
	{
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
		{
			m_IsCraftingReady = true;
			
			float specialty_weight = m_ModuleRecipesManager.GetRecipeSpecialty(id);
			float length = m_ModuleRecipesManager.GetRecipeLengthInSecs(id);
			CreateCraftingMeta(id, item1, item2, GetPosition(), length, specialty_weight);
			m_ModuleRecipesManager.OnCraftingSetUpClient( GetCraftingMeta(), this );
			Debug.Log("SetUpCrafting2 called for id: "+ id.ToString()+ " on player: "+ this.ToString(),"recipes");
			if ( craft_type != AT_WORLD_CRAFT )
			{
				ActionManagerClient mngr = GetActionManager();
				mngr.DisableActions();
				ActionTarget actionTarget;		
				if ( item2 == GetItemInHands() ) 
				{
					actionTarget = new ActionTarget(item1, -1, vector.Zero, -1);
					mngr.InjectContinuousAction(craft_type,actionTarget,item2);
				}
				else
				{
					actionTarget = new ActionTarget(item2, -1, vector.Zero, -1);
					mngr.InjectContinuousAction(craft_type,actionTarget,item1);
				}
			}			
		}

	}*/

	// -------------------------------------------------------------------------
	/*void DisableCrafting()
	{
		m_IsCraftingReady = false;
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
		{	
			ActionManagerClient mngr = GetActionManager();
			if ( mngr) 
			{
				mngr.EnableActions();
				GetCraftingMeta() = NULL;
			}
		}
	}*/

	//--------------------------------------------------------------------------
	void OnScheduledTick(float deltaTime)
	{
		if( !IsPlayerSelected() || !IsAlive() ) return;
		if( m_ModifiersManager ) m_ModifiersManager.OnScheduledTick(deltaTime);
		if( m_NotifiersManager ) m_NotifiersManager.OnScheduledTick();
		if( m_TrasferValues ) m_TrasferValues.OnScheduledTick(deltaTime);
		if( m_DisplayStatus ) m_DisplayStatus.OnScheduledTick();
	}
	
	void OnCommandHandlerTick(float deltaTime)
	{
		if( !IsPlayerSelected() || !IsAlive() ) return;
		if( m_DebugMonitorValues ) m_DebugMonitorValues.OnScheduledTick(deltaTime);
		if( GetStateManager() ) GetStateManager().OnScheduledTick(deltaTime);//needs to stay in command handler tick as it's playing animations
		
		#ifdef DEVELOPER
		if( m_WeaponDebug && ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT ))
		{ 
			m_WeaponDebug.OnCommandHandlerUpdate();
		}
		#endif
	}
	
	bool m_ShowDbgUI = true;
	int m_DbgListSelection = 0;
	float m_DbgSliderValue = 0.0;
	ref array<string> m_DbgOptions = {"jedna", "dva", "tri"};
	
	NotifiersManager GetNotifiersManager()
	{
		return m_NotifiersManager;
	}
	//--------------------------------------------------------------------------
	void OnTick()
	{
		float deltaT = (GetGame().GetTime() - m_LastTick) / 1000;
		if ( m_LastTick < 0 )  deltaT = 0;//first tick protection
		m_LastTick = GetGame().GetTime();

		//PrintString("deltaT: " + deltaT);
		//PrintString("at time: " + m_LastTick);
		OnScheduledTick(deltaT);		
	}
	// -------------------------------------------------------------------------
	override void EEItemIntoHands(EntityAI item)
	{
		if (this == PlayerBase.Cast( GetGame().GetPlayer() ))
		{
			UIScriptedMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if (menu)
			{
				menu.Refresh();
			}
		}
		Weapon_Base w;
		if (item && Class.CastTo(w, item))
		{
			w.ResetWeaponAnimState();
		}
		//TODO temporary hack, re-work
		/*if ( item.IsKindOf("BandanaMask_ColorBase") || item.IsKindOf("BandanaUntieable_ColorBase") )
		{
			RemoveItemFromHands(item);
			item.Delete();
		}*/
	
		if (ItemToInventory && ( item.IsKindOf("BandanaMask_ColorBase") || item.IsKindOf("BandanaUntieable_ColorBase") ))
			{
				ItemToInventory = false;
				SwitchBandana(item);
			}
	}

	override void EEItemOutOfHands(EntityAI item)
	{	
		//Print("PSOVIS: EEItemOutOfHands");
		if (this == PlayerBase.Cast( GetGame().GetPlayer() ))
		{
			UIScriptedMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if (menu)
			{
				menu.Refresh();
			}
		}
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{	
		// lower implement 
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);

		m_dT = pDt;
		/*
		m_IsSwimming = pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM;
		m_IsClimbingLadder pCurrentCommandID != DayZPlayerConstants.COMMANDID_LADDER
		*/
		ProcessHoldBreath();
		HandleInventory(pDt);
		ActionManagerBase		mngr = GetActionManager();
		HumanInputController hic = GetInputController();

		if( m_EmoteManager ) 
		{
			m_EmoteManager.Update(pDt);	
		}
		if( m_VehicleManager ) 
		{
			m_VehicleManager.Update(pDt);	
		}
		if( m_StanceIndicator ) 
		{
			m_StanceIndicator.Update();
		}
		if( m_StaminaHandler ) 
		{
			m_StaminaHandler.Update(pDt);
		}
		if( m_InjuryHandler )
		{
			m_InjuryHandler.Update(pDt);
		}
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
		{
			if ( m_Environment )
			{
				m_Environment.Update(pDt);
			}
			if ( m_ItemsGenerator )
			{
				m_ItemsGenerator.Update(pDt);
			}
		}	
		
		if (mngr && hic)
		{
			mngr.Update(pCurrentCommandID);
			
			if (hic.IsSingleUse())
			{
				Print("PlayerBase.c OnSingleUse");
				mngr.OnSingleUse();
			//if ( GetGame().IsServer() )  mngr.SetDeliveredTarget(synceddata.GetParam.Param1);
			}
			if (hic.IsContinuousUseStart())
			{
				Print("PlayerBase.c OnContinuousUseStart");
				mngr.OnContinuousStart();
				//if ( GetGame().IsServer() )  mngr.SetDeliveredTarget(synceddata.GetParam.Param1);
			}
			if (hic.IsContinuousUseEnd())
			{
				Print("PlayerBase.c OnContinuousUseEnd");
				mngr.OnContinuousCancel();
			}
			if (hic.IsImmediateAction())
			{
				Print("PlayerBase.c OnAction");
				mngr.OnInteractAction();
			//if ( GetGame().IsServer()  )  mngr.SetDeliveredTarget(synceddata.GetParam.Param1);
			}
			
			// quickbar use
			int quickBarSlot = hic.IsQuickBarSlot();
			if( quickBarSlot )
			{
				if (hic.IsQuickBarSingleUse())
				{
					OnQuickBarSingleUse(quickBarSlot);
					Print("PlayerBase.c IsQuickBarSingleUse - slot: " + quickBarSlot.ToString());
				}
				if (hic.IsQuickBarContinuousUseStart())
				{
					OnQuickBarContinuousUseStart(quickBarSlot);
					Print("PlayerBase.c IsQuickBarContinuousUseStart - slot: " + quickBarSlot.ToString());
				}
				if (hic.IsQuickBarContinuousUseEnd())
				{
					OnQuickBarContinuousUseEnd(quickBarSlot);
					Print("PlayerBase.c IsQuickBarContinuousUseEnd - slot: " + quickBarSlot.ToString());
				}
			}
			/*if ((pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE || pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER || pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM ) ) 
			{
				mngr.Update(); // checks for suitable action and sets it
			}*/
		}
		if (m_StaminaHandler && hic)
		{
			if ( !CanConsumeStamina(EStaminaConsumers.SPRINT) )
				hic.LimitsDisableSprint(true);
			else
				hic.LimitsDisableSprint(false);
		}
		//OnScheduledTick(pDt);
		
		if ( m_CancelAction )
		{
			if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
			{
				PrintString("pb INTERPUT recived - server");
			}
			else
			{
				PrintString("pb INTERPUT recived - client");
			}
			ActionBaseCB callback = ActionBaseCB.Cast( GetCommandModifier_Action() );
			if (!callback)
			{
				callback = ActionBaseCB.Cast( GetCommand_Action() );
			}
			if ( callback && callback.IsUserActionCallback() )
			{
				if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
				{
					PrintString("pb callback found - server");
				}
				else
				{
					PrintString("pb callback found - client");
				}
				callback.Cancel();
			}
			m_CancelAction = false;
		}
		//ladders
		if ( GetCommand_Ladder() && !m_IsClimbingLadder )
		{
			m_IsClimbingLadder = true;
			OnClimbLadderStart();
		}
		else if ( !GetCommand_Ladder() && m_IsClimbingLadder )
		{
			m_IsClimbingLadder = false;
			OnClimbLadderStop();
		}
		//swimming
		if ( GetCommand_Swim() && !m_IsSwimming )
		{
			m_IsSwimming = true;
			OnSwimmingStart();
		}
		else if ( !GetCommand_Swim() && m_IsSwimming && !m_IsClimbingLadder)
		{
			m_IsSwimming = false;
			OnSwimmingStop();
		}
		OnCommandHandlerTick(pDt);
	}
	
	void OnSwimmingStart()
	{
		if ( GetItemInHands() ) GetItemInHands().SetInvisible(true);
		if( GetInventory() ) GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		CloseInventoryMenu();
	}
	
	void OnSwimmingStop()
	{
		if ( GetItemInHands() )	GetItemInHands().SetInvisible(false);
		if( GetInventory() ) GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	void OnClimbLadderStart()
	{
		if ( GetItemInHands() ) GetItemInHands().SetInvisible(true);
		if( GetInventory() ) GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		CloseInventoryMenu();
	}
	
	void OnClimbLadderStop()
	{
		if ( GetItemInHands() )	GetItemInHands().SetInvisible(false);
		if( GetInventory() ) GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	
	override void EOnFrame(IEntity other, float timeslice)
	{
		
	}

	override void EOnPostFrame(IEntity other, int extra)
	{
		float delta_time = (GetGame().GetTime() - m_LastPostFrameTickTime) / 1000;
		m_LastPostFrameTickTime = GetGame().GetTime();
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			m_HUD.Update();
			
			if( GetBulletHitReaction() ) 
			{
				GetBulletHitReaction().Update(delta_time);
			}
			
			if( GetHologramLocal() )
			{
				GetHologramLocal().UpdateHologram();
			}

			m_InventoryActionHandler.OnUpdate();
#ifdef DEVELOPER
			if(m_WeaponDebug)
			{
				m_WeaponDebug.OnPostFrameUpdate();
			}

			if(m_MeleeCombat)
			{
				if(DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE))
				{
					InventoryItem itemInHands = GetItemInHands();
					GetMovementState(m_MovementState);
					m_MeleeCombat.Debug(itemInHands, m_MeleeCombat.GetHitMask());
				}
			}
			if(m_Environment)
			{
				if(DiagMenu.GetBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE))
				{
					m_Environment.ShowEnvDebugPlayerInfo(DiagMenu.GetBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE));
				}
			}
			
			if(GetPluginManager())
			{
				PluginDrawCheckerboard drawCheckerboard = PluginDrawCheckerboard.Cast(GetPluginManager().GetPluginByType(PluginDrawCheckerboard));
				if(drawCheckerboard && !drawCheckerboard.IsActive())
				{
					drawCheckerboard.ShowWidgets(DiagMenu.GetBool(DiagMenuIDs.DM_DRAW_CHECKERBOARD));
				}
			}
#endif
		}
	}

	override void DepleteStamina(EStaminaModifiers modifier)
	{
		if( GetStaminaHandler() )
			GetStaminaHandler().DepleteStamina(modifier);		
	}

	override bool CanConsumeStamina(EStaminaConsumers consumer)
	{
		if(!GetStaminaHandler()) return false;
		
		return (GetStaminaHandler().HasEnoughStaminaFor(consumer) && !IsOverloaded());
	}
	
	// -------------------------------------------------------------------------
	// 		USER ACTIONS
	// -------------------------------------------------------------------------

	bool IsFacingTarget( Object target )
	{
		vector pdir = GetDirection();
		vector ptv = target.GetPosition() - GetPosition();
		pdir.Normalize();
		ptv.Normalize();
		//MessageAction(ToString(pdir)+"  "+ToString(ptv)+"  ");
		if (Math.AbsFloat(pdir[0]-ptv[0]) < 0.5 && Math.AbsFloat(pdir[2]-ptv[2]) < 0.5 )
		{
			return true;
		}
		return false;
	}

	//---------------------------------------------------------
	void OnQuickBarSingleUse(int slotClicked)
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER )
			return;

		if( IsRaised() || GetCommand_Melee() || IsSwimming() || IsClimbingLadder() )
			return;
		
		
		//TODO MW change locking method
		//if( GetDayZPlayerInventory().HasLockedHands() )
		//	return;
			
		EntityAI quickBarEntity = GetQuickBarEntity(slotClicked - 1);//GetEntityInQuickBar(slotClicked - 1);
		
		if(!quickBarEntity)
			return;
		
		Magazine mag;
		Weapon_Base wpn;
		
		if( Class.CastTo(mag, quickBarEntity) && Class.CastTo(wpn, mag.GetHierarchyParent()) )
			return;

		EntityAI inHandEntity = GetHumanInventory().GetEntityInHands();

		InventoryLocation handInventoryLocation = new InventoryLocation;
		handInventoryLocation.SetHands(this,quickBarEntity);
		if( this.GetInventory().HasInventoryReservation(quickBarEntity, handInventoryLocation ) )
			return;
		
		if( inHandEntity == quickBarEntity )
		{
			// @TODO: move to prev loc?
			PredictiveMoveItemFromHandsToInventory();
		}
		else
		{
			InventoryLocation invLocQBItem = new InventoryLocation;
			quickBarEntity.GetInventory().GetCurrentInventoryLocation(invLocQBItem);
			if( GetInventory().HasInventoryReservation(quickBarEntity,invLocQBItem) )
				return;
				
			if (inHandEntity)
			{
				InventoryLocation il = new InventoryLocation;
				quickBarEntity.GetInventory().GetCurrentInventoryLocation(il);
				if( il.GetSlot()!= InventorySlots.INVALID && il.GetSlot() == inHandEntity.GetInventory().GetSlotId() )
				{
					PredictiveSwapEntities( inHandEntity, quickBarEntity );	
				}
				else if(GameInventory.CanSwapEntities( inHandEntity, quickBarEntity ))
				{
					PredictiveSwapEntities( inHandEntity, quickBarEntity );
				}
				else if(GameInventory.CanForceSwapEntities( quickBarEntity, inHandEntity, il ))
				{
					GetHumanInventory().ForceSwapEntities( InventoryMode.PREDICTIVE, quickBarEntity, inHandEntity, il );
				}
				else
				{
					PredictiveTakeEntityToHands( quickBarEntity );
				}
			}
			else
			{
				if( !GetInventory().AddInventoryReservation(quickBarEntity,handInventoryLocation,1000) )
					return;
				
				PredictiveTakeEntityToHands( quickBarEntity );		
			}
		}	
	}
	//---------------------------------------------------------
	void OnQuickBarContinuousUseStart(int slotClicked)
	{
		if ( IsSwimming() || IsClimbingLadder() || GetCommand_Melee())
			return;
		EntityAI quickBarEntity = GetQuickBarEntity(slotClicked - 1);

		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
		{
			if( GetHumanInventory().GetEntityInHands()!= quickBarEntity )
			{
				Weapon_Base wpn;
				Magazine mag;
				Class.CastTo(wpn,  GetHumanInventory().GetEntityInHands() );
				Class.CastTo(mag,  quickBarEntity );
				if(GetWeaponManager().CanLoadBullet(wpn,mag) )				
					GetWeaponManager().LoadBullet(mag);
				else if(GetWeaponManager().CanAttachMagazine(wpn,mag) )
					GetWeaponManager().AttachMagazine(mag);
				else if(GetWeaponManager().CanSwapMagazine(wpn,mag) )
					GetWeaponManager().SwapMagazine(mag);
				else
				{
					if( IsRaised() || GetCommand_Melee() )
						return;

					ActionManagerClient mngr;
					if( Class.CastTo(mngr, GetActionManager()) && quickBarEntity )
					{
						m_QuickBarHold = true;
						mngr.ForceTarget(quickBarEntity);
					}
				}
			}
		}
	}
	//---------------------------------------------------------
	void OnQuickBarContinuousUseEnd(int slotClicked)
	{
		if(  GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			ActionManagerClient mngr;
			if( Class.CastTo(mngr, GetActionManager()) && m_QuickBarHold )	
			{
				mngr.ClearForceTarget();
			}
		}
	}
	
	//---------------------------------------------------------
	//	RADIAL QUICKBAR AND RELOAD ACTIONS
	//---------------------------------------------------------
	//the same functionality as normal quick bar slot key press
	void RadialQuickBarSingleUse( int slotClicked )
	{
		OnQuickBarSingleUse( slotClicked );				
	}
	
	//removed the need for holding down quick bar slot key
	void RadialQuickBarCombine( int slotClicked )
	{
		EntityAI quickBarEntity = GetQuickBarEntity( slotClicked - 1 );
		EntityAI entity_in_hands = GetHumanInventory().GetEntityInHands();
		
		ReloadWeapon( entity_in_hands, quickBarEntity );	
	}

	//removed the need for holding down quick bar slot key
	void QuickReloadWeapon( EntityAI weapon )
	{
		EntityAI magazine = GetMagazineToReload( weapon );
		ReloadWeapon( weapon, magazine );
	}
	
	//Reload weapon with given magazine
	void ReloadWeapon( EntityAI weapon, EntityAI magazine )
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			if( GetHumanInventory().GetEntityInHands()!= magazine )
			{
				Weapon_Base wpn;
				Magazine mag;
				Class.CastTo( wpn,  weapon );
				Class.CastTo( mag,  magazine );
				if(GetWeaponManager().CanLoadBullet( wpn, mag ) )				
					GetWeaponManager().LoadBullet( mag );
				else if(GetWeaponManager().CanAttachMagazine( wpn, mag ) )
					GetWeaponManager().AttachMagazine( mag );
				else if(GetWeaponManager().CanSwapMagazine( wpn, mag ) )
					GetWeaponManager().SwapMagazine( mag );
				else
				{
				}
			}
		}		
	}

	//returns compatible magazine from player inventory with highest ammo count
	EntityAI GetMagazineToReload( EntityAI weapon )
	{
		Weapon_Base weapon_base = Weapon_Base.Cast( weapon );
		WeaponManager weapon_manager = GetWeaponManager();
		EntityAI magazine_to_reload;
		
		int max_mag_ammo_count = 0;
		
		//Get all magazines in (player) inventory
		for ( int att_i = 0; att_i < GetInventory().AttachmentCount(); ++att_i )
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex( att_i );
			ref Cargo attachment_cargo = attachment.GetInventory().GetCargo();
			
			if ( attachment_cargo )
			{
				for ( int cgo_i = 0; cgo_i < attachment_cargo.GetItemCount(); ++cgo_i )
				{
					EntityAI cargo_item = attachment_cargo.GetItem( cgo_i );
					
					//check for proper magazine
					if ( cargo_item.IsMagazine() )
					{
						Magazine magazine = Magazine.Cast( cargo_item );
						
						if ( weapon_manager.CanLoadBullet( weapon_base, magazine ) || weapon_manager.CanAttachMagazine( weapon_base, magazine ) || weapon_manager.CanSwapMagazine( weapon_base, magazine ) )
						{
							if ( magazine.GetAmmoCount() > 0 && magazine.GetAmmoCount() > max_mag_ammo_count )
							{
								magazine_to_reload = EntityAI.Cast( magazine );
								max_mag_ammo_count = magazine.GetAmmoCount();
							}
						}
					}
				}
			}
		}

		return  magazine_to_reload;	
	}
	//---------------------------------------------------------
	
	
	//---------------------------------------------------------
	void OnSpawnedFromConsole()
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
		{
			GetGame().GetMission().AddDummyPlayerToScheduler( this );
		}
	}
	
	//---------------------------------------------------------
	bool GetLiquidTendencyDrain()
	{
		return m_LiquidTendencyDrain;
	}
	//---------------------------------------------------------
	void SetLiquidTendencyDrain(bool state)
	{
		m_LiquidTendencyDrain = state;
		SetSynchDirty();
	}
	
	override SoundOnVehicle PlaySound(string sound_name, float range, bool create_local = false)
	{
		return BetaSound.SaySound(this, sound_name, range, false);
	}
	
	float GetPlayerLoad()
	{
		return m_CargoLoad;
	}
	
	bool IsOverloaded()
	{
		if ( GetPlayerLoad() >= OVERLOAD_LIMIT ) return true;
		return false;
	}

	void SetPlayerLoad(float load)
	{
		m_CargoLoad = load;
		
		//Log(ToString(this) + "'s load weight is " + ftoa(m_CargoLoad) + " g.", LogTemplates.TEMPLATE_PLAYER_WEIGHT);
	}

	void AddPlayerLoad(float addedload)
	{
		float newload = GetPlayerLoad() + addedload;
		SetPlayerLoad(newload);
		
		// Do not need -> Log is in SetPlayerLoad
		//PrintString(ToString(this) + "'s load weight is " + ToString(m_CargoLoad) + " g.");
	}

	bool IsItemInInventory(EntityAI entity)
	{
		return GetInventory().HasEntityInInventory(entity);
	}

	override bool NeedInventoryJunctureFromServer (notnull EntityAI item, EntityAI currParent, EntityAI newParent)
	{
		if (GetGame().IsMultiplayer())
		{
			DayZPlayerInstanceType t = GetInstanceType();
			switch (t)
			{
				case DayZPlayerInstanceType.INSTANCETYPE_AI_SINGLEPLAYER:
					return false;
				case DayZPlayerInstanceType.INSTANCETYPE_CLIENT:
				case DayZPlayerInstanceType.INSTANCETYPE_SERVER:
				case DayZPlayerInstanceType.INSTANCETYPE_AI_SERVER:
				case DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE:
				case DayZPlayerInstanceType.INSTANCETYPE_REMOTE:
					syncDebugPrint("[syncinv] NeedInventoryJunctureFromServer item=" + item + " currPar=" + currParent + " newPar=" + newParent);
					bool i_owned = GetHumanInventory().HasEntityInInventory(item);
					bool cp_owned = false;
					if (currParent)
						cp_owned = GetHumanInventory().HasEntityInInventory(currParent);
					bool np_owned = false;
					if (newParent)
						np_owned = GetHumanInventory().HasEntityInInventory(newParent);

					bool all_owned = i_owned && cp_owned && (np_owned || (newParent == null));
					syncDebugPrint("[syncinv] NeedInventoryJunctureFromServer=" + !all_owned + " i_pwn=" + i_owned + " cp_pwn=" + cp_owned + " np_pwn=" + np_owned);
					return !all_owned;
				default:
					Error("[syncinv] unsupported instance type t=" + t);
					break;
			}
		}
		return false;
	}

	void CalculatePlayerLoad()
	{
		EntityAI attachment;
		ItemBase itemHands;
		
		itemHands = GetItemInHands();
		int attcount = this.GetInventory().AttachmentCount();
		int total_load = 0;
		
		for (int att = 0; att < attcount; att++)
		{	
			attachment = GetInventory().GetAttachmentFromIndex(att);
			if ( attachment.IsItemBase() )
			{
				ItemBase attachmentIB;
				Class.CastTo(attachmentIB, attachment);
				total_load += attachmentIB.GetItemWeight();
			}
		}

		if ( itemHands ) // adds weight of item carried in hands
			total_load += itemHands.GetItemWeight();

		SetPlayerLoad(total_load);
	}

	void ChangePlayerLoad(EntityAI attachment, bool decrease)
	{
		int total_load = 0;
		if ( attachment.IsItemBase() )
		{
			ItemBase attachmentIB;
			Class.CastTo(attachmentIB, attachment);
			total_load = attachmentIB.GetItemWeight();
			
			if ( decrease )
			{
				AddPlayerLoad(-total_load);
			} 
			else
			{
				AddPlayerLoad(total_load);
			}
		}
	}
	//!returns player's immunity strength between 0..1
	float GetImmunity()
	{
		if(	GetPlayerStats() ) 
		{
			float max_health = GetMaxHealth("GlobalHealth", "Health") + 0.01;//addition to prevent divisioin by zero in case of some messup
			float max_blood = GetMaxHealth("GlobalHealth", "Blood") + 0.01;//addition to prevent divisioin by zero in case of some messup
			return ( GetStatDiet().GetNormalized() + GetStatEnergy().GetNormalized() + (this.GetHealth("GlobalHealth", "Health") / max_health) + (this.GetHealth("GlobalHealth", "Blood") / max_blood) ) / 4;
		}
		return 0;
	}
	
	bool IsSprinting() //jtomasik - Actions cannot be performed while sprinting, later should be parameter of action itself
	{
		if (m_MovementState.m_iMovement == 3) return true;	//  jtomasik - DayZPlayerConstants.MOVEMENT_SPRINT ?
		return false;
	}

	bool IsRaised() // rrada - until we have some event about changed state
	{
		GetMovementState(m_MovementState);
		return m_MovementState.IsRaised();
	}
	
	bool IsSwimming()
	{
		return m_IsSwimming;
	}
	
	bool IsClimbingLadder()
	{
		return m_IsClimbingLadder;
	}
	
	
	void OnItemInventoryEnter(EntityAI item)
	{
		//ChangePlayerLoad(item, false);
		CalculatePlayerLoad();
	}
	
	void OnItemInventoryExit(EntityAI item)
	{
		//ChangePlayerLoad(item, true);
		CalculatePlayerLoad();
	}
	
	// -------------------------------------------------------------------------
	override void RPC(int rpc_type, array<ref Param> params, bool guaranteed, PlayerIdentity recipient = NULL)
	{
		super.RPC(rpc_type, params, guaranteed, recipient);
	}

	// -------------------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch(rpc_type)
		{
			case ERPCs.RPC_SYNC_DISPLAY_STATUS:
				if( GetDisplayStatus() ) 
				{
					GetDisplayStatus().OnRPC(ctx);
				}
			break;
	  
			case ERPCs.RPC_PLAYER_STATES_ON:
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPC(ERPCs.RPC_PLAYER_STATES_ON, ctx);
				}
			break;
			 
			case ERPCs.RPC_PLAYER_STATES_OFF:
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPC(ERPCs.RPC_PLAYER_STATES_OFF, ctx);
				}
			break;

			case ERPCs.RPC_PLAYER_STATES_DEBUG_ON:
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPCDebug(ERPCs.RPC_PLAYER_STATES_DEBUG_ON, ctx);
				}
			break;

			case ERPCs.RPC_PLAYER_STATES_DEBUG:
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPCDebug(ERPCs.RPC_PLAYER_STATES_DEBUG, ctx);
				}
			break;
			
			case ERPCs.RPC_PLAYER_STATES_EXIT:
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPCDebug(ERPCs.RPC_PLAYER_STATES_EXIT, ctx);
				}
			break;		
			
			case ERPCs.RPC_USER_ACTION_MESSAGE:
				if( !GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT ) 
				{
					break;
				}
				if ( ctx.Read(m_UAParamMessage) )
				{
					string actiontext = m_UAParamMessage.param1;
					MessageAction(actiontext);
				}
			break;
			
			case ERPCs.RPC_USER_ACTION_MESSAGES:
				if( !GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
				{
					break;
				}
				if ( ctx.Read(m_UAParam) && GetActionManager() )
				{
					string actiontext2 = GetActionManager().GetAction(m_UAParam.param1).GetMessageText(m_UAParam.param2);
					MessageAction(actiontext2);
				}
			break;
			
			case ERPCs.RPC_STAMINA:
				if( !GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
				{
					break;
				}
				if ( ctx.Read(m_StaminaParam) && m_StaminaHandler )
				{
					m_StaminaHandler.SyncStamina(m_StaminaParam.param1,m_StaminaParam.param2);
				}
			break; 
		
			case ERPCs.RPC_CRAFTING_INVENTORY_INSTANT:
				ref Param3<int, ItemBase, ItemBase> craftParam = new Param3<int, ItemBase, ItemBase>(-1, NULL, NULL);
				if (ctx.Read( craftParam ) )
				{
					m_ModuleRecipesManager.PerformRecipeServer(craftParam.param1, craftParam.param2, craftParam.param3, this);				
				}
			break;
				
		
			/*
			case ERPCs.RPC_RECIPE_SEND:
				if ( m_ModuleRecipesManager )
				{
					//m_ModuleRecipesManager.OnRPC(ctx,this);
				}
			break; 
			*/
			case ERPCs.RPC_ON_SET_CAPTIVE:
				CallMethod( CALL_ID_HIDE_INVENTORY, NULL );
			break;
			
			case ERPCs.RPC_DAMAGE_VALUE_SYNC:
				if(m_TrasferValues) m_TrasferValues.OnRPC(ctx);
			break; 
			case ERPCs.RPC_DEBUG_MONITOR_FLT:
				if(m_DebugMonitorValues) m_DebugMonitorValues.OnRPCFloat(ctx);
			break;
			case ERPCs.RPC_DEBUG_MONITOR_STR:
				if(m_DebugMonitorValues) m_DebugMonitorValues.OnRPCString(ctx);
			break;
			case ERPCs.RPC_SOFT_SKILLS_SPECIALTY_SYNC:
				ref Param1<float> p_synch = new Param1<float>(0);
				ctx.Read( p_synch );
				float specialty_level = p_synch.param1;
				GetSoftSkillManager().SetSpecialtyLevel( specialty_level );
			break;

			case ERPCs.RPC_SOFT_SKILLS_STATS_SYNC:
				ref Param5<float, float, float, float, bool> p_debug_synch = new Param5<float, float ,float, float, bool>(0, 0, 0, 0, false);
				ctx.Read( p_debug_synch );
				float general_bonus_before = p_debug_synch.param1;
				float general_bonus_after = p_debug_synch.param2;
				float last_UA_value = p_debug_synch.param3;
				float cooldown_value = p_debug_synch.param4;
				float cooldown_active = p_debug_synch.param5;
				GetSoftSkillManager().SetGeneralBonusBefore( general_bonus_before );
				GetSoftSkillManager().SetGeneralBonusAfter( general_bonus_after );
				GetSoftSkillManager().SetLastUAValue( last_UA_value );
				GetSoftSkillManager().SetCoolDownValue( cooldown_value );
				GetSoftSkillManager().SetCoolDown( cooldown_active );
			break;
			
			case ERPCs.RPC_INIT_SET_QUICKBAR:
				ref Param1<int> count = new Param1<int>(0); 
				if( ctx.Read( count ) );
				{
					for( int i = 0; i < count.param1 ; i++)
					{
						m_QuickBarBase.OnSetEntityRPC(ctx);	
					}
				}
			break;
			
			case ERPCs.DEV_REQUEST_PLAYER_DEBUG:
				PluginRemotePlayerDebugServer plugin_remote_server = PluginRemotePlayerDebugServer.Cast( GetPlugin(PluginRemotePlayerDebugServer) );
				plugin_remote_server.OnRPC(ctx, this);
			break;
			
			case ERPCs.DEV_PLAYER_DEBUG_DATA:
				PluginRemotePlayerDebugClient plugin_remote_client = PluginRemotePlayerDebugClient.Cast( GetPlugin(PluginRemotePlayerDebugClient) );
				plugin_remote_client.OnRPC(ctx);
			break;
			
			/*
			case ERPCs.RPC_ITEM_SPLIT:
				Param1<ItemBase> param = new Param1<ItemBase>(null);
				if( ctx.Read( param ) );
				{
					param.param1.SplitItem(this);
				}
			break;
			*/
		}
#ifdef DEVELOPER
			PluginDeveloper module_rc = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
			if(module_rc) module_rc.OnRPC(this, rpc_type, ctx);
			
			PluginDeveloperSync module_rcs = PluginDeveloperSync.Cast( GetPlugin(PluginDeveloperSync) );
			if(module_rcs) module_rcs.OnRPC(this, rpc_type, ctx);
			
			PluginDiagMenu plugin_diag_menu = PluginDiagMenu.Cast( GetPlugin(PluginDiagMenu) );
			if(plugin_diag_menu) plugin_diag_menu.OnRPC(this, rpc_type, ctx);
#endif
	}
	
	// -------------------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		m_ModuleLifespan.SynchLifespanVisual( this, m_LifeSpanState, m_HasBloodyHandsVisible, m_HasBloodTypeVisible, m_BloodType );
	}
	//--------------------------------------------------------------------------
	void OnSelectPlayer()
	{
		m_QuickBarBase.updateSlotsCount();

		m_PlayerSelected = true;

		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
		{
			//! add callbacks for ai target system
			SetAITargetCallbacks(new AITargetCallbacksPlayer(this));

			array<ref Param> params = new array<ref Param>;	
			if( m_aQuickBarLoad )
			{
				int count = m_aQuickBarLoad.Count();
				ref Param1<int> paramCount = new Param1<int>(count);
				params.Insert(paramCount);
				for(int i = 0; i < count; i++)
				{
					m_QuickBarBase.OnSetEntityNoSync(m_aQuickBarLoad.Get(i).param1,m_aQuickBarLoad.Get(i).param2);
					params.Insert(m_aQuickBarLoad.Get(i));
				}					
							
				if(count > 0 && GetGame().IsMultiplayer() )
				{
					GetGame().RPC(this, ERPCs.RPC_INIT_SET_QUICKBAR, params, true, GetIdentity());
				}
				m_aQuickBarLoad = NULL;
			}		
				
			m_SoftSkillsManager.InitSpecialty( GetStatSpecialty().Get() );
			GetModifiersManager().SetModifiers(true);
			
			SetSynchDirty();
		}
		
		if(GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			m_ActionManager = new ActionManagerServer(this);

		}
		else
		{
			m_ActionManager = new ActionManagerClient(this);
		}
		
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			PPEffects.ResetAll();
			CheckForBurlap();
			
			if ( g_Game.IsNewCharacter() )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(SetNewCharName);
			}
			
#ifdef BOT
            m_Bot = new Bot(this);
            m_Bot.Start(true, new MyBotTrigger(this));
#endif
		}
	}
	
	void SetNewCharName()
	{
		if ( g_Game.GetMenuData().GetLastPlayedCharacter() > (g_Game.m_OriginalCharactersCount - 1) ) //Checks whether new character has been saved
		{
			g_Game.GetMenuData().SetCharacterName(g_Game.GetMenuData().GetLastPlayedCharacter(), g_Game.GetPlayerGameName());
		}
	}

	void CheckForBurlap()
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			EntityAI attachment;
			Class.CastTo(attachment, GetInventory().FindAttachment(InventorySlots.HEADGEAR));
			if ( attachment && attachment.GetType() == "BurlapSackCover" )
			{
				PPEffects.EnableBurlapSackBlindness();
			}
		}
	}
	
	
	// -------------------------------------------------------------------------
	override bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if( super.OnInputUserDataProcess(userDataType, ctx) )
			return true;
		
		if( m_QuickBarBase.OnInputUserDataProcess(userDataType, ctx) )
			return true;
		
		if( m_WeaponManager.OnInputUserDataProcess(userDataType, ctx) )
			return true;

		if( HandleRemoteItemManipulation(userDataType, ctx) )
			return true;
		
		if ( userDataType == INPUT_UDT_INVENTORY && GetHumanInventory().OnInputUserDataProcess(ctx) )
			return true;
		
		if ( TogglePlacingServer( userDataType, ctx ) )
			return true;
			
		if( m_ActionManager )
			return m_ActionManager.OnInputUserDataProcess(userDataType, ctx);
		
		return false;
	}

	bool HandleRemoteItemManipulation(int userDataType, ParamsReadContext ctx)
	{
		if( userDataType == INPUT_UDT_ITEM_MANIPULATION )
		{
			int type = -1;
			ItemBase item1 = null;
			ItemBase item2 = null;
			bool use_stack_max = false;
			int slot_id = -1;

			if (!ctx.Read(type))
				return false;
			if (!ctx.Read(item1))
				return false;
			if (!ctx.Read(item2))
				return false;
			if (!ctx.Read(use_stack_max))
				return false;
			if (!ctx.Read(slot_id))
				return false;
					
			if (type == -1)//combine
			{
				if ( item1 && item2 )
					item1.CombineItems(item2, use_stack_max);
			}
			else
			{
				if ( use_stack_max )
					item1.SplitIntoStackMax(item2, slot_id);
				else
					item1.SplitItem(this);
			}
			return true;
		}
		return false;
	}
	
	// -------------------------------------------------------------------------
	ItemBase GetItemInHands()
	{
		ItemBase item = ItemBase.Cast( GetHumanInventory().GetEntityInHands() );
		return item;
	}

	//--------------------------------------------------------------------------


	override EntityAI SpawnEntityOnGroundPos (string object_name, vector pos)
	{
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
		{
			bool is_AI = GetGame().IsKindOf( object_name, "DZ_LightAI");
			if (is_AI)
			{
				return SpawnAI(object_name, pos);
			}
			else
			{
				InventoryLocation inv_loc = new InventoryLocation;
				vector mtx[4];
				Math3D.MatrixIdentity4(mtx);
				mtx[3] = pos;
				inv_loc.SetGround(NULL, mtx);
				return GetGame().SpawnEntity(object_name, inv_loc);
			}
		}
		return null;
	}

	EntityAI SpawnEntityOnGroundOnCursorDir (string object_name, float distance)
	{
		vector position = GetPosition() + ( GetDirection() * distance );
		return SpawnEntityOnGroundPos(object_name, position);
	}

	EntityAI SpawnAI (string object_name, vector pos)
	{
		bool is_ai = GetGame().IsKindOf( object_name, "DZ_LightAI");
		if (is_ai)
		{
			return EntityAI.Cast( GetGame().CreateObject(object_name, pos, false, is_ai) );
		}
		return NULL;
	}


	/*EntityAI SpawnAttachmentOnPointerPos (string object_name, bool full_quantity, EntityAI attachmentObject = NULL)
	{
		Object cursor_obj = GetCursorObject();
		if ( cursor_obj != NULL && cursor_obj.IsInherited(EntityAI) )
		{
			EntityAI eai = (EntityAI)cursor_obj;
			eai.TakeEntityAsAttachment(item);
		}
		else if ( attachmentObject != NULL)
		{
			attachmentObject.TakeEntityAsAttachment(item);
		}
	}*/

	/**
	\brief Spawn item on server side
		\param object_name \p string Name of item class
		\param spawn_type \int(const) Type of spawning const: SPAWNTYPE_GROUND, SPAWNTYPE_HANDS, SPAWNTYPE_INVENTORY
		\param full_quantity \bool Spawn with full quantity of item
		\param distance \float Distance of spawn position from player (Only if spawn_type = SPAWNTYPE_GROUND)
		\returns \p ItemBase Spawned item
		@code
			PlayerBase player = GetGame().GetPlayer();
			
			player.SpawnItem("Fruit_Apple", SPAWNTYPE_GROUND, true, 5);
		@endcode
	*/
	/*EntityAI SpawnItem (string object_name, bool full_quantity, float distance = 0, bool usePosition = false, vector pos = "0 0 0", EntityAI attachmentObject = NULL)
	{
		EntityAI entity = NULL;

		// Creat Object
		bool is_ai = GetGame().IsKindOf( object_name, "DZ_LightAI");
		vector item_position;
		if( usePosition )
		{
			item_position = pos;
		}
		else
		{
			item_position = GetPosition() + ( GetDirection() * distance );
		}
		entity = GetGame().CreateObject(object_name, item_position, false, is_ai);

		if ( !entity )
		{
			string s = "Cannot spawn entity: "+object_name;
			Print(s);
			//g_Game.GetUIManager().ShowDialog("Spawn error", "Cannot spawn item: "+object_name, 0, DBT_OK, DBB_YES, DMT_EXCLAMATION, this);
			return NULL;
		}

		if ( entity.IsInherited( ItemBase ) )
		{
			ItemBase item = entity;

			// Set full quantity
			if ( full_quantity )
			{
				item.SetQuantity(item.GetQuantityMax());
			}

			// Spawn In Inventory
			if ( spawn_type == SPAWNTYPE_INVENTORY )
			{
				TakeItemToInventory(item);
			}
			// Spawn In Hands
			if ( spawn_type == SPAWNTYPE_HANDS )
			{
				TakeItemToHands(item);
			}
			// Spawn As Attachment
			if ( spawn_type == SPAWNTYPE_ATTACHMENT )
			{
				Object cursor_obj = GetCursorObject();
				if ( cursor_obj != NULL && cursor_obj.IsInherited(EntityAI) )
				{
					EntityAI eai = (EntityAI)cursor_obj;
					eai.TakeEntityAsAttachment(item);
				}
				else if ( attachmentObject != NULL)
				{
					attachmentObject.TakeEntityAsAttachment(item);
				}
			}
		}

		return entity;
	}*/

	//--------------------------------------------------------------------------
	bool DropItem(ItemBase item)
	{
		bool can_be_dropped = CanDropEntity( item );
		if( can_be_dropped )
		{
			can_be_dropped = PredictiveDropEntity( item );
		}

		vector 	pos_spawn	= GetPosition() + GetDirection();
		pos_spawn[0]		= pos_spawn[0] + Math.RandomFloat(-0.2, 0.2);
		pos_spawn[2]		= pos_spawn[2] + Math.RandomFloat(-0.2, 0.2);

		item.SetPosition(pos_spawn);
		item.PlaceOnSurface();
		return can_be_dropped;
	}

	/*bool TakeItemToInventory (bool synchronize, ItemBase item)
	{
		ItemBase itemInHands;
		itemInHands = GetItemInHands();

		if ( itemInHands == item )
		{
			SynchronizedMoveItemFromHandsToInventory(synchronize);
			return true;
		}

		// in case the player have a cargo item in hands we put item from vicinity into it
		if ( itemInHands && itemInHands.GetInventory().GetCargo() )
		{
			if( itemInHands.GetInventory().CanAddEntityIntoInventory(item) )
			{
				EntityAI entity;
				Class.CastTo(entity, item);
				itemInHands.SynchronizedTakeEntityToCargo(do_synchronize, entity);
				return true;
			}
		}

		if ( GetInventory().CanAddEntityToInventory(item) )
		{
			SynchronizedTakeEntityToInventory(synchronize, item);
			return true;
		}

		//g_Game.GetUIManager().ShowDialog("Spawn error", "Cannot take item into inventory! ("+object_name+")", 0, DBT_OK, DBB_YES, DMT_EXCLAMATION, this);
		return false;
	}

	void TakeItemToHands(ItemBase item)
	{
		TakeEntityToHands(item);
	}*/

	// -------------------------------------------------------------------------
	/**
	/brief Creates \p ItemBase in players inventory. If player dont have enough slots for new item, item will be created on the ground under player.
		\param item_name \p string Class name of ItemBase in configs
		\returns \p ItemBase Created item
		@code
			ItemBase item = g_Game.GetPlayer().CreateInInventory("Consumable_GardenLime", "cargo_weapon");
		@endcode
	*/
	
	
	EntityAI CreateInInventory( string item_name, string cargo_type = "", bool full_quantity = false ) // TODO: Create item in cargo
	{
		InventoryLocation inv_loc = new InventoryLocation;
		if (GetInventory().FindFirstFreeLocationForNewEntity(item_name, FindInventoryLocationType.ANY, inv_loc))
		{
			return SpawnItemOnLocation(item_name, inv_loc, full_quantity);
		}
		return NULL;
	}


	// -------------------------------------------------------------------------
	/**
	/brief Creates copy of \p ItemBase in players inventory. If player dont have enough slots for new item, item will be created on the ground under player.
		\param item_name \p string Class name of ItemBase in configs
		\returns \p ItemBase Created item
		@code
			ItemBase item = g_Game.GetPlayer().CreateInInventory("Consumable_GardenLime", "cargo_weapon");
		@endcode
	*/
	ItemBase CopyInventoryItem( ItemBase orig_item )
	{
		ItemBase item = ItemBase.Cast( GetInventory().CreateInInventory( orig_item.GetType() ) );
		if ( item == NULL )
		{
			return NULL;
		}

		// Copy of quantity
		item.SetQuantity( orig_item.GetQuantity() );

		// Copy of damage
		item.SetHealth( "", "", orig_item.GetHealth("", "") );

		return item;
	}

	// -------------------------------------------------------------------------
	/**
	colorStatusChannel
	colorAction
	colorFriendly
	colorImportant
	*/
	void Message( string text, string style )
	{
		if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			GetGame().ChatMP(this, text, style );
		}
		else
		{
			GetGame().Chat( text, style );
		}
	}

	// -------------------------------------------------------------------------
	void MessageStatus( string text )
	{
		Message( text, "colorStatusChannel" );
	}

	// -------------------------------------------------------------------------
	void MessageAction( string text )
	{
		Message( text, "colorAction" );
	}

	// -------------------------------------------------------------------------
	void MessageFriendly( string text )
	{
		Message( text, "colorFriendly" );
	}

	// -------------------------------------------------------------------------
	void MessageImportant( string text )
	{
		Message( text, "colorImportant" );
	}

	void CloseInventoryMenu()
	{
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
			if( menu && menu.GetID() == MENU_INVENTORY )
			{
				GetGame().GetUIManager().Back();
			}
		}
	}
	
	// -------------------------------------------------------------------------
	/**
	/brief Delete all items in inventory
		\returns \p void
		@code
			PlayerBase player = GetGame().GetPlayer();
			player.ClearInventory();
		@endcode
	*/
	void ClearInventory( )
	{
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer() )
		{
			ItemBase item;
			for ( int i = 0; i < GetInventory().GetSlotsCount(); ++i )
			{
				item = ItemBase.Cast( GetInventory().FindAttachment(i) );
				if ( item )
				{
					GetGame().ObjectDelete(item);
				}
			}

			ItemBase item_in_hands = ItemBase.Cast( GetHumanInventory().GetEntityInHands() );

			if ( item_in_hands )
			{
				LocalDestroyEntityInHands();
			}
		}
	}
	
	//! Drops all clothes/wearables this character is carrying on themselves.
	void DropAllItems()
	{
		array<EntityAI> itemsArray = new array<EntityAI>;
		ItemBase item;
		GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, itemsArray);
		int count = itemsArray.Count();
		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			Class.CastTo(item, itemsArray.Get(i));
			
			if (item  &&  !item.IsInherited(SurvivorBase))	
			{
				ServerDropEntity(item);
			}
		}
	}

	string GetPlayerClass()
	{
		string type;
		GetGame().ObjectGetType( this, type );
		return type;
	}

	// --------------------------------------------------
	// Lifespan
	//---------------------------------------------------
	
	void ShavePlayer()
	{
		SetLastShavedSeconds( StatGet("playtime") );

		m_ModuleLifespan.UpdateLifespan( this, true );
	}

	bool CanShave()
	{
		if ( GetLifeSpanState() )
		{
			return true;		
		}
		else
		{
			return false;			
		}
	}

	void SetLifeSpanStateVisible( int show_state )
	{
		m_LifeSpanState = show_state;
		SetSynchDirty();
	}
	
	int GetLifeSpanState()
	{
		return m_LifeSpanState;
	}
	
	int GetLastShavedSeconds()
	{
		return m_LastShavedSeconds;
	}

	void SetLastShavedSeconds( int last_shaved_seconds )
	{
		m_LastShavedSeconds = last_shaved_seconds;
	}
	
	bool HasBloodyHands()
	{
		return m_HasBloodyHandsVisible;
	}

	void SetBloodyHands( bool show )
	{
		m_HasBloodyHandsVisible = show;
		SetSynchDirty();
	}
	
	bool HasBloodTypeVisible()
	{
		return m_HasBloodTypeVisible;
	}
	
	void SetBloodTypeVisible( bool show )
	{
		m_HasBloodTypeVisible = show;
		SetSynchDirty();
	}
			
	int GetBloodType()
	{
		return m_BloodType;
	}
	
	void SetBloodType( int blood_type )
	{
		m_BloodType = blood_type;
		SetSynchDirty();
	}

	// --------------------------------------------------
	// Soft Skills
	//---------------------------------------------------
	
	SoftSkillsManager GetSoftSkillManager()
	{
		return m_SoftSkillsManager;
	}
	
	// -------------------------------------------------------------------------
	
	int GetStoreLoadVersion()
	{
		return m_StoreLoadVersion;
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write( ACT_STORE_SAVE_VERSION );
		super.OnStoreSave( ctx );

		GetHumanInventory().OnStoreSave(ctx); // FSM of hands
		OnStoreSaveLifespan( ctx );

		if ( GetDayZGame().IsServer() && GetDayZGame().IsMultiplayer() )
		{
			// TODO vratajin :: this is potentionally dangerous
			//                  you have no info about what was written and what's not
			if( GetPlayerStats() ) GetPlayerStats().SaveStats(ctx);// save stats
			if( m_ModifiersManager ) m_ModifiersManager.OnStoreSave(ctx);// save modifiers
			if( m_AgentPool ) m_AgentPool.OnStoreSave(ctx);//save agents
			if( GetStateManager() ) GetStateManager().OnStoreSave(ctx);//save states
			if( GetBleedingManager() ) GetBleedingManager().OnStoreSave(ctx);//save bleeding sources
		}
	}

	void SetLoadedQuickBarItemBind(EntityAI entity, int index)
	{
		//if( m_aQuickBarLoad )
			m_aQuickBarLoad.Insert(new Param2<EntityAI, int>(entity,index));
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		m_aQuickBarLoad = new array<ref Param2<EntityAI,int>>;

		ctx.Read( m_StoreLoadVersion );
		super.OnStoreLoad( ctx );

		GetHumanInventory().OnStoreLoad(ctx); // FSM of hands
		OnStoreLoadLifespan( ctx );

		if ( GetDayZGame().IsServer() && GetDayZGame().IsMultiplayer() )
		{
			// TODO vratajin :: this is potentionally dangerous
			//                  you have no info about what was written and what's not
			if( GetPlayerStats() ) GetPlayerStats().LoadStats(ctx); // load stats
			if( m_ModifiersManager ) m_ModifiersManager.OnStoreLoad(ctx);// load modifiers
			if( m_AgentPool ) m_AgentPool.OnStoreLoad(ctx);//load agents
			if( GetStateManager() ) GetStateManager().OnStoreLoad(ctx);//save states
			if( GetBleedingManager() ) GetBleedingManager().OnStoreLoad(ctx);//save bleeding sources
		}
	}

	void AfterStoreLoad()
	{
		GetHumanInventory().OnAfterStoreLoad();
		//SetSynchDirty();		
	}

	void OnStoreSaveLifespan( ParamsWriteContext ctx )
	{		
		ctx.Write( m_LastShavedSeconds );		
		ctx.Write( m_HasBloodyHandsVisible );
		ctx.Write( m_HasBloodTypeVisible );
		ctx.Write( m_BloodType );
	}

	void OnStoreLoadLifespan( ParamsReadContext ctx )
	{	
		int last_shaved = 0;
		ctx.Read( last_shaved );
		m_LastShavedSeconds = last_shaved;
		
		bool bloody_hands = false;
		ctx.Read( bloody_hands );
		m_HasBloodyHandsVisible = bloody_hands;
		
		bool blood_visible = false;
		ctx.Read( blood_visible );
		m_HasBloodTypeVisible = blood_visible;
		
		int blood_type = 0;
		ctx.Read( blood_type );
		m_BloodType = blood_type;
	}

	void OnConnect()
	{
		Debug.Log("Player connected:"+this.ToString(),"Connect");

		// NEW STATS API
		StatRegister("playtime");
		StatRegister("dist");
	}

	void SetModifiers(bool enable)
	{
		GetModifiersManager().SetModifiers(enable);
	}
	
#ifdef DEVELOPER
	void SetMeleeBlockingStance(bool enabled)
	{
		m_MeleeCombat.SetBlockingStance(enabled);
	}
	void SetMeleeFight(bool enabled)
	{
		m_MeleeCombat.SetMeleeFight(enabled);
	}
#endif

	void Consume(ItemBase source, float amount, EConsumeType consume_type = EConsumeType.ITEM )
	{
		PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast( GetPlugin(PluginTransmissionAgents) );
		if( consume_type == EConsumeType.ITEM )
		{
			if(source)
			{
				
				plugin.TransmitAgents(source, this, AGT_UACTION_CONSUME, amount);
				source.Consume(this,amount);
				ProcessNutritions(source.GetNutritionalProfile(),amount);
			}
		}
		if( consume_type == EConsumeType.ENVIRO_POND || consume_type == EConsumeType.ENVIRO_WELL )
		{
			ProcessNutritions(Liquid.GetNutritionalProfile(LIQUID_WATER),amount);
			if ( consume_type == EConsumeType.ENVIRO_POND ) plugin.TransmitAgents(NULL, this, AGT_WATER_POND, amount);
		}
	}
	
	void ProcessNutritions( NutritionalProfile profile, float consumedquantity )
	{	
		float energy_per_unit = profile.GetEnergy() / 100;
		float water_per_unit = profile.GetWaterContent() / 100;
		float nutritions = profile.GetNutritionalIndex();
		float fullness_index = profile.GetFullnessIndex();
		float toxicity = profile.GetToxicity();
		
		float consumedwater = (consumedquantity * water_per_unit);
		
		if ( consumedquantity > 0 )
		{
			float water_consumed = consumedquantity * water_per_unit;
			float solids_consumed = consumedquantity - water_consumed;
			GetStatStomachEnergy().Add( consumedquantity * energy_per_unit );
			GetStatStomachWater().Add( water_consumed );
			GetStatStomachSolid().Add(solids_consumed * fullness_index );
		}
		else
		{
			Print("ZERO VOLUME! Fix config");
		}
	}
	
	void UpdateQuickBarExtraSlots()
	{
			m_QuickBarBase.updateSlotsCount();
	}


	void OnDisconnect()
	{
		Debug.Log("Player disconnected:"+this.ToString(),"Connect");

		// force update of the stats
		// if player disconnect too soon, UpdatePlayersStats() is not called
		StatUpdateByTime("playtime");
		StatUpdateByPosition("dist");
		StatSyncToClient();
	}

	bool Save()
	{
		// saved player must be alive and not captive
		if (GetPlayerState() == EPlayerStates.ALIVE && !IsRestrained())
		{
			GetHive().CharacterSave(this);
			Debug.Log("Player "+this.ToString()+ " saved as alive");
			
			return true;
		}
		else
		{
			GetHive().CharacterKill(this);
			Debug.Log("Player "+this.ToString()+ " saved as dead");
			
			return false;
		}		
	}

	// agent transfer

	bool IsInfected()
	{
		if( m_AgentPool && m_AgentPool.GetAgents().Count() != 0) 
		{
			return true;			
		}
		else
		{
			return false;
		}
	}
	 
	//--------------------------------------------------------------------------------------------
	override int GetAgents()
	{
		return PluginTransmissionAgents.MakeMaskFromArray( m_AgentPool.GetAgents() );
	}

	//--------------------------------------------------------------------------------------------
	override void RemoveAgent(int agent_id) //removes a single agent type to the player agent pool
	{
		m_AgentPool.RemoveAgent(agent_id);
	}

	//--------------------------------------------------------------------------------------------
	override void RemoveAllAgents()
	{
		m_AgentPool.RemoveAllAgents();
	}


	//--------------------------------------------------------------------------------------------
	override void InsertAgent(int agent, int count) //adds a single agent type to the player agent pool
	{
		m_AgentPool.AddAgent(agent,count);
	}

	//--------------------------------------------------------------------------------------------
	int GetSingleAgentCount(int agent_id)
	{
		return m_AgentPool.GetSingleAgentCount(agent_id);
	}

	//--------------------------------------------------------------------------------------------
	float GetSingleAgentCountNormalized(int agent_id)
	{
		int max_count = PluginTransmissionAgents.GetAgentMaxCount(agent_id);
		return m_AgentPool.GetSingleAgentCount(agent_id) / max_count;
	}

	int GetTotalAgentCount()
	{
		return m_AgentPool.GetTotalAgentCount();
	}

	void PrintAgents()
	{
		m_AgentPool.PrintAgents();
	}

	void ImmuneSystemTick(float value, float deltaT)
	{
		m_AgentPool.ImmuneSystemTick(value, deltaT);
	}

	//Get aim (player crosshair) position
	vector GetAimPosition()
	{
		float min_distance = 0.5;		//min distance, default = 5m
		
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + (GetGame().GetCurrentCameraDirection() * min_distance);
		vector contactPos;
		vector contactDir;
		int contactComponent;
		
		DayZPhysics.RaycastRV( from, to, contactPos, contactDir, contactComponent, NULL, NULL, NULL, false, true );
		
		return contactPos;
	}

	//get modifier manager
	ModifiersManager GetModifiersManager()
	{
		return m_ModifiersManager;
	}

	PlayerStat<float> GetStatStomachSolid()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.STOMACH_SOLID));
			return stat;
		}
		return null;
	}

	PlayerStat<float> GetStatWater()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.WATER));
			return stat;
		}
		return null;
	}

	PlayerStat<float> GetStatStomachWater()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.STOMACH_WATER));
			return stat;
		}
		return null;
	}

	PlayerStat<float> GetStatEnergy()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.ENERGY));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatShock()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.SHOCK));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatUnconscious()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.UNCONSCIOUS));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatTemperature()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.TEMPERATURE));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatHeatComfort()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.HEATCOMFORT));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatHeatIsolation()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.HEATISOLATION));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatTremor()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.TREMOR));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatWet()
	{
		if( GetPlayerStats() ) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.WET));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatStomachEnergy()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.STOMACH_ENERGY));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatDiet()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.DIET));
			return stat;
		}
		return null;
	}
	
	PlayerStat<float> GetStatStamina()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.STAMINA));
			return stat;
		}
		return null;
	}

	PlayerStat<float> GetStatSpecialty()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.SPECIALTY));
			return stat;
		}
		return null;
	}
	
	PlayerStat<int> GetStatBloodType()
	{
		if( GetPlayerStats() )
		{
			PlayerStat<int> stat = PlayerStat<int>.Cast(GetPlayerStats().GetStatObject(EPlayerStats.BLOODTYPE));
			return stat;
		}
		return null;
	}

	//! -------------------------------------------------------------------------
	//! Injured stat
	
	//! update injured state immediately
	void ForceUpdateInjuredState()
	{
		m_InjuryHandler.CheckValue(true);
	}
	
	// -------------------------------------------------------------------------
	void GetDebugActions(out TSelectableActionInfoArray outputList)
	{
		PluginTransmissionAgents plugin_trans_agents = PluginTransmissionAgents.Cast( GetPlugin(PluginTransmissionAgents) );
		
		string name;
		int id;
		map<int,string> agent_list = plugin_trans_agents.GetSimpleAgentList();
		
		for(int i = 0; i < agent_list.Count(); i++)
		{
			name = agent_list.GetElement(i);
			id = agent_list.GetKey(i);
			
			outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION,EActions.DEBUG_AGENTS_RANGE_INJECT_START + id, "Inject " + name));
			outputList.Insert(new TSelectableActionInfo(SAT_DEBUG_ACTION,EActions.DEBUG_AGENTS_RANGE_REMOVE_START + id, "Remove " + name));
		}
	}		

	//-------------------------------------------------------------
	//!
	//! SyncJunctures
	//! 

	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		super.OnSyncJuncture(pJunctureID, pCtx);
			
		switch( pJunctureID )
		{
			case DayZPlayerSyncJunctures.SJ_INJURY:
				float damage;
				bool enable;
				DayZPlayerSyncJunctures.ReadInjuryParams(pCtx, enable, damage);
				m_InjuryHandler.SetInjuryCommandParams(enable, damage);
				break;
			case DayZPlayerSyncJunctures.SJ_ACTION_INTERRUPT:
				m_CancelAction = true;
				break;

			case DayZPlayerSyncJunctures.SJ_PLAYER_STATES:
				GetStateManager().SetAnimation(pCtx);
				break;
			case DayZPlayerSyncJunctures.SJ_QUICKBAR_SET_SHORTCUT:		
				OnQuickbarSetEntityRequest(pCtx);
				break;
			case DayZPlayerSyncJunctures.SJ_INVENTORY:
				GetInventory().OnInventoryJunctureFromServer(pCtx);
				break;
			case DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT:
			case DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT:
				m_ActionManager.OnSyncJuncture(pJunctureID,pCtx);
				break;
			case DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_ACCEPT:
			case DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_REJECT:
				m_WeaponManager.OnSyncJuncture(pJunctureID,pCtx);
				break;
		}
	}
}
