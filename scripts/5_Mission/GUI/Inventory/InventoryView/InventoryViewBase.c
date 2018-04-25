class InventoryViewBase: InventoryGridController
{
	protected InventoryContainer m_container;
	protected InventoryGrid m_icon_grid;
	protected InventoryGrid m_container_icon_grid;
	protected Widget m_contentGrid;
	protected ref array<InventoryGrid> m_cargo_grids;
	protected ref array<Cargo> m_cargos;	
	protected EntityAI m_entity;
	protected ref Timer m_update_lock_timer;
	protected AutoHeightSpacer m_body_spacer;
	protected AutoHeightSpacer m_content_spacer;
	protected ref TItemsMap m_items;//tmp
	protected int m_properties;
	
	protected Widget m_container_icon;
	protected Widget m_root;
	protected Widget m_content;
	protected Widget m_body;
	protected Widget m_image_frame;
	protected TextWidget m_header;
	protected Widget m_minimize_widget;
	protected Widget m_maximize_widget;
	protected bool m_IsExpandable;
	protected EntityAI m_owner_entity;
	
	void InventoryViewBase( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = entity;
		m_cargo_grids = new array<InventoryGrid>;
		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
	}

	void ~InventoryViewBase()
	{
	}

	void CreateLayout();
	
	void UpdateItems();
	
	protected InventoryItem FindItem(InventoryGrid grid, int row, int col);

	protected int FindCargoIndex(InventoryGrid grid);
	
	void UpdateSort()
	{
		if ( m_container.GetContentListWidget() && m_root )
		{
			m_container.GetContentListWidget().RemoveChild(m_root);
			m_container.GetContentListWidget().AddChild(m_root);
		}

		if ( m_container.GetContentGridWidget() && m_container_icon )
		{
			m_container.GetContentGridWidget().RemoveChild(m_container_icon);
			m_container.GetContentGridWidget().AddChild(m_container_icon);
		}
	}
	
	void SetHeaderText(string text)
	{
		if (m_header) m_header.SetText(text);
	}

	void AddProxyCargo(Cargo proxyCargo)
	{
	}
	
	void UpdateLock()
	{
		m_update_lock_timer.Run(0.5, this, "UpdateUnlock", NULL, false);
	}
	
	void UpdateUnlock()
	{
		//UpdateItems();
	}

	void InitAttachmentGrid( int panel_type )
	{	
	}
	
	void InitIconGrid()
	{
	}
	
	void InitCargoGrid(Cargo cargo, int panel_type)
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
				cargo_grid.SetGridItemsSize(manager.GetNormalItemSize(),manager.GetNormalItemSize());
				cargo_grid.SetGridGapSize(manager.GRID_CARGO_GAP);
				cargo_grid.SetGridSize(cargo.GetWidth(), cargo.GetHeight());
				cargo_grid.SetController(this);
				cargo_grid.GenerateBackgroundTiles(cargo.GetWidth() * cargo.GetHeight());
				cargo_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				UpdateCargoGrid(m_cargos.Count() - 1);
			}
		}
	}
	
	protected void UpdateAttachmentGrid()
	{
	}
	
	void UpdateIconGrid()
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

	void SetOwnerEntity(EntityAI entity)
	{
		m_owner_entity = entity;
	}
	
	bool MoveItem(InventoryGrid grid, int row, int col, bool testOnly)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		InventoryGrid gridSrc = manager.GetGrid();
		int index_from, index_to, flags;
		vector data;
		if ( !(m_entity == NULL || grid == NULL))
		{
			if (manager.IsDragging() == false || dragged_item == NULL) return false;	
			
			if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
					return false;
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
				else if (flags == InventoryCombinationFlags.ADD_AS_ATTACHMENT || flags == InventoryCombinationFlags.ADD_AS_CARGO )
				{
					if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
					else
					{
						if (grid.DebugOutput) player.MessageStatus(m_entity.GetDisplayName() + " :TakeEntityToInventory( " + dragged_item.GetDisplayName() + " )");
						
						player.PredictiveTakeEntityToTargetInventory(m_entity, FindInventoryLocationType.ANY, dragged_item);
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
				else if ( flags == InventoryCombinationFlags.SWAP/* || flags == InventoryCombinationFlags.SWAP_MAGAZINE*/)
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
						
						/*if( flags == InventoryCombinationFlags.SWAP )
						{*/
							player.PredictiveSwapEntities(dragged_item, itemAtPos);
						/*}
						else
						{
								InventoryLocation invLoc;
								invLoc = new InventoryLocation;
								Magazine mag;
								Magazine mag2;
			
								if ( Class.CastTo(mag,  m_am_entity1 ) && Class.CastTo(mag2,  m_am_entity2 ) )
								{
									mag.GetInventory().GetCurrentInventoryLocation(invLoc);
									if( Class.CastTo(wpn, invLoc.GetParent()) )
									{
										m_player.GetWeaponManager().SwapMagazine(mag2);	
									}
									else
									{
										m_player.GetWeaponManager().SwapMagazine(mag);	
									}
								}
						}*/

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
						
						m_entity.Ptr().TakeEntityToInventory(dragged_item);
					}
					return true;
				}
			}
			*/
			else
			{
				// dragging over empty cargo slot
				int cargo_index = FindCargoIndex(grid);
				
				if (cargo_index != INDEX_NOT_FOUND)	
				{
					Cargo cargo = m_cargos.Get(cargo_index);
					int idx = 0;
					
					// use cargo index here
					if (cargo.GetParent().GetInventory().CanAddEntityInCargoEx(dragged_item, idx, row, col))			
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
								
							player.PredictiveTakeEntityToTargetCargoEx(m_entity, dragged_item, idx, row, col);
						}
						return true;
					}
				}		
			}
			}
		else
		{
			if( m_owner_entity )
			{
				if( dragged_item != NULL && m_owner_entity.GetInventory().CanAddEntityToInventory( dragged_item ) )
				{
					if( testOnly )
					{
							manager.SetItemColor( InventoryGrid.ITEM_COLOR_GOOD );
					}
					else
					{
							player.PredictiveTakeEntityToTargetInventory( m_owner_entity, FindInventoryLocationType.ANY, dragged_item );
					}
					return true;
				}
			}
			else
			{	
				if( dragged_item != NULL && player.GetInventory().CanAddEntityToInventory( dragged_item ) )
				{
					if( testOnly )
					{
							manager.SetItemColor( InventoryGrid.ITEM_COLOR_GOOD );
					}
					else
					{
							player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, dragged_item );
					}
					return true;
				}
			}
		}
		if ( testOnly ) manager.SetItemColor( InventoryGrid.ITEM_COLOR_WRONG );
		return false;
	}
	
	protected bool HasProperty(int mask) 
	{
		if (m_properties & mask) return true;
		return false;	
	}	
	
		EntityAI GetEntity() {
		return m_entity;
	}
	
	// ScriptedWidgetEventHandler override
	void UpdateCargoGrid(int cargo_index)
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
		
	// InventoryGridController override
	override void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
	}
	
	override void OnItemLeave(InventoryGrid grid, Widget w)
	{
	}
	
	override void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
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
	}

	override void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		MoveItem(grid, row, col, false);
		manager.Drop();
		UpdateLock();
	}
	
	override void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col)
	{
		if ( (m_entity && m_entity == NULL ) || m_entity == NULL || grid == NULL) return;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		
		if ( itemAtPos && itemAtPos.IsItemBase() )
		{
			itemAtPos.OnRightClick();
			if( GetDayZGame().IsLeftCtrlDown() ) manager.ShowActionMenu(itemAtPos);		
		}

		UpdateItems();
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
	
	override int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if ( dragged_item ) return 0;
		int color = InventoryGrid.ITEM_COLOR_NORMAL;
		// if item icon is main icon (view, viewgrid )
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( item.GetHierarchyParent() == player || item.GetHierarchyParent() == NULL )
		{
			color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
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
		InventoryManager manager = InventoryManager.GetInstance();
		int has_quantity = manager.HasItemQuantity( item );
		return has_quantity;
	}

	override float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}

	Widget GetRootWidget() 
	{
  		return m_root;
  	}

  	Widget GetContainerIcon() 
	{
  		return m_container_icon;
  	}

  	Widget GetContentGridWidget() {
		return m_contentGrid;
	}
}
