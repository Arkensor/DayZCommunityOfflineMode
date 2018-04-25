class InventoryManager
{
	private static InventoryManager s_instance;
	
	protected PlayerBase m_player;
	// place to hands
	//private ref Timer m_timer;
	//protected ref Link<EntityAI> m_entity_to_take;
	protected ref array<int> m_sorted_keys;
	
	// icon sizes	
	const float MAX_CARGO_GRID_WIDTH = 10;	
	const float MAX_ATTACHMENT_GRID_WIDTH = 7;	
	const float MAX_HANDS_ATTACHMENT_GRID_WIDTH = 7;
	const float GRID_CARGO_GAP = 1;
	const float GRID_ATTACHMENT_GAP = 2;

	protected	float m_normal_item_size;
	protected	float m_hands_normal_item_size;
	protected	float m_attachment_item_size;
	protected	float m_hands_attachment_item_size;
	protected	float m_icon_item_size;
	protected	float m_quantity_stack_panel_size;
	protected float m_panels_size;
	
	// drag & drop 	
	protected bool m_is_dragging;
	protected InventoryItem m_drag_item;
	protected InventoryGrid m_grid;
	//!InventoryDragFromType
	protected int m_from_type;

	// action menu
	protected EntityAI m_am_entity1;
	protected EntityAI m_am_entity2;
	protected int m_am_itemes_combination_flags;
	protected int m_am_pos_x;
	protected int m_am_pos_y;

	// tooltip
	private Widget m_tooltip_widget;
	private ItemPreviewWidget m_item_preview_widget;
	PluginRecipesManager plugin_recipes_manager;
	void InventoryManager()
	{
		s_instance = this;
		plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin(PluginRecipesManager) );
		m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		//m_entity_to_take = new Link<EntityAI>(NULL);
		//m_timer = new Timer();

		// sort order of items for player's inventory
		m_sorted_keys = new array<int>;
		m_sorted_keys.Insert( InventorySlots.SHOULDER );
		m_sorted_keys.Insert( InventorySlots.BOW );
		m_sorted_keys.Insert( InventorySlots.MELEE );
		m_sorted_keys.Insert( InventorySlots.VEST );
		m_sorted_keys.Insert( InventorySlots.BODY );
		m_sorted_keys.Insert( InventorySlots.HIPS );
		m_sorted_keys.Insert( InventorySlots.LEGS );
		m_sorted_keys.Insert( InventorySlots.BACK );
		m_sorted_keys.Insert( InventorySlots.HEADGEAR );
		m_sorted_keys.Insert( InventorySlots.MASK );
		m_sorted_keys.Insert( InventorySlots.EYEWEAR );
		m_sorted_keys.Insert( InventorySlots.GLOVES );
		m_sorted_keys.Insert( InventorySlots.FEET );
		m_sorted_keys.Insert( InventorySlots.ARMBAND );

		// tooltip
		m_tooltip_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_tooltip.layout", NULL );
		m_tooltip_widget.Show( false );
	}

	void ~InventoryManager()
	{
		s_instance = NULL;
		delete m_tooltip_widget;
	}
	
	InventoryView m_grid2;
	InventoryView GetDragGrid()
	{
		return m_grid2;
	}

	void SetDragGrid(InventoryView grid)
	{
		m_grid2 = grid;
	}
	
	// drop to the ground
	void DropItem(InventoryItem item)
	{
		if (item && item.GetHierarchyParent() != NULL && (m_player.GetHumanInventory().GetEntityInHands() != GetItem() || m_player.GetHumanInventory().CanRemoveEntityInHands()))
		{
			bool handled_by_controller = false;
			Magazine mag;
			Weapon_Base wpn;
			if (Class.CastTo(mag, item) && Class.CastTo(wpn, mag.GetHierarchyParent()) )
			{
				if (m_player.GetHumanInventory().GetEntityInHands() == wpn)
				{
					vector m4[4];
					Math3D.MatrixIdentity4(m4);
					GameInventory.PrepareDropEntityPos(m_player, mag, m4);
					InventoryLocation il_mag_next = new InventoryLocation;
					il_mag_next.SetGround(mag, m4);
					m_player.GetWeaponManager().DetachMagazine(il_mag_next);
					handled_by_controller = true;
				}
			}

			if ( !handled_by_controller && m_player.CanDropEntity( item ) )
			{
				m_player.PredictiveDropEntity( item );
			}

			InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
			if ( menu )
			{
				menu.RefreshQuickbar();
			}
		}
	}

	// place to hands
	void TryTakeToHands()
	{
		//EntityAI item_in_hands = m_player.GetHumanInventory().GetEntityInHands();
		
		//if (item_in_hands) return; // wait longer
		
		//if (m_entity_to_take.IsNull() == false)
		//{
		//	if (m_player.GetHumanInventory().CanAddEntityInHands(m_entity_to_take.Ptr()))
		//	{
		//		PrintString("TryTakeToHands tick, retry!");
		//		m_player.TakeEntityToHands(m_entity_to_take.Ptr());
		//		//delete m_entity_to_take;
		//		//m_entity_to_take = new Link<EntityAI>(NULL);
		//	}
		//}
		//m_timer.Stop();
	}
	
	void TakeToHands(InventoryItem item)
	{
		if( m_player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			m_player.PredictiveTakeEntityToHands( item );
		}
	}

	/*void TakeAsOwned(EntityAI entity1, EntityAI entity2)
	{
		//m_timer.Stop();
		if (!entity1 || !entity2) return;
		
		if (entity1.IsInherited( Weapon) && entity2.IsInherited( Magazine))
		{
			TryReloadWeapon(m_player, entity1, entity2);
		}
		else
		{
			entity1.TakeEntityAsOwned(entity2);
		}
	}*/
	
	// drag & drop 	
	void Drag(int type, InventoryItem item, InventoryGrid grid)
	{
		// Print("Drag start: ");
		// Print(item);
		if (item) PrintString( "class InventoryManager >>> Drag() >>> " + item.GetDisplayName() );
		m_is_dragging = true;
		m_drag_item = item;
		m_grid = grid;
		m_from_type = type;
		
		SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
		
		int item_w;
		int item_h;
		GetGame().GetInventoryItemSize(item, item_w, item_h);
		
		SetItemSize((item_w * GetNormalItemSize()) + (GRID_CARGO_GAP * (item_w - 1)), (item_h * GetNormalItemSize()) + (GRID_CARGO_GAP * (item_h - 1)));
	}

	void Drop()
	{
		// Print("Drag stop");
		if (m_grid && m_drag_item) m_grid.ResetItem(m_drag_item);
		
		m_grid = NULL;
		m_drag_item = NULL;
		
		m_is_dragging = false;

		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	void SetItemColor(int color)
	{
		if (m_is_dragging && m_grid && m_drag_item)
		{
			m_grid.SetItemColor(m_drag_item, color);
		}	
	}

	void SetItemSize(int pixels_w, int pixels_h)
	{
		if (m_is_dragging && m_grid && m_drag_item)
		{
			m_grid.SetItemSize(m_drag_item, pixels_w, pixels_h);
		}	
	}

	bool IsDragging() 
	{
		return m_is_dragging;
	}	
	InventoryItem GetItem() 
	{
		return m_drag_item;	
	}
	InventoryGrid GetGrid() 
	{
		return m_grid;		
	}
	
	array<int> GetSortedKeys() 
	{
		return m_sorted_keys;		
	}
	
	/*void InjectValidRecipes(EntityAI entity1, EntityAI entity2,ContextMenu cmenu, bool recipes_anywhere = false)
	{
		PlayerBase p = m_player;
		array<int> recipes_ids = p.m_Recipes;

		plugin_recipes_manager.GetValidRecipes(entity1,entity2,recipes_ids, m_player);

		for(int x = 0;x < recipes_ids.Count();x++)
		{
			int id = recipes_ids.Get(x);
			string name = plugin_recipes_manager.GetRecipeName(id);

			cmenu.Add(name, this, "OnPerformRecipe",new Param1<int>(id));
		}
	}*/

	int GetRecipeCount(bool recipe_anywhere, EntityAI entity1, EntityAI entity2 )
	{
		return plugin_recipes_manager.GetValidRecipes( ItemBase.Cast( entity1 ),ItemBase.Cast( entity2 ),NULL, m_player);
	}
	


	void OnSelectAction(ItemBase item, int actionId)
	{
		m_player.GetActionManager().OnInstantAction(AT_DEBUG,new Param2<ItemBase,int>(item,actionId));
		//item.OnAction(actionId, m_player, NULL);
	}
	
	void OnPerformRecipe(int id)
	{
		if (m_am_entity1 == NULL || m_am_entity2 == NULL) return;

		Debug.Log("OnPerformRecipe called for id:"+id.ToString(),"recipes");
		//m_player.RequestCraftingSetup(id,m_am_entity1,m_am_entity2);
		m_player.GetCraftingManager().SetInventoryCraft(id, ItemBase.Cast( m_am_entity1 ), ItemBase.Cast( m_am_entity2 ) );
		//plugin_recipes_manager.PerformRecipeClientRequest(id,m_am_entity1,m_am_entity2,m_player);
		//PluginRecipesManager plugin = GetPlugin(PluginRecipesManager);
		//plugin.PerformRecipeClientRequest(id,m_am_entity1,m_am_entity2);
		//RequestCrafting
	}	
		
	void InspectItem(InventoryItem item)
	{
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		InspectMenu inspect_menu = InspectMenu.Cast( menu.EnterScriptedMenu(MENU_INSPECT) );
		if (inspect_menu)
		{
			inspect_menu.SetItem(item);
		}
	}

	void ShowActionMenu(InventoryItem item)
	{
		HideTooltip();
		//if (item) PrintString("Showing action menu for " + item.GetName());
		m_am_entity1 = item;
		m_am_entity2 = NULL;
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		
		cmenu.Hide();
		cmenu.Clear();
		
		if (m_am_entity1 == NULL) return;
		
		if (m_player.GetHumanInventory().GetEntityInHands() == item && item.GetInventory().CanRemoveEntity()) cmenu.Add("#inv_context_remove_from_hands", this, "TakeToHands", new Param1<EntityAI>(NULL));
		if (m_player.GetHumanInventory().CanAddEntityInHands(item) && item.GetInventory().CanRemoveEntity()) cmenu.Add("#inv_context_take_to_hands", this, "TakeToHands", new Param1<EntityAI>(item));

		// get actions
		ref TSelectableActionInfoArray customActions = new TSelectableActionInfoArray;
		ItemBase itemBase = ItemBase.Cast( item );
		itemBase.GetRecipesActions(m_player, customActions);

		if ( ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS )
		{	
			itemBase.GetDebugActions(customActions);
		}
		if( ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS )
		{
			m_player.GetDebugActions(customActions);
		}
		
		int actionsCount = customActions.Count();
		for ( int i = 0; i < customActions.Count(); i++ )
		{
			TSelectableActionInfo actionInfo = customActions.Get(i);
			if( actionInfo )
			{
				int actionId = actionInfo.param2;
				string actionText = actionInfo.param3;
				
				cmenu.Add(actionText, this, "OnSelectAction", new Param2<ItemBase, int>(itemBase, actionId));
			}
		}
		
		// if ( !IsInherited( item, ZombieBase ) && !IsInherited( item, Car ) ) 
		// {
			cmenu.Add("#inv_context_inspect", this, "InspectItem", new Param1<InventoryItem>(item));
		// }

		GetMousePos(m_am_pos_x, m_am_pos_y);
		cmenu.Show(m_am_pos_x, m_am_pos_y);
	}

	void ShowActionMenuCombine(EntityAI entity1, EntityAI entity2, int combinationFlags)
	{
		if (entity1 && entity2) PrintString("Showing action menu for " + entity1.GetDisplayName() + " and " + entity2.GetDisplayName());
		m_am_entity1 = entity1;
		m_am_entity2 = entity2;
		
		m_am_itemes_combination_flags = combinationFlags;
		
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		
		cmenu.Hide();
		cmenu.Clear();
		

			/*if(combinationFlags & InventoryCombinationFlags.RECIPE_HANDS)
			{
				GetGame().ProfilerStart("RECIPES_PROFILER");
			
				InjectValidRecipes(entity1,entity2,cmenu,false);
			
				GetGame().ProfilerStop("RECIPES_PROFILER");
			}
			else if( combinationFlags & InventoryCombinationFlags.RECIPE_ANYWHERE )
			{
				InjectValidRecipes(entity1,entity2,cmenu, true);
			}*/

			if(combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY2)
			{
				ItemBase entity = ItemBase.Cast( entity1 );
				entity.CombineItemsClient(ItemBase.Cast( entity2));
				return;
			}
		
		if (m_am_entity1 == NULL || m_am_entity2 == NULL) return;
		
		if (combinationFlags == InventoryCombinationFlags.NONE) return;
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT)
		{
			cmenu.Add("#inv_context_add_as_attachment", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.ADD_AS_ATTACHMENT));		
		}
		if (combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER)
		{
			cmenu.Add("#inv_context_load_chamber", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.LOAD_CHAMBER));		
		}
		if (combinationFlags & InventoryCombinationFlags.ATTACH_MAGAZINE)
		{
			cmenu.Add("#inv_context_attach_magazine", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.ATTACH_MAGAZINE));		
		}
		
		if (combinationFlags & InventoryCombinationFlags.SWAP_MAGAZINE)
		{
			cmenu.Add("#inv_context_swap_magazine", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.SWAP_MAGAZINE));		
		}
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO)
		{
			cmenu.Add("#inv_context_add_as_cargo", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.ADD_AS_CARGO));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.SWAP)
		{
			cmenu.Add("#inv_context_swap", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.SWAP));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS)
		{
			cmenu.Add("#inv_context_take_to_hands", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.TAKE_TO_HANDS));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY)
		{
			cmenu.Add("#inv_context_combine", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.COMBINE_QUANTITY));			
		}
		
		//if (combinationFlags & InventoryCombinationFlags.ACTIONS)
		//{
		//	cmenu.Add("#inv_receiving", NULL, "", NULL);
		//	entity2.GetActions(m_player, entity1);
		//}	
		
		GetMousePos(m_am_pos_x, m_am_pos_y);
		m_am_pos_x -= 5;
		m_am_pos_y -= 5;
		
		cmenu.Show(m_am_pos_x, m_am_pos_y);
	}

	//! Returns combionation of InventoryCombinationFlags
	int GetCombinationFlags(EntityAI entity1, EntityAI entity2)
	{
		int flags = 0;
		EntityAI entity_in_hands = m_player.GetHumanInventory().GetEntityInHands();
		
		if (!entity1 || !entity2) return flags;
		
		Magazine swapmag1;
		Magazine swapmag2;
		bool skipSwap = false;
		
		if( Class.CastTo(swapmag1, entity1) && Class.CastTo(swapmag2, entity2) )
		{
			InventoryLocation invLoc;
			invLoc = new InventoryLocation;
			
			Weapon_Base parentWpn;
			
			swapmag1.GetInventory().GetCurrentInventoryLocation(invLoc);
			if(	Class.CastTo(parentWpn, invLoc.GetParent())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag2) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE; 
			}
			swapmag2.GetInventory().GetCurrentInventoryLocation(invLoc);
			if(	Class.CastTo(parentWpn, invLoc.GetParent())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag1) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE; 
				
			}
		}
		
		
		if ( !skipSwap )
		{
			if (entity1 == m_player.GetHumanInventory().GetEntityInHands()) flags = flags | InventoryCombinationFlags.TAKE_TO_HANDS;
			else if ( GameInventory.CanSwapEntities( entity1, entity2 ) )
			{
				if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( Car ) )
				{
					flags = flags | InventoryCombinationFlags.SWAP;
				}
			}
		}
		
		//if (entity2.HasActions(m_player, entity1)) flags = flags | InventoryCombinationFlags.ACTIONS;
		/*
		if ( entity1.IsInherited( ItemBase) && entity2.IsInherited( ItemBase))
		{
			ItemBase entity1ItemBase = entity1;
			if (entity1ItemBase.CanQuantityCombine(entity2)) flags = flags | InventoryCombinationFlags.COMBINE_QUANTITY;
		}
		*/
		if ( entity1.IsInherited( ItemBase) && entity2.IsInherited( ItemBase))
		{
			ItemBase ent1 = ItemBase.Cast( entity1 );
			if (ent1.CanBeCombined(ItemBase.Cast( entity2 ),m_player)) flags = flags | InventoryCombinationFlags.COMBINE_QUANTITY2;
		}
		
		Weapon_Base wpn;
		Magazine mag;
		if (Class.CastTo(wpn, entity1) && Class.CastTo(mag, entity2))
		{
			int muzzleIndex = wpn.GetCurrentMuzzle();
			if ( m_player.GetWeaponManager().CanLoadBullet(wpn, mag) )
			{
				flags = flags | InventoryCombinationFlags.LOAD_CHAMBER;
			}
			else if ( m_player.GetWeaponManager().CanAttachMagazine(wpn, mag) )
			{
				flags = flags | InventoryCombinationFlags.ATTACH_MAGAZINE;
			}
		}
		else if (entity1.GetInventory().CanAddAttachment(entity2)) 
		{
			if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( Car ) && !entity2.IsInherited( ZombieBase ) && !entity2.IsInherited( Car ) )
			{
				flags = flags | InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			}
		}
		if (entity1.GetInventory().CanAddEntityInCargo(entity2)) flags = flags | InventoryCombinationFlags.ADD_AS_CARGO;
		
		if(entity1 == m_player.GetHumanInventory().GetEntityInHands() || entity2 == m_player.GetHumanInventory().GetEntityInHands())
		{
		//Debug.Log("GetCombinationFlags called, setting comb. flag","recipes");
			if( GetRecipeCount(false, entity1, entity2) > 0 )
			{
				flags = flags | InventoryCombinationFlags.RECIPE_HANDS;
			}
			
			ActionManagerClient amc;
			Class.CastTo(amc, m_player.GetActionManager());
			if( entity1 == m_player.GetHumanInventory().GetEntityInHands() )
			{
				if( amc.GetContinuousActionForTargetItem( ItemBase.Cast( entity2 ) ) > -1 )
				{
					flags = flags | InventoryCombinationFlags.SET_ACTION;					
				}
			}
			else
			{
				if( amc.GetContinuousActionForTargetItem( ItemBase.Cast( entity1 ) ) > -1 )
				{
					flags = flags | InventoryCombinationFlags.SET_ACTION;					
				}				
			}
		}
		
		if( GetRecipeCount(true, entity1, entity2) > 0 )
		{
			flags = flags | InventoryCombinationFlags.RECIPE_ANYWHERE;
		}
		return flags;
	}	
	
	// icon sizes
	float GetNormalItemSize() {
		return m_normal_item_size;
	}
	
	float GetAttachmentItemSize() {
		return m_attachment_item_size;
	}
	float GetIconItemSize() {
		return m_icon_item_size;
	}
	float GetHandsNormalItemSize() {
		return m_hands_normal_item_size;
	}
	float GetHandsAttachmentItemSize() {
		return m_hands_attachment_item_size;
	}
	float GetPanelsSize() {
		return m_panels_size;
	}

	float GetQuantityStackPanelSize() {
		return m_quantity_stack_panel_size;
	}

	void RecalculateIconSize(float maxGridWidth)
	{
		m_normal_item_size = 0.97 * ( maxGridWidth / MAX_CARGO_GRID_WIDTH ); //48 floor((maxGridWidth - ((MAX_CARGO_GRID_WIDTH - 1) * GRID_GAP)) / MAX_CARGO_GRID_WIDTH);
		m_attachment_item_size = 0.97 * ( maxGridWidth / MAX_ATTACHMENT_GRID_WIDTH ); // 46; //46 70 floor(m_normal_item_size * 1.75);
		m_icon_item_size = 70; //floor( m_normal_item_size * 1.85);
		
		m_hands_normal_item_size = 0.97 * ( maxGridWidth / MAX_CARGO_GRID_WIDTH ); //48; //floor((maxGridWidth - ((MAX_CARGO_GRID_WIDTH - 1) * GRID_GAP)) / MAX_CARGO_GRID_WIDTH);
		m_hands_attachment_item_size = 0.97 * ( maxGridWidth / MAX_HANDS_ATTACHMENT_GRID_WIDTH );//80;
		m_quantity_stack_panel_size = m_normal_item_size * 0.5;
	}

	void ShowTooltip( EntityAI item )
	{
		if ( IsDragging() ) return;
		if ( item.IsInherited( InventoryItem) )
		{
			UpdateItemInfo( m_tooltip_widget, item );
			int x, y, screen_w, screen_h;
			float w, h;

			GetMousePos(x,y);
			GetScreenSize(screen_w, screen_h);
			m_tooltip_widget.GetScreenSize(w,h);

			screen_w -= 10;
			screen_h -= 10;
			x += m_normal_item_size;
			y += m_normal_item_size;

			int right_edge = x + w;
			if (right_edge > screen_w) 
			{
				x = screen_w - w;
			}

			int bottom_edge = y + h;
			if (bottom_edge > screen_h)
			{
				y = y - h - (2*m_normal_item_size);
			}

			m_tooltip_widget.SetPos(x, y);
			m_tooltip_widget.Show(true);

			// item preview
			delete m_item_preview_widget;
			
			Widget preview_frame = m_tooltip_widget.FindAnyWidget("ItemFrameWidget");
			if (preview_frame)
			{
				float ww;
				float hh;
				preview_frame.GetSize(ww, hh);
				m_item_preview_widget = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 210, preview_frame) );
				m_item_preview_widget.SetItem(item);
			}
		}
	}
	
	void HideTooltip()
	{
		m_tooltip_widget.Show(false);
		delete m_item_preview_widget;
	}
	
	// singleton 	
	static InventoryManager GetInstance() 
	{ 
		return s_instance; 
	}

	string GetItemQuantityText( EntityAI item )
	{
		string quantity_text = "";
		if ( item.IsInherited( InventoryItem) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			float quantity = item_base.GetQuantity();
			int ammo;
			if ( item.IsInherited( Magazine) )
			{
				Magazine magazine_item = Magazine.Cast( item );
				ammo = magazine_item.GetAmmoCount();
				quantity_text = ammo.ToString();
				return quantity_text;
			}
			else if ( item.IsInherited( ItemBook) )
			{
				return "";
			}
			int max = item.ConfigGetInt("varQuantityMax");
			string unit = item.ConfigGetString("stackedUnit");

			if (max > 0)
			{
				if (max == 1)
				{
					float tmp = Math.Round( ( quantity / max ) * 100 );
					quantity_text = tmp.ToString() + "%";
				}
				else
				{
					quantity_text = quantity.ToString();
					// if (unit == "ml")
					// {
					// 	float liters = round(quantity / 1000.0);
					// 	if ( quantity < 2000 )
					// 	{
					// 		liters =  ( round( (quantity / 100.0) ) ) / 10;
					// 	}
					// 	quantity_text = ftoa(liters) + "l";
					// }
					// else
					// {
					// 	quantity_text = itoa(quantity) + unit;
					// }
				}
			}
		}
		return quantity_text;
	}
	
	int HasItemQuantity( EntityAI item )
	{
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			if ( item.IsInherited( Magazine) )
			{
				return QUANTITY_COUNT;
			}
			else if ( item.IsInherited( ItemBook) )
			{
				return QUANTITY_HIDDEN;
			}
			int max = item.ConfigGetInt("varQuantityMax");
			bool bar = item.ConfigGetBool("quantityBar");
			if ( max > 0 )
			{
				if ( max == 1 || bar )
				{
					return QUANTITY_PROGRESS;
				}
				else
				{
					return QUANTITY_COUNT;
				}
			}
		}
		return QUANTITY_HIDDEN;
	}
	
	float GetItemQuantity( InventoryItem item )
	{
		float quantity = 0;
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			if ( item.IsInherited( Magazine) )
			{
				Magazine magazine_item = Magazine.Cast( item );
				quantity = magazine_item.GetAmmoCount();
			}
			else
			{
				quantity = item_base.GetQuantity();
			}
		}
		return quantity;
	}
	
	void OnPerformCombination(int combinationFlags)
	{
		if (m_am_entity1 == NULL || m_am_entity2 == NULL) return;
		
		if (combinationFlags == InventoryCombinationFlags.NONE) return;

		Weapon_Base wpn;
		Magazine mag;
		if (combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER)
		{
			if ( Class.CastTo(wpn,  m_am_entity1 ) && Class.CastTo(mag,  m_am_entity2 ) )
			{
				if( m_player.GetWeaponManager().CanLoadBullet(wpn, mag) )
				{
					m_player.GetWeaponManager().LoadBullet(mag);
					return;					
				}
			}
		}
		if (combinationFlags & InventoryCombinationFlags.ATTACH_MAGAZINE)
		{
			if ( Class.CastTo(wpn,  m_am_entity1 ) && Class.CastTo(mag,  m_am_entity2 ) )
			{
				if( m_player.GetWeaponManager().CanAttachMagazine(wpn, mag) )
				{
					m_player.GetWeaponManager().AttachMagazine(mag);	
				}
			}
			return;
		}
		if (combinationFlags & InventoryCombinationFlags.SWAP_MAGAZINE)
		{
			Magazine mag2;
			
			if ( Class.CastTo(mag,  m_am_entity1 ) && Class.CastTo(mag2,  m_am_entity2 ) )
			{
				if( Class.CastTo(wpn, mag.GetHierarchyParent()) )
				{
					m_player.GetWeaponManager().SwapMagazine(mag2);	
				}
				else
				{
					m_player.GetWeaponManager().SwapMagazine(mag);	
				}
			}
			return;
		}
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT)
		{
			m_player.PredictiveTakeEntityToTargetAttachment(m_am_entity1, m_am_entity2);
		}
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO)
		{
			m_player.PredictiveTakeEntityToTargetCargo(m_am_entity1, m_am_entity2);
		}
		
		if (combinationFlags & InventoryCombinationFlags.SWAP)
		{
			m_player.PredictiveSwapEntities(m_am_entity1, m_am_entity2);
			GetDragGrid().GetContainer().HideExpandView(m_am_entity2);			
		}
		
		if (combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS)
		{
			TakeToHands( InventoryItem.Cast( m_am_entity2 ));
		}
		/*
		if (m_am_entity1.IsInherited(ItemBase) && m_am_entity2.IsInherited(ItemBase))
		{
			ItemBase entity1ItemBase = m_am_entity1;
			if (combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY)
			{
				entity1ItemBase.QuantityCombine(m_am_entity2);
			}
		}*/
	}
}
