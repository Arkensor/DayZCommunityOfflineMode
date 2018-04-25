class InventoryViewGridView: InventoryViewGridBase
{
  
  void InventoryViewGridView ( InventoryContainer container, EntityAI entity, int properties )
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
  
  void SetView( InventoryView view )
  {
    m_view = view;
  }
  
  override void InitIconGrid()
  {
    InventoryManager manager = InventoryManager.GetInstance();
    Widget content_grid_widget;
    
    content_grid_widget = m_view.GetContentGridWidget();
    if ( content_grid_widget ) 
    {
      m_container_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", content_grid_widget );
      if (m_container_icon) m_container_icon.GetScript(m_container_icon_grid);		
      if (m_container_icon_grid)
      {
        m_container_icon_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
        m_container_icon_grid.SetGridSize(1,1);
        m_container_icon_grid.SetController(this);		
        m_container_icon_grid.GenerateBackgroundTiles(1);
        m_container_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
        m_container_icon_grid.AddItem( InventoryItem.Cast( m_entity ), Vector(0, 1, 1), Vector(0,0,0) );
      }
      m_container_icon.Show( true );
      m_container_icon.SetColor( 0x00000000 ); // transparent background under item preview
    }
  }
}