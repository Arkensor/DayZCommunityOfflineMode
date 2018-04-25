/*
TODO: make base class and override function InitIconGrid for container and view
*/

class InventoryViewGridBase: InventoryViewBase
{
	protected InventoryGrid m_attachment_grid;
	protected bool m_view_related;
	protected bool m_double_clicked;
	protected InventoryView m_view;
	protected Widget m_grid_ghost;
	protected ImageWidget m_ghost_icon;
	protected string m_icon_name;
	protected string m_attachmentCategory;
	protected EntityAI m_entityCat;
	
	void InventoryViewGridBase( InventoryContainer container, EntityAI entity, int properties )
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

	void ~InventoryViewGridBase()
	{
		SaveHistory();
		delete m_root;
		delete m_container_icon;
	}

	override void CreateLayout()
	{
		InitIconGrid();
	}

	override void UpdateItems()
	{
		if (m_entity == NULL) return;
		if (m_update_lock_timer.IsRunning()) return;	

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
		// m_update_lock_timer.Run(0.5, this, "UpdateUnlock", NULL, false);
	}

	override void UpdateUnlock()
	{
		// UpdateItems();
	}

	override void InitIconGrid()
	{
	}

	void SetEntity(EntityAI entity)
	{
		m_entity = entity;
	}

	void RemoveEntity()
	{
		m_entity = NULL;
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

	override protected bool HasProperty(int mask) 
	{
		if (m_properties & mask) return true;
		return false;	
	}	
	
	override EntityAI GetEntity() 
	{
		return m_entity;
	}
	
	// ScriptedWidgetEventHandler override
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( w == m_header || w == m_minimize_widget || w == m_maximize_widget )
		{
			if  ( m_body && m_body_spacer && m_content_spacer )
			{
				if (m_body_spacer.MaxHeight)
				{
					m_body_spacer.MaxHeight = 0;
					if ( m_IsExpandable )
					{
						m_minimize_widget.Show( true );
						m_maximize_widget.Show( false );
					}
				}
				else
				{
					InventoryManager manager = InventoryManager.GetInstance();
					int gap = m_content_spacer.Gap;			
					m_body_spacer.MaxHeight = manager.GetAttachmentItemSize() + gap;
					if ( m_IsExpandable )
					{
						m_minimize_widget.Show( false );
						m_maximize_widget.Show( true );
					}
				}
				
				m_body_spacer.Update();
			}
		}
		return false;
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

	override void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		if ( m_entity == NULL ) return;
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem item = FindItem(grid, row, col);
		
		manager.Drag(InventoryDragFromType.GRID, item, grid);
	}

