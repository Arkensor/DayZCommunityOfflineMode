class ItemWithElectricManager: ClosableContainer
{
// 	EntityAI m_Entity;
	ref Attachments m_Atts;
	
	void ItemWithElectricManager( Container parent )
	{
		m_Parent = parent;
	}

	override void UpdateInterval()
	{
		if( m_Entity )
		{
			m_Atts.RefreshAtt();
			RefreshSocketIcons();
			ElectricityIcon();
		}
	}
	
	void ElectricityIcon()
	{
		bool show_electricity_icon = m_Entity.GetCompEM().HasElectricityIcon();
		this.GetMainPanel().FindAnyWidget( "electricity" ).Show( show_electricity_icon );
		
		bool show_electricity_can_work_icon = m_Entity.GetCompEM().CanWork() && !m_Entity.IsRuined();
		this.GetMainPanel().FindAnyWidget( "electricity_can_work" ).Show( show_electricity_can_work_icon );
	}
	
	void RefreshSocketIcons()
	{
		int count = m_Entity.GetCompEM().GetSocketsCount();

		Widget ghost_widget;
		ImageWidget ghost_icon;
		for ( int i = 0; i < count; i++ )
		{
			string icon_name = "missing";

			if ( ghost_icon )
			{
				ghost_icon.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
			}
			
			int row = ( i / 7 ) + 2;
			int column = i % 7;
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_Body.Get( row ).GetMainPanel().FindAnyWidget( "Icon" + column ).FindAnyWidget( "Render" + column ) );
			Widget icon = m_Body.Get( row ).GetMainPanel().FindAnyWidget( "Icon" + column );
			
			ImageWidget image_widget = ImageWidget.Cast( icon.FindAnyWidget( "GhostSlot" + column ) );

			bool show_ghost_image = m_Entity.GetCompEM().GetDeviceBySocketID( i ) == NULL;
			image_widget.Show( show_ghost_image );

			image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
			item_preview.SetItem( m_Entity.GetCompEM().GetDeviceBySocketID( i ) );
			item_preview.SetModelOrientation( Vector( 0, 0, 0 ) );
			item_preview.SetView( 1 );
		}
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
		name.Replace( "Col", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
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
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
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
	
	void DraggingOverHeader2(Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader(w, x, y, receiver );
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
	
	void OnDropReceivedFromHeader2( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void TakeAsAttachment( Widget w, Widget receiver )
	{
		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = receiver_iw.GetItem();
		
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
	}
	

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;

		Header h = Header.Cast( m_Body.Get( 0 ) );
		h.SetName( entity.GetDisplayName() );
		h.SetItemPreview( m_Entity );
		m_Atts = new Attachments( this, entity );
		m_Atts.InitAttachmentGrid();
		
		//START - InitSocketGrid();
			int count = m_Entity.GetCompEM().GetSocketsCount();

			if( count > 0 )
			{
				for ( int i = 0; i < ( count / 7 ) + 1; i++ )
				{
					IconsContainer ic = new IconsContainer( this );
					this.Insert( ic );
				}
			}
			
			RefreshSocketIcons();
		//END - InitSocketGrid();
		
		ElectricityIcon();

		( Container.Cast( m_Parent ) ).Insert( this );

		m_Parent.m_Parent.Refresh();
	}

	void ~ItemWithElectricManager()
	{
		if( this && m_Parent )
		{
			//((Container)m_Parent).Remove( this );
			delete this.GetMainPanel();

			( VicinityContainer.Cast( m_Parent ) ).UpdateSpacer();
			( LeftArea.Cast( m_Parent.m_Parent ) ).UpdateSpacer();
		}
	}

	EntityAI GetEntity()
	{
		return m_Entity;
	}
}
