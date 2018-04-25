class InventoryQuickbar: InventoryGridController
{
	
	protected ref TItemsMap m_items;//tmp
	protected InventoryGrid m_grid;
	protected int m_dragging_index;
	protected int m_quickbar_size;

	private const int QUICKBAR_GRID_WIDTH = 50;
	
	void InventoryQuickbar(Widget quickbarGridWidget)
	{
		m_dragging_index = INDEX_NOT_FOUND;
		m_items = new TItemsMap;
		UpdateItems( quickbarGridWidget );
	}

	void ~InventoryQuickbar()
	{
	}
	
	void UpdateItems( Widget quickbarGridWidget )
	{
		if( !quickbarGridWidget )
		{
			return;
		}
		int i;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		
		if (player == NULL) return;
		m_items.Clear();

		// create grid and align it to center
		if (quickbarGridWidget) quickbarGridWidget.GetScript(m_grid);
		if (m_grid)
		{
			if ( m_quickbar_size != player.GetQuickBarSize() )
			{
				// clear/remove
				m_items.Clear();
				m_grid.UpdateQuickbarItems(m_items);
				while(m_grid.GetRoot().GetChildren()) { delete m_grid.GetRoot().GetChildren(); }

				m_quickbar_size = player.GetQuickBarSize();
				m_grid.SetGridItemsSize( QUICKBAR_GRID_WIDTH, QUICKBAR_GRID_WIDTH );
				m_grid.SetGridSize(m_quickbar_size,1);
				m_grid.SetController(this);		
				m_grid.GenerateQuickBarBackgroundTiles(m_quickbar_size);
				float size = QUICKBAR_GRID_WIDTH / 2.5;
				m_grid.SetQuantityPanelSize( size );
			}
		}
		
		// align quickbar to center 
		float width, height, x, y;
		int screen_w, screen_h;
		quickbarGridWidget.GetSize( width, height );
		quickbarGridWidget.GetPos(x, y);
		GetScreenSize(screen_w, screen_h);
		float new_x =  0.5 - ( ( width / screen_w ) * 0.5 );
		quickbarGridWidget.SetPos( new_x, y );
		quickbarGridWidget.SetColor( 0x7FFFFFFF );

		m_quickbar_size = player.GetQuickBarSize();
		for ( i = 0; i < m_quickbar_size; i++)
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
			if (item) 
			{
				m_items.Set(item, Vector(i, 1, 1));
			}
		}
		
		if (m_grid) m_grid.UpdateQuickbarItems(m_items);
	}

	void Remove(InventoryItem itemToRemove)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		for (int i = 0; i < m_quickbar_size; i++)
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
			if (item == itemToRemove) 
			{
				player.RemoveQuickBarEntityShortcut(item);
			}
		}
	}
	
	// InventoryGridController override
	override void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
		Widget quickbar = grid.GetRoot();
		if ( quickbar )
		{
			// quickbar.SetColor( 0xFFFFFFFF );
		}
	}

	override void OnItemLeave(InventoryGrid grid, Widget w)
	{
		Widget quickbar = grid.GetRoot();
		if ( quickbar )
		{
			// quickbar.SetColor( 0x7FFFFFFF );
		}
	}

	override void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		bool old_inventory = false;
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if ( menu )
		{
			// menu.HidePanelBorders();
		}
		ItemPreviewWidget iw;
		InventoryItem dragged_item;
		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )
		{
			dragged_item = manager.GetItem();
			if( dragged_item )
			{
				old_inventory = true;
			}
			else
			{
				iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
				if( !iw )
				{
				  string name = w.GetName();
				  name.Replace( "PanelWidget", "Render" );
				  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				} 
				if( !iw || !iw.GetItem() )
				{
					return;
				}
			}
			if(!dragged_item && iw)
			dragged_item = InventoryItem.Cast( iw.GetItem() );

			if ( dragged_item == NULL) return;
			
			if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
			{
				if( old_inventory )
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				}
				else
				{
					int color;

					color = ARGB( 150, 255, 1, 1 );
	
					if( w.FindAnyWidget("Color") )
					{
						w.FindAnyWidget("Color").SetColor( color );
					}
					else
					{
						name = w.GetName();
						name.Replace( "PanelWidget", "Col" );
						w.FindAnyWidget( name ).SetColor( color );
					}
				}				
			}
			else
			{
				if( old_inventory )
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				}
				else
				{
					color = ARGB( 150, 1, 255, 1 );

					if( w.FindAnyWidget("Color") )
					{
						w.FindAnyWidget("Color").SetColor( color );
					}
					else
					{
						name = w.GetName();
						name.Replace( "PanelWidget", "Col" );
						w.FindAnyWidget( name ).SetColor( color );
					}
				}
			}
		}
	}

	override void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		if( GetGame().IsOldInventory() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			InventoryManager manager = InventoryManager.GetInstance();
			if ( manager )
			{
				InventoryItem dragged_item = manager.GetItem();
				
				ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
				if(!dragged_item && iw)
				dragged_item = InventoryItem.Cast( iw.GetItem() );
				
				InventoryItem quickbar_item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
				
				/*if (manager.IsDragging() == false && m_dragging_index == INDEX_NOT_FOUND)
				{
					return;
				}*/
				
				if (m_dragging_index != INDEX_NOT_FOUND)
				{
					dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_dragging_index) );
				}
					
				if (dragged_item && dragged_item.GetInventory().CanRemoveEntity())
				{
					player.SetQuickBarEntityShortcut(dragged_item, col);
					
					if (quickbar_item && quickbar_item != dragged_item && m_dragging_index != INDEX_NOT_FOUND)
					{
						player.SetQuickBarEntityShortcut(quickbar_item, m_dragging_index);
					}
				}
				m_dragging_index = INDEX_NOT_FOUND;
				//manager.Drop();
				InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
				if ( menu )
				{
					menu.RefreshQuickbar();
				}
			}
		}
		else
		{
			player = PlayerBase.Cast( GetGame().GetPlayer() );
			manager = InventoryManager.GetInstance();
			if ( manager )
			{
				dragged_item = manager.GetItem();
				
				iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
				if( !iw )
				{
				  string name = w.GetName();
				  name.Replace( "PanelWidget", "Render" );
				  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				}
				
				if(!dragged_item && iw)
				dragged_item = InventoryItem.Cast( iw.GetItem() );
				
				quickbar_item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
				
				if (m_dragging_index != INDEX_NOT_FOUND)
				{
					dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_dragging_index) );
				}
					
				if (dragged_item && dragged_item.GetInventory().CanRemoveEntity())
				{
					player.SetQuickBarEntityShortcut(dragged_item, col);
					
					if (quickbar_item && quickbar_item != dragged_item && m_dragging_index != INDEX_NOT_FOUND)
					{
						player.SetQuickBarEntityShortcut(quickbar_item, m_dragging_index);
					}
				}
				m_dragging_index = INDEX_NOT_FOUND;
				//manager.Drop();
				menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
				if ( menu )
				{
					menu.RefreshQuickbar();
				}
			}
		}
	}
	
	override void OnItemDrop(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_dragging_index != INDEX_NOT_FOUND)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			InventoryItem dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_dragging_index) );
			Remove(dragged_item);
			m_dragging_index = INDEX_NOT_FOUND;
		}
		
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
		ItemManager.GetInstance().HideDropzones();
	}

	override void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );		
		InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
		if (item)
		{	
			m_dragging_index = col;
			m_grid.SetItemColor(item, InventoryGrid.ITEM_COLOR_DRAG);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
	}
	
	int GetQuickbarWidth()
	{
		return QUICKBAR_GRID_WIDTH;
	}
	
	override int GetQuickbarItemColor(InventoryGrid grid, InventoryItem item)
	{
		int color = InventoryGrid.ITEM_COLOR_QUICKBAR_NORMAL;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI itemInHand = player.GetHumanInventory().GetEntityInHands();
		if( player && itemInHand == item )
		{
			if(player.GetInventory().CanAddEntityToInventory(item))
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_H_GOOD;
			else
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_H_BAD;
		}
		else if( itemInHand )
		{
			if(!player.GetInventory().CanAddEntityToInventory(itemInHand) && !GameInventory.CanSwapEntities(item, itemInHand) )	
			{
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_I_BAD;
			}
			
		}
		return color;
	}

	
	override int HasItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )  
		{
			if ( item )
			{
				if ( item.IsInherited( InventoryItem) )
				{
					int has_quantity = manager.HasItemQuantity( item );
					return has_quantity;
				}
			}
		}
		return false;
	}

	override string GetItemQuantityText( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		string quantity_text = manager.GetItemQuantityText( item );
		return quantity_text;
	}

	override float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}

	/*void OnItemEnter(InventoryGrid grid, Widget w, int row, int col);
	void OnItemLeave(InventoryGrid grid, Widget w);
	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDrop(InventoryGrid grid, Widget w);
	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col);
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col);*/
}
