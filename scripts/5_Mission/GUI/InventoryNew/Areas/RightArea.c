class RightArea: Container
{
	ref PlayerContainer m_PlayerContainer;
	void RightArea( ContainerBase parent )
	{
		m_MainPanel.Show( true );
		m_MainPanel = m_MainPanel.FindAnyWidget( "Content" );
		
		m_PlayerContainer = new PlayerContainer( this );
		m_PlayerContainer.SetPlayer( PlayerBase.Cast( GetGame().GetPlayer() ) );
		m_Body.Insert( m_PlayerContainer );

		m_MainPanel.GetScript( m_Spacer );
	}
	
	EntityAI GetFocusedItem()
	{
		return m_PlayerContainer.GetFocusedItem();
	}
	
	override void MoveGridCursor( int direction )
	{
		m_PlayerContainer.MoveGridCursor( direction );
	}
	
	override void Select()
	{
		m_PlayerContainer.Select();
	}
	
	override void Combine()
	{
		m_PlayerContainer.Combine();
	}
	
	override void EquipItem()
	{
		m_PlayerContainer.EquipItem();
	}
	
	override void TransferItem()
	{
		m_PlayerContainer.TransferItem();
	}
	
	void TransferItemToVicinity()
	{
		m_PlayerContainer.TransferItemToVicinity();
	}
	
	override void UnfocusGrid()
	{
		m_PlayerContainer.UnfocusGrid();
	}
	
	override void ResetFocusedContainer()
	{
		m_PlayerContainer.ResetFocusedContainer();
	}
	
	bool IsPlayerEquipmentActive()
	{
		return m_PlayerContainer.IsPlayerEquipmentActive();
	}
	
	void ExpandCollapseContainer()
	{
		m_PlayerContainer.ExpandCollapseContainer();
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_PlayerContainer.RefreshItemPosition( item_to_refresh );
	}
	
	override void SetActive( bool active )
	{
		m_PlayerContainer.SetActive( active );
		if( active )
		{
			m_PlayerContainer.SetFirstActive();
		}
		ScrollBarContainer scroll_bar_container;
		GetMainPanel().GetParent().GetScript( scroll_bar_container );
		scroll_bar_container.ScrollToTop();
	}
	
	override void SetNextActive()
	{
		ScrollBarContainer scroll_bar_container;
		GetMainPanel().GetParent().GetScript( scroll_bar_container );
		if( m_PlayerContainer.IsLastContainerFocused() )
		{
			scroll_bar_container.ScrollToTop();
		}
		m_PlayerContainer.SetNextActive();
		float amount = m_PlayerContainer.GetFocusedContainerHeight() + m_PlayerContainer.GetFocusedContainerYPos();
		if( amount > scroll_bar_container.GetRootHeight() )
		{
			scroll_bar_container.ScrollFixedAmount( true, m_PlayerContainer.GetFocusedContainerHeight() );
		}
	}
	
	override void SetPreviousActive()
	{
		ScrollBarContainer scroll_bar_container;
		float amount = m_PlayerContainer.GetFocusedContainerHeight() + m_PlayerContainer.GetFocusedContainerYPos();
		GetMainPanel().GetParent().GetScript( scroll_bar_container );
		if( amount > scroll_bar_container.GetRootHeight() )
		{
			scroll_bar_container.ScrollFixedAmount( false, m_PlayerContainer.GetFocusedContainerHeight() );
		}
		if( m_PlayerContainer.IsFirstContainerFocused() )
		{
			scroll_bar_container.ScrollToBottom();
		}
		m_PlayerContainer.SetPreviousActive();
		
	}
	
	override bool IsActive()
	{
		return m_PlayerContainer.IsActive( );
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.LeftArea;
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_PlayerContainer.RefreshQuantity( item_to_refresh );
	}

	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainPanel().FindAnyWidget( "RightPanel" );
	}

	override void OnShow()
	{
		super.OnShow();
		Refresh();
	}
}
