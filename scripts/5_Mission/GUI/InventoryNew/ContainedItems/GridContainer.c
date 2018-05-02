class GridContainer: ContainerBase
{
	protected int m_NumberRow;
	protected int m_NumberColumnFocused;
	protected EntityAI m_Entity;	
	protected int m_Width;
	protected ItemsContainer m_ParentContainer;
	
	void GridContainer( ContainerBase parent )
	{
		m_ParentContainer = ItemsContainer.Cast( parent );
		
		for ( int i = 0; i < 10; i++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "Icon" + i ),  this, "Column" + i );
		}
		
		for ( i = 0; i < 10; i++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "Icon" + i ),  this, "ColumnOnDraggingOver" + i );
		}
		
		//TODO: create layout for GridContainer and set alpha to 225 there
		m_ParentWidget.SetAlpha(255);
	}
	
	void UnfocusAll()
	{
		for ( int i = 0; i < 10; i++ )
		{
			//GetMainPanel().FindAnyWidget( "Cursor" + i ).Show( false );
			
			Entity entity = m_Entity.GetInventory().GetCargo().FindEntityInCargoOn( m_NumberRow, i );
			if ( entity )
			{
				Icon icon = m_ParentContainer.GetIcon( entity.GetID() );
				
				if ( icon )
				{
					icon.SetActive( false );
				}
			}
			else
			{
				GetMainPanel().FindAnyWidget( "Cursor" + i ).Show( false );
			}
		}
		
		m_NumberColumnFocused = 0;
	}
	
	void SetFocus( int column )
	{	
		m_NumberColumnFocused = column;
		
		//GetMainPanel().FindAnyWidget( "Cursor" + column ).Show( true );
		
		// Show selector
		Entity focused_item_new = m_Entity.GetInventory().GetCargo().FindEntityInCargoOn( m_NumberRow, m_NumberColumnFocused );
		if ( focused_item_new )
		{
			Icon icon = m_ParentContainer.GetIcon( focused_item_new.GetID() );
			
			if ( icon )
			{
				icon.SetActive( true );
			}			
		}
		else
		{
			GetMainPanel().FindAnyWidget( "Cursor" + column ).Show( true );
		}
	}
	
	bool GetIconSize(int row, int column, out int width, out int height)
	{
		Entity focused_item_new = m_Entity.GetInventory().GetCargo().FindEntityInCargoOn( row, column );
		if ( focused_item_new )
		{
			Icon icon = m_ParentContainer.GetIcon( focused_item_new.GetID() );
			
			if ( icon )
			{
				width = icon.m_sizeX;
				height = icon.m_sizeY;
				
				return true;
			}
		}
		
		return false;
	}
	
	bool GetIconPosition(int row, int column, out int x, out int y)
	{
		Entity focused_item_new = m_Entity.GetInventory().GetCargo().FindEntityInCargoOn( row, column );
		if ( focused_item_new )
		{
			Icon icon = m_ParentContainer.GetIcon( focused_item_new.GetID() );
			
			if ( icon )
			{
				x = icon.m_posX;
				y = icon.m_posY;
				
				return true;
			}
		}
		
		return false;
	}
	
	int GetFocusedColumn()
	{
		return m_NumberColumnFocused;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.GridContainer;
	}
	
	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;
	}
	
	void ColumnOnDraggingOver0( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 0, NULL );
		}
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 0, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 0, NULL );
		}
	}
	
	void ColumnOnDraggingOver1( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 1, NULL );
		}		
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 1, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 1, NULL );
		}
	}
	
	void ColumnOnDraggingOver2( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 2, NULL );
		}		
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 2, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 2, NULL );
		}
	}
	
	void ColumnOnDraggingOver3( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo.Cast( m_Parent.m_Parent ).DraggingOverGrid( w, m_NumberRow, 3, NULL );
		}		
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 3, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 3, NULL );
		}
	}
	
	void ColumnOnDraggingOver4( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 4, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 4, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 4, NULL );
		}
	}
	
	void ColumnOnDraggingOver5( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 5, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 5, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 5, NULL );
		}
	}
	
	void ColumnOnDraggingOver6( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 6, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 6, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 6, NULL );
		}
	}
	
	void ColumnOnDraggingOver7( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 7, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 7, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 7, NULL );
		}
	}
	
	void ColumnOnDraggingOver8( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 8, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 8, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 8, NULL );
		}
	}
	
	void ColumnOnDraggingOver9( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 9, NULL );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 9, NULL );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DraggingOverGrid( w, m_NumberRow, 9, NULL );
		}
	}
	
	void Column0( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 0 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 0 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 0 );
		}
	}
	
	void Column1( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 1 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 1 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 1 );
		}
	}
	
	void Column2( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 2 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 2 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 2 );
		}
	}
	
	void Column3( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 3 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 3 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 3 );
		}
	}
	
	void Column4( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 4 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 4 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 4 );
		}
	}
	
	void Column5( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 5 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 5 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 5 );
		}
	}
	
	void Column6( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 6 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 6 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 6 );
		}
	}
	
	void Column7( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 7 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 7 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 7 );
		}
	}
	
	void Column8( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 8 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 8 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 8 );
		}
	}
	
	void Column9( Widget w )
	{
		if( m_Parent.m_Parent.IsInherited( ItemWithCargo ) )
		( ItemWithCargo.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 9 );
		else if( m_Parent.m_Parent.IsInherited( ItemWithCargoAndAttachments ) )
		{
			( ItemWithCargoAndAttachments.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 9 );
		}
		else if ( m_Parent.m_Parent.IsInherited( HandsContainer ) )
		{
			( HandsContainer.Cast( m_Parent.m_Parent ) ).DropReceived( w, m_NumberRow, 9 );
		}
	}
	
	void SetNumber( int number )
	{
		m_NumberRow = number;
	}
	
	int GetWidth()
	{
		return m_Width;
	}

	void SetWidth( int width )
	{
		m_Width = width;
		for ( int i = width; i < 10; i++ )
		{
			GetMainPanel().FindAnyWidget( "Icon" + i ).Show( false );
		}
	}
}
