const float MAX_INVENTORY_DISTANCE = 2.5;

class COMCustomInventory : Inventory
{
	bool m1Down;
	bool isBeingHeld;

	ref Icon m_Icon;
    ItemBase m_Item;

	vector playerPosition;
    
	void COMCustomInventory( ContainerBase parent )
	{
        Print( "COMCustomInventory::COMCustomInventory" );

        // Removes the player view in the center
        // GetMainPanel().FindAnyWidget( "CharacterPanel" ).Unlink();
        GetMainPanel().FindAnyWidget( "CharacterPanelWidget" ).Unlink();

        delete m_PlayerPreview;

        // GetMainPanel().FindAnyWidget( "DropzoneX" ).Unlink();

        GetMainPanel().FindAnyWidget( "LeftBackground" ).Unlink();
        GetMainPanel().FindAnyWidget( "LeftPanel" ).Unlink();
        GetMainPanel().FindAnyWidget( "RightBackground" ).Unlink();

		
		delete m_LeftArea;

		//WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "DropzoneX" ), this, "OnCenterPanelDropReceived" );
		//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "DropzoneX" ), this, "DraggingOverCenterPanel" );
		//WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "InventoryWindow" ), this, "OnCenterPanelDropReceived" );
		//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "InventoryWindow" ), this, "DraggingOverCenterPanel" );
    }

	override void OnShow()
	{
		//start update
		SetFocus( GetMainPanel() );
		//Deserialize();

		//m_MainPanel.Update(); 
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player && player.IsPlacingLocal() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(player.TogglePlacingLocal);
		}

		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.SetSpecialtyMeterVisibility( true );
				hud.HideQuickbar( true, true );
				hud.ToggleHud( true, true );
			}
		}
		#ifdef PLATFORM_CONSOLE
				ResetFocusedContainers();
		#endif	
		RefreshQuickbar();
		UpdateInterval();
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.UpdateInterval);
		UpdateConsoleToolbar();
	}

	override void OnHide()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);
		
		// Serialize();
		ItemManager.GetInstance().HideTooltip();
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.SetSpecialtyMeterVisibility( false );
				if( hud.GetQuickBarState() )
				{
					hud.ShowQuickbar();
				}
				
				hud.ToggleHud( hud.GetHudState(), true );
			}
		}
		ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
	}

	bool PlayerMoved()
	{
		return playerPosition != GetPlayer().GetPosition();
	}

    override void UpdateInterval()
	{
        // Print( "COMCustomInventory::UpdateInterval" );

		GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
		GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);

 		Object o = GetPointerObject( GetPlayer(), 0.001, NULL );

        if ( o && o.IsInherited( ItemBase ) )
        {
            m_Item = o;
        } else if ( ( !isBeingHeld && m1Down ) || PlayerMoved() ) {
			m_Item = NULL;
		}

		if ( !m_Item && m_Icon && !isBeingHeld )
		{
			delete m_Icon;
		} else if( m_Item && !m_Icon && m1Down )
		{
			m_Icon = new Icon( this, false );

			if( m_Icon )
			{
				m_Icon.Init( m_Item );
				m_Icon.FullScreen();

			    m_Icon.Refresh();
			}
		} else if ( m_Icon )
		{
			m_Icon.Refresh();
			m_Icon.FullScreen();

			for ( int i = 0; i < m_Icon.m_WhiteBackgrounds.Count(); i++ )
			{
				delete m_Icon.m_WhiteBackgrounds.Get( i );
			}

			m_Icon.GetMainPanel().FindAnyWidget( "Render" ).Show( false );
		}

        super.UpdateInterval();

		playerPosition = GetPlayer().GetPosition();
    }

    void OnMouseButtonDownCenter( int button )
    {       
        Print( "COMCustomInventory::OnMouseButtonDownCenter" );
    
		if ( m1Down ) 
		{
			isBeingHeld = true;
			m1Down = false;
		}

		m1Down = true;
    }

	void OnMouseButtonReleaseCenter( int button )
	{
        Print( "COMCustomInventory::OnMouseButtonReleaseCenter" );

		isBeingHeld = false;
	}
    
	override void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::OnLeftPanelDropReceived" );
        PlaceObject( w, x, y, receiver );
	}

	override void DraggingOverLeftPanel( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::DraggingOverLeftPanel" );
        DragObject( w, x, y, receiver );
	}

	override void OnRightPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::OnLeftPanelDropReceived" );
        PlaceObject( w, x, y, receiver );
	}

	override void DraggingOverRightPanel( Widget w, int x, int y, Widget receiver )
	{
        Print( "COMCustomInventory::DraggingOverLeftPanel" );
        DragObject( w, x, y, receiver );
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

	vector GetPlayerHeadBone()
	{
		int boneIndex = GetPlayer().GetBoneIndexByName( "Head" );
		return GetPlayer().GetBonePositionWS( boneIndex );
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


			vector position = GetPointerPos();

			if ( vector.Distance( GetPlayerHeadBone(), position ) > MAX_INVENTORY_DISTANCE )
			{
				return;
			}

/*
			if ( item == m_Item )
			{
				return;
			}
*/

			Object tObject = NULL;

			
			GetPlayer().PredictiveDropEntity( item );

			InventoryLocation source = new InventoryLocation;
			InventoryLocation destination = new InventoryLocation;

			if ( item.GetInventory().GetCurrentInventoryLocation( source ) )
			{
				tObject = GetGame().CreateObject( "HouseNoDestruct", position, false );

				vector m4[4];
				Math3D.MatrixIdentity4(m4);
				GameInventory.PrepareDropEntityPos( tObject, item, m4);
				destination.SetGround( item, m4 );

				item.PredictiveTakeToDst( source, destination );
			} 

			if ( tObject )
			{
				GetGame().ObjectDelete( tObject );
			}
		}
    }

    void DragObject( Widget w, int x, int y, Widget receiver )
    {
        if ( w )
        {
			if ( vector.Distance( GetPlayerHeadBone(), GetPointerPos() ) > MAX_INVENTORY_DISTANCE )
			{
		    	ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			} else {
		    	ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
        }
    }
}