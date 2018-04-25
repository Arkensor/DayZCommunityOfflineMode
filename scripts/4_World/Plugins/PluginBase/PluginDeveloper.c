class PluginDeveloper extends PluginBase
{
	protected bool									m_IsWinHolding;	
	protected bool 									m_GodModeEnabled;
	protected int									m_FeaturesMask;
	
	static PluginDeveloper GetInstance()
	{
		return PluginDeveloper.Cast( GetPlugin( PluginDeveloper ) );
	}
	
	//! Set Player position at his cursor position in the world
	void TeleportAtCursor()
	{
		DeveloperTeleport.TeleportAtCursor();
	}
	
	//! Teleport player at position
	void Teleport(PlayerBase player, vector position)
	{
		DeveloperTeleport.SetPlayerPosition(player, position);
	}
	
	//! Enable / Disable Free camera (Fly mod)
	void ToggleFreeCameraBackPos()
	{
		DeveloperFreeCamera.FreeCameraToggle( PlayerBase.Cast( GetGame().GetPlayer() ), false );
	}
	
	//! Enable / Disable Free camera (Fly mod) - disable of camera will teleport player at current free camera position.
	void ToggleFreeCamera()
	{
		DeveloperFreeCamera.FreeCameraToggle( PlayerBase.Cast( GetGame().GetPlayer() ), true );
	}
		
	bool IsEnabledFreeCamera()
	{
		return DeveloperFreeCamera.IsFreeCameraEnabled();
	}	
	
	// System Public Events
	void PluginDeveloper()
	{
		
	}

	override void OnInit()
	{
		super.OnInit();
		
		m_GodModeEnabled = false;
		
		DeveloperFreeCamera.OnInit();
	}
	
	void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		switch(rpc_type)
		{
			case ERPCs.DEV_RPC_SPAWN_ITEM_ON_GROUND:
			{
				OnRPCSpawnEntityOnGround(player, ctx); break;
			}
			case ERPCs.DEV_RPC_SPAWN_ITEM_ON_CURSOR:
			{
				OnRPCSpawnEntityOnCursorDir(player, ctx); break;
			}
			case ERPCs.DEV_RPC_SPAWN_ITEM_IN_INVENTORY:
			{
				OnRPCSpawnEntity(player, ctx); break;
			}
			case ERPCs.DEV_RPC_CLEAR_INV:
			{
				OnRPCClearInventory(player); break;
			}
			case ERPCs.DEV_RPC_SEND_SERVER_LOG:
			{
				OnRPCServerLogRecieved( ctx ); break;
			}
			case ERPCs.DEV_RPC_TOGGLE_GOD_MODE:
			{
				OnRPCToggleGodMode( player ); break;
			}
			case ERPCs.RPC_SYNC_SCENE_OBJECT:
			{
				OnRPCSyncSceneObject( ctx ); break;
			}
			case ERPCs.DEV_RPC_PLUGIN_DZCREATURE_DEBUG:
			{
				OnRPCPluginDayzCreatureDebug(player, rpc_type, ctx);
			}
		}
		
		DeveloperTeleport.OnRPC(player, rpc_type, ctx);
	}
	
	// Public API
	// Send Print to scripted console
	void PrintLogClient(string msg_log)
	{	
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			Param1<string> msg_p = new Param1<string>(msg_log);
			CallMethod(CALL_ID_SCR_CNSL_ADD_PRINT, msg_p);
		}
	}	
	
	// Server Log Synch: Server Side
	void SendServerLogToClient(string msg)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
			
		for ( int i = 0; i < players.Count(); ++i )
		{
			ref Param1<string> param = new Param1<string>( msg );
			Man player = players.Get(i);
			players.Get(i).RPCSingleParam(ERPCs.DEV_RPC_SEND_SERVER_LOG, param, true, player.GetIdentity());
		}
	}
	
	void OnRPCSyncSceneObject( ParamsReadContext ctx )
	{
		Param3<string, EntityAI, Param> par = new Param3<string, EntityAI, Param>( "", NULL, new Param );

		if ( ctx.Read( par ) )
		{		
			PluginSceneManager module_scene_editor = PluginSceneManager.Cast( GetPlugin( PluginSceneManager ) );
			SceneData	scene_data = module_scene_editor.GetLoadedScene();
			SceneObject scene_object = scene_data.GetSceneObjectByEntityAI( par.param2 );
			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallByName( scene_object, par.param1, par.param3 );
		}
	}
	
	void OnRPCPluginDayzCreatureDebug(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		PluginDayZCreatureAIDebug pluginDZCreatureDebug = PluginDayZCreatureAIDebug.Cast( GetPlugin( PluginDayZCreatureAIDebug ) );
		if (pluginDZCreatureDebug)
		{
			pluginDZCreatureDebug.OnRpc(player, rpc_type, ctx);
		}
	}

	// RPC Events
	// Server Log Synch: Client Side
	void OnRPCServerLogRecieved(ParamsReadContext ctx)
	{
		ref Param1<string> param = new Param1<string>( "" );
		
		if ( ctx.Read(param) && param.param1 != "" )
		{
			Debug.ReceivedLogMessageFromServer(param.param1);
		}
	}
	
	void OnRPCToggleGodMode( PlayerBase player )
	{
		ToggleGodMode( player );
	}
	
	void OnRPCClearInventory(PlayerBase player)
	{
		ClearInventory(player);
	}
	
	// Client -> Server Spawning: Server Side
	void OnRPCSpawnEntityOnCursorDir(PlayerBase player, ParamsReadContext ctx)
	{
		ref Param4<string, float, float, float> p = new Param4<string, float, float, float>("", 0, 0, 0);
		if ( ctx.Read(p) )
		{
			SpawnEntityOnCursorDir(player, p.param1, p.param2,  p.param3,  p.param4);
		}
	}
	void OnRPCSpawnEntityOnGround(PlayerBase player, ParamsReadContext ctx)
	{
		ref Param4<string, float, float, vector> p = new Param4<string, float, float, vector>("", 0, 0, "0 0 0");
		if ( ctx.Read(p) )
		{
			SpawnEntityOnGroundPos(player, p.param1, p.param2,  p.param3,  p.param4);
		}
	}
	void OnRPCSpawnEntity(PlayerBase player, ParamsReadContext ctx)
	{
		ref Param3<string, float, float> p = new Param3<string, float, float>("", 0, 0);
		if ( ctx.Read(p) )
		{
			SpawnEntityInInventory(player, p.param1, p.param2,  p.param3);
		}
	}

	
	void OnSetFreeCameraEvent( PlayerBase player, FreeDebugCamera camera )
	{
		DeveloperFreeCamera.OnSetFreeCameraEvent( player, camera );
	}
	
	// God mode
	bool IsGodMode()
	{
		return m_GodModeEnabled;
	}

	void ToggleGodModeThisPlayer()
	{
		ToggleGodMode( PlayerBase.Cast( GetGame().GetPlayer() ) );
	}

	void ToggleGodMode( PlayerBase player )
	{
		if ( GetGame().IsServer() )
		{
			if ( !m_GodModeEnabled )
			{
				player.SetAllowDamage( false );
				player.MessageStatus( "God mode ENABLED" );
				m_GodModeEnabled = true;
			}
			else
			{
				player.SetAllowDamage( true );
				player.MessageStatus( "God mode DISABLED" );
				m_GodModeEnabled = false;
			}
		}
		else
		{
			if ( !m_GodModeEnabled )
			{
				m_GodModeEnabled = true;
			}
			else
			{
				m_GodModeEnabled = false;
			}
			player.RPCSingleParam( ERPCs.DEV_RPC_TOGGLE_GOD_MODE, NULL, true );
		}
	}
	
	void OnSpawnErrorReport (string name)
	{
		PrintString("PluginDeveloper.SpawnEntity() Warning- Cant spawn object: " + name);
		DumpStack();
		PrintString("PluginDeveloper.SpawnEntity() Warning END");
	}

	void SetupSpawnedEntity (EntityAI entity, float health, float quantity)
	{
		if ( entity.IsInherited( PlayerBase ) ) 
		{
			PlayerBase plr = PlayerBase.Cast( entity );
			plr.OnSpawnedFromConsole();
		}
		else if ( entity.IsInherited(ItemBase) )
		{
			ItemBase item = ItemBase.Cast( entity );
			SetupSpawnedItem(item, health, quantity);
		}
	}
	
	/**
	 * @fn		SpawnEntityOnGroundPos
	 * @brief	spawns entity on exact position
	 * @param[in]	health	\p	health of the item
	 * @param[in]	quantity	\p	quantity to set if item.HasQuantity() (-1 == set to max)
	 * @param[in]	pos	\p	position where to spawn
	 * @return	entity if ok, null otherwise
	 **/
	EntityAI SpawnEntityOnGroundPos (PlayerBase player, string item_name, float health, float quantity, vector pos)
	{
		if ( GetGame().IsServer() )
		{		
			EntityAI entity = player.SpawnEntityOnGroundPos(item_name, pos);
			if (entity)
				SetupSpawnedEntity(entity, health, quantity);
			else
				OnSpawnErrorReport(item_name);
			return entity;
		}
		else
		{		
			ref Param4<string, float, float, vector> params = new Param4<string, float, float, vector>(item_name, health, quantity, pos);
			player.RPCSingleParam(ERPCs.DEV_RPC_SPAWN_ITEM_ON_GROUND, params, true);
		}
		return NULL;
	}
	/**
	 * @fn		SpawnEntityOnCursorDir
	 * @brief	spawns entity in direction of cursor at specified distance
	 * @param[in]	health	\p	health of the item
	 * @param[in]	quantity	\p	quantity to set if item.HasQuantity() (-1 == set to max)
	 * @param[in]	distance	\p	distance of the item from player
	 * @return	entity if ok, null otherwise
	 **/
	EntityAI SpawnEntityOnCursorDir (PlayerBase player, string item_name, float health, float quantity, float distance)
	{
		if ( GetGame().IsServer() )
		{		
			// Client -> Server Spawning: Server Side
			EntityAI entity = player.SpawnEntityOnGroundOnCursorDir(item_name, distance);
			if (entity)
				SetupSpawnedEntity(entity, health, quantity);
			else
				OnSpawnErrorReport(item_name);
			return entity;
		}
		else
		{		
			// Client -> Server Spawning: Client Side
			ref Param4<string, float, float, float> params = new Param4<string, float, float, float>(item_name, health, quantity, distance);
			player.RPCSingleParam(ERPCs.DEV_RPC_SPAWN_ITEM_ON_CURSOR, params, true);
		}
		return NULL;
	}
	/**
	 * @fn		SpawnEntityInInventory
	 * @brief	tries to spawn item somewhere in inventory
	 *
	 * SpawnEntityInInventory queries future parent about free inventory location of the future item
	 *
	 * @param[in]	health	\p	health of the item
	 * @param[in]	quantity	\p	quantity to set if item.HasQuantity() (-1 == set to max)
	 * @return	entity if ok, null otherwise
	 **/
	EntityAI SpawnEntityInInventory (PlayerBase player, string item_name, float health, float quantity)
	{
		if ( GetGame().IsServer() )
		{		
			InventoryLocation il = new InventoryLocation;
			if (player.GetInventory().FindFirstFreeLocationForNewEntity(item_name, FindInventoryLocationType.ANY, il))
			{
				Weapon_Base wpn = Weapon_Base.Cast(il.GetParent());
				bool is_mag = il.GetSlot() == InventorySlots.MAGAZINE || il.GetSlot() == InventorySlots.MAGAZINE2 || il.GetSlot() == InventorySlots.MAGAZINE3;
				if (wpn && is_mag)
				{
					vector pos = player.GetPosition();
					EntityAI eai_gnd = SpawnEntityOnGroundPos(player, item_name, health, quantity, pos);
					Magazine mag_gnd = Magazine.Cast(eai_gnd);
					if (mag_gnd && player.GetWeaponManager().CanAttachMagazine(wpn, mag_gnd))
					{
						player.GetWeaponManager().AttachMagazine(mag_gnd);
					}
					return eai_gnd;
				}
				else
				{
					EntityAI eai = GetGame().SpawnEntity(item_name, il);
					if ( eai && eai.IsInherited(ItemBase) )
					{
						ItemBase i = ItemBase.Cast( eai );
						SetupSpawnedItem(i, health, quantity);
					}
					return eai;
				}
			}
			else
				OnSpawnErrorReport(item_name);
			return NULL;
		}
		else
		{		
			// Client -> Server Spawning: Client Side
			ref Param3<string, float, float> params = new Param3<string, float, float>(item_name, health, quantity);
			player.RPCSingleParam(ERPCs.DEV_RPC_SPAWN_ITEM_IN_INVENTORY, params, true);
		}
		return NULL;
	}
	/**
	 * @fn		SpawnEntityAsAttachment
	 * @brief	tries to spawn item as attachment
	 * @param[in]	health	\p	health of the item
	 * @param[in]	quantity	\p	quantity to set if item.HasQuantity() (-1 == set to max)
	 * @return	entity if ok, null otherwise
	 **/
	EntityAI SpawnEntityAsAttachment (PlayerBase player, EntityAI parent, string att_name, float health, float quantity)
	{
		if ( GetGame().IsServer() )
		{
			Weapon_Base wpn = Weapon_Base.Cast(parent);
			if (wpn)
			{
				vector pos = player.GetPosition();
				EntityAI eai_gnd = SpawnEntityOnGroundPos(player, att_name, health, quantity, pos);
				Magazine mag_gnd = Magazine.Cast(eai_gnd);
				if (mag_gnd && player.GetWeaponManager().CanAttachMagazine(wpn, mag_gnd))
				{
					player.GetWeaponManager().AttachMagazine(mag_gnd);
				}
				return eai_gnd;
			}
			else
			{
				EntityAI eai = parent.GetInventory().CreateAttachment(att_name);
				if (eai)
				{
					if ( eai.IsInherited(ItemBase) )
					{
						ItemBase i = ItemBase.Cast( eai );
						SetupSpawnedItem(ItemBase.Cast( eai ), health, quantity);
					}
					return eai;
				}
				else
					OnSpawnErrorReport(att_name);
			}
			return NULL;
		}
		else
		{		
			// Client -> Server Spawning: Client Side
			ref Param3<string, float, float> params = new Param3<string, float, float>(att_name, health, quantity);
			player.RPCSingleParam(ERPCs.DEV_RPC_SPAWN_ITEM_AS_ATTACHMENT, params, true);
		}
		return NULL;
	}
	
	EntityAI SpawnFromClipboard()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr == NULL )
		{			
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( player )
			{
				vector pos_player = player.GetPosition();
				
				// Get item from clipboard
				string		item_name;
				GetGame().CopyFromClipboard(item_name);
							
				// Spawn & Synch Item
				EntityAI object_spawned_from_clipboard = SpawnEntityOnCursorDir(player, item_name, 100, -1, 1.0);
				Print(object_spawned_from_clipboard);
				return object_spawned_from_clipboard;
			}
		}
		
		return NULL;
	}
	
	// Clear Player Inventory
	void ClearInventory(PlayerBase player)
	{
		if ( GetGame().IsServer() )
		{
			player.ClearInventory();
		}
		else
		{
			Param1<int> params = new Param1<int>(0);
			player.RPCSingleParam(ERPCs.DEV_RPC_CLEAR_INV, params, true);
		}
	}	
	
	void ToggleHelpScreen()
	{
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_HELP_SCREEN, NULL);
		}
		else if ( g_Game.GetUIManager().IsMenuOpen(MENU_HELP_SCREEN) )
		{
			g_Game.GetUIManager().Back();
		}
	}

	void ToggleScriptConsole()
	{
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_SCRIPTCONSOLE, NULL);
		}
		else if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			g_Game.GetUIManager().Back();
		}
	}
	
	// Mission Editor
	void ToggleMissionLoader()
	{
		g_Game.GetUIManager().OpenWindow( GUI_WINDOW_MISSION_LOADER );
	}
	
	// Script Editor History
	private void ScriptHistoryNext()
	{
		// Console key press
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			CallMethod(CALL_ID_SCR_CNSL_HISTORY_NEXT, NULL);
		}
	}

	private	void ScriptHistoryBack()
	{
		// Console key press
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			CallMethod(CALL_ID_SCR_CNSL_HISTORY_BACK, NULL);
		}
	}
	
	private bool IsIngame()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr == NULL )
		{			
			return true;
		}
		
		return false;
	}

	private bool IsInConsole()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr != NULL && menu_curr.GetID() == MENU_SCRIPTCONSOLE )
		{			
			return true;
		}
		
		return false;
	}
	
	// Tools
	int QuickSortPartition( TStringArray arr, int left, int right )
	{
		string pivot = arr.Get( left );
		int i = left;
		int j = right + 1;
		string temp;

		while ( true )
		{
			while ( true )
			{ 
				i++; 
				if ( i > right || arr.Get(i) > pivot )
				{
					break;
				}
			}
			
			while ( true )
			{
				j--; 
				if ( arr.Get(j) <= pivot )
				{
					break;
				}
			}
			
			if ( i >= j ) 
			{
				break;
			}
			
			temp = arr.Get( i ); 
			arr.Set( i, arr.Get(j) ); 
			arr.Set( j, temp );
		}
		
		temp = arr.Get( left ); 
		arr.Set( left, arr.Get(j) );
		arr.Set( j, temp );
	   
		return j;
	}

	void QuickSort( TStringArray arr, int left, int right )
	{
		int j;

		if ( left < right ) 
		{
			j = QuickSortPartition( arr, left, right );
			QuickSort( arr, left, j - 1 );
			QuickSort( arr, j + 1, right );
		}
	}

	void SortStringArray( TStringArray arr )
	{
		QuickSort( arr, 0, arr.Count() - 1 );
	}
}
