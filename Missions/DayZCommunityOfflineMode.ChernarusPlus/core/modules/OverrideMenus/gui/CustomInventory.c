class COMCustomInventory : Inventory
{
	ref Icon icon;
    ItemBase item;
    
	void COMCustomInventory( ContainerBase parent )
	{
        Print( "COMCustomInventory::COMCustomInventory" );

        // Removes the player view in the center
        GetMainPanel().FindAnyWidget( "CharacterPanelWidget" ).Unlink();
        delete m_PlayerPreview;

		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "InventoryWindow" ), this, "OnCenterPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "InventoryWindow" ), this, "DraggingOverCenterPanel" );
    }

    override void UpdateInterval()
	{
        // Print( "COMCustomInventory::UpdateInterval" );

        if( item && !icon )
		{
			icon = new Icon( this, true );

			if( icon )
			{
				icon.Init( item );
				icon.FullScreen();

			    icon.Refresh();
			}
		}
		else if ( icon )
		{
			icon.Refresh();
			icon.FullScreen();
		}

        super.UpdateInterval();
    }

    void OnMouseButtonDownCenter( int button )
    {       
        Print( "COMCustomInventory::OnMouseButtonDownCenter" );
    
        ItemBase base = ItemBase.Cast( GetPointerObject() );

        if ( base )
        {
            item = base;
        }
    }
    
	override void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::OnLeftPanelDropReceived" );
        if ( w )
        {
            super.OnLeftPanelDropReceived( w, x, y, receiver );
        } else {
            //GetPointerObject();
        }
	}

	override void DraggingOverLeftPanel( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::DraggingOverLeftPanel" );
        if ( w )
        {
            super.DraggingOverLeftPanel( w, x, y, receiver );
        } else {
            //GetPointerObject();
        }
	}
    

    override void OnCenterPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::OnCenterPanelDropReceived" );
        PlaceObject( w, x, y, receiver );
	}

	override void DraggingOverCenterPanel( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::DraggingOverCenterPanel" );
        DragObject( w, x, y, receiver );
	}

    void PlaceObject( Widget w, int x, int y, Widget receiver )
    {
        if ( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w );
					if( !ipw )
					{
						return;
					}
				}
			}
			
			EntityAI item = ipw.GetItem();
			
			GetPlayer().PredictiveDropEntity( item );

			item.SetPosition( GetPointerPos() );
		}
    }

    void DragObject( Widget w, int x, int y, Widget receiver )
    {
        if ( w )
        {
		    ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
        }
    }
}