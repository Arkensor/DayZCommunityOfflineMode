class HandsContainer: Container
{
	ref HandsHeader m_CollapsibleHeader;
	protected bool m_Hidden;
	protected ref Attachments m_Atts;
	protected ref CargoGrid m_Cargo;
	EntityAI m_am_entity1, m_am_entity2;
	ref ItemsContainer m_ItemsContainer;

	void HandsContainer( ContainerBase parent )
	{
		m_CollapsibleHeader = new HandsHeader( this, "CollapseButtonOnMouseButtonDown" );

		m_Body = new array<ref ContainerBase>;
		HandsPreview hands_preview = new HandsPreview( this );
		ItemManager.GetInstance().SetHandsPreview( hands_preview );
		m_Body.Insert( hands_preview );

		LoadDefaultState();
		m_Spacer.Update();
		m_MainPanel.Update();
	}

	void MouseClick2( Widget w, int x, int y, int button )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		
		if( !item_preview )
		{
			return;
		}
		
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		
		if( item )
		{
			if ( button == MouseState.MIDDLE )
			{
				InspectItem( itemAtPos );
			}
		}
	}
		
	void DraggingOverHeader2(Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader(w, x, y, receiver );
	}
	
	override void TransferItem()
	{
		EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
		if( item_in_hands && GetGame().GetPlayer().GetHumanInventory().CanRemoveEntityInHands() )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, item_in_hands );
		}
	}
	
	override void EquipItem()
	{
		EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
		if( item_in_hands && !item_in_hands.IsInherited( Magazine ) && GetGame().GetPlayer().GetHumanInventory().CanRemoveEntityInHands() )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, item_in_hands );
		}
	}
	
	void TransferItemToVicinity()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		if( player.CanDropEntity( item_in_hands ) && GetGame().GetPlayer().GetHumanInventory().CanRemoveEntityInHands() )
		{
			player.PredictiveDropEntity( item_in_hands );
		}
	}

	override void Select()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		if( item_in_hands )
		{
			ItemManager.GetInstance().SetSelectedItem( item_in_hands, GetMainPanel().FindAnyWidget( "Selected" ) );
			GetMainPanel().FindAnyWidget("Selected").Show(true);
		}
		else
		{
			EntityAI selected_entity = ItemManager.GetInstance().GetSelectedItem();
			if( selected_entity && player.GetHumanInventory().CanAddEntityInHands( selected_entity ) )
			{
				player.PredictiveTakeEntityToHands( selected_entity );
			}
		}
	}

	void SelectCombine()
	{
		HandsPreview preview = HandsPreview.Cast( m_Body.Get( 0 ) );
		preview.SelectCombine();
	}

	void SelectSwap()
	{
		HandsPreview preview = HandsPreview.Cast( m_Body.Get( 0 ) );
		preview.SelectSwap();
	}

	EntityAI GetItemPreviewItem( Widget w )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return NULL;
		}
		return ipw.GetItem();
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
		InventoryItem receiver_entity = InventoryItem.Cast( p.GetHumanInventory().GetEntityInHands() );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		if( receiver_entity != NULL && GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			//if( m_HandsIcon )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			/*else
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
				ItemManager.GetInstance().ShowSourceDropzone( w_entity );
			}*/
		}
	}

	void OnDropReceivedFromHeader2( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item = p.GetHumanInventory().GetEntityInHands();

		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}

		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		if( !ipw.GetItem() )
		{
			return;
		}

		EntityAI receiver_item;
		name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = receiver_iw.GetItem();


		Weapon_Base wpn;
		Magazine mag;
		if ( Class.CastTo(wpn,  item ) && Class.CastTo(mag,  ipw.GetItem() ) )
		{
			if( p.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				p.GetWeaponManager().AttachMagazine( mag );
			}
		}
		else if (receiver_item)
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( ipw.GetItem() ) ) )
			{
				( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( ipw.GetItem() ) );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, ipw.GetItem() ) )
			{
				GetGame().GetPlayer().PredictiveSwapEntities( ipw.GetItem(), receiver_item );
			}
		}
		else if( item.GetInventory().CanAddAttachment( ipw.GetItem() ) )
		{
			p.PredictiveTakeEntityToTargetAttachment( item, ipw.GetItem() );
		}

	}
