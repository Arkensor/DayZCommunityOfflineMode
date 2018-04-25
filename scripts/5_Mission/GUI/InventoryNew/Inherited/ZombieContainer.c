class ZombieContainer: CollapsibleContainer
{
	protected ref Container m_Container;
	protected ref map<int, ref Widget> m_InventorySlots;
	protected ref map<EntityAI, ref Container> m_ShowedItems = new ref map<EntityAI, ref Container>;
	protected EntityAI m_ZombieEntity;
	
	void ZombieContainer( ContainerBase parent )
	{
		m_InventorySlots = new ref map<int, ref Widget>;
		m_Container = new Container( this );
		m_Container.Insert( new IconsContainer( m_Container ) );
		m_Body.Insert( m_Container );

		SetHeaderName();
	}
	
	void ~ZombieContainer()
	{
		if ( GetMainPanel() && GetMainPanel().GetParent() )
		{
			delete GetMainPanel().GetParent();
		}
	}
	
	void SetEntity( EntityAI zombie_entity )
	{
		m_ZombieEntity = zombie_entity;
		InitGhostSlots();
		( Container.Cast( m_Parent ) ).m_Body.Insert( this );
		m_Parent.Refresh();
	}
	
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
		name.Replace( "Col", "RadialIcon" );
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
		name.Replace( "Col", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		ipw.Show( false );
		name.Replace( "Render", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
	}
	
	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
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
			return;	
		}
		
		EntityAI item = ipw.GetItem();
		
		if (m_ZombieEntity.GetInventory().CanAddEntityInCargo( item ))
			GetGame().GetPlayer().PredictiveTakeEntityToTargetCargo( m_ZombieEntity, ipw.GetItem() );
		else if( m_ZombieEntity.GetInventory().CanAddEntityToInventory( item ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory( m_ZombieEntity, FindInventoryLocationType.ANY, ipw.GetItem() );
		}
	}
	
	void OnDropReceivedFromGhostArea( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		string name = receiver.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget( name ) );
		EntityAI receiver_item;
		if( receiver_iw )
		{
			receiver_item = receiver_iw.GetItem();
		}
		
		if( !ipw )
		{
		  name = w.GetName();
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
		
		EntityAI item = ipw.GetItem();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if( receiver_item )
		{
			if( receiver_item.GetInventory().CanAddAttachment( item ) )
			{
				player.PredictiveTakeEntityToTargetAttachment( receiver_item, item );
				return;
			}
			else if( receiver_item.GetInventory().CanAddEntityInCargo( item ) && !receiver_item.GetInventory().HasEntityInCargo( item ) )
			{
				player.PredictiveTakeEntityToTargetInventory( receiver_item, FindInventoryLocationType.ANY, item );
				return;
			}
		}
		
		if( item )
		{
			if( m_ZombieEntity.GetInventory().CanAddAttachment( item ) )
			{
				player.PredictiveTakeEntityToTargetAttachment( m_ZombieEntity, item );
			}
			else if( m_ZombieEntity.GetInventory().CanAddEntityToInventory( item ) )
			{
				player.PredictiveTakeEntityToTargetInventory( m_ZombieEntity, FindInventoryLocationType.ANY, ipw.GetItem() );
			}
		}
	}

	void SetHeaderName()
	{
		Header h = Header.Cast( m_Body.Get( 0 ) );
		h.SetName( "ZOMBIE CONTAINER" );
	}

	override void UpdateInterval()
	{
		if( m_ZombieEntity == NULL )
		{
			return;
		}
		int attachments_count = m_ZombieEntity.GetInventory().AttachmentCount();
		ref map<EntityAI, ref Container> new_showed_items = new ref map<EntityAI, ref Container>;
		ref array<EntityAI> showed_player_ghost_entities = new ref array<EntityAI>;
		for ( int i = 0; i < attachments_count; i++ )
		{
			EntityAI entity = m_ZombieEntity.GetInventory().GetAttachmentFromIndex( i );
			if ( entity )
			{
				int slot_id = entity.GetInventory().GetSlotId();
				ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_InventorySlots.Get( slot_id ) );
				if( ipw == NULL )
				{
					return;
				}
				string name2 = ipw.GetParent().GetName();
				
				ipw.Show( true );
				ipw.GetParent().Show( true );
				name2.Replace( "PanelWidget", "GhostSlot" );
				ipw.GetParent().GetParent().FindAnyWidget( name2 ).Show( false );
				ipw.SetItem( entity );
				showed_player_ghost_entities.Insert( entity );

				string config = "CfgVehicles " + entity.GetType() + " GUIInventoryAttachmentsProps";

					if( m_ShowedItems.Contains( entity ) == false )
					{
						string name;
						if( entity.GetInventory().GetCargo() )
						{
							ItemWithCargo iwc = new ItemWithCargo( this );
							iwc.SetEntity( entity );
							new_showed_items.Insert( entity, iwc );
							ipw.GetParent().SetUserID( ( new_showed_items.Count() - 1) );
							WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( ipw.GetParent(),  this, "ToggleWidget" );

							name = ipw.GetName();
							name.Replace( "Icon", "RadialIcon" );
							ipw.GetParent().FindAnyWidget( name ).Show(true);
						}
						else if( entity.GetSlotsCountCorrect() > 0 )
						{
							ItemWithCargoAndAttachments iwca = new ItemWithCargoAndAttachments( this );
							iwca.SetEntity( entity );
							new_showed_items.Insert( entity, iwca );
							ipw.GetParent().SetUserID( ( new_showed_items.Count() - 1 ) );
							WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( ipw.GetParent(),  this, "ToggleWidget" );

							name = ipw.GetName();
							name.Replace( "Icon", "RadialIcon" );
							ipw.GetParent().FindAnyWidget( name ).Show( true );
						}
					}
					else
					{
						new_showed_items.Insert( entity, m_ShowedItems.Get( entity ) );
					}
			}
		}
		
		if( m_ShowedItems.Contains( m_ZombieEntity ) == false )
		{
			if( m_ZombieEntity.GetInventory().GetCargo() )
			{
				ItemWithCargo iwcc = new ItemWithCargo( this );
				iwcc.SetEntity( m_ZombieEntity );
				new_showed_items.Insert( m_ZombieEntity, iwcc );
			}
		}
		else
		{
			new_showed_items.Insert( m_ZombieEntity, m_ShowedItems.Get( m_ZombieEntity ) );
		}

		for ( i = 0; i < m_ShowedItems.Count(); i++ )
		{
			EntityAI ent = m_ShowedItems.GetKey( i );
			m_ShowedItems.GetElement( i ).UpdateInterval();
			if( new_showed_items.Contains( ent ) == false )
			{
				Container con = m_ShowedItems.GetElement( i );
				( Container.Cast( con.m_Parent ) ).Remove( con );
			}
		}
		
		for ( i = 0; i < m_InventorySlots.Count(); i++)
		{
			ipw = ItemPreviewWidget.Cast( m_InventorySlots.GetElement(i) );
			if( ipw.GetItem() != NULL && showed_player_ghost_entities.Find( ipw.GetItem() ) == INDEX_NOT_FOUND )
			{
				name2 = ipw.GetParent().GetName();
				ipw.Show( false );
				name2.Replace( "PanelWidget", "GhostSlot" );
				ipw.GetParent().GetParent().FindAnyWidget( name2 ).Show( true );
				ipw.SetItem( NULL );
			}
			
		}

		m_ShowedItems = new_showed_items;
	}

	void ToggleContainer( Widget w , int number )
	{
		ClosableContainer c = ClosableContainer.Cast( m_ShowedItems.GetElement( number ) );
		if( c.IsOpened() )
		{
			c.Close();
		}
		else
		{
			c.Open();
		}

		string name = w.GetName();
		name.Replace( "PanelWidget", "RadialIcon" );
		w.GetParent().FindAnyWidget( name ).Show( !w.FindAnyWidget( name ).IsVisible() );
	}

	void ToggleWidget( Widget w )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( ipw.GetItem() )
		{
			ToggleContainer( w, w.GetUserID() );
		}
	}

	void InitGhostSlots()
	{
		string config_path_ghosts_slots = "CfgVehicles ZombieBase InventoryEquipment playerSlots";
		ref array<string> player_ghosts_slots = new array<string>;
		GetGame().ConfigGetTextArray( config_path_ghosts_slots, player_ghosts_slots );

		for ( int i = 0; i < player_ghosts_slots.Count(); i++ )
		{
			string slot_name = player_ghosts_slots.Get ( i );
			string path = "CfgSlots" + " " + slot_name;

			if ( GetGame().ConfigIsExisting( path ) )
			{
				string icon_name;
				GetGame().ConfigGetText( path + " ghostIcon", icon_name );

				Widget item_preview = GetWidgetSlot( i );
				LoadIconIntoWidgetSlot( icon_name, i );

				GetGame().ConfigGetText( path + " name", slot_name );
				int slot_id = InventorySlots.GetSlotIdFromString( slot_name );
				m_InventorySlots.Set( slot_id, item_preview );
			}
		}
	}

	Widget GetWidgetSlot( int slot_number )
	{
		int row = slot_number / ITEMS_IN_ROW;
		int column = slot_number % ITEMS_IN_ROW;

		Widget item_preview = m_Container.Get( row ).GetMainPanel().FindAnyWidget( "Render" + column );
		if( !item_preview )
		{
			item_preview = m_Container.Get( row ).GetMainPanel().FindAnyWidget( "Icon" + column );
		}
		WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview.GetParent(),  this, "OnIconDrag" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview.GetParent(),  this, "OnIconDrop" );
		WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview.GetParent(),  this, "DoubleClick" );
		return item_preview;
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
				
		if (player.GetInventory().HasEntityInInventory( item ) && GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			bool found = false;
			if( item.GetInventory().CanRemoveEntity())
			{
				InventoryLocation il = new InventoryLocation;
				found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ATTACHMENT,il);
				if (found)
				{
					if ( il.GetParent() != player )
					{
						found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ANY_CARGO,il);
					}
				}
				else
				{
					found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ANY,il);
				}
			}
			if ( found )
			{
				//player.GetInventory().PredictiveTakeToDst(true,item,il);
				player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, item);
			}
			else
			{
				if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}
		iw.SetItem( NULL );
		name.Replace( "Render", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
		
		ItemManager.GetInstance().HideTooltip();
	}
	
	void LoadIconIntoWidgetSlot( string icon_name, int slot_number  )
	{
		int column = slot_number % ITEMS_IN_ROW;
		Widget item_preview = GetWidgetSlot( slot_number );
		ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + column ) );
		if( !image_widget )
		{
			image_widget = ImageWidget.Cast( item_preview.GetParent().GetParent().FindAnyWidget( "GhostSlot" + column ) );
		}
		image_widget.Show( true );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( item_preview.GetParent().GetParent().FindAnyWidget( "PanelWidget" + column ),  this, "OnDropReceivedFromGhostArea" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( item_preview.GetParent().GetParent().FindAnyWidget( "GhostSlot" + column ),  this, "OnDropReceivedFromGhostArea" );
		image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
	}

	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
		m_RootSpacer.Update();
		( LeftArea.Cast( m_Parent ) ).UpdateSpacer();
	}
}
