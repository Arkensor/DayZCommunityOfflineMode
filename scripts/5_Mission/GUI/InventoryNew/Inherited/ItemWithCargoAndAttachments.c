class ItemWithCargoAndAttachments: ClosableContainer
{
	//protected EntityAI m_Entity;
	protected ref Attachments m_Atts;
	protected ref ItemsContainer m_ItemsContainer;
	protected ref CargoGrid m_CargoGrid;

	void ItemWithCargoAndAttachments( Container parent )
	{
		m_Parent = parent;
	}

	override void UpdateInterval()
	{
		m_Atts.RefreshAtt();
		if( m_CargoGrid )
		{
			m_CargoGrid.UpdateInterval();
		}
	}
	
	EntityAI GetFocusedItem()
	{
		return m_CargoGrid.GetFocusedItem();
	}	
	
	/*void SetLastActive()
	{
		
	}*/

	override void Select()
	{
		m_Atts.Select();
	}

	void SetDefaultFocus()
	{
		//m_CargoGrid.SetDefaultFocus();
		m_Atts.SetDefaultFocus();
	}

	override void UnfocusAll()
	{
		//m_CargoGrid.Unfocus();
		m_Atts.UnfocusAll();
	}

	override void MoveGridCursor( int direction )
	{
		//m_CargoGrid.MoveGridCursor( direction );
		m_Atts.MoveGridCursor( direction );
	}

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;

		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName( entity.GetDisplayName() );
		h.SetItemPreview( m_Entity );
		
		m_Atts = new Attachments( this, entity );
		m_Atts.InitAttachmentGrid();

		if( entity.GetInventory().GetCargo() != NULL)
		{
			m_ItemsContainer = new ItemsContainer( this );
			m_ItemsContainer.SetGap( 1 );
			m_CargoGrid = new CargoGrid( entity, m_ItemsContainer );
			this.Insert( m_ItemsContainer );
		}
		( Container.Cast( m_Parent ) ).Insert( this );
		SetOpenState( ItemManager.GetInstance().GetDefaultOpenState( m_Entity.GetType() ) );
		m_Parent.m_Parent.Refresh();
	}

	void ~ItemWithCargoAndAttachments()
	{
		if( this )
		{
			if( this.GetMainPanel() )
			{
				delete this.GetMainPanel();
			}
			if( m_Parent )
			{
				( Container.Cast( m_Parent ) ).UpdateSpacer();
				( Container.Cast( m_Parent.m_Parent ) ).UpdateSpacer();
			}
		}
	}

	EntityAI GetEntity()
	{
		return m_Entity;
	}

	ItemPreviewWidget GetItemPreviewWidgetDragOrDrop( Widget w )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		return ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
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
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return NULL;
		}
		return ipw.GetItem();
	}

	Widget GetItemPreviewWidget( Widget w )
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
		return ipw;
	}

	void MouseClick2( Widget w, int x, int y, int button )
	{
		bool inventory_locked = GetGame().GetPlayer().GetInventory().IsInventoryLocked();
		ItemManager.GetInstance().SetWidgetDraggable( w, !inventory_locked );
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

	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
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
		name.Replace( "RadialIcon", "Temperature" );
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
	}

	void DropReceived( Widget w, int x, int y )
	{
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		int idx = 0;
		if( m_Entity.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			bool handled_by_controller = false;
			Magazine dragged_mag;
			Weapon_Base wpn;
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if( Class.CastTo(dragged_mag, item) && Class.CastTo(wpn, dragged_mag.GetHierarchyParent()))
			{
				if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == wpn)
				{
					InventoryLocation next = new InventoryLocation;
					next.SetCargo(m_Entity, dragged_mag, idx, x, y);

					player.GetWeaponManager().DetachMagazine(next);
					handled_by_controller = true;
				}
			}
			else
			{
				if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == item)
				{
					player.PredictiveTakeEntityToTargetCargoEx(m_Entity, item, idx, x, y );
					handled_by_controller = true;
				}
			}
			if (!handled_by_controller)
			{
				player.PredictiveTakeEntityToTargetCargoEx( m_Entity, item, idx, x, y );
			}
			
			Icon icon = m_ItemsContainer.GetIcon( item.GetID() );
			
			if( icon )
			{
				//w.FindAnyWidget("Color").SetColor( ColorManager.BASE_COLOR );
				icon.RefreshPos( x, y );
				icon.Refresh();
				Refresh();
			}
		}
	}

	void TakeAsAttachment( Widget w, Widget receiver )
	{
		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		
		if( receiver_iw )
		{
			receiver_item = receiver_iw.GetItem();
		}
		
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
		{
			if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				player.GetWeaponManager().AttachMagazine( mag );
			}
		}
		else if (receiver_item)
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				GetGame().GetPlayer().PredictiveSwapEntities( item, receiver_item );
			}
		}
		else if( m_Entity.GetInventory().CanAddAttachment( item ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( m_Entity, item );
		}
		else if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetCargo( m_Entity, item );
		}
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void OnDropReceivedFromHeader2( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void DoubleClick(Widget w, int x, int y, int button)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( w == NULL || player.GetInventory().IsInventoryLocked() )
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

		if ( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			if (player.GetInventory().CanAddEntityToInventory( item ) && item.GetInventory().CanRemoveEntity())
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

	bool DraggingOverGrid( Widget w,  int x, int y, Widget reciever )
	{
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
		if( m_Entity.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			color = ColorManager.GREEN_COLOR;
		}
		else
		{
			color = ColorManager.RED_COLOR;
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

	override void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader( w, x, y, receiver );
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = receiver_iw.GetItem();

		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
		{
			if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
		}
		else if( receiver_item )
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item )) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			}
		}
		else
		if( m_Entity.GetInventory().CanAddAttachment( item ) )
		{
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
		{
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else
		{
			ItemManager.GetInstance().ShowSourceDropzone( item );
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
		}
	}

}
