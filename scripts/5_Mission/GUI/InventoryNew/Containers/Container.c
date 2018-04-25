class Container: ContainerBase
{
	protected ref array<ref ContainerBase> m_Body;
	protected ref array<ref ContainerBase> m_OpenedContainers;
	protected AutoHeightSpacer m_Spacer;
	protected AutoHeightSpacer m_SpacerBody;
	
	const int ITEMS_IN_ROW = 7;

	void Container( ContainerBase parent )
	{
		m_Body = new array<ref ContainerBase>;
		m_OpenedContainers = new array<ref ContainerBase>;
		m_MainPanel.GetScript( m_Spacer );
		/*if( m_LayoutName == WidgetLayoutName.ClosableContainer )
		{
			m_MainPanel = m_MainPanel.FindAnyWidget( "body" );
			m_MainPanel.GetScript( m_SpacerBody );
		}*/
	}
	
	float GetFocusedContainerHeight()
	{
		float x, y;
		m_FocusedContainer.GetMainPanel().GetScreenSize( x, y );
		return y;
	}
	
	float GetFocusedContainerYPos()
	{
		float x, y;
		m_FocusedContainer.GetMainPanel().GetPos( x, y );
		return y;
	}

	int Count()
	{
		return m_Body.Count();
	}
	
	void MoveGridCursor( int direction )
	{
		
	}
	
	void Select()
	{
		m_FocusedContainer.Select();
	}
	
	void Combine()
	{
		if(m_FocusedContainer)
		{
			m_FocusedContainer.Combine();
		}
	}
	
	void TransferItem( )
	{
		m_FocusedContainer.TransferItem();
	}
	
	void EquipItem()
	{
		m_FocusedContainer.EquipItem();
	}

	void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver ) {}

	void DraggingOver( Widget w, int x, int y, Widget receiver ) {}

	void DraggingOverHeader( Widget w, int x, int y, Widget receiver ) {}

	override void UpdateInterval()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).UpdateInterval();
		}
	}

	override void SetActive( bool active )
	{
		m_MainPanel.FindAnyWidget("SelectedContainer").Show(active);
		if( active )
		{
			if( this.IsInherited( ItemWithCargo ) )
			{
				ItemWithCargo iwc = ItemWithCargo.Cast( this );
				if( iwc )
				{
					iwc.SetDefaultFocus();
				}
			}
			else if( this.IsInherited( ItemWithCargoAndAttachments ) )
			{
				ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( this );
				if( iwca )
				{
					iwca.SetDefaultFocus();
				}
			}
			else if(this.IsInherited( ItemWithElectricManager ))
			{
			}
			else if(this.IsInherited( AttRow ))
			{
			}
			else
			{

				ContainerBase cnt = ContainerBase.Cast( m_Body.Get( 0 ) );
				cnt.GetMainPanel().FindAnyWidget( "Cursor" + 0 ).Show( true );
			}
		}
	}

	void UnfocusAll()
	{
		for ( int i = 0; i < Count(); i++ )
		{
			for ( int j = 0; j < ITEMS_IN_ROW; j++ )
			{
				Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}

	void UnfocusGrid()
	{
		if(m_FocusedContainer)
		{
			m_FocusedRow = 0;
			m_FocusedColumn = 0;
			m_FocusedContainer.UnfocusAll();
		}
		/*if( m_FocusedContainer && !m_FocusedContainer.IsInherited( ItemWithCargo ) && !m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) && !m_FocusedContainer.IsInherited( AttRow ) )
		{
			UnfocusAll();
			m_FocusedRow = 0;
			m_FocusedColumn = 0;
		}
		else
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			AttRow iwca2 = AttRow.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.UnfocusGrid();
			}
			else if( iwca )
			{
				iwca.UnfocusGrid();
			}
			else if( iwca2 )
			{
				iwca2.UnfocusAll();
			}
		}*/
	}

	int activeIndex = 1;
	bool lastIndex;
	ref Container m_FocusedContainer;

	bool IsLastIndex()
	{
		return lastIndex;
	}
	
	bool IsFirstIndex()
	{
		return activeIndex == 1; 
	}
	
	bool IsFirstContainerFocused()
	{
		Container first = Container.Cast( m_Body[1] );
		return m_FocusedContainer == first;
	}
	
	bool IsLastContainerFocused()
	{
		Container first = Container.Cast( m_Body[ m_Body.Count() - 1 ] );
		return m_FocusedContainer == first;
	}
	
	void SetFocused()
	{
		m_FocusedContainer = this;
	}
	
	void ResetFocusedContainer()
	{
		m_FocusedContainer = NULL;
	}

	void SetNextActive()
	{
		Container active = Container.Cast( m_OpenedContainers[activeIndex] );
		if( !active.IsActive() )
		{
			return;
		}
		active.SetActive( false );
		Container next;
		if( ++activeIndex < m_OpenedContainers.Count() )
		{
			next = Container.Cast( m_OpenedContainers[activeIndex] );
			if( activeIndex == m_OpenedContainers.Count() - 1 )
			{
				lastIndex = true;
			}
			else
			{
				lastIndex = false;
			}
		}
		while( next && !next.GetMainPanel().IsVisible() )
		{
			if( ++activeIndex < m_OpenedContainers.Count() )
			{
				next = Container.Cast( m_OpenedContainers[activeIndex] );
			}
		}
		if( next )
		{
			next.SetActive( true );
			m_FocusedContainer = next;
		}
		else
		{
			activeIndex = 1;
			Container first = Container.Cast( m_OpenedContainers[activeIndex] );
			first.SetActive( true );
			m_FocusedContainer = first;
		}
	}

	void SetPreviousActive()
	{
		Container active = Container.Cast( m_OpenedContainers[activeIndex] );
		if( !active.IsActive() )
		{
			return;
		}
		active.SetActive( false );
		Container prev;
		if( --activeIndex > 0 )
		{
			prev = Container.Cast( m_OpenedContainers[activeIndex] );
			if( activeIndex == m_OpenedContainers.Count() - 1 )
			{
				lastIndex = true;
			}
			else
			{
				lastIndex = false;
			}
		}
		while( prev && !prev.GetMainPanel().IsVisible() )
		{
			if( --activeIndex > 0 )
			{
				prev = Container.Cast( m_OpenedContainers[activeIndex] );
			}
		}
		if(prev)
		{
			prev.SetActive( true );
			m_FocusedContainer = prev;
		}
		else
		{
			activeIndex = m_OpenedContainers.Count() - 1;
			Container first = Container.Cast( m_OpenedContainers[activeIndex] );
			first.SetActive( true );
			m_FocusedContainer = first;
		}
	}
	
	void RecomputeOpenedContainers()
	{
		m_OpenedContainers.Clear();
		m_OpenedContainers.Insert( m_Body[0] );
		m_OpenedContainers.Insert( m_Body[1] );
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			ClosableContainer cnt = ClosableContainer.Cast( m_Body.Get( i ) );
			if( cnt && cnt.IsOpened() )
			{
				m_OpenedContainers.Insert( cnt );
			}
		}
	}

	void SetGap( int gap )
	{
		m_Spacer.Gap = gap;
		if(m_SpacerBody)
		m_SpacerBody.Gap = gap;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	void Insert( ContainerBase container )
	{
		m_Body.Insert( container );
	}

	void Remove( ContainerBase container )
	{
		if( m_Body )
		{
			m_Body.RemoveItem( container );
		}
	}

	/*void Remove( int x )
	{
		if( m_Body )
		{
			m_Body.Remove( x );
		}
	}*/


	ContainerBase Get( int x )
	{
		return m_Body.Get( x );
	}

	override void Refresh()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).Refresh();
		}

		UpdateSpacer();
	}

	void UpdateSpacer()
	{
		m_Spacer.Update();
		if(m_SpacerBody)
			m_SpacerBody.Update();
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
	}

	void UpdateBodySpacers()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			Container c = Container.Cast( m_Body.Get( i ) );
			if( c.IsInherited( Container ) )
			{
				c.UpdateSpacer();
			}
		}
		m_Spacer.Update();
				if(m_SpacerBody)
		m_SpacerBody.Update();
	}
}
