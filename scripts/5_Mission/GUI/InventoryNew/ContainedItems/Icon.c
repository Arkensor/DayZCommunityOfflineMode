class Icon: ContainerBase
{
	int m_sizeX, m_sizeY, m_posX, m_posY;
	protected Widget m_WhiteBackground ;
	protected EntityAI m_Obj;
	ref array<Widget> m_WhiteBackgrounds;
	ref array<ref Pos> m_Pos;
	protected bool m_HandsIcon;
	protected int m_CargoPos;
	protected bool m_IsDragged;
	protected EntityAI m_am_entity1, m_am_entity2;
	protected ref array<ref Timer> m_Timers;

	const int NUMBER_OF_TIMERS = 2;

	void Icon( ContainerBase parent , bool hands_icon = false )
	{
		m_HandsIcon = hands_icon;
		m_Timers = new ref array<ref Timer>;
		for ( int i = 0; i < NUMBER_OF_TIMERS; i++ )
		{
			m_Timers.Insert( new Timer() );
		}
		m_WhiteBackgrounds = new array<Widget>;
		m_Pos = new array<ref Pos>;
		ItemManager.GetInstance().SetSelectedItem( NULL, NULL );
		
		SetActive( false );
	}

	void ~Icon()
	{
		delete GetMainPanel();
	}

	bool IsDragged()
	{
		return m_IsDragged;
	}

	override void SetActive( bool active )
	{
		GetMainPanel().FindAnyWidget("Selected").Show( active );
	}

	void RefreshQuickbar()
	{
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		ItemManager.GetInstance().HideTooltip();
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	void DoubleClick(Widget w, int x, int y, int button)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( player.GetInventory().HasInventoryReservation( m_Obj, NULL ) || player.GetInventory().IsInventoryLocked() )
		{
			return;
		}
		
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !iw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !iw )
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		{
			return;
		}

		EntityAI item = iw.GetItem();

		InventoryManager manager = InventoryManager.GetInstance();

		if( m_HandsIcon )
		{
			if( player.GetHumanInventory().CanRemoveEntityInHands() )
			{
				player.PredictiveMoveItemFromHandsToInventory();
			}
			ItemManager.GetInstance().HideTooltip();
			if ( menu )
			{
				menu.RefreshQuickbar();
			}
			return;
		}

		InventoryLocation i1 = new InventoryLocation;
		EntityAI hands_item = player.GetHumanInventory().GetEntityInHands();
		EntityAI item_root_owner = item.GetHierarchyRoot();
		
		
		if ( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else if( hands_item && item_root_owner == GetGame().GetPlayer() )
		{
			/*if( hands_item.GetInventory().CanAddAttachment( item ) )
			{
				CombineItems( hands_item, item ); //INVTODO Check if wanted.
			}
			else */if( player.GetInventory().CanSwapEntities( item, hands_item ) )
			{
				player.PredictiveTakeEntityToHands( item );
			}
			else if( player.GetInventory().CanForceSwapEntities( item, hands_item, i1 ) )
			{
				player.PredictiveSwapEntities( item, hands_item );
			}
		}
		else
		{
			bool found = false;
			if( item.GetInventory().CanRemoveEntity())
			{
				InventoryLocation i2 = new InventoryLocation;
				found = player.GetInventory().FindFreeLocationFor( item,FindInventoryLocationType.ANY, i2 );
				if ( found )
				{
					if ( i2.GetType() == FindInventoryLocationType.ATTACHMENT )
					{
						if ( i2.GetParent() != player )
							found = false;
					}
				}
			}
			if ( found )
			{
				player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, item);
			}
			else
			{
				if( player.GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}

		ItemManager.GetInstance().HideTooltip();
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	void DraggingOverSwap( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );

		ItemPreviewWidget receiver_ipw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( "Render" ) );
		if( m_HandsIcon )
		{
			receiver_ipw = ItemPreviewWidget.Cast( receiver.GetParent().FindAnyWidget( "Render" ) );
		}

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		InventoryItem receiver_entity = InventoryItem.Cast( receiver_ipw.GetItem() );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		if( GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			if( m_HandsIcon )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
				ItemManager.GetInstance().ShowSourceDropzone( w_entity );
			}
		}
	}

	void DraggingOverCombine( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !iw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !iw )
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		{
			return;
		}

		int flags = GetCombinationFlags( GetGame().GetPlayer().GetHumanInventory().GetEntityInHands(), iw.GetItem() );
		ShowActionMenuCombine( GetGame().GetPlayer().GetHumanInventory().GetEntityInHands(), iw.GetItem(), flags , w , true);
	}

	bool MouseEnter(Widget w, int x, int y)
	{
		ItemManager.GetInstance().PrepareTooltip( EntityAI.Cast( m_Obj ) );
		return true;
	}

	bool MouseLeave( Widget w, Widget s, int x, int y	)
	{
		ItemManager.GetInstance().HideTooltip();
		return true;
	}

	void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		ItemManager.GetInstance().HideDropzones();
		EntityAI entity = EntityAI.Cast( m_Obj );

		if( w == NULL )
		{
			return;
		}
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );

		ItemPreviewWidget receiver_ipw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( "Render" ) );
		if( m_HandsIcon )
		{
			receiver_ipw = ItemPreviewWidget.Cast( receiver.GetParent().FindAnyWidget( "Render" ) );
		}

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		InventoryItem receiver_entity = InventoryItem.Cast( receiver_ipw.GetItem() );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		if( ( ItemBase.Cast( receiver_entity ) ).CanBeCombined( ItemBase.Cast( w_entity ) ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			if( entity.GetHierarchyParent().GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else if( !m_HandsIcon )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
		}
		else if( GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			if( entity.GetHierarchyParent().GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else if( !m_HandsIcon )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( w_entity );
		}
	}

	int GetCombinationFlags( EntityAI entity1, EntityAI entity2 )
	{
		int flags = 0;
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI entity_in_hands = m_player.GetHumanInventory().GetEntityInHands();

		if ( !entity1 || !entity2 ) return flags;

		//Magazine swapmag1;
		//Magazine swapmag2;
		//bool skipSwap = false;
/*
		if( Class.CastTo(swapmag1, entity1) && Class.CastTo(swapmag2, entity2) )
		{
			Weapon_Base parentWpn;

			if(	Class.CastTo(parentWpn, swapmag1.GetOwner())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag2) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE;
			}

			if(	Class.CastTo(parentWpn, swapmag2.GetOwner())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag1) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE;

			}
		}
*/
		if ( entity1.IsInherited( ItemBase ) && entity2.IsInherited( ItemBase ) )
		{
			ItemBase ent1 = ItemBase.Cast( entity1 );
			if ( ent1.CanBeCombined( ItemBase.Cast( entity2 ) ) ) flags = flags | InventoryCombinationFlags.COMBINE_QUANTITY2;
		}

		Weapon_Base wpn;
		Magazine mag;
		if ( Class.CastTo(wpn,  entity1 ) && Class.CastTo(mag,  entity2 ) )
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
			else if ( m_player.GetWeaponManager().CanSwapMagazine(wpn, mag))
			{
				flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE;
			}
		}
		else if ( entity1.GetInventory().CanAddAttachment( entity2 ) )
		{
			if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( Car ) && !entity2.IsInherited( ZombieBase ) && !entity2.IsInherited( Car ) )
			{
				flags = flags | InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			}
		}
		if ( entity1.GetInventory().CanAddEntityInCargo( entity2 ) ) flags = flags | InventoryCombinationFlags.ADD_AS_CARGO;

		if( entity1 == m_player.GetHumanInventory().GetEntityInHands() || entity2 == m_player.GetHumanInventory().GetEntityInHands())
		{
		//Debug.Log("GetCombinationFlags called, setting comb. flag","recipes");
			if( GetRecipeCount( false, entity1, entity2 ) > 0 )
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

		if( GetRecipeCount( true, entity1, entity2 ) > 0 )
		{
			flags = flags | InventoryCombinationFlags.RECIPE_ANYWHERE;
		}
		return flags;
	}

	int GetRecipeCount( bool recipe_anywhere, EntityAI entity1, EntityAI entity2 )
	{
		PluginRecipesManager plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin( PluginRecipesManager ) );
		return plugin_recipes_manager.GetValidRecipes( ItemBase.Cast( entity1 ), ItemBase.Cast( entity2 ), NULL, PlayerBase.Cast( GetGame().GetPlayer() ) );
	}

	/*void InjectValidRecipes( out int out_id , EntityAI entity1, EntityAI entity2,ContextMenu cmenu, bool recipes_anywhere = false)
	{
		PlayerBase p = GetGame().GetPlayer();
		array<int> recipes_ids = p.m_Recipes;
		PluginRecipesManager plugin_recipes_manager = GetPlugin( PluginRecipesManager );
		plugin_recipes_manager.GetValidRecipes( entity1, entity2, recipes_ids, p );

		for( int x = 0;x < recipes_ids.Count();x++ )
		{
			int id = recipes_ids.Get( x );
			string name = plugin_recipes_manager.GetRecipeName( id );

			cmenu.Add( name, this, "OnPerformRecipe",new Param1<int>( id ) );
			out_id = id;
		}
	}*/

	void RefreshDraggedQuant()
	{
		Icon dragged_icon = ItemManager.GetInstance().GetDraggedIcon();
		if( dragged_icon )
		{
			dragged_icon.SetQuantity( dragged_icon.GetMainPanel() );
		}
		m_Timers[1].Stop();
	}

	void OnPerformCombination( int combinationFlags )
	{
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( m_am_entity1 == NULL || m_am_entity2 == NULL ) return;

		if ( combinationFlags == InventoryCombinationFlags.NONE ) return;

		Weapon_Base wpn;
		Magazine mag;

		if ( combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER )
		{

			if ( Class.CastTo(wpn,  m_am_entity1 ) && Class.CastTo(mag,  m_am_entity2 ) )
			{
				if( m_player.GetWeaponManager().CanLoadBullet(wpn, mag) )
				{
					m_player.GetWeaponManager().LoadBullet(mag);
					m_Timers[1].Run(1.5, this, "RefreshDraggedQuant",  NULL, true);
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
			if ( Class.CastTo(wpn,  m_am_entity1 ) && Class.CastTo(mag,  m_am_entity2 ) )
			{
				m_player.GetWeaponManager().SwapMagazine(mag);
			}
			return;
		}
		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT )
		{
			m_player.PredictiveTakeEntityToTargetAttachment(m_am_entity1, m_am_entity2);
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO )
		{
			m_player.PredictiveTakeEntityToTargetCargo(m_am_entity1, m_am_entity2);
		}

		if ( combinationFlags & InventoryCombinationFlags.SWAP )
		{
			if( !m_player.PredictiveSwapEntities( m_am_entity1, m_am_entity2 ) && m_player.GetHumanInventory().CanAddEntityInHands( m_am_entity2 ) )
			{
				m_player.PredictiveTakeEntityToHands( m_am_entity2 );
			}
//			GetDragGrid().GetContainer().HideExpandView(m_am_entity2);
		}

		if ( combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS )
		{
			if( m_player.GetHumanInventory().CanAddEntityInHands( m_am_entity2 ) )
			{
				m_player.PredictiveTakeEntityToHands( m_am_entity2 );
			}
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
		if ( combinationFlags & InventoryCombinationFlags.SET_ACTION )
		{

			ActionManagerClient amc;
			Class.CastTo(amc, m_player.GetActionManager());
			int ActionID;

			if( m_am_entity1 == m_player.GetHumanInventory().GetEntityInHands() )
			{
				ActionID = amc.GetContinuousActionForTargetItem( ItemBase.Cast( m_am_entity2 ) );
				m_player.GetInventoryActionHandler().SetAction(InventoryActionHandler.IAH_CONTINUOUS,ActionID,ItemBase.Cast( m_am_entity2 ) );
			}
			else
			{
				ActionID = amc.GetContinuousActionForTargetItem( ItemBase.Cast( m_am_entity1 ) );
				m_player.GetInventoryActionHandler().SetAction(InventoryActionHandler.IAH_CONTINUOUS,ActionID,ItemBase.Cast( m_am_entity1 ) );
			}
		}
	}

	void ShowActionMenu(InventoryItem item)
	{
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemManager.GetInstance().HideTooltip();
		m_am_entity1 = item;
		m_am_entity2 = NULL;
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();

		cmenu.Hide();
		cmenu.Clear();

		if (m_am_entity1 == NULL) return;

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

		int m_am_pos_x,  m_am_pos_y;
		GetMousePos( m_am_pos_x, m_am_pos_y );
				m_am_pos_x -= 5;
		m_am_pos_y -= 5;
		cmenu.Show(m_am_pos_x, m_am_pos_y);
	}

	void OnSelectAction(ItemBase item, int actionId)
	{
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		m_player.GetActionManager().OnInstantAction(AT_DEBUG,new Param2<ItemBase,int>(item,actionId));
		//item.OnAction(actionId, m_player, NULL);
	}

	void ShowActionMenuCombine( EntityAI entity1, EntityAI entity2, int combinationFlags, Widget w , bool color_test )
	{
		int current_flag;
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		m_am_entity1 = entity1;
		m_am_entity2 = entity2;
		cmenu.Hide();
		cmenu.Clear();
		int id = -1;

		if( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY2 )
		{
			if( color_test )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				ItemBase entity = ItemBase.Cast( entity1 );
				entity.CombineItemsClient( ItemBase.Cast( entity2 ) );
				m_Timers[1].Run(0.1, this, "RefreshDraggedQuant",  NULL, true);
			}
			return;
		}

		if ( entity1 == NULL || entity2 == NULL ) return;

		if ( combinationFlags == InventoryCombinationFlags.NONE )
		{
			if( color_test )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
				ItemManager.GetInstance().ShowSourceDropzone( entity2 );
			}
			return;
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT )
		{
			current_flag = InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			cmenu.Add( "#inv_context_add_as_attachment", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}
		if ( combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER )
		{
			current_flag = InventoryCombinationFlags.LOAD_CHAMBER;
			cmenu.Add( "#inv_context_load_chamber", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}
		if (combinationFlags & InventoryCombinationFlags.ATTACH_MAGAZINE)
		{
			current_flag = InventoryCombinationFlags.ATTACH_MAGAZINE;
			cmenu.Add("#inv_context_attach_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if (combinationFlags & InventoryCombinationFlags.SWAP_MAGAZINE)
		{
			current_flag = InventoryCombinationFlags.SWAP_MAGAZINE;
			cmenu.Add("#inv_context_swap_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO )
		{
			current_flag = InventoryCombinationFlags.ADD_AS_CARGO;
			cmenu.Add( "#inv_context_add_as_cargo", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.SWAP )
		{
			current_flag = InventoryCombinationFlags.SWAP;
			cmenu.Add( "#inv_context_swap", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY )
		{
			current_flag = InventoryCombinationFlags.COMBINE_QUANTITY;
			cmenu.Add( "#inv_context_combine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if (combinationFlags & InventoryCombinationFlags.SET_ACTION)
		{
			current_flag = InventoryCombinationFlags.SET_ACTION;
			cmenu.Add("#inv_context_attach_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		int m_am_pos_x,  m_am_pos_y;
		GetMousePos( m_am_pos_x, m_am_pos_y );
		m_am_pos_x -= 5;
		m_am_pos_y -= 5;

		if( color_test )
		{
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
		}
		if( combinationFlags & InventoryCombinationFlags.RECIPE_HANDS ||  combinationFlags & InventoryCombinationFlags.RECIPE_ANYWHERE )
		{
			if( !color_test )
			{
				OnPerformRecipe( id );
			}
		}
		else if( cmenu.Count() == 1 )
		{
			if( !color_test )
			{
				OnPerformCombination( current_flag );
			}
		}
		else
		{
			if( color_test )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
				ItemManager.GetInstance().ShowSourceDropzone( entity2 );
			}
			else
			{
				cmenu.Show( m_am_pos_x, m_am_pos_y );
			}
		}
	}

	void OnPerformRecipe(int id)
	{
		if ( m_am_entity1 == NULL || m_am_entity2 == NULL ) return;

		Debug.Log("OnPerformRecipe called for id:"+id.ToString(),"recipes");
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		player.GetCraftingManager().SetInventoryCraft( id, ItemBase.Cast( m_am_entity1 ), ItemBase.Cast( m_am_entity2 ) );
	}

	void SelectCombine()
	{
		Combine( ItemManager.GetInstance().GetSelectedWidget().GetParent(), 0, 0, GetMainPanel().FindAnyWidget( "Combine" ) );
	}

	void SelectSwap()
	{
		Widget w = ItemManager.GetInstance().GetSelectedWidget().GetParent();
		string name = w.GetName();
		if( name != "Icon" )
		{
			name.Replace( "Icon", "PanelWidget" );
			w = w.FindAnyWidget( name );
		}
		Swap( w, 0, 0, GetMainPanel().FindAnyWidget( "Swap" ) );
		ItemManager.GetInstance().SetSelectedItem( NULL, NULL );
	}

	void Combine( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !iw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !iw )
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		{
			return;
		}

		int flags = GetCombinationFlags( GetGame().GetPlayer().GetHumanInventory().GetEntityInHands(), iw.GetItem() );
		ShowActionMenuCombine( GetGame().GetPlayer().GetHumanInventory().GetEntityInHands(), iw.GetItem(), flags , w, false );
	}
	
	void CombineItems( EntityAI entity1, EntityAI entity2 )
	{
		int flags = GetCombinationFlags( entity1, entity2 );
		FlagAction( entity1, entity2, flags );
	}
	
	void FlagAction( EntityAI entity1, EntityAI entity2, int combinationFlags )
	{
		int current_flag;
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		m_am_entity1 = entity1;
		m_am_entity2 = entity2;
		cmenu.Hide();
		cmenu.Clear();
		int id = -1;

		if( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY2 )
		{
				ItemBase entity = ItemBase.Cast( entity1 );
				entity.CombineItemsClient( ItemBase.Cast( entity2 ) );
				m_Timers[1].Run(0.1, this, "RefreshDraggedQuant",  NULL, true);
			return;
		}

		if ( entity1 == NULL || entity2 == NULL ) return;

		if ( combinationFlags == InventoryCombinationFlags.NONE )
		{
			return;
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT )
		{
			current_flag = InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			cmenu.Add( "#inv_context_add_as_attachment", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}
		if ( combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER )
		{
			current_flag = InventoryCombinationFlags.LOAD_CHAMBER;
			cmenu.Add( "#inv_context_load_chamber", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}
		if (combinationFlags & InventoryCombinationFlags.ATTACH_MAGAZINE)
		{
			current_flag = InventoryCombinationFlags.ATTACH_MAGAZINE;
			cmenu.Add("#inv_context_attach_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if (combinationFlags & InventoryCombinationFlags.SWAP_MAGAZINE)
		{
			current_flag = InventoryCombinationFlags.SWAP_MAGAZINE;
			cmenu.Add("#inv_context_swap_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO )
		{
			current_flag = InventoryCombinationFlags.ADD_AS_CARGO;
			cmenu.Add( "#inv_context_add_as_cargo", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.SWAP )
		{
			current_flag = InventoryCombinationFlags.SWAP;
			cmenu.Add( "#inv_context_swap", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY )
		{
			current_flag = InventoryCombinationFlags.COMBINE_QUANTITY;
			cmenu.Add( "#inv_context_combine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		if (combinationFlags & InventoryCombinationFlags.SET_ACTION)
		{
			current_flag = InventoryCombinationFlags.SET_ACTION;
			cmenu.Add("#inv_context_attach_magazine", this, "OnPerformCombination", new Param1<int>( current_flag ) );
		}

		int m_am_pos_x,  m_am_pos_y;
		GetMousePos( m_am_pos_x, m_am_pos_y );
		m_am_pos_x -= 5;
		m_am_pos_y -= 5;

		if( combinationFlags & InventoryCombinationFlags.RECIPE_HANDS ||  combinationFlags & InventoryCombinationFlags.RECIPE_ANYWHERE )
		{
				OnPerformRecipe( id );
		}
		else if( cmenu.Count() == 1 )
		{
				OnPerformCombination( current_flag );
		}
		else
		{
				cmenu.Show( m_am_pos_x, m_am_pos_y );
		}
	}

	void MouseClick( Widget w, int x, int y, int button )
	{
		InventoryItem itemAtPos = InventoryItem.Cast( m_Obj );
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		bool draggable = !player.GetInventory().HasInventoryReservation( m_Obj, NULL ) && !player.GetInventory().IsInventoryLocked() ;
		
		ItemManager.GetInstance().SetWidgetDraggable( w, draggable );
		
		if ( button == MouseState.RIGHT )
		{
			if ( itemAtPos && itemAtPos.IsItemBase() )
			{
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( GetMainPanel().FindAnyWidget( "Render" ) );
				itemAtPos.OnRightClick();
				SetQuantity( item_preview.GetParent() );
				if( GetDayZGame().IsLeftCtrlDown() ) ShowActionMenu(itemAtPos);
			}
		}
		else if ( button == MouseState.MIDDLE )
		{
			InspectItem( itemAtPos );
		}
	}

	void DropReceivedFromMain( Widget w, int x, int y, Widget receiver )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		ItemPreviewWidget receiver_ipw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( "Render" ) );
		if( m_HandsIcon )
		{
			receiver_ipw = ItemPreviewWidget.Cast( receiver.GetParent().FindAnyWidget( "Render" ) );
		}

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		InventoryItem receiver_entity = InventoryItem.Cast( receiver_ipw.GetItem() );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		if ( ( ItemBase.Cast( receiver_entity ) ).CanBeCombined( ItemBase.Cast( w_entity ) ) )
		{
			( ItemBase.Cast( receiver_entity ) ).CombineItemsClient( ItemBase.Cast( w_entity ) );
			SetQuantity( receiver_ipw.GetParent() );
			//m_Timers[0].Run( 0.1, this, "RefreshQuantity",  NULL, true );
			//m_Timers[1].Run(0.1, this, "RefreshDraggedQuant",  NULL, true);
			Refresh();
		}
		else if( GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			Magazine mag = Magazine.Cast(w_entity);
			if( mag )
			{
				Weapon_Base wpn = Weapon_Base.Cast(mag.GetHierarchyParent());
				if( wpn )
				{
					if( player.GetWeaponManager().CanSwapMagazine( wpn,  Magazine.Cast(receiver_entity) ) )
						player.GetWeaponManager().SwapMagazine( Magazine.Cast(receiver_entity) );
					return;
				}
			}
			
			player.PredictiveSwapEntities( w_entity, receiver_entity );

			Icon icon = ItemManager.GetInstance().GetDraggedIcon();
			if( m_Parent.IsInherited( ItemsContainer ) )
			{
				m_Timers[0].Run(0.1, this, "ToRefresh",  new Param2<Icon, Icon>(this, icon), true);
			}
		}
		else
		{
			if( m_HandsIcon && player.GetHumanInventory().CanAddEntityInHands( w_entity ) )
			{
				player.PredictiveTakeEntityToHands( w_entity );
			}
		}
	}

	void Swap( Widget w, int x, int y, Widget receiver )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );

		ItemPreviewWidget receiver_ipw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( "Render" ) );
		if( m_HandsIcon )
		{
			receiver_ipw = ItemPreviewWidget.Cast( receiver.GetParent().FindAnyWidget( "Render" ) );
		}

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		InventoryItem receiver_entity = InventoryItem.Cast( receiver_ipw.GetItem() );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		Weapon_Base wpn;
		Magazine mag;

		if ( Class.CastTo(wpn,  item_in_hands ) && Class.CastTo(mag,  w_entity ) && Class.CastTo(wpn,  mag.GetHierarchyParent() ) )
		{
			return;
		}

		if( GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			if( !player.GetInventory().HasInventoryReservation( item_in_hands, NULL ) )
			{
				GetGame().GetPlayer().PredictiveSwapEntities( w_entity, receiver_entity );
				
				Icon icon = ItemManager.GetInstance().GetDraggedIcon();
				if( m_Parent.IsInherited( ItemsContainer ) )
				{
					m_Timers[0].Run(0.1, this, "ToRefresh",  new Param2<Icon, Icon>(this, icon), true);
				}
			}
		}
		else
		{
			if( m_HandsIcon && !player.GetInventory().HasInventoryReservation( item_in_hands, NULL ) )
			{
				GetGame().GetPlayer().PredictiveSwapEntities( w_entity, item_in_hands );
			}
		}
	}

	void RefreshQuantity( )
	{
		SetQuantity( GetMainPanel() );
		m_Timers[0].Stop();
	}

	void ToRefresh( Icon icon, Icon icon2 )
	{
		( ItemsContainer.Cast( m_Parent ) ).RemoveItem( icon );
		( ItemsContainer.Cast( m_Parent ) ).RemoveItem( icon2 );
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.Icon;
	}

	void SetSize( int x, int y )
	{
		m_sizeX = x;
		m_sizeY = y;
	}

	int GetCargoPos()
	{
		return m_CargoPos;
	}

	void SetCargoPos( int x )
	{
		m_CargoPos = x;
	}

	void DestroyWhiteBackground()
	{
		ItemManager.GetInstance().HideDropzones();

		ItemManager.GetInstance().SetIsDragging( false );
		m_IsDragged = false;
		GetMainPanel().FindAnyWidget("Color").SetColor( ColorManager.BASE_COLOR );
		
		if( m_HandsIcon )
		{
			Refresh();
			FullScreen();
			return;
		}

		for ( int i = 0; i < m_WhiteBackgrounds.Count(); i++ )
		{
			delete m_WhiteBackgrounds.Get( i );
		}

		m_WhiteBackgrounds.Clear();
		m_Pos.Clear();

		delete m_WhiteBackground;

		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	void CreateWhiteBackground()
	{
		ItemManager.GetInstance().SetDraggedIcon(this);
		m_IsDragged = true;
		ItemManager.GetInstance().SetIsDragging( true );
		int ww, hh;
		GetGame().GetInventoryItemSize( InventoryItem.Cast( GetObject() ), ww, hh );
		SetSize( ww, hh );
		Refresh();

		if(m_HandsIcon)
		{
			return;
		}

		Widget w = m_Parent.GetMainPanel();
		int row = m_posX;
		int column = m_posY;
		float x, y, icon_x, icon_y;
		Container parent = Container.Cast( m_Parent );
		w.FindAnyWidget( "Icon0" ).GetChildren().GetScreenSize( icon_x, icon_y );
		parent.Get( 0 ).GetMainPanel().GetPos( x, y );

		for ( int i = 0; i < m_sizeX; i++ )
		{
			for ( int j = 0; j < m_sizeY; j++ )
			{
				Widget m_WhiteBackground2 = GetGame().GetWorkspace().CreateWidgets( m_LayoutName );
				m_WhiteBackground2.FindAnyWidget( "Color" ).SetColor( ColorManager.ITEM_BACKGROUND_COLOR );
				m_WhiteBackground2.SetPos( ( row + i ) * icon_x + ( row + i ), y + ( column + j ) * icon_y + ( column + j ) );
				if( i == ( m_sizeX - 1 ) )
				{
					if( j == ( m_sizeY - 1 ) )
					{
						m_WhiteBackground2.SetSize( icon_x, icon_y );
					}
					else
					{
						m_WhiteBackground2.SetSize( icon_x, icon_y + 1 );
					}
				}
				else
				{
					if( j == ( m_sizeY - 1 ) )
					{
						if( i == ( m_sizeX - 1 ) )
						{
							m_WhiteBackground2.SetSize( icon_x, icon_y );
						}
						else
						{
							m_WhiteBackground2.SetSize( icon_x + 1, icon_y );
						}

					}
					else
					{
						m_WhiteBackground2.SetSize( icon_x + 1, icon_y + 1 );
					}
				}

				w.AddChild( m_WhiteBackground2 );
				Pos l = new Pos( i, j );
				m_WhiteBackground2.SetUserData( l );


				m_WhiteBackgrounds.Insert( m_WhiteBackground2 );
				m_Pos.Insert( l );
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_WhiteBackground2,  this, "DropReceivedFromBackground" );
				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_WhiteBackground2,  this, "OnDraggingOverBackground" );

			}
		}
	}

	void OnDraggingOverBackground( Widget w, int x, int y, Widget reciever  )
	{
		ItemManager.GetInstance().HideDropzones();
		EntityAI entity = EntityAI.Cast( m_Obj );
		EntityAI owner = entity.GetHierarchyParent();
		if( owner && owner.GetHierarchyParent() == GetGame().GetPlayer() )
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else if( !m_HandsIcon )
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}

		if( w == NULL || reciever == NULL )
		{
			return;
		}
		Pos pa;
		reciever.GetUserData( pa );
		if( m_Parent )
		{
			ItemWithCargo item = ItemWithCargo.Cast( m_Parent.m_Parent );
			if( item )
			{
				item.DraggingOverGrid( w, m_posY + pa.y, m_posX + pa.x, reciever );
			}
			HandsContainer hands_item = HandsContainer.Cast( m_Parent.m_Parent );
			if( hands_item )
			{
				hands_item.DraggingOverGrid( w, m_posY + pa.y, m_posX + pa.x, reciever );
			}
		}
	}

	void DropReceivedFromBackground( Widget w, int x, int y, Widget reciever  )
	{
		Pos pa;
		if( reciever )
		{
			reciever.GetUserData( pa );
			if( m_Parent )
			{
				ItemWithCargo item = ItemWithCargo.Cast( m_Parent.m_Parent );
				if( item )
				{
					item.DropReceived(w, m_posY + pa.y, m_posX + pa.x );
				}
			}
		}
	}

	override void Refresh()
	{
		super.Refresh();

		SetPos( m_posX, m_posY );
		SetSize( m_sizeX, m_sizeY );
		if( !ItemManager.GetInstance().IsDragging() )
		{
			SetTemperature();
		}
		Weapon_Base wpn = Weapon_Base.Cast( GetObject() );
		if ( wpn )
		{
			int mi = wpn.GetCurrentMuzzle();
			GetMainPanel().FindAnyWidget( "AmmoIcon" ).Show( wpn.IsChamberFull( mi ) );
		}
		SetQuantity( GetMainPanel() );
	}

	void SetTemperature()
	{
		ItemManager.GetInstance().SetIconTemperature( EntityAI.Cast( m_Obj ), GetMainPanel().FindAnyWidget( "Render" ).GetParent() );
	}

	void RefreshIconPos()
	{
		Refresh();
		GetMainPanel().Update();
		m_Timers[0].Stop();
	}

	void FullScreen()
	{
		if( m_IsDragged )
		{
			return;
		}
		GetMainPanel().ClearFlags( WidgetFlags.HEXACTSIZE + WidgetFlags.VEXACTSIZE );
		GetMainPanel().SetSize( 1, 1 );
		GetMainPanel().SetFlags( WidgetFlags.HEXACTSIZE + WidgetFlags.VEXACTSIZE );
		GetMainPanel().FindAnyWidget( "Color" ).SetColor( ARGB( 0, 0, 0, 0 ) );
	}

	void RefreshPos( int row, int column )
	{
		if( row != m_posY || column != m_posX )
		{
			m_posX = column;
			m_posY = row;
			SetPos( m_posX, m_posY );
			m_Timers[0].Run( 0, this, "RefreshIconPos",  NULL, true ); // call next frame, because imeediate function call didn't work
		}
	}

	Object GetObject()
	{
		return m_Obj;
	}

	void SetQuantity( Widget item_w )
	{
		if( m_Obj == NULL )
		{
			return;
		}
		EntityAI item = EntityAI.Cast( m_Obj );
		int has_quantity = QuantityConversions.HasItemQuantity( EntityAI.Cast( m_Obj ) );
		Widget quantity_panel = item_w.FindAnyWidget( "QuantityPanel" );
		TextWidget item_quantity = TextWidget.Cast( item_w.FindAnyWidget( "Quantity" ) );
		ProgressBarWidget quantity_progress = ProgressBarWidget.Cast( item_w.FindAnyWidget( "QuantityBar" ) );
		Widget quantity_stack = item_w.FindAnyWidget( "QuantityStackPanel" );
		if ( has_quantity == QUANTITY_HIDDEN )
		{
			quantity_panel.Show( false );
		}
		else
		{
			quantity_panel.Show( true );
			if ( has_quantity == QUANTITY_COUNT )
			{
				item_quantity.SetText( QuantityConversions.GetItemQuantityText( item ) );
				quantity_stack.Show( true );
				quantity_progress.Show( false );
			}
			else if ( has_quantity == QUANTITY_PROGRESS )
			{

				float progress_max = quantity_progress.GetMax();
				int max = item.ConfigGetInt( "varQuantityMax" );
				int count = item.ConfigGetInt( "count" );
					float quantity = QuantityConversions.GetItemQuantity( InventoryItem.Cast( item ) );

				if ( count > 0 )
				{
					max = count;
				}
				if ( max > 0 )
				{

					float value = Math.Round( ( quantity / max ) * 100 );
					quantity_progress.SetCurrent( value );
				}
				quantity_stack.Show( false );
				quantity_progress.Show( true );
			}
		}
	}

	void Init( EntityAI obj )
	{
		m_Obj = obj;
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( GetMainPanel().FindAnyWidget( "Render" ) );
		item_preview.Show( true );
		item_preview.SetItem( EntityAI.Cast( obj ) );
		item_preview.SetModelOrientation( "0 0 0" );
		item_preview.SetView( 1 );

		Widget item_w = item_preview.GetParent();
		if ( item_w )
		{
			SetQuantity( item_w );
		}

		WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainPanel(),  this, "CreateWhiteBackground" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainPanel(),  this, "DestroyWhiteBackground" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel(),  this, "DropReceivedFromMain" );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( GetMainPanel(),  this, "MouseClick" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "Swap" ),  this, "Swap" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "Swap" ),  this, "DraggingOverSwap" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "Combine" ),  this, "Combine" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "Combine" ),  this, "DraggingOverCombine" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel(),  this, "DraggingOver" );
		WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainPanel(),  this, "MouseEnter" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainPanel(),  this, "MouseLeave" );
		WidgetEventHandler.GetInstance().RegisterOnDoubleClick( GetMainPanel(),  this, "DoubleClick" );
		
		Refresh();
	}

	void SetPos( int row, int column )
	{
		float x, y, this_x, this_y, icon_x, icon_y;
		Container parent = Container.Cast( m_Parent );
		Widget w = m_Parent.GetMainPanel();

		float x_content, y_content;
		if( w.FindAnyWidget( "Icon0" ) )
		{
			w.FindAnyWidget( "Icon0" ).GetChildren().GetScreenSize( x_content, y_content );
		}

		icon_x = x_content / 10;
		icon_y = x_content / 10;

		if( !m_HandsIcon )
		{
			parent.Get( 0 ).GetMainPanel().GetPos( x, y );
			GetMainPanel().GetPos( this_x, this_y );
			GetMainPanel().SetPos( row * x_content + row, y + column * y_content + column );
		}

		GetMainPanel().SetSize( x_content * m_sizeX + m_sizeX , y_content * m_sizeY + m_sizeY  );
		GetMainPanel().Update();
		
		//quickfix for hands icon, will redo
		if( m_HandsIcon )
		{
			m_Parent.m_Parent.m_Parent.GetMainPanel().GetScreenSize( x_content, y_content );
			icon_x = x_content / 10;
			icon_y = x_content / 10;
			GetMainPanel().SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );
			GetMainPanel().Update();
		}
	}
}

class Pos
{
	int x, y;

	void Pos( int _x, int _y )
	{
		x = _x;
		y = _y;
	}
}