/*	void InjectValidRecipes( EntityAI entity1, EntityAI entity2,ContextMenu cmenu, bool recipes_anywhere = false )
	{
		PlayerBase p = GetGame().GetPlayer();
		array<int> recipes_ids = p.m_Recipes;
		PluginRecipesManager plugin_recipes_manager = GetPlugin( PluginRecipesManager );
		plugin_recipes_manager.GetValidRecipes( entity1,entity2,recipes_ids, p );

		for( int x = 0;x < recipes_ids.Count();x++ )
		{
			int id = recipes_ids.Get( x );
			string name = plugin_recipes_manager.GetRecipeName( id );

			cmenu.Add( name, this, "OnPerformRecipe",new Param1<int>( id ) );
		}
	}*/

	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;

		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );

		m_Parent.m_Parent.GetMainPanel().FindAnyWidget( "body" ).GetScreenSize( x_content, y_content );
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );

		if( !ipw.GetItem() )
		{
			return;
		}
		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
	}

	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();

		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Temperature", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
		name.Replace( "GhostSlot", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
	}

	void OnPerformCombination(int combinationFlags)
	{
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( m_am_entity1 == NULL || m_am_entity2 == NULL ) return;

		if ( combinationFlags == InventoryCombinationFlags.NONE ) return;

		if ( combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER )
		{
			Magazine mag;
			if ( Class.CastTo(mag,  m_am_entity2 ) )
			{
				m_player.GetWeaponManager().LoadBullet( mag );
				return;
			}
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT )
		{
			if( m_Atts )
			{
				m_Atts.Remove();
				delete m_Atts;
			}
			if( m_am_entity2.GetSlotsCountCorrect() > 0 )
			{
				m_Atts = new Attachments( this, m_am_entity2 );
				m_Atts.InitAttachmentGrid();

			}

			this.Refresh();
			m_Parent.Refresh();
			m_Spacer.Update();

			Weapon_Base wpn1;
			Magazine mag1;
			if ( Class.CastTo(wpn1,  m_am_entity1 ) && Class.CastTo(mag1,  m_am_entity2 ) )
			{
				if( m_player.GetWeaponManager().CanAttachMagazine(wpn1, mag1) )
				{
					m_player.GetWeaponManager().AttachMagazine(mag1);
					return;
				}
			}
			m_player.PredictiveTakeEntityToTargetAttachment( m_am_entity1, m_am_entity2 );
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO )
		{
			m_player.PredictiveTakeEntityToTargetCargo( m_am_entity1, m_am_entity2 );
		}

		if ( combinationFlags & InventoryCombinationFlags.SWAP )
		{
			Magazine swapmag1;
			Magazine swapmag2;
			if (Class.CastTo(swapmag1,  m_am_entity1 ) && Class.CastTo(swapmag2,  m_am_entity2 ) )
			{
				if( m_player.GetWeaponManager().CanSwapMagazine( Weapon_Base.Cast( swapmag1.GetParent() ), Magazine.Cast( swapmag2 ) ) )
				{
					m_player.GetWeaponManager().SwapMagazine(swapmag2);
					return;
				}

				if( m_player.GetWeaponManager().CanSwapMagazine(Weapon_Base.Cast( swapmag2.GetParent() ), Magazine.Cast( swapmag1 )) )
				{
					m_player.GetWeaponManager().SwapMagazine(swapmag1);
					return;
				}
			}
			if( !m_player.PredictiveSwapEntities( m_am_entity1, m_am_entity2 ) && m_player.GetHumanInventory().CanAddEntityInHands( m_am_entity1 ) )
			{
				m_player.PredictiveTakeEntityToHands( m_am_entity1 );
			}
//			GetDragGrid().GetContainer().HideExpandView(m_am_entity2);
		}

		if ( combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS )
		{
			if( m_Atts )
			{
				m_Atts.Remove();
				delete m_Atts;
			}

			if( m_am_entity2.GetSlotsCountCorrect() > 0 )
			{
				m_Atts = new Attachments( this, m_am_entity2 );
				m_Atts.InitAttachmentGrid();

			}
			this.Refresh();
			m_Parent.Refresh();
			m_Spacer.Update();
			
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
	}

	void ShowActionMenuCombine( EntityAI entity1, EntityAI entity2, int combinationFlags )
	{
		if ( entity1 && entity2 ) PrintString( "Showing action menu for " + entity1.GetDisplayName() + " and " + entity2.GetDisplayName() );

		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		m_am_entity1 = entity1;
		m_am_entity2 = entity2;
		cmenu.Hide();
		cmenu.Clear();

		/*if( combinationFlags & InventoryCombinationFlags.RECIPE_HANDS )
		{
			GetGame().ProfilerStart( "RECIPES_PROFILER" );

			InjectValidRecipes( entity1, entity2, cmenu, false );

			GetGame().ProfilerStop( "RECIPES_PROFILER" );
		}
		else if( combinationFlags & InventoryCombinationFlags.RECIPE_ANYWHERE )
		{
			InjectValidRecipes( entity1, entity2, cmenu, true );
		}*/

		if( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY2 )
		{
			ItemBase entity = ItemBase.Cast( entity1 );
			entity.CombineItemsClient( ItemBase.Cast( entity2 ) );
			return;
		}

		if ( entity1 == NULL || entity2 == NULL ) return;

		if ( combinationFlags == InventoryCombinationFlags.NONE ) return;

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT )
		{
			cmenu.Add( "#inv_context_add_as_attachment", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.ADD_AS_ATTACHMENT ) );
		}
		if ( combinationFlags & InventoryCombinationFlags.LOAD_CHAMBER )
		{
			cmenu.Add( "#inv_context_load_chamber", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.LOAD_CHAMBER ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO )
		{
			cmenu.Add( "#inv_context_add_as_cargo", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.ADD_AS_CARGO ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.SWAP )
		{
			cmenu.Add( "#inv_context_swap", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.SWAP ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS )
		{
			cmenu.Add( "#inv_context_take_to_hands", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.TAKE_TO_HANDS ) );
		}

		if ( combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY )
		{
			cmenu.Add( "#inv_context_combine", this, "OnPerformCombination", new Param1<int>( InventoryCombinationFlags.COMBINE_QUANTITY ) );
		}

		int m_am_pos_x,  m_am_pos_y;
		GetMousePos( m_am_pos_x, m_am_pos_y );
		m_am_pos_x -= 5;
		m_am_pos_y -= 5;

		cmenu.Show( m_am_pos_x, m_am_pos_y );
	}

	override bool OnDrag( Widget w, int x, int y )
	{
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );

		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;

		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );

		m_Parent.m_Parent.GetMainPanel().FindAnyWidget( "body" ).GetScreenSize( x_content, y_content );
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );

		if( !ipw.GetItem() )
		{
			return false;
		}

		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		ItemManager.GetInstance().SetDraggedItem( ipw.GetItem() );

		return false;
	}

	override bool OnDrop( Widget w, int x, int y, Widget reciever )
	{
		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Col", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		ipw.SetItem( NULL );
		ItemManager.GetInstance().SetDraggedItem( NULL );

		return false;
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		//HandsPreview preview = m_Body.Get( 0 );
		//preview.DropReceived( w, x, y, receiver );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );

		ItemPreviewWidget receiver_ipw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( "Render" ) );
		/*if( m_HandsIcon )
		{
			receiver_ipw = receiver.GetParent().FindAnyWidget( "Render" );
		}*/

		ItemPreviewWidget w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( w_ipw == NULL )
		{
			w_ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		}
		if( w_ipw == NULL )
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		InventoryItem receiver_entity = InventoryItem.Cast( item_in_hands );
		InventoryItem w_entity = InventoryItem.Cast( w_ipw.GetItem() );
		if( !w_entity )
		{
			return;
		}

		Weapon_Base wpn;
		Magazine mag;

		if ( Class.CastTo(wpn,  item_in_hands ) && Class.CastTo(mag,  w_entity ) && Class.CastTo(wpn,  mag.GetHierarchyParent() ) )
		{
			return;
		}

		if( receiver_entity != NULL && GameInventory.CanSwapEntities( receiver_entity, w_entity ) )
		{
			GetGame().GetPlayer().PredictiveSwapEntities( w_entity, receiver_entity );

			Icon icon = ItemManager.GetInstance().GetDraggedIcon();
			if( m_Parent.IsInherited( ItemsContainer ) )
			{
				//m_Timers[0].Run(0.1, this, "ToRefresh",  new Param2<Icon, Icon>(this, icon), true);
			}
		}
		else
		{
				InventoryLocation inv_loc_src = new InventoryLocation;
				InventoryLocation inv_loc_dst = new InventoryLocation;
				if( item_in_hands )
				{
					item_in_hands.GetInventory().GetCurrentInventoryLocation( inv_loc_src );
					player.GetInventory().FindFreeLocationFor( item_in_hands, FindInventoryLocationType.ANY, inv_loc_dst );

					// todle je nejaky divny ne?
					//TryMoveEntity( inv_loc_src, inv_loc_dst );
					if( inv_loc_dst.IsValid() && player.GetHumanInventory().CanAddEntityInHands( w_entity ) )
					{
						player.PredictiveTakeEntityToHands( w_entity );
					}
				}
				else
				{
					if( player.GetHumanInventory().CanAddEntityInHands( w_entity ) )
					{
						player.PredictiveTakeEntityToHands( w_entity );
					}
				}
		}
	}

	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		HandsPreview preview = HandsPreview.Cast( m_Body.Get( 0 ) );
		preview.RefreshQuantity( item_to_refresh );
	}

	int GetCombinationFlags( EntityAI entity1, EntityAI entity2 )
	{
		int flags = 0;
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI entity_in_hands = m_player.GetHumanInventory().GetEntityInHands();

		if ( !entity1 || !entity2 ) return flags;

		Magazine swapmag1;
		Magazine swapmag2;
		bool skipSwap = false;

		if( Class.CastTo(swapmag1, entity1) && Class.CastTo(swapmag2, entity2) )
		{
			Weapon_Base parentWpn;

			if(	Class.CastTo(parentWpn, swapmag1.GetHierarchyParent())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag2) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE;
			}

			if(	Class.CastTo(parentWpn, swapmag2.GetHierarchyParent())	)
			{
				skipSwap = true;
				if( m_player.GetWeaponManager().CanSwapMagazine(parentWpn,swapmag1) )
					flags = flags | InventoryCombinationFlags.SWAP_MAGAZINE;

			}
		}

		if ( !skipSwap )
		{
			if ( entity1 == m_player.GetHumanInventory().GetEntityInHands() ) flags = flags | InventoryCombinationFlags.TAKE_TO_HANDS;
			else if ( GameInventory.CanSwapEntities( entity1, entity2 ) )
			{
				if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( Car ) )
				{
					flags = flags | InventoryCombinationFlags.SWAP;
				}
			}
		}

		if ( entity1.IsInherited( ItemBase) && entity2.IsInherited( ItemBase ) )
		{
			ItemBase ent1 = ItemBase.Cast( entity1 );
			if ( ent1.CanBeCombined( ItemBase.Cast( entity2 ), PlayerBase.Cast( m_player ) ) ) flags = flags | InventoryCombinationFlags.COMBINE_QUANTITY2;
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
		}
		else if ( entity1.GetInventory().CanAddAttachment( entity2 ) )
		{
			if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( Car ) && !entity2.IsInherited( ZombieBase ) && !entity2.IsInherited( Car ) )
			{
				flags = flags | InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			}
		}
		if ( entity1.GetInventory().CanAddEntityInCargo( entity2 ) ) flags = flags | InventoryCombinationFlags.ADD_AS_CARGO;

		if( entity1 == m_player.GetHumanInventory().GetEntityInHands() || entity2 == m_player.GetHumanInventory().GetEntityInHands() )
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

	override void UpdateSpacer()
	{
		super.UpdateSpacer();
		m_Spacer.Update();
	}
	
	void DestroyAtt()
	{
		if( m_Atts )
		{
			m_Atts.Remove();
			delete m_Atts;
		}

		this.Refresh();
		m_Parent.Refresh();
		m_Spacer.Update();
	}
	
	void DestroyCargo()
	{
		if( m_Cargo )
		{
			this.Remove(m_ItemsContainer);
			m_Cargo.Remove();
			delete m_Cargo;
		}

		this.Refresh();
		m_Parent.Refresh();
		m_Spacer.Update();
	}
	
	void DropReceived( Widget w,  int x, int y )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		int idx = 0;
		if( item_in_hands.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			bool handled_by_controller = false;
			Magazine dragged_mag;
			Weapon_Base wpn;
			if( Class.CastTo(dragged_mag, item) && Class.CastTo(wpn, dragged_mag.GetHierarchyParent()))
			{
				if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == wpn)
				{
					InventoryLocation next = new InventoryLocation;
					next.SetCargo(item_in_hands, dragged_mag, idx, x, y);

					player.GetWeaponManager().DetachMagazine(next);
					handled_by_controller = true;
				}
			}
			else
			{
				if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == item)
				{
					player.PredictiveTakeEntityToTargetCargoEx(item_in_hands, item, idx, x, y );
					handled_by_controller = true;
				}
			}
			if (!handled_by_controller)
			{
				player.PredictiveTakeEntityToTargetCargoEx( item_in_hands, item, idx, x, y );
			}
			
			Icon icon = m_ItemsContainer.GetIcon( item.GetID() );
			
			if( icon )
			{
				w.FindAnyWidget("Color").SetColor( ColorManager.BASE_COLOR );
				icon.RefreshPos( x, y );
				icon.Refresh();
				Refresh();
			}
		}
	}
	
	bool DraggingOverGrid( Widget w,  int x, int y, Widget reciever )
	{
		EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
		if( w == NULL )
		{
			return false;
		}
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return false;
		}

		int color;
		int idx = 0;
		if( item_in_hands.GetInventory().CanAddEntityInCargoEx( item, 0, x, y ) )
		{
			ItemManager.GetInstance().HideDropzones();
			/*if( item_in_hands.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}*/
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			color = ColorManager.GREEN_COLOR;
		}
		else
		{
			color = ColorManager.RED_COLOR;
			ItemManager.GetInstance().ShowSourceDropzone( item );
		}

		if( w.FindAnyWidget("Color") )
		{
			w.FindAnyWidget("Color").SetColor( color );
		}
		else
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Col" );
			w.FindAnyWidget( name ).SetColor( color );
		}

		return true;
	}

	void ShowAtt( EntityAI entity )
	{
		if( entity.GetSlotsCountCorrect() > 0 )
		{
			m_Atts = new Attachments( this, entity );
			m_Atts.InitAttachmentGrid();
		}
		
		if( entity.GetInventory().GetCargo() )
		{
			m_ItemsContainer = new ItemsContainer( this );
			m_ItemsContainer.SetGap( 1 );
			this.Insert( m_ItemsContainer );
			m_Cargo = new CargoGrid( entity, m_ItemsContainer );
		}

		if( m_Hidden )
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}
		}
		else
		{
			for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}
		}

		this.Refresh();
		m_Parent.Refresh();
		m_Spacer.Update();
	}

	override void UpdateInterval()
	{
		super.UpdateInterval();
		if( m_Atts )
		{
			m_Atts.RefreshAtt();
		}
		if( m_Cargo )
		{
			m_Cargo.UpdateInterval();
		}
		m_CollapsibleHeader.UpdateInterval();
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	override void OnShow()
	{
		super.OnShow();

		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).OnShow();
		}
		m_Spacer.Update();
	}

	void DoubleClick(Widget w, int x, int y, int button)
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

		EntityAI item = iw.GetItem();

		InventoryManager manager = InventoryManager.GetInstance();

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		Weapon_Base wpn;
		Magazine mag;
		if ( Class.CastTo(wpn,  item_in_hands ) )
		{
			return;
		}

		if ( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			if ( player.GetInventory().CanAddEntityToInventory( item ) && player.GetHumanInventory().CanRemoveEntityInHands() )
			{
				player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, item );
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

		name = w.GetName();
		name.Replace( "PanelWidget", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
	}

	void LoadDefaultState()
	{
		//m_Hidden = !ItemManager.GetInstance().GetDefaultOpenStateHands( );

		if( m_Hidden )
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}
		}
		else
		{
			for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}
		}

		GetMainPanel().FindAnyWidget("opened").Show(!m_Hidden);
		GetMainPanel().FindAnyWidget("closed").Show(m_Hidden);
	}

	void CollapseButtonOnMouseButtonDown( Widget w )
	{
		if( !m_Hidden )
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}
			m_Hidden = true;
		}
		else
		{
			for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}
			m_Hidden = false;
		}

		w.FindAnyWidget("opened").Show(!m_Hidden);
		w.FindAnyWidget("closed").Show(m_Hidden);

		ItemManager.GetInstance().SetDefaultOpenStateHands( !m_Hidden );

		m_Parent.Refresh();
	}
}
