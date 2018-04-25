class ItemsContainer: Container
{
	protected ref map<int, ref Icon> m_EntitiesMap = new map<int, ref Icon>;

	void ItemsContainer( ContainerBase parent )
	{
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel(),  this, "OnDropReceivedFromHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel(),  this, "DraggingOver" );	
	}	

	override void OnDropReceivedFromHeader ( Widget w, int x, int y, Widget receiver )
	{
		( Container.Cast( m_Parent ) ).OnDropReceivedFromHeader( w, x, y, receiver );
	}
	
	override void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		( Container.Cast( m_Parent ) ).DraggingOver( w, x, y, receiver );
	}

	override void UnfocusAll()
	{
		for ( int i = 0; i < Count(); i++ )
		{
			for ( int j = 0; j < ITEMS_IN_ROW; j++ )
			{
				Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
		
		
		for ( i = 0; i < m_EntitiesMap.Count(); ++i )
		{
			m_EntitiesMap.GetElement(i).SetActive(false);
		}
	}
	
	void AddItem( Icon icon )
	{	
		if( !m_EntitiesMap.Contains( icon.GetObject().GetID() ) )
		{
			m_EntitiesMap.Insert( icon.GetObject().GetID(), icon);
		}
	}
	
	Icon GetIcon( int entity_id )
	{
		return m_EntitiesMap.Get( entity_id  );
	}

	void RemoveItem( Icon icon )
	{
		if( icon )
		{
			Icon icon_copy = icon; // for some reason garbage collector collects icon too soon, so copy has to be made
			m_EntitiesMap.Remove( icon.GetObject().GetID() );
		}
	}

	void RemoveItem( int entity )
	{
		m_EntitiesMap.Remove( entity );
	}
	
	bool ContainsEntity( EntityAI entity )
	{
		return m_EntitiesMap.Contains( entity.GetID() );
	}
	
	void UpdateItemsTemperature()
	{
		if( !ItemManager.GetInstance().IsDragging() )
		{
			for( int i = 0; i < m_EntitiesMap.Count(); i++ )
			{
				Icon icon = m_EntitiesMap.GetElement(i);
				icon.SetTemperature();
			}			
		}
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		Icon icon = GetIcon( item_to_refresh.GetID() );
		if( icon )
		{
			icon.RefreshQuantity();
		}
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		InventoryLocation src = new InventoryLocation;
		int row, column;
		if( item_to_refresh.GetInventory().GetCurrentInventoryLocation(src) )
		{
			row = src.GetRow();
			column = src.GetCol();
		}
		
		Icon icon = GetIcon( item_to_refresh.GetID() );
		if( icon )
		{
			icon.RefreshPos( row, column );
		}
	}

	override void Refresh()
	{
		super.Refresh();

		for( int i = 0; i < m_EntitiesMap.Count(); i++ )
		{
			m_EntitiesMap.GetElement(i).Refresh();
		}
	}
}