	override void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		MoveItem(grid, row, col, true);
	}

	override void OnItemDrop(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		manager.Drop();
		manager.HideTooltip();
		UpdateLock();
		
		if ( m_itemAtPos && ( m_itemAtPos.GetSlotsCountCorrect() > 0 ||  m_itemAtPos.GetInventory().GetCargo() ) )
		{
  		manager.HideTooltip();
			InventoryContainerEntity container = InventoryContainerEntity.Cast( m_container );
			container.HideExpandView( m_itemAtPos );
		}
	}

	override void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		MoveItem(grid, row, col, false);
		manager.Drop();
		UpdateLock();
	}
	/*
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_entity.IsNull() || grid == NULL) return;
		
		PlayerBase player = GetGame().GetPlayer();
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		
		if (itemAtPos)
		{
			manager.ShowActionMenu(itemAtPos);		
		}
	}
*/

	void SetAttachmentCategory( string cat, EntityAI ent )
	{
		m_attachmentCategory = cat;
		m_entityCat = ent;
	}
	
	private InventoryItem m_itemAtPos;
	override void OnItemLeftClick(InventoryGrid grid, Widget w, int row, int col)
	{
		
			if( m_container.IsInherited( InventoryContainerZombie ) )
			{
				if( m_attachmentCategory == "Cargo" )
				{
					InventoryContainerZombie zmb_con = InventoryContainerZombie.Cast( m_container );
					zmb_con.CargoClick();
				}
			}
			else if ( m_container.IsInherited( InventoryContainerAttachmentCategories ) )
			{
				InventoryContainerAttachmentCategories att_cat_con = InventoryContainerAttachmentCategories.Cast( m_container );
				if( m_attachmentCategory == "Cargo" && !att_cat_con.Exists( "Cargo" ) )
				{
					
					InventoryView m_cargo_view = new InventoryView( m_container, m_entityCat, InventoryViewProperties.CARGOS | InventoryViewProperties.ATTACHMENT_CATEGORIES );
					m_cargo_view.SetAttachmentCategory( "Cargo" );			
					m_cargo_view.CreateLayout();
					m_cargo_view.SetHeaderText( "Cargo" );
					att_cat_con.AddAttachmentView( "Cargo" , m_cargo_view );
				}
				else
				{
					if( !att_cat_con.Exists( m_attachmentCategory ) )
					{
						InventoryView m_att_cat_view = new InventoryView( m_container, m_entityCat, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.ATTACHMENT_CATEGORIES );
						m_att_cat_view.SetAttachmentCategory( m_attachmentCategory );			
						m_att_cat_view.CreateLayout();
						m_att_cat_view.SetHeaderText( att_cat_con.GetAttachmentCatName( m_attachmentCategory ) );
						att_cat_con.AddAttachmentView( m_attachmentCategory, m_att_cat_view );
					}
					else
					{
						att_cat_con.HideAttachmentView( m_attachmentCategory );
						att_cat_con.Remove( m_attachmentCategory );
					}
				}
			}
		
		if ( m_entity == NULL || grid == NULL) return;
		m_itemAtPos = FindItem(grid, row, col);
		
		if (m_itemAtPos && ( m_itemAtPos.GetSlotsCountCorrect() > 0 ||  m_itemAtPos.GetInventory().GetCargo() ) )
		{
			InventoryManager manager = InventoryManager.GetInstance();
  			manager.HideTooltip();
			InventoryContainerEntity container = InventoryContainerEntity.Cast( m_container );
			if( !m_double_clicked )
			{
				container.ToggleExpandView( m_itemAtPos );
			}
			m_double_clicked = false;
		}
	}
	
	override void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryItem itemAtPos = FindItem(grid, row, col);
		if (m_container && itemAtPos) m_container.OnItemDoubleClick(this, itemAtPos);
		InventoryManager manager = InventoryManager.GetInstance();	
		manager.HideTooltip();
		InventoryContainerEntity container = InventoryContainerEntity.Cast( m_container );
		container.HideExpandView( itemAtPos );
		m_double_clicked = true;
	}
	
	override vector GetItemSize(InventoryGrid grid, InventoryItem item)
	{
		int w = 1;
		int h = 1;
		
		if (grid != m_attachment_grid) 
		{
			GetGame().GetInventoryItemSize(item, w, h);
		}
		
		return Vector(0, w, h);
	}

	override InventoryItem FindItem(InventoryGrid grid, int row, int col)
	{
		if ( m_entity == NULL  || grid == NULL) return NULL;

		if (grid == m_attachment_grid) 
		{
			int index = grid.GetIndex(row, col);
			if (index >= m_entity.GetInventory().AttachmentCount()) return NULL;
			
			return InventoryItem.Cast( m_entity.GetInventory().GetAttachmentFromIndex(index) );
		}
		
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
		
		return NULL;
	}
	
	override int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if ( dragged_item ) return 0;
		// int color = InventoryGrid.ITEM_COLOR_NORMAL;
		int color = InventoryGrid.ITEM_COLOR_TRANSPARENT;

		// if item icon is main icon (view, viewgrid )
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		// if ( item.GetOwner() == player || item.GetOwner() == NULL )
		if ( item.IsInherited(InventoryItem) && item.GetHierarchyParent() && item.GetHierarchyParent() != player )
		{
			// color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
			color = InventoryGrid.ITEM_COLOR_NORMAL;
		}

		if( item.IsInherited( InventoryItem) ) 
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
		if ( item  )
		{
			InventoryManager manager = InventoryManager.GetInstance();
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
		if ( m_entity == NULL) return false;
		
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

	void ClearHistory()
	{
		for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		{
			s_expanded_history[i] = -1;
		}
		s_expanded_history_index = 0;
	}

	void SaveHistory()
	{
		if ( m_entity == NULL ) return;
		
		if  ( m_body && m_body_spacer && m_content_spacer )
		{
			if (m_body_spacer.MaxHeight == 0)
			{
				s_expanded_history[s_expanded_history_index] = m_entity.GetID();
				s_expanded_history_index++;
			}		
		}
	}

	Widget GetGhotsIcon()
	{
		return m_ghost_icon;
	}

	Widget GetGridGhost()
	{
		return m_grid_ghost;
	}

	void SetGhostIcon( string icon_name )
	{
		m_icon_name = icon_name;
		if ( m_ghost_icon && icon_name )
		{
			m_ghost_icon.LoadImageFile(0,"set:dayz_inventory image:" + icon_name );
		}
	}

	InventoryGrid GetContainerIconGrid()
	{
		return m_container_icon_grid;
	}

	override Widget GetContainerIcon()
	{
		return m_container_icon;
	}
}
