class LeftArea: Container
{
	protected ref VicinityContainer m_VicinityContainer;
	
	void LeftArea( ContainerBase parent )
	{
		m_MainPanel.Show( true );
		m_MainPanel = m_MainPanel.FindAnyWidget( "Content" );

		m_VicinityContainer = new VicinityContainer( this );
		m_Body.Insert( m_VicinityContainer );

		m_MainPanel.GetScript( m_Spacer );
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.RefreshItemPosition( item_to_refresh );
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.RefreshQuantity( item_to_refresh );
	}
	
	override void MoveGridCursor( int direction )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.MoveGridCursor( direction );
		//m_VicinityContainer.MoveGridCursor( direction );
	}
	
	override void Select()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.Select();
	}
	
	override void Combine()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.Combine();
	}
	
	override void TransferItem()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.TransferItem();
	}
	
	override void EquipItem()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.EquipItem();
	}
	
	override void UnfocusGrid()
	{
		Container active_container;
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			active_container = Container.Cast( m_Body.Get( i ) );
			active_container.UnfocusGrid();
		}
	}
	
	override void ResetFocusedContainer()
	{
		m_ActiveIndex = 0;
		Container active_container;
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			active_container = Container.Cast( m_Body.Get( i ) );
			active_container.ResetFocusedContainer();
		}
	}
	
	override bool IsActive()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		return active_container.IsActive( );
	}
	
	override void SetActive( bool active )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.SetActive( active );
		m_ActiveIndex = 0;
		if( active )
		{
			if( active_container.IsInherited( CollapsibleContainer ) )
			{
				CollapsibleContainer.Cast( active_container ).SetFirstActive();
			}
		}
		ScrollBarContainer scroll_bar_container;
		GetMainPanel().GetParent().GetScript( scroll_bar_container );
		scroll_bar_container.ScrollToTop();
	}
	
	int m_ActiveIndex;
	override void SetNextActive()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		if( !active_container.IsActive() )
		{
			return;
		}
		if( active_container.IsLastContainerFocused() )
		{
			++m_ActiveIndex;
			if( m_ActiveIndex < m_Body.Count() )
			{
			}
			else
			{
				m_ActiveIndex = 0;
			}

				active_container.SetActive( false );
				active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
				active_container.SetActive( true );
				if( active_container.IsInherited( CollapsibleContainer ) )
				{
					CollapsibleContainer.Cast( active_container ).SetFirstActive();
				}
				ScrollBarContainer scroll_bar_container;
				GetMainPanel().GetParent().GetScript( scroll_bar_container );
				scroll_bar_container.ScrollToTop();
		}
		else
		{
			scroll_bar_container;
			GetMainPanel().GetParent().GetScript( scroll_bar_container );
			active_container.SetNextActive( );
			
			float amount = active_container.GetFocusedContainerHeight() + active_container.GetFocusedContainerYPos();
			if( amount > scroll_bar_container.GetRootHeight() )
			{
				scroll_bar_container.ScrollFixedAmount( true, active_container.GetFocusedContainerHeight() );
			}
		}
	}
	
	override void SetPreviousActive()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		if( !active_container.IsActive() )
		{
			return;
		}
		
		if( active_container.IsFirstIndex() )
		{
			--m_ActiveIndex;
			if ( m_ActiveIndex < 0 )
			{
				m_ActiveIndex = m_Body.Count() - 1;
			}
			
			active_container.SetActive( false );
			active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
			active_container.SetActive( true );
			if( active_container.IsInherited( CollapsibleContainer ) )
			{
				CollapsibleContainer.Cast( active_container ).SetLastActive();
			}
			ScrollBarContainer scroll_bar_container;
			GetMainPanel().GetParent().GetScript( scroll_bar_container );
			scroll_bar_container.ScrollToBottom();
		}
		else
		{
			scroll_bar_container;
			float amount = active_container.GetFocusedContainerHeight() + active_container.GetFocusedContainerYPos();
			GetMainPanel().GetParent().GetScript( scroll_bar_container );
			if( amount > scroll_bar_container.GetRootHeight() )
			{
				scroll_bar_container.ScrollFixedAmount( false, active_container.GetFocusedContainerHeight() );
			}
			if( active_container.IsFirstContainerFocused() )
			{
				scroll_bar_container.ScrollToBottom();
			}
			active_container.SetPreviousActive();
		}
		
	}
	
	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_VicinityContainer.OnLeftPanelDropReceived( w, x, y, receiver );
	}
	
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.LeftArea;
	}
	
	VicinityContainer GetVicinityContainer()
	{
		return m_VicinityContainer;
	}
	
	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainPanel().FindAnyWidget( "LeftPanel" );
	}
	
	override void OnShow()
	{
		super.OnShow();
		Refresh();
	}
}
