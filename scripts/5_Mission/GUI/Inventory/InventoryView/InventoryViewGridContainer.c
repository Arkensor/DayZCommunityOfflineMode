class InventoryViewGridContainer: InventoryViewGridBase
{
	void InventoryViewGridContainer ( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = entity;
		m_cargo_grids = new array<InventoryGrid>;
		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
		m_view_related = false;
	}

	override void InitIconGrid()
	{
		InventoryManager manager = InventoryManager.GetInstance();
		Widget content_grid_widget;

		// grid view
		if ( m_container.GetContentGridWidget() )
		{
			content_grid_widget = m_container.GetContentGridWidget();
			m_container_icon = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/inventory/inventoryIcon.layout", content_grid_widget );
			if ( m_container_icon ) m_container_icon.GetScript( m_container_icon_grid );		
			if ( m_container_icon_grid )
			{
				m_container_icon_grid.SetGridItemsSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				m_container_icon_grid.SetGridSize( 1,1 );
				m_container_icon_grid.SetController( this );		
				m_container_icon_grid.GenerateBackgroundTiles( 1 );
				m_container_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				m_container_icon_grid.AddItem( InventoryItem.Cast( m_entity ), Vector(0, 1, 1), Vector(0,0,0) );
			}
			if (m_container_icon) 
			{
				m_grid_ghost = m_container_icon.FindAnyWidget("GridGhost");
				m_ghost_icon = ImageWidget.Cast( m_container_icon.FindAnyWidget("GhostIcon") );
			}
		}
	}
}