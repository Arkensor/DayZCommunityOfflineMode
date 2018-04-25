class HandsArea: ContainerBase
{
	ref HandsContainer		m_HandsContainer;
	ref AutoHeightSpacer	m_Spacer;

	void HandsArea( ContainerBase parent )
	{
		Widget root_panel = m_MainPanel;
		
		m_MainPanel = m_MainPanel.FindAnyWidget( "Content" );

		m_HandsContainer = new HandsContainer( this );

		m_MainPanel.GetScript( m_Spacer );
		m_MainPanel.Update();
		m_Spacer.Update();

		//WidgetEventHandler.GetInstance().RegisterOnDropReceived( root_panel.GetParent().FindAnyWidget("DropzoneX"),  m_HandsContainer, "OnDropReceivedFromHeader" );
		//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( root_panel.GetParent().FindAnyWidget("DropzoneX"),  m_HandsContainer, "DraggingOverHeader" );
		//DraggingOverHeader
	}

	override void SetActive( bool active )
	{
		m_MainPanel.FindAnyWidget("SelectedContainer").Show(active);
	}

	void SetCombineActive( bool active )
	{
		if( m_MainPanel.FindAnyWidget( "CombineCursor" ) )
		{
			m_MainPanel.FindAnyWidget( "Combine" ).Show( active );
			m_MainPanel.FindAnyWidget( "CombineCursor" ).Show( active );
		}
	}

	void SetSwapActive( bool active )
	{
		if( m_MainPanel.FindAnyWidget( "SwapCursor" ) )
		{
			m_MainPanel.FindAnyWidget( "Swap" ).Show( active );
			m_MainPanel.FindAnyWidget( "SwapCursor" ).Show( active );
		}
	}

	override bool IsActive()
	{
		return m_MainPanel.FindAnyWidget("SelectedContainer").IsVisible();
	}

	bool IsSwapActive()
	{
		Widget swap_cursor = m_MainPanel.FindAnyWidget("SwapCursor");
		if( swap_cursor )
		{
			return swap_cursor.IsVisible();
		}
		else
		{
			return false;
		}
	}

	bool IsCombineActive()
	{
		Widget combine_cursor = m_MainPanel.FindAnyWidget("CombineCursor");

		if( combine_cursor )
		{
			return combine_cursor.IsVisible();
		}
		else
		{
			return false;
		}
	}

	void SelectCombine()
	{
		m_HandsContainer.SelectCombine();
	}
	
	void EquipItem()
	{
		m_HandsContainer.EquipItem();
	}

	void SelectSwap()
	{
		m_HandsContainer.SelectSwap();
	}

	void Select()
	{
		m_HandsContainer.Select();
	}
	
	void TransferItem()
	{
		m_HandsContainer.TransferItem();
	}
	
	void TransferItemToVicinity()
	{
		m_HandsContainer.TransferItemToVicinity();
	}

	override void UpdateInterval()
	{
		float x, y;
		GetMainPanel().GetSize( x, y );
		if( y > 500 )
		{
			y = 500;
		}
		GetMainPanel().GetParent().SetSize( x, y );
		m_HandsContainer.UpdateInterval();
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_HandsContainer.RefreshQuantity( item_to_refresh );
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.HandsArea;
	}

	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainPanel().FindAnyWidget( "HandsPanel" );
	}

	override void OnShow()
	{
		super.OnShow();
		m_HandsContainer.OnShow();
		m_Spacer.Update();
	}

	override void Refresh()
	{
		super.Refresh();
		m_HandsContainer.Refresh();
		m_MainPanel.Update();
		m_Spacer.Update();
	}
	
	void DraggingOverHandsPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		ItemManager.GetInstance().HideDropzones();
		ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
	}
	
	void OnHandsPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		
		if( !ipw || ipw.GetItem() == NULL || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}
		
		if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( ipw.GetItem() ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToHands( ipw.GetItem() );		
		}
	}
}
