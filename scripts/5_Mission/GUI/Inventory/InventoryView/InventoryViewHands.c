class InventoryViewHands extends InventoryView
{
	void InventoryViewHands( InventoryContainer container, EntityAI entity, int properties )
	{
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		// Print("HANDS OnMouseButtonDown ");
		if ( w == m_header_bg || w == m_header || w == m_minimize_widget || w == m_maximize_widget )
		{
			// Print( w.GetName());
			// Print( ToString( m_body) );
			// Print( ToString( m_body_spacer) );
			// Print( ToString( m_content_spacer) );
			if  ( m_body && m_body_spacer && m_content_spacer )
			{
				if ( m_body.IsVisible() )
				{
					m_minimize_widget.Show( false );
					m_maximize_widget.Show( true );
					m_body.Show( false );
				}	
				else
				{
					m_minimize_widget.Show( true );
					m_maximize_widget.Show( false );
					m_body.Show( true );
				}
				// m_body_spacer.Update();
				// Widget container_root = m_root; //m_container.GetRootWidget();
				// AutoHeightSpacer spacer;
				// container_root.GetScript(spacer);
				// spacer.Update();
			}

			m_body_spacer.Update();
			m_content_spacer.Update();

			Widget container_root = m_root; //m_container.GetRootWidget();
			AutoHeightSpacer spacer;
			container_root.GetScript(spacer);
			spacer.Update();
			AlignToBottom();
		}
		return true;
	}
	
	override void CreateLayout()
	{
		if ( m_entity == NULL ) 
		{
			m_root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryViewHandsEmpty.layout", m_container.GetContentListWidget());
			m_content = m_root.FindAnyWidget("Content");
			m_contentGrid = m_root.FindAnyWidget("ContentGrid");
			m_body = m_root.FindAnyWidget("Body");
			m_image_frame = m_root.FindAnyWidget("ImageFrame");
			m_header = TextWidget.Cast( m_root.FindAnyWidget("Header") );
			m_header_bg = m_root.FindAnyWidget("HeaderBar");
			if (m_content) m_content.GetScript(m_content_spacer);
			if (m_body) m_body.GetScript(m_body_spacer);

			if (m_header_bg) m_header_bg.SetHandler(this);
			if (m_header) m_header.SetHandler(this);
			InitIconGrid();
			return;
		}
		else
		{
			m_root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryViewHands.layout", m_container.GetContentListWidget());
			m_content = m_root.FindAnyWidget("Content");
			m_contentGrid = m_root.FindAnyWidget("ContentGrid");
			m_body = m_root.FindAnyWidget("Body");
			m_attachment_grid = m_root.FindAnyWidget("AttachmentGrid");
			m_image_frame = m_root.FindAnyWidget("ImageFrame");
			m_header = TextWidget.Cast( m_root.FindAnyWidget("Header") );
			m_header_bg = m_root.FindAnyWidget("HeaderBar");
			m_minimize_widget = m_root.FindAnyWidget("MinimizeWidget");
			m_maximize_widget = m_root.FindAnyWidget("MaximizeWidget");
			
			InventoryManager manager = InventoryManager.GetInstance();
			if (m_content) m_content.GetScript(m_content_spacer);
			if (m_body) m_body.GetScript(m_body_spacer);
			if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_spacer);
			
			InitAttachmentGrid( TYPE_PANEL_HANDS );
			InitIconGrid();
			InitCargoGrid(m_entity.GetInventory().GetCargo(), TYPE_PANEL_HANDS);

			string name = m_entity.GetDisplayName();	
			name.ToUpper();
			SetHeaderText(name);

			UpdateHeight( TYPE_PANEL_HANDS );

			// if container is expandable (minimize/maximize button is visible)
			m_minimize_widget.Show( false );
			m_maximize_widget.Show( false );
			m_IsExpandable = false;
			if ( m_entity.GetInventory().GetCargo() && m_entity.GetInventory().GetCargo().GetHeight() > 0 )
			{
				m_IsExpandable = true;
			}
			else if ( m_entity.GetSlotsCountCorrect() > 0 )
			{
				m_IsExpandable = true;
			}
			
			if ( IsHistoryExpanded() && m_IsExpandable )
			{
				m_minimize_widget.Show( false );
				m_maximize_widget.Show( true );
				m_body.Show( false );
			}
			else if ( m_IsExpandable )
			{
				m_minimize_widget.Show( true );
				m_maximize_widget.Show( false );
				m_body.Show( true );
			}
			else
			{
				m_body.Show( false );
			}

			if ( m_IsExpandable )
			{
				if (m_header_bg) m_header_bg.SetHandler(this);
				if (m_header) m_header.SetHandler(this);
				if (m_minimize_widget) m_minimize_widget.SetHandler(this);	
				if (m_maximize_widget) m_maximize_widget.SetHandler(this);
			}

			AlignToBottom();
			m_body_spacer.Update();
			m_content_spacer.Update();

			Widget container_root = m_root; //m_container.GetRootWidget();
			AutoHeightSpacer spacer;
			container_root.GetScript(spacer);
			if ( spacer )
			{
				spacer.Update();
			}
		}
	}
	
	override void UpdateItems()
	{
		AlignToBottom();
		if (m_entity == NULL) return;
		if (m_update_lock_timer.IsRunning()) return;	
		
		UpdateAttachmentGrid();
		
		int i;
		int c = m_cargos.Count();
		for (i = 0; i < c; i++)
		{
			UpdateCargoGrid(i);
		}

		UpdateIconGrid();

		if ( m_container_icon )
		{
			InventoryManager manager = InventoryManager.GetInstance();
			TextWidget item_quantity = TextWidget.Cast( m_container_icon.FindAnyWidget("Quantity") );
			if ( item_quantity )
			{
				item_quantity.SetText( manager.GetItemQuantityText( m_entity ) );
			}
		}

		UpdateHeight( TYPE_PANEL_HANDS );
	}
	
	override void InitIconGrid()
	{
		Widget imageFrame = m_image_frame.FindAnyWidget("ImageFrame");
		Widget imageGridW = m_image_frame.FindAnyWidget("Grid");
		InventoryManager manager = InventoryManager.GetInstance();
		if( !manager )
		{
			return;
		}

		float image_frame_width, image_frame_height;
		float icon_width, icon_height;
		float aw, ah;
		int item_w, item_h;
		int ww, hh;
		
		if ( m_entity == NULL )
		{
			if (imageGridW) imageGridW.Show(true);
			if (imageGridW)	imageGridW.GetScript(m_icon_grid);
			if (m_icon_grid)
			{
				m_icon_grid.SetController(this);
				m_image_frame.GetScreenSize( image_frame_width, image_frame_height );
				m_icon_grid.SetGridItemsSize( image_frame_width, image_frame_height );
				m_icon_grid.GenerateBackgroundTiles(1);
				m_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
			}
			return;
		}

		if( !m_entity.IsInherited(InventoryItem) ) 
		{
			if (imageGridW) imageGridW.Show(false);
			return;
		}

		if (imageGridW) imageGridW.Show(true);

		if (imageGridW)	imageGridW.GetScript(m_icon_grid);

		if (m_icon_grid)
		{
			m_icon_grid.SetController(this);

			
			vector rotation = Vector( 0,0,0 );

			// get sizes
			m_image_frame.GetScreenSize( image_frame_width, image_frame_height );
			GetGame().GetInventoryItemSize( InventoryItem.Cast( m_entity ), item_w, item_h );
			imageGridW.GetSize( icon_width, icon_height );
			m_image_frame.GetSize( aw, ah );

			ww = image_frame_width;
			hh = image_frame_height;
			// if ( item_h > item_w )
			// {
			// 	rotation = Vector( 0,0,90 );
			// }

			//LogInfo(ToString( "InitIconGridHands:") + m_entity.Ptr().GetName() + " item_w=" + itoa(item_w) + ",item_h=" + itoa( item_h) + ",aw=" + ftoa(aw) + ",ah=" + ftoa(ah) + ",ww=" + itoa(ww) + ",hh=" + itoa(hh) + ",cargo="+ToString(m_entity.Ptr().GetCargo() )+ ",attachments="+itoa(  m_entity.Ptr().AttachmentsCount() ) + ",attachments="+itoa(  m_entity.Ptr().GetSlotsCountCorrect() ) , LogTemplates.TEMPLATE_JANOSIK );
			// InitIconGridHands:'Combat Knife item_w=1,item_h=2,aw=1,ah=80,ww=383,hh=80,hands=140,cargo=NULL,attachments=0,attachments=0 | n/a 
			// FNX45 item_w=2,item_h=2,aw=0.3,ah=1,ww=113,hh=150,hands=140,cargo=NULL,attachments=1,attachments=3 | n/a
			// M4A1 item_w=6,item_h=3,aw=0.99,ah=120,ww=376,hh=120,hands=140,cargo=NULL,attachments=1,attachments=9 | n/a
			// Green Oil Barrel item_w=5,item_h=10,aw=0.3,ah=1,ww=113,hh=1500,hands=140,cargo=Cargo<44917b80>,attachments=0,attachments=0 | n/a 
			// Summer Gorka Military Pants item_w=2,item_h=1,aw=0.99,ah=120,ww=376,hh=48,hands=140,cargo=Cargo<475cacb0>,attachments=0,attachments=0 | n/a 
			// Combat Knife item_w=1,item_h=2,aw=0.99,ah=96,ww=376,hh=96,hands=140,cargo=NULL,attachments=0,attachments=0 | n/a
			// Small Fish Trap item_w=1,item_h=2,aw=0.3,ah=300,ww=113,hh=300,hands=140,cargo=Cargo<442a0ee0>,attachments=0,attachments=0 | n/a
			// Blue Mountain Backpack item_w=5,item_h=5,aw=1,ah=0.7,ww=383,hh=80,hands=140,cargo=Cargo<4aba9d80>,attachments=0,attachments=0 | n/a

			m_image_frame.SetSize( aw, ah );
			m_icon_grid.SetGridItemsSize( ww, hh );
			m_icon_grid.GenerateBackgroundTiles(1);
			m_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
			m_icon_grid.AddItem( InventoryItem.Cast( m_entity ), Vector(0, 1, 1), rotation );
		}
	}
	
	void AlignToBottom()
	{
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if ( menu )
		{
			Widget center_panel = menu.GetCenterPanel();
			if ( center_panel )
			{
				float w, h, cw, ch;
				m_root.GetSize( w, h );
				center_panel.GetSize( cw, ch );
				center_panel.SetSize( cw, h );
			}
		}
	}
	
	override void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();

		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if ( menu )
		{
			menu.HidePanelBorders();
			menu.ShowHandsPanelBorder();
		}
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		if ( item_in_hands == NULL && m_container.IsInherited(InventoryContainerHands) )
		{
			InventoryItem dragged_item = manager.GetItem();
			if (manager.IsDragging() == false || dragged_item == NULL) return;
			if (dragged_item && !(player.GetHumanInventory().GetEntityInHands() != dragged_item || player.GetHumanInventory().CanRemoveEntityInHands()))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			}
		}
		else
		{
			MoveItem(grid, row, col, true);
		}
	}
}
