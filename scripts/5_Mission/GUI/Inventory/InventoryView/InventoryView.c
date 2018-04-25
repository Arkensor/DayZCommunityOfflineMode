class InventoryView: InventoryViewBase
{
	protected InventoryGrid m_attachment_icon_grid;
	protected InventoryGrid m_attachment_slot_grid;
	protected ref map<EntityAI, InventoryGrid> m_attachment_grids;
	protected AutoHeightSpacer m_attachment_spacer;

	protected ref array<EntityAI> m_entities;
	protected ref map<EntityAI, ref InventoryViewBase> m_view_attachments_grids;	// inventory grids
	protected ref map<int, Widget> m_attachment_slots;							// attachment ghost icons
	protected AutoHeightSpacer m_content_grid_spacer;
	
	protected Widget m_attachment_grid;
	protected Widget m_header_bar;
	protected Widget m_header_bg;
	protected Widget m_close_button;
	protected string m_attachmentCategory;
	
	void InventoryView( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = entity;
		m_cargo_grids = new array<InventoryGrid>;
		m_attachment_grids = new map<EntityAI, InventoryGrid>;

		m_entities = new array<EntityAI>;
		m_view_attachments_grids = new map<EntityAI, ref InventoryViewBase>;
		m_attachment_slots = new map<int, Widget>;

		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
	}
	
	InventoryContainer GetContainer()
	{
		return m_container;
	}

	void ~InventoryView()
	{
		delete m_root;
		delete m_container_icon;
	}

	override void CreateLayout()
	{
		InventoryManager manager = InventoryManager.GetInstance();
		if( HasProperty( InventoryViewProperties.ATTACHMENT_CATEGORIES ) )
		{
			m_root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryViewRightAtt.layout", m_container.GetContentListWidget());
		}
		else
		{
			m_root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryViewRight.layout", m_container.GetContentListWidget());
		}
		m_content = m_root.FindAnyWidget("Content");
		m_attachment_grid = m_root.FindAnyWidget("AttachmentGrid");
		m_body = m_root.FindAnyWidget("Body");
		m_image_frame = m_root.FindAnyWidget("ImageFrame");
		m_header_bar = m_root.FindAnyWidget("HeaderBar");
		m_header = TextWidget.Cast( m_root.FindAnyWidget("Header") );

		m_contentGrid = m_root.FindAnyWidget("ContentGrid");
		m_close_button = m_root.FindAnyWidget("CloseButton");
		
		if (m_header_bar) m_header_bar.SetHandler(this);
		if (m_header) m_header.SetHandler(this);
		if (m_body) m_body.SetHandler(this);
		if (m_close_button) m_close_button.SetHandler(this);

		if (m_content) m_content.GetScript(m_content_spacer);
		if (m_body) m_body.GetScript(m_body_spacer);
		if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_spacer);
		if (m_contentGrid) m_contentGrid.GetScript(m_content_grid_spacer);

		if (m_entity == NULL) return;
		
		InitIconGrid();
		InitAttachmentGrid( TYPE_PANEL_DEFAULT );
		UpdateItems();
		InitCargoGrid(m_entity.GetInventory().GetCargo(), TYPE_PANEL_DEFAULT);

		string name = m_entity.GetDisplayName();	
		name.ToUpper();
		SetHeaderText(name);
		
		UpdateHeight( TYPE_PANEL_DEFAULT );

		// transparent background under item preview in view header
		Widget item_ww = m_root.FindAnyWidget("GridBck");
		if ( item_ww )
		{
			item_ww.SetColor( 0x00000000 );
		}

		// disable dragging for zombies
		if ( m_entity.IsInherited(ZombieBase) || m_entity.IsInherited(Car) )
		{
			if ( m_icon_grid )
			{
				Widget item_w = m_icon_grid.GetItemBackground(0);
				if ( item_w )
				{
					item_w.ClearFlags(WidgetFlags.DRAGGABLE);
				}
			}
		}
	}
	
	override void UpdateItems()
	{
		if (m_entity == NULL) return;
		if (m_update_lock_timer.IsRunning()) return;	
		InventoryManager manager = InventoryManager.GetInstance();
		
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
			TextWidget item_quantity = TextWidget.Cast( m_container_icon.FindAnyWidget("Quantity") );
			if ( item_quantity )
			{
				item_quantity.SetText( manager.GetItemQuantityText( m_entity ) );
			}	
		}

		UpdateHeight( TYPE_PANEL_DEFAULT );
		
		m_body_spacer.Update();
		m_content_spacer.Update();
	}
	
	void UpdateHeight( int panel_type )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		if( !manager )
		{
			return;
		}
		
		int cargo_item_height = manager.GetNormalItemSize();
		int icon_height = manager.GetAttachmentItemSize();
		int max_grid_width = manager.MAX_ATTACHMENT_GRID_WIDTH;
		if ( panel_type == TYPE_PANEL_HANDS )
		{
			cargo_item_height = manager.GetHandsNormalItemSize();
			icon_height = manager.GetHandsAttachmentItemSize();
			max_grid_width = manager.MAX_HANDS_ATTACHMENT_GRID_WIDTH;
		}

		// update size
		int cargo_height = 0;
		int attachment_height = 0;
		if ( m_entity.GetInventory().GetCargo() && m_entity.GetInventory().GetCargo().GetHeight() > 0 && HasProperty( InventoryViewProperties.CARGOS ) )
		{
			cargo_height = m_entity.GetInventory().GetCargo().GetHeight();
			cargo_height = cargo_height * ( cargo_item_height + manager.GRID_CARGO_GAP );
		}
		int ac = m_entity.GetInventory().AttachmentCount();
		int sc = m_entity.GetSlotsCountCorrect();

		/*
		if ( sc > 0 && !IsInherited(m_entity.Ptr(), ZombieBase) )
		{
			float x, y;
			m_attachment_grid.GetPos(x, y);
			m_attachment_grid.SetPos(x, cargo_height );
			attachment_height = ac;
			if ( attachment_height >= max_grid_width )
			{
				attachment_height = attachment_height / max_grid_width;
				attachment_height = attachment_height + 1;
			}
			else
			{
				attachment_height = 1;
			}
			attachment_height = attachment_height * icon_height;
		}
		int height = cargo_height + attachment_height;
		*/
			
		float ww, hh;
		m_contentGrid.GetSize(ww, hh);
		int height = cargo_height + hh;
		if ( cargo_height && attachment_height )
		{
			height = height + m_content_spacer.Gap;
		}
		m_body_spacer.MinHeight = height;
		m_body_spacer.MaxHeight = height;
	}
	
	override void SetHeaderText(string text)
	{
		if (m_header) m_header.SetText(text);
	}
	
	override void AddProxyCargo(Cargo proxyCargo)
	{
		if (m_cargos.Find(proxyCargo) == INDEX_NOT_FOUND)
		{
			InitCargoGrid(proxyCargo, TYPE_PANEL_DEFAULT);
		}
	}
	
	override void UpdateLock()
	{
		m_update_lock_timer.Run(0.5, this, "UpdateUnlock", NULL, false);
	}
	
	override void UpdateUnlock()
	{
		UpdateItems();
	}

	// return array of all attachments slots for entity
	array<string> GetItemSlots(EntityAI e)
	{
		TStringArray searching_in = new TStringArray;
		searching_in.Insert(CFG_VEHICLESPATH);
		searching_in.Insert(CFG_WEAPONSPATH);
		searching_in.Insert(CFG_MAGAZINESPATH);

		array<string> attachments_slots	= new array<string>;
		
		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string cfg_name = searching_in.Get(s);
			string path = cfg_name + " " + e.GetType();   

			if ( GetGame().ConfigIsExisting( path ) )
			{
				g_Game.ConfigGetTextArray(path + " attachments", attachments_slots);
				if ( e.IsWeapon() )
				{
					attachments_slots.Insert("magazine");
				}
				return attachments_slots;
			}
		}
		if ( e.IsWeapon() )
		{
			attachments_slots.Insert("magazine");
		}
		return attachments_slots;
	}
	
	void SetAttachmentCategory( string att_cat )
	{
		m_attachmentCategory = att_cat;
	}
	
	override void InitAttachmentGrid( int panel_type )
	{
		if ( !HasProperty(InventoryViewProperties.ATTACHMENTS) ) return;

		InventoryManager manager = InventoryManager.GetInstance();
		int i;
		int ac = m_entity.GetInventory().AttachmentCount();
		int	sc = m_entity.GetSlotsCountCorrect();
		
		// get array of all attachment slots for entity
		ref array<string> attachments_slots = new array<string>;
		ref array<string> player_ghosts_slots = new array<string>;
		attachments_slots = GetItemSlots( m_entity );

		string icon_name;
		string slot_name;
		string path;
		Widget ghost_widget;
		ImageWidget ghost_icon;
		int slot_id;
		int count;
		
		array<string> slots;
		
		// create ghost icon for attachment slots
		if ( attachments_slots )
		{
			if( HasProperty( InventoryViewProperties.ATTACHMENT_CATEGORIES ) )
			{
				if ( m_contentGrid ) m_contentGrid.Show( true );

				string config_path_ghosts_slots2 = "CfgVehicles " + m_entity.GetType() + " GUIInventoryAttachmentsProps " + m_attachmentCategory + " attachmentSlots";
				GetGame().ConfigGetTextArray ( config_path_ghosts_slots2, player_ghosts_slots );
				
				slots = player_ghosts_slots;
				count = player_ghosts_slots.Count();
				path = "CfgSlots" + " Slot_";
			}
			else
			{
				slots = attachments_slots;
				count = attachments_slots.Count();
				path = "CfgSlots" + " ";
				
				if ( count > 0 ) 
				{
					if ( m_contentGrid ) m_contentGrid.Show( true );
				}
			}
			
			for ( i = 0; i < count; i++ )
			{
				slot_name = slots.Get ( i );
				
					GetGame().ConfigGetText(path + " ghostIcon", icon_name);
					icon_name = "missing";
			
					ghost_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGhost.layout", m_contentGrid );
					ghost_widget.Show(true);
					ghost_widget.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
					ghost_icon = ImageWidget.Cast( ghost_widget.FindAnyWidget("GhostIcon") );
					if ( ghost_icon )
					{
						ghost_icon.LoadImageFile(0,"set:dayz_inventory image:" + icon_name );
					}
	
					GetGame().ConfigGetText(path + " name", slot_name);
					slot_id = InventorySlots.GetSlotIdFromString( slot_name );
					m_attachment_slots.Set( slot_id, ghost_widget );
				}
			}
		
		/*
		InventoryViewGrid view;
		int c = m_entity.Ptr().GetInventory().AttachmentCount();
		for (i = 0; i < c; i++)
		{
			EntityAI entity = m_entity.Ptr().GetAttachmentFromIndex(i);
			if (entity ) // && entity.IsInventoryItem()
			{
				if ( m_view_attachments_grids.Contains(entity) == false )
				{	
					int attachment_slot_id = entity.GetSlotId();
					bool is_valid_slot = m_attachment_slots.Contains( attachment_slot_id );
					if ( is_valid_slot )
					{
						view = new InventoryViewGrid(m_container, entity, 0 );
						view.SetRelatedToView( true, this );
						//added_item = true;
						//GetContentGridWidget().RemoveChild( m_inventory_slots.Get(attachment_slot_id) );
						m_attachment_slots.Get(attachment_slot_id).Show( false ); //hide ghost icon
						view.CreateLayout();
						m_view_attachments_grids.Set(entity, view );
					}
				}
				m_entities.Insert(entity);
			}
		}

		int item_width = manager.GetAttachmentItemSize();
		int item_height = manager.GetAttachmentItemSize();
		int max_grid_width = manager.MAX_ATTACHMENT_GRID_WIDTH;
		if ( panel_type == TYPE_PANEL_HANDS )
		{
			item_width = manager.GetHandsAttachmentItemSize();
			item_height = manager.GetHandsAttachmentItemSize();
			max_grid_width = manager.MAX_HANDS_ATTACHMENT_GRID_WIDTH;
		}

		bool is_binocular = m_entity.Ptr().IsInherited(BinocularsBase);

		// weapons has extra slot for muzzle, but binoculars inherit from weapon too, but dont have this extra slot... cool
		if (is_binocular == false)
		{
			if (m_entity.Ptr().IsWeapon())
			{
				// weapons muzzles fix
				Weapon weapon = m_entity.Ptr();
				sc += weapon.GetMuzzleCount();
			}
				
			if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
		}
		
		if (sc == 0) return;
		
		if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_icon_grid);
		if ( m_attachment_grid )
		{
			for (int i = 0; i < ac; i++)
			{	
				Widget attachment_icon;
				attachment_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_attachment_grid );

				if (attachment_icon) attachment_icon.GetScript( m_attachment_slot_grid );
				if ( m_attachment_slot_grid )
				{
					EntityAI entity = m_entity.Ptr().GetAttachmentFromIndex(i);
					m_attachment_grids.Set( entity, m_attachment_slot_grid );

					if ( entity )
					{
						m_attachment_slot_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
						m_attachment_slot_grid.SetGridSize(1,1);
						m_attachment_slot_grid.SetController( this );		
						m_attachment_slot_grid.GenerateBackgroundTiles(1);
						m_attachment_slot_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
						m_attachment_slot_grid.AddItem( entity, Vector(0, 1, 1), Vector(0,0,0) );
					}
				}
			}

			if ( ac < sc )
			{
				m_empty_attachment_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout", m_attachment_grid );
				m_empty_attachment_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				m_empty_attachment_slot.SetHandler(this);
			}
		}
		*/
		UpdateAttachmentGrid();
	}
	
	override void InitIconGrid()
	{
		Widget imageGridW = m_image_frame.FindAnyWidget("Grid");
		Widget imageW = m_image_frame.FindAnyWidget("Icon");
		InventoryManager manager = InventoryManager.GetInstance();
		
		if ( imageGridW )
		{
			if( HasProperty( InventoryViewProperties.ATTACHMENT_CATEGORIES ) )
			{
				imageGridW.Show(false);
			}
			else
			{
				imageGridW.Show(true);
			}
		}
		if (imageW) imageW.Show(false);
		if (imageGridW)	imageGridW.GetScript(m_icon_grid);
		// list view
		if (m_icon_grid)
		{
			m_icon_grid.SetController(this);
			float x,y;
			m_image_frame.GetSize( x, y );
			m_icon_grid.SetGridItemsSize( x, manager.GetIconItemSize() );
			m_icon_grid.SetXShift( -( ( x / 2 ) - ( manager.GetIconItemSize() / 2 ) ) );
			m_icon_grid.GenerateBackgroundTiles(1);
			m_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
			m_icon_grid.AddItem( InventoryItem.Cast( m_entity ), Vector(0, 1, 1), Vector(0,0,0) );
		}
	}
	
	override void InitCargoGrid(Cargo cargo, int panel_type)
	{
		if ( !HasProperty(InventoryViewProperties.CARGOS) ) return;
		
		if (cargo)
		{
			InventoryManager manager = InventoryManager.GetInstance();
			InventoryGrid cargo_grid;

			m_cargos.Insert(cargo);
			
			Widget cargoGridW;
			if (m_container && m_container.IsOwnedByPlayer())
			{
				cargoGridW = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridRight.layout", m_content);
			}
			else
			{
				cargoGridW = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGrid.layout", m_content);
			}

			if (cargoGridW) cargoGridW.GetScript(cargo_grid);		
			if (cargo_grid)
			{
				m_cargo_grids.Insert(cargo_grid);
				int width = manager.GetNormalItemSize();
				if ( panel_type == TYPE_PANEL_HANDS )
				{
					width = manager.GetHandsNormalItemSize();
				}
				cargo_grid.SetGridItemsSize( width, width );
				cargo_grid.SetGridGapSize(manager.GRID_CARGO_GAP);
				cargo_grid.SetGridSize(cargo.GetWidth(), cargo.GetHeight());
				cargo_grid.SetController(this);
				cargo_grid.GenerateBackgroundTiles(cargo.GetWidth() * cargo.GetHeight());
				cargo_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				
				UpdateCargoGrid(m_cargos.Count() - 1);
			}
		}
	}
	
	override void UpdateAttachmentGrid()
	{
		if ( !HasProperty(InventoryViewProperties.ATTACHMENTS) ) return;
	
		AutoHeightSpacer spacer;
	
		InventoryManager manager = InventoryManager.GetInstance();

		int i, c;
		InventoryItemBase item;
		int sc = m_entity.GetSlotsCountCorrect();
		int ac = m_entity.GetInventory().AttachmentCount();
		
		Widget attachment_icon;

		m_entities.Clear();

		InventoryViewGridView view_grid;
		EntityAI entity, attachment_entity;
		int attachment_slot_id;
		bool is_slot_occupied;

		// create InventoryViewGrid for attachments ( if it's not created already )
		int y = m_entity.GetInventory().AttachmentCount();
		for (i = 0; i < m_entity.GetInventory().AttachmentCount(); i++)
		{
			attachment_entity = m_entity.GetInventory().GetAttachmentFromIndex(i);
			if (attachment_entity ) // && attachment_entity.IsInventoryItem()
			{
				if ( m_view_attachments_grids.Contains(attachment_entity) == false )
				{	
					attachment_slot_id = attachment_entity.GetInventory().GetSlotId();
					is_slot_occupied = m_attachment_slots.Contains( attachment_slot_id );
					//PrintString("SLOT " + itoa(attachment_slot_id) + " = " +  itoa(is_slot_occupied) );
					// check if attachment slot is occupied
					if ( is_slot_occupied )
					{
						m_attachment_slots.Get(attachment_slot_id).Show( false ); //hide ghost icon

						view_grid = new InventoryViewGridView(m_container, attachment_entity, 0 );
						view_grid.SetView( this ); // so we can distinguish different behaviour for view_grid or container
						view_grid.CreateLayout();
						m_view_attachments_grids.Set(attachment_entity, view_grid );

						
						//added_item = true;
					}
				}
				m_entities.Insert( attachment_entity );
			}
		}

		// remove unnecesary attachments
		c = m_view_attachments_grids.Count();
		for ( i = 0; i < c; i++ )
		{
			attachment_entity = m_view_attachments_grids.GetKey(i);
			m_view_attachments_grids.GetElement(i).UpdateIconGrid();
			if( attachment_entity )
			{
				if ( m_entities.Find( attachment_entity ) == INDEX_NOT_FOUND )
				{
					m_view_attachments_grids.RemoveElement(i);
					if( attachment_entity )
					{
						//show ghost icon
						attachment_slot_id = attachment_entity.GetInventory().GetSlotId();
						m_attachment_slots.Get(attachment_slot_id).Show( true );
					}
					c--;
					i--;
				}
			}
		}
		
		if (m_attachment_spacer) m_attachment_spacer.Update();

		m_body_spacer.Update();
		m_content_spacer.Update();
		m_contentGrid.Update();
		
		m_contentGrid.GetScript(spacer);
		if ( spacer )
		{
			spacer.Update();
		}

		Widget container_root = m_root; //m_container.GetRootWidget();
		container_root.GetScript(spacer);
		if ( spacer )
		{
			spacer.Update();
		}

		return;
		/*
		if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_icon_grid);
		if ( m_attachment_grid )
		{
			for ( i = 0; i < ac; i++)
			{	
				Widget attachment_icon;
				attachment_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_attachment_grid );

				if (attachment_icon) attachment_icon.GetScript( m_attachment_slot_grid );
				if ( m_attachment_slot_grid )
				{
					EntityAI entity = m_entity.Ptr().GetAttachmentFromIndex(i);
					m_attachment_grids.Set( entity, m_attachment_slot_grid );

					if ( entity )
					{
						m_attachment_slot_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
						m_attachment_slot_grid.SetGridSize(1,1);
						m_attachment_slot_grid.SetController( this );		
						m_attachment_slot_grid.GenerateBackgroundTiles(1);
						m_attachment_slot_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
						m_attachment_slot_grid.AddItem( entity, Vector(0, 1, 1), Vector(0,0,0) );
						PrintString("SLOT ID = " + ToString( entity.GetSlotId() ) );
					}
				}
			}

			if ( ac < sc )
			{
				m_empty_attachment_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout", m_attachment_grid );
				m_empty_attachment_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				m_empty_attachment_slot.SetHandler(this);
			}
		}
		*/

		// get list of all attachments
		for (i = 0; i < ac; i++)
		{
			entity = m_entity.GetInventory().GetAttachmentFromIndex(i);
			m_entities.Insert(entity);
			
			// add new attachments
			if ( m_attachment_grids.Contains(entity) == false)
			{

				// InventoryViewBase view = new InventoryViewGrid(m_container, entity, 0 );
				// view.CreateLayout();
				attachment_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_attachment_grid );
				if (attachment_icon) attachment_icon.GetScript( m_attachment_slot_grid );
				if ( m_attachment_slot_grid )
				{
					EntityAI attachment_entity2 = m_entity.GetInventory().GetAttachmentFromIndex(i);
					m_attachment_grids.Set( attachment_entity2, m_attachment_slot_grid );

					if ( attachment_entity2 )
					{
						m_attachment_slot_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
						m_attachment_slot_grid.SetGridSize(1,1);
						m_attachment_slot_grid.SetController( this );		
						m_attachment_slot_grid.GenerateBackgroundTiles(1);
						m_attachment_slot_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
						m_attachment_slot_grid.AddItem( InventoryItem.Cast( attachment_entity2 ), Vector(0, 1, 1), Vector(0,0,0) );

						/*
						int attachment_slot_id = attachment_entity2.GetSlotId();
						bool is_valid_slot = m_attachment_slots.Contains( attachment_slot_id );
						if ( is_valid_slot )
						{
							m_attachment_slots.Get(attachment_slot_id).Show( false );
						}
						*/
					}
				}
			}
			else
			{			
				attachment_icon = m_attachment_grids.Get(entity).GetRoot();
			}

			// quantity
			/*
			if ( attachment_icon )
			{
				Widget quantity_panel = attachment_icon.FindAnyWidget("QuantityPanel");
				TextWidget item_quantity = attachment_icon.FindAnyWidget("Quantity");
				ProgressBarWidget quantity_progress = attachment_icon.FindAnyWidget("QuantityBar");
				Widget quantity_stack = attachment_icon.FindAnyWidget("QuantityStackPanel");
				if ( quantity_progress ) quantity_progress.Show( false );
				if ( quantity_stack ) quantity_stack.Show( false );

				int has_quantity = manager.HasItemQuantity( entity );
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					if ( quantity_panel ) quantity_panel.Show( false );
				}
				else
				{
					if ( quantity_panel ) quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT && item_quantity )
					{
						item_quantity.SetText( manager.GetItemQuantityText( entity ) );
						quantity_stack.Show( true );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						if ( quantity_progress )
						{
							float progress_max = quantity_progress.GetMax();
							int max = entity.ConfigGetInt("stackedMax");
							int count = entity.ConfigGetInt("count");
							float quantity = manager.GetItemQuantity( entity );
							if ( count > 0 )
							{
								max = count;
							}
							if ( max > 0 )
							{

								float value = round( ( quantity / max ) * 100 );
								// LogInfo( " ==" + ftoa( value ), LogTemplates.TEMPLATE_JANOSIK );
								quantity_progress.SetCurrent( value );
							}
							quantity_progress.Show( true );
						}
					}
				}

				// temperature
				Widget item_w = attachment_icon.FindAnyWidget("GridItem");
				if ( item_w )
				{
					int color = GetItemColor( this, entity);
					if ( color )
					{
						item_w.SetColor( color );
					}
				}
			}
			*/
		}
		
		

		/*
		ac = m_entity.Ptr().GetInventory().AttachmentCount();
		if ( ac == sc )
		{
			if (m_empty_attachment_slot) m_empty_attachment_slot.Destroy();
			m_empty_attachment_slot = NULL;
		}
		else if ( m_empty_attachment_slot == NULL )
		{
			m_empty_attachment_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout", m_attachment_grid );
			m_empty_attachment_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
			m_empty_attachment_slot.SetHandler(this);
		}
		*/

		if (m_attachment_spacer) m_attachment_spacer.Update();

		m_body_spacer.Update();
		m_content_spacer.Update();

		container_root = m_root; //m_container.GetRootWidget();
		container_root.GetScript(spacer);
		if ( spacer )
		{
			spacer.Update();
		}
	}
	
	override void UpdateIconGrid()
	{
		if ( m_icon_grid )
		{
			m_icon_grid.UpdateQuantityItems();
			m_icon_grid.UpdateTemperatureItems();
		}

		if ( m_container_icon_grid )
		{
			m_container_icon_grid.UpdateQuantityItems();
			m_container_icon_grid.UpdateTemperatureItems();
		}
	}
	
	override void UpdateCargoGrid(int cargo_index)
	{
		if ( !HasProperty(InventoryViewProperties.CARGOS) ) return;
		
		if (cargo_index < m_cargos.Count())
		{
			int c, i, index, x, y, w, h;
			InventoryItemBase item;
			Cargo cargo = m_cargos.Get(cargo_index);
		
			InventoryGrid cargo_grid = m_cargo_grids.Get(cargo_index);
			if (cargo && cargo_grid)
			{
				m_items.Clear();
				c = cargo.GetItemCount();
				
				for (i = 0; i < c; i++)
				{
					item = InventoryItemBase.Cast( cargo.GetItem(i) );
					cargo.GetItemPos(i, x, y);
					cargo.GetItemSize(i, w, h);
					index = x + (y * cargo.GetWidth());
					m_items.Set(item, Vector(index, w, h));
				}
				
				cargo_grid.UpdateItems(m_items,true,true);
			}
		}
	}
	
	override InventoryItem FindItem(InventoryGrid grid, int row, int col)
	{
		if (m_entity == NULL || grid == NULL) return NULL;

		if (grid == m_icon_grid)
		{
			return InventoryItem.Cast( m_entity );		
		}
		
		if (grid == m_container_icon_grid)
		{
			return InventoryItem.Cast( m_entity );	
		}

		int cargo_index = FindCargoIndex(grid);
		// use cargo index here
		if (cargo_index != INDEX_NOT_FOUND)	return InventoryItem.Cast( m_cargos.Get(cargo_index).FindEntityInCargoOn(row, col) );

		EntityAI entity = m_attachment_grids.GetKeyByValue( grid );
		if ( entity ) return InventoryItem.Cast( entity );
		
		return NULL;
	}
	
	override int FindCargoIndex(InventoryGrid grid)
	{
		for (int cargo_index = 0; cargo_index < m_cargo_grids.Count(); cargo_index++)
		{
			InventoryGrid cargo_grid = m_cargo_grids.Get(cargo_index);
			if (grid == cargo_grid)
			{
				return cargo_index;
			}
		}
		
		return INDEX_NOT_FOUND;
	}
	
	override bool MoveItem(InventoryGrid grid, int row, int col, bool testOnly)
	{
		if (m_entity == NULL || grid == NULL) return false;

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		InventoryGrid gridSrc = manager.GetGrid();
		int index_from, index_to, flags;
		vector data;
		bool weaponInHandsOp = false;
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;	
		
		if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
		{
			if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
		}
		
		Magazine dragged_mag;
		if (Class.CastTo(dragged_mag, dragged_item))
		{
			InventoryLocation curr = new InventoryLocation;
			if (dragged_mag.GetInventory().GetCurrentInventoryLocation(curr))
			{
				EntityAI hnd_item = player.GetHumanInventory().GetEntityInHands();
				if (curr.GetType() == InventoryLocationType.ATTACHMENT && curr.GetParent() == hnd_item)
					weaponInHandsOp = true;
			}
		}
		
		if (grid == m_icon_grid || grid == m_container_icon_grid)
		{
			// dragging icon
			if (dragged_item == m_entity)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
				return false;
			}
			
			flags = manager.GetCombinationFlags(m_entity, dragged_item);
			if (flags == InventoryCombinationFlags.NONE)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
			}
			else if (flags == InventoryCombinationFlags.ADD_AS_ATTACHMENT || flags == InventoryCombinationFlags.ADD_AS_CARGO || flags == InventoryCombinationFlags.LOAD_CHAMBER || flags == InventoryCombinationFlags.ATTACH_MAGAZINE )
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				else
				{
					if (grid.DebugOutput) player.MessageStatus(m_entity.GetDisplayName() + " :TakeEntityAsOwned( " + dragged_item.GetDisplayName() + " )");

					if (gridSrc) gridSrc.RemoveItem(dragged_item);
							
					player.PredictiveTakeEntityToTargetAttachment(m_entity, dragged_item);
				}
				return true;
			}
			else if (flags == InventoryCombinationFlags.TAKE_TO_HANDS)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_SWAP);
				else
				{
					manager.TakeToHands(dragged_item);
				}
				return true;
			}
			else
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_MULTIPLE);
				else manager.ShowActionMenuCombine(itemAtPos, dragged_item, flags);
				return true;
			}
		}
		else if (itemAtPos && dragged_item != itemAtPos)
		{
			flags = manager.GetCombinationFlags(itemAtPos, dragged_item);

			if (flags == InventoryCombinationFlags.NONE)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
			}
			else if (flags == InventoryCombinationFlags.SWAP)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_SWAP);
				else
				{
					if (grid.DebugOutput) player.MessageStatus("Swap: " + dragged_item.GetDisplayName() + " , " + itemAtPos.GetDisplayName());
					
					if (grid.HasItem(dragged_item))
					{
						gridSrc = grid;
					}
					
					if (gridSrc) index_from = Math.Round(gridSrc.GetItems().Get(dragged_item)[0]); 				
					index_to = Math.Round(grid.GetItems().Get(itemAtPos)[0]);				
									
					if (gridSrc) gridSrc.RemoveItem(dragged_item);	
					grid.RemoveItem(itemAtPos);
					
					if (gridSrc) 
					{
						data = gridSrc.GetController().GetItemSize(gridSrc, itemAtPos);
						data[0] = index_from;
						gridSrc.AddItem(itemAtPos, data, Vector(0,0,0) );
					}
					
					data = grid.GetController().GetItemSize(grid, dragged_item);
					data[0] = index_to;				
					grid.AddItem(dragged_item, data, Vector(0,0,0) );
					
					player.PredictiveSwapEntities(dragged_item, itemAtPos);
				}
				return true;
			}
			else
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_MULTIPLE);
				else
				{
					manager.ShowActionMenuCombine(itemAtPos, dragged_item, flags);
					gridSrc.RemoveItem(dragged_item);
				}
				return true;
			}
		}
		/*
		else if (grid == m_attachment_grid) 
		{
			// dragging over empty attachment slot
			if (m_entity.Ptr().CanAddAttachment(dragged_item))
			{
				if (testOnly) 
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				}
				else
				{
					if (grid.DebugOutput) player.MessageStatus(m_entity.Ptr().GetName() + " :TakeEntityAsAttachment( " + dragged_item.GetName() + " )");
					
					if (gridSrc) gridSrc.RemoveItem(dragged_item);
					
					data = grid.GetController().GetItemSize(grid, dragged_item);
					data[0] = grid.GetIndex(row, col);
					grid.AddItem(dragged_item, data, Vector(0,0,0) );				
					
					m_entity.Ptr().TakeEntityAsOwned(dragged_item);
				}
				return true;
			}
		}
		*/
		else
		{
			// dragging over empty cargo slot
			int cargo_index = FindCargoIndex(grid);
			int cargo_idx = 0;
			
			if (cargo_index != INDEX_NOT_FOUND)	
			{
				Cargo cargo = m_cargos.Get(cargo_index);
				
				// use cargo index here
				if (cargo.GetParent().GetInventory().CanAddEntityInCargoEx(dragged_item, cargo_idx, row, col))			
				{
					if (testOnly)
					{
						manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
					}
					else
					{
						if (grid.DebugOutput) player.MessageStatus(m_entity.GetDisplayName() + " :TakeEntityToCargoEx( " + dragged_item.GetDisplayName() + " )");
						
						if (gridSrc) 
						{
							if (dragged_item.GetHierarchyParent() != m_entity) 
							{
								if( !gridSrc.IsInherited( InventoryGridIcon ) )
								gridSrc.RemoveItem(dragged_item);
							}
							else
							{
								grid.RemoveItem(dragged_item);
							}
						}
						
					
						data = grid.GetController().GetItemSize(grid, dragged_item);
						data[0] = grid.GetIndex(row, col);
						grid.AddItem(dragged_item, data, Vector(0,0,0) );			
							
						if (weaponInHandsOp)
						{
							InventoryLocation next = new InventoryLocation;
							next.SetCargo(m_entity, dragged_mag, 0, row, col);
							player.GetWeaponManager().DetachMagazine(next);
						}
						else
							player.PredictiveTakeEntityToTargetCargoEx(cargo.GetParent(), dragged_item, cargo_idx, row, col);
					}
					return true;
				}
			}		
		}
		
		if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		return false;
	}
	
	override protected bool HasProperty(int mask) 
	{
		if (m_properties & mask) return true;
		return false;	
	}	
	
	override EntityAI GetEntity() 
	{
		return m_entity;
	}
	
	void Hide()
	{
		m_root.Show( false );
		m_body_spacer.Update();
		m_content_spacer.Update();
		
		Widget container_root = m_root; //m_container.GetRootWidget();
		AutoHeightSpacer spacer;
		container_root.GetScript( spacer );
		spacer.Update();
		m_container.Ref();
	}
	
	// ScriptedWidgetEventHandler override
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( w == m_close_button )
		{
			if  ( m_body && m_body_spacer && m_content_spacer )
			{
				InventoryContainerAttachmentCategories att_cat_cont = InventoryContainerAttachmentCategories.Cast( m_container );
				if( att_cat_cont.IsInherited( InventoryContainerAttachmentCategories ) )
				{
					Hide();
					att_cat_cont.Remove( m_attachmentCategory );
					delete this;
				}
				else
				{
					m_container.ToggleExpandView( m_entity );
				}	
			
			}
		}
		return true;
	}
		
	// InventoryGridController override
	override void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem item = FindItem( grid, row, col );
		if ( item )
		{
			manager.ShowTooltip( item );
		}
	}
	
	override void OnItemLeave(InventoryGrid grid, Widget w)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		manager.HideTooltip();
	}
	
	private InventoryItem m_item;
	override void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_entity == NULL) return;
		
		InventoryManager manager = InventoryManager.GetInstance();
		m_item = FindItem(grid, row, col);
		manager.SetDragGrid(this);
		
		manager.Drag(InventoryDragFromType.GRID, m_item, grid);
	}
	
	override void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();


		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if ( menu )
		{
			menu.HidePanelBorders();
			if ( m_container == menu.m_equipment_container )
			{
				menu.ShowRightPanelBorder();
			}
			else if ( m_container == menu.m_ground_container )
			{
				menu.ShowLeftPanelBorder();
			}
		}

		// disable dragging on zombie
		if (m_entity.IsInherited(ZombieBase) && grid == m_icon_grid )
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		if ( item_in_hands == NULL && m_container.IsInherited(InventoryContainerHands) )
		{
			InventoryItem dragged_item = manager.GetItem();
			if (manager.IsDragging() == false || dragged_item == NULL) return;
			if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
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
	
	override void OnItemDrop(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryItem itemAtPos = FindItem(grid, row, col);
		InventoryManager manager = InventoryManager.GetInstance();
		manager.Drop();
		manager.HideTooltip();
		InventoryView view = manager.GetDragGrid();
		view.UpdateItems();
		UpdateLock();

		if ( itemAtPos && ( itemAtPos.GetSlotsCountCorrect() > 0 ||  itemAtPos.GetInventory().GetCargo() ) )
		{
  			manager.HideTooltip();
			InventoryContainerEntity container = InventoryContainerEntity.Cast( m_container );
			container.HideExpandView( itemAtPos );
		}
	}

	override void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		
		// disable dropping on zombie
		if ( m_entity && m_entity && grid == m_icon_grid && m_entity.IsInherited(ZombieBase)  )
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		if ( item_in_hands == NULL && m_container.IsInherited(InventoryContainerHands) )
		{
			InventoryItem dragged_item = manager.GetItem();
			
			if (manager.IsDragging() == false || dragged_item == NULL) return;
			
			if ( player.GetHumanInventory().CanAddEntityInHands(dragged_item) && (player.GetHumanInventory().GetEntityInHands() != dragged_item || player.GetHumanInventory().CanRemoveEntityInHands()))
			{
				manager.TakeToHands(dragged_item);
				manager.Drop();	
			}
		}
		else
		{
			MoveItem(grid, row, col, false);
			manager.Drop();
			UpdateLock();
		}
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if ( menu )
		{
			menu.HidePanelBorders();
		}
	}

	override void OnItemLeftClick(InventoryGrid grid, Widget w, int row, int col)
	{
	}

	override void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryItem itemAtPos = FindItem(grid, row, col);
		if (m_container && itemAtPos) m_container.OnItemDoubleClick(this, itemAtPos);
		InventoryManager manager = InventoryManager.GetInstance();	
		manager.HideTooltip();
	}	
	
	override vector GetItemSize(InventoryGrid grid, InventoryItem item)
	{
		int w = 1;
		int h = 1;
		
		if (grid != m_container_icon_grid) 
		{
			GetGame().GetInventoryItemSize(item, w, h);
		}
		
		return Vector(0, w, h);
	}

	override int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		bool is_view = grid.IsInherited( InventoryView );
		if ( dragged_item ) return 0;
		// int color = InventoryGrid.ITEM_COLOR_NORMAL;
		int color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
		if ( is_view ) 
		{
			color = InventoryGrid.ITEM_COLOR_GRID;
		}

		// if item icon is main icon (view, viewgrid )
		if ( item.IsInherited( InventoryItem) && item.GetHierarchyParent() && item.GetHierarchyParent() != player )
		{
			color = InventoryGrid.ITEM_COLOR_NORMAL;
		}

		if( item.IsInherited( InventoryItem)  ) 
		{
			ItemBase item_base = ItemBase.Cast( item );
			float temperature = item_base.GetTemperature();
			if ( temperature )
			{
				color = GetTemperatureColor( temperature );
			}
		}
		return color;
	}
	
	override string GetItemQuantityText( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		string quantity_text = manager.GetItemQuantityText( item );
		return quantity_text;
	}

	override int HasItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		
		if( manager )
		{
			int has_quantity = manager.HasItemQuantity( item );
			return has_quantity;
		}
		return QUANTITY_HIDDEN;
	}

	override float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}
	
	protected const int TYPE_PANEL_DEFAULT = 0;
	protected const int TYPE_PANEL_HANDS = 1;

	// history saving
	protected static const int EXPANDED_HISTORY_SIZE = 20;
	protected static int s_expanded_history[EXPANDED_HISTORY_SIZE];
	protected static int s_expanded_history_index = 0;
	
	bool IsHistoryExpanded()
	{
		if (m_entity == NULL) return false;
		
		int id = m_entity.GetID();
		
		for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		{
			if (s_expanded_history[i] == id) 
			{
				return true;
			}
		}
		
		return false;
	}

	static void ClearHistory()
	{
		for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		{
			s_expanded_history[i] = -1;
		}
		s_expanded_history_index = 0;
	}

	/*
	void SaveHistory()
	{
		if (m_entity.IsNull()) return;
		
		if  ( m_body && m_body_spacer && m_content_spacer )
		{
			if (m_body_spacer.MaxHeight == 0)
			{
				s_expanded_history[s_expanded_history_index] = m_entity.Ptr().GetID();
				s_expanded_history_index++;
			}		
		}
	}
	*/

	// ScriptedWidgetEventHandler override	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		OnItemDraggingOver( m_icon_grid, w, 0, 0 );
		return true;
	}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		OnItemDropReceived( m_icon_grid, w, 0, 0 );
		return true;
	}
	
}
