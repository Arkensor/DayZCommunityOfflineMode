enum Direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN
}

class Inventory: ContainerBase
{
	protected ref LeftArea			m_LeftArea;
	protected ref RightArea			m_RightArea;
	protected ref CharacterArea		m_CharacterArea;
	protected ref HandsArea			m_HandsArea;
	protected ref PlayerPreview		m_PlayerPreview;
	protected ref InventoryQuickbar	m_Quickbar;
	
	protected Widget				m_QuickbarWidget;
	protected Widget				m_ConsoleToolbar;

	protected ref ContextMenu m_context_menu;
	
	void Inventory( ContainerBase parent )
	{
		m_ControllerRightStickTimer = new Timer();
		new ItemManager( GetMainPanel() );
		new ColorManager();
		//Deserialize();
		m_LeftArea = new LeftArea( this );
		m_RightArea = new RightArea( this );
		m_HandsArea = new HandsArea( this );
		m_PlayerPreview = new PlayerPreview( this );

#ifdef PLATFORM_XBOX
#else
#ifdef PLATFORM_PS4
#else
		m_QuickbarWidget = GetMainPanel().FindAnyWidget( "QuickbarGrid" );
		m_Quickbar = new InventoryQuickbar( m_QuickbarWidget );
		m_Quickbar.UpdateItems( m_QuickbarWidget );
#endif
#endif			
		
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "LeftPanel" ),  this, "OnLeftPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "LeftPanel" ),  this, "DraggingOverLeftPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "RightPanel" ),  this, "OnRightPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "RightPanel" ),  this, "DraggingOverRightPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "OnRightPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "DraggingOverRightPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "HandsPanel" ),  this, "OnHandsPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "HandsPanel" ),  this, "DraggingOverHandsPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "InventoryWindow" ),  this, "OnLeftPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "InventoryWindow" ),  this, "DraggingOverLeftPanel" );

		WidgetEventHandler.GetInstance().RegisterOnController( GetMainPanel(),  this, "Controller" );
	}

	protected int GetProperControllerStickAngle( int angle )
	{
		int proper_angle = ( 360 - angle ) % 360;	//rotation correction

		return proper_angle;
	}

	protected int AngleToDirection( int angle )
	{
		if( angle < 45 || angle > 315)
		{
			return Direction.RIGHT;
		}
		else if ( angle < 135 && angle > 45 )
		{
			return Direction.DOWN;
		}
		else if ( angle < 225 && angle > 135 )
		{
			return Direction.LEFT;
		}
		else if ( angle < 315 && angle > 225 )
		{
			return Direction.UP;
		}
		return -1;
	}

	void TimerEnd()
	{
		m_ControllerRightStickTimerEnd = true;
		m_ControllerRightStickTimer.Stop();
	}
	int once;
	protected int m_ControllerAngle;
	protected int m_ControllerTilt;
	protected bool m_ControllerRightStickTimerEnd = true;
	ref Timer m_ControllerRightStickTimer;


	bool Controller( Widget w, int control, int value )
	{
		//Print("control: "+ control +" value: "+value);
		
		//Right stick
		
		if ( control == ControlID.CID_RADIALMENU )
		{
			m_ControllerAngle 	= value >> 4; 	// <0,360>
			m_ControllerTilt	= value & 0xF; 	// <0,10>

			m_ControllerAngle = GetProperControllerStickAngle(m_ControllerAngle);

			m_ControllerAngle = AngleToDirection(m_ControllerAngle);


			if(m_ControllerTilt>5)
			{
				if( m_ControllerRightStickTimerEnd )
				{
					m_RightArea.MoveGridCursor(m_ControllerAngle);
					m_LeftArea.MoveGridCursor(m_ControllerAngle);
					m_ControllerRightStickTimerEnd = false;
					m_ControllerRightStickTimer.Run( 0.1, this, "TimerEnd" );
				}
			}
			return true;
		}
		
		if ( control == ControlID.CID_RIGHT && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.RIGHT);
			m_LeftArea.MoveGridCursor(Direction.RIGHT);
		}
		else if ( control == ControlID.CID_LEFT && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.LEFT);
			m_LeftArea.MoveGridCursor(Direction.LEFT);
		}
		else if ( control == ControlID.CID_UP && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.UP);
			m_LeftArea.MoveGridCursor(Direction.UP);
		}
		else if ( control == ControlID.CID_DOWN && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.DOWN);
			m_LeftArea.MoveGridCursor(Direction.DOWN);
		}
		
		
		once++;
		if( control == ControlID.CID_SELECT && value != 0 )
		{
				if( m_RightArea.IsActive() )
				{
					m_RightArea.Select();
				}
				if( m_LeftArea.IsActive() )
				{
					m_LeftArea.Select();
				}
				/*if( m_HandsArea.IsActive() )
				{
					m_HandsArea.Select();
				}*/
				if( m_HandsArea.IsCombineActive() )
				{
					m_HandsArea.SelectCombine();
				}
				if( m_HandsArea.IsSwapActive() )
				{
					m_HandsArea.SelectSwap();
					m_HandsArea.SetSwapActive( false );
					m_HandsArea.SetActive( true );
				}
			return true;
		}

		if( control == ControlID.CID_BACK && value != 0 )
		{
			if( GetMainPanel().IsVisible() )
			{
				if( m_RightArea.IsActive() )
				{
					m_RightArea.Combine();
				}
				if( m_LeftArea.IsActive() )
				{
					m_LeftArea.Combine();
				}
			}
			//ItemManager.GetInstance().SetSelectedItem(NULL,NULL);
			return true;
		}

		return false;
	}

	void Serialize()
	{
		//m_LeftArea.GetVicinityContainer().TraverseShowedItems();
		ItemManager.GetInstance().SerializeDefaultOpenStates();
		ItemManager.GetInstance().SerializeDefaultHeaderOpenStates();
		//ItemManager.GetInstance().SetDefaultOpenStateHands( true );
	}

	void Deserialize()
	{
		ItemManager.GetInstance().DeserializeDefaultOpenStates();
		ItemManager.GetInstance().DeserializeDefaultHeaderOpenStates();
	}

	void ~Inventory()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_RightArea.RefreshQuantity( item_to_refresh );
		m_HandsArea.RefreshQuantity( item_to_refresh );
		m_LeftArea.RefreshQuantity( item_to_refresh );
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_LeftArea.RefreshItemPosition( item_to_refresh );
		m_RightArea.RefreshItemPosition( item_to_refresh );
	}

	void DraggingOverHandsPanel( Widget w, int x, int y, Widget receiver )
	{
		ItemManager.GetInstance().HideDropzones();
		//GetMainPanel().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		m_HandsArea.DraggingOverHandsPanel( w, x, y, receiver );
	}
	
	void OnHandsPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_HandsArea.OnHandsPanelDropReceived( w, x, y, receiver );
	}

	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_LeftArea.OnLeftPanelDropReceived( w, x, y, receiver );
	}

	void OnRightPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		if( w )
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
			if( item )
			{
				PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				if( player && ( player.GetInventory().CanAddEntityToInventory( item ) && !player.GetInventory().HasEntityInInventory( item ) ) || player.GetHumanInventory().HasEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, item );
				}
			}
		}
	}

	void DraggingOverLeftPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Preview" ) );
					if( !ipw )
					{
						ipw = ItemPreviewWidget.Cast( w );
						if( !ipw || !ipw.GetItem() )
						{
							return;
						}
					}
				}
			}
			
			ItemBase item = ItemBase.Cast( ipw.GetItem() );
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if( player && item && player.CanDropEntity( item ) )
			{
				ItemManager.GetInstance().HideDropzones();
				GetMainPanel().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
			else if( item )
			{
				ItemManager.GetInstance().ShowSourceDropzone( item );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}

	void DraggingOverRightPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w )
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
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if( player && item && ( player.GetInventory().CanAddEntityToInventory( item ) && !player.GetInventory().HasEntityInInventory( item ) ) || player && item && player.GetHumanInventory().HasEntityInHands( item ) )
			{
				InventoryLocation inv_loc = new InventoryLocation;
				player.GetInventory().FindFreeLocationFor( item , FindInventoryLocationType.ANY, inv_loc );
	
				ItemManager.GetInstance().HideDropzones();
				GetMainPanel().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				if( inv_loc.GetType() == 4 )
				{
					ItemManager.GetInstance().HideDropzones();
					GetMainPanel().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
	
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
			else if( item )
			{
				ItemManager.GetInstance().ShowSourceDropzone( item );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}

	override void UpdateInterval()
	{
		PlayerBase player;
		
		if( GetGame().GetInput().GetActionDown( UAUIExpandCollapseContainer, false ) )
		{
			if( m_RightArea.IsActive() )
			{
				m_RightArea.ExpandCollapseContainer();
			}
			if( m_LeftArea.IsActive() )
			{
				//m_LeftArea.ExpandCollapseContainer();
			}
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.EquipItem();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.EquipItem();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.EquipItem();
			}
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastTransferToVicinity, false ) )
		{
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.TransferItemToVicinity();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.TransferItemToVicinity();
			}
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastTransferItem, false ) )
		{
			if( m_RightArea.IsActive() )
			{
				m_RightArea.TransferItem();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.TransferItem();
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.TransferItem();
			}
		}
		
		if( GetGame().GetInput().GetActionDown( UAUINextUp, false ) )
		{		
			m_RightArea.UnfocusGrid();
			m_LeftArea.UnfocusGrid();
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.SetPreviousActive( );
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.SetPreviousActive( );
			}
			
#ifdef PLATFORM_XBOX
			if ( m_RightArea.IsActive() )
			{
				UpdateConsoleToolbar();
			}
#endif

#ifdef PLATFORM_PS4
			if ( m_RightArea.IsActive() )
			{
				UpdateConsoleToolbar();
			}
#endif			
		}

		if( GetGame().GetInput().GetActionDown( UAUINextDown, false ) )
		{
			m_RightArea.UnfocusGrid();
			m_LeftArea.UnfocusGrid();
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.SetNextActive();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.SetNextActive();
			}
			
#ifdef PLATFORM_XBOX
			if ( m_RightArea.IsActive() )
			{
				UpdateConsoleToolbar();
			}
#endif

#ifdef PLATFORM_PS4
			if ( m_RightArea.IsActive() )
			{
				UpdateConsoleToolbar();
			}
#endif
		}

		if( GetGame().GetInput().GetActionDown( UAUITabLeft, false ) )
		{
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.UnfocusGrid();
				m_LeftArea.SetActive( false );
				m_RightArea.SetActive( true );
				
#ifdef PLATFORM_XBOX
			UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
			UpdateConsoleToolbar();
#endif	
			}
			else if( m_RightArea.IsActive() )
			{
				m_RightArea.UnfocusGrid();
				m_RightArea.SetActive( false );
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				EntityAI item_in_hands = player.GetItemInHands();
				EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
				if( item_in_hands && selected_item && item_in_hands != selected_item )
				{
					m_HandsArea.SetSwapActive( true );
				}
				else
				{
					m_HandsArea.SetActive( true );
				}

#ifdef PLATFORM_XBOX
			UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
			UpdateConsoleToolbar();
#endif			
			}
			else if( m_HandsArea.IsSwapActive() )
			{
				m_HandsArea.SetSwapActive( false );
				m_HandsArea.SetCombineActive( true );
			}
			else if( m_HandsArea.IsCombineActive() )
			{
				m_HandsArea.SetCombineActive( false );
				m_HandsArea.SetActive( false );
				m_LeftArea.SetActive( true );
			}
			else if( m_HandsArea.IsActive() )
			{
				m_HandsArea.SetActive( false );
				m_LeftArea.SetActive( true );
				
#ifdef PLATFORM_XBOX
				UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
				UpdateConsoleToolbar();
#endif				
			}
		}

		if( GetGame().GetInput().GetActionDown( UAUITabRight, false ) )
		{
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.UnfocusGrid();
				m_LeftArea.SetActive( false );
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				item_in_hands = player.GetItemInHands();
				selected_item = ItemManager.GetInstance().GetSelectedItem();
				if( item_in_hands && selected_item && item_in_hands != selected_item )
				{
					m_HandsArea.SetCombineActive( true );
				}
				else
				{
					m_HandsArea.SetActive( true );
				}
				
#ifdef PLATFORM_XBOX
				UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
				UpdateConsoleToolbar();
#endif				
			}
			else if( m_HandsArea.IsCombineActive() )
			{
				m_HandsArea.SetCombineActive( false );
				m_HandsArea.SetSwapActive( true );
			}
			else if( m_HandsArea.IsSwapActive() )
			{
				m_HandsArea.SetSwapActive( false );
				m_RightArea.SetActive( true );
			}
			else if( m_RightArea.IsActive() )
			{
				m_RightArea.UnfocusGrid();
				m_RightArea.SetActive( false );
				m_LeftArea.SetActive( true );
				
#ifdef PLATFORM_XBOX
				UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
				UpdateConsoleToolbar();
#endif			
			}
			else if( m_HandsArea.IsActive() )
			{
				m_HandsArea.SetActive( false );
				m_RightArea.SetActive( true );
				
#ifdef PLATFORM_XBOX
				UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
				UpdateConsoleToolbar();
#endif			
			}
		}

#ifdef PLATFORM_XBOX
		//Open Quickbar radial menu
		if( GetGame().GetInput().GetActionDown( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//assign item
			EntityAI item_to_assign;
						
			if ( m_HandsArea.IsActive() )
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				item_to_assign = player.GetItemInHands();
			}
			else if ( m_RightArea.IsActive() )
			{
				item_to_assign = m_RightArea.GetFocusedItem();
			}
			
			if ( item_to_assign )
			{
				RadialQuickbarMenu.SetItemToAssign( item_to_assign );
				
				//open radial quickbar menu
				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
				{
					InventoryMenuNew inventory_menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
					RadialQuickbarMenu.OpenMenu( inventory_menu );
				}				
			}
		}
#endif

#ifdef PLATFORM_PS4
		//Open Quickbar radial menu
		if( GetGame().GetInput().GetActionDown( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//assign item
			EntityAI item_to_assign;
						
			if ( m_HandsArea.IsActive() )
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				item_to_assign = player.GetItemInHands();
			}
			else if ( m_RightArea.IsActive() )
			{
				item_to_assign = ItemManager.GetInstance().GetSelectedItem();
			}
			
			if ( item_to_assign )
			{
				RadialQuickbarMenu.SetItemToAssign( item_to_assign );
				
				//open radial quickbar menu
				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
				{
					InventoryMenuNew inventory_menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
					RadialQuickbarMenu.OpenMenu( inventory_menu );
				}
			}
		}
#endif	
	
		m_LeftArea.UpdateInterval();
		m_RightArea.UpdateInterval();
		m_HandsArea.UpdateInterval();
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.Inventory;
	}

	void Init() {}

	void Reset()
	{
		m_LeftArea.Refresh();
		m_RightArea.Refresh();
		m_HandsArea.Refresh();
	}
	
	void ResetFocusedContainers()
	{
		m_RightArea.UnfocusGrid();
		m_LeftArea.UnfocusGrid();
		m_LeftArea.SetActive( false );
		m_HandsArea.SetActive( false );
		m_RightArea.SetActive( false );
		m_RightArea.ResetFocusedContainer();
		m_LeftArea.ResetFocusedContainer();
		
		m_LeftArea.SetActive( true );
		m_HandsArea.SetCombineActive( false );
		m_HandsArea.SetSwapActive( false );
	}

	override void OnShow()
	{
		//start update
		SetFocus( GetMainPanel() );
		Deserialize();
		m_MainPanel.Show(true);
		m_MainPanel.Update();

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
				//hud.SetSpecialtyMeterVisibility( true ); specialty meter temporary disabled
				hud.SetSpecialtyMeterVisibility( false );
				hud.HideQuickbar( true, true );
			}
		}

#ifdef PLATFORM_XBOX
		GetGame().GetUIManager().ShowUICursor( false );
		ResetFocusedContainers();
		GetMainPanel().FindAnyWidget( "CursorCharacter" ).Show( false );

		//console inventory toolbar
		m_ConsoleToolbar = GetMainPanel().FindAnyWidget( "ConsoleToolbar" );
		if ( !m_ConsoleToolbar ) 
		{
			m_ConsoleToolbar = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/inventory_new/console_inventory_toolbar.layout", GetMainPanel() );
			m_ConsoleToolbar.SetPos( 0, 0 );
		}
		UpdateConsoleToolbar();
#endif

#ifdef PLATFORM_PS4
		GetGame().GetUIManager().ShowUICursor( false );
		ResetFocusedContainers();
		GetMainPanel().FindAnyWidget( "CursorCharacter" ).Show( false );

		//console inventory toolbar
		m_ConsoleToolbar = GetMainPanel().FindAnyWidget( "ConsoleToolbar" );
		if ( !m_ConsoleToolbar ) 
		{
			m_ConsoleToolbar = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/inventory_new/console_inventory_toolbar.layout", GetMainPanel() );
			m_ConsoleToolbar.SetPos( 0, 0 );
		}
		UpdateConsoleToolbar();
#endif

		RefreshQuickbar();
		
		UpdateInterval();
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.UpdateInterval);
	}

	override void OnHide()
	{
		//start update
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);
		
		Serialize();
		m_MainPanel.Show( false );
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
			}
		}
		ItemManager.GetInstance().SetSelectedItem( NULL, NULL );
		#ifdef PLATFORM_XBOX
			ResetFocusedContainers();
		#endif
		
		#ifdef PLATFORM_PS4
			ResetFocusedContainers();
		#endif
	}

	void RefreshQuickbar()
	{
#ifdef PLATFORM_XBOX
#else
#ifdef PLATFORM_PS4
#else
		if ( m_Quickbar )
		{
			m_Quickbar.UpdateItems( m_QuickbarWidget );
		}
#endif
#endif	
	}


	void ShowQuickbar()
	{
#ifdef PLATFORM_XBOX
#else
#ifdef PLATFORM_PS4
#else
		if ( m_QuickbarWidget )
		{
			m_QuickbarWidget.Show( true );
		}
#endif
#endif
	}
	
	//Console toolbar
	void UpdateConsoleToolbar()
	{
		string general_text;
		string context_text;
		
#ifdef PLATFORM_XBOX
		general_text = "Switch column <image set=\"xbox_buttons\" name=\"LB\" />/<image set=\"xbox_buttons\" name=\"RB\" />        Switch cargo container <image set=\"xbox_buttons\" name=\"LT\" />/<image set=\"xbox_buttons\" name=\"RT\"/>";
		
		if ( m_LeftArea.IsActive() )
		{
			context_text = "Open/Close container <image set=\"xbox_buttons\" name=\"RS\" />    To hands/swap <image set=\"xbox_buttons\" name=\"A\" />    Combine <image set=\"xbox_buttons\" name=\"B\" />    Equip <image set=\"xbox_buttons\" name=\"X\" />    To inventory <image set=\"xbox_buttons\" name=\"Y\" />";
		}
		else if ( m_RightArea.IsActive() )
		{
			if ( m_RightArea.IsPlayerEquipmentActive() )
			{
				context_text = "Open/Close container <image set=\"xbox_buttons\" name=\"RS\" />    To hands/swap <image set=\"xbox_buttons\" name=\"A\" />    Combine <image set=\"xbox_buttons\" name=\"B\" />    To inventory <image set=\"xbox_buttons\" name=\"Y\" />    Drop <image set=\"xbox_buttons\" name=\"Y\" />(hold)";
			}
			else
			{
				context_text = "Open/Close container <image set=\"xbox_buttons\" name=\"RS\" />    To hands/swap <image set=\"xbox_buttons\" name=\"A\" />    Combine <image set=\"xbox_buttons\" name=\"B\" />    Equip <image set=\"xbox_buttons\" name=\"X\" />    Drop <image set=\"xbox_buttons\" name=\"Y\" />(hold)";
			}
		}
		else if ( m_HandsArea.IsActive() )
		{
			context_text = "Equip <image set=\"xbox_buttons\" name=\"X\" />    To inventory <image set=\"xbox_buttons\" name=\"Y\" />    Drop <image set=\"xbox_buttons\" name=\"Y\" />(hold)";
		}		
#endif

#ifdef PLATFORM_PS4
		general_text = "Switch column <image set=\"playstation_buttons\" name=\"L1\" />/<image set=\"playstation_buttons\" name=\"R1\" />        Switch cargo container <image set=\"playstation_buttons\" name=\"L2\" />/<image set=\"playstation_buttons\" name=\"R2\"/>";
		
		if ( m_LeftArea.IsActive() )
		{
			context_text = "Open/Close container <image set=\"playstation_buttons\" name=\"R3\" />    To hands/swap <image set=\"playstation_buttons\" name=\"cross\" />    Combine <image set=\"playstation_buttons\" name=\"circle\" />    Equip <image set=\"playstation_buttons\" name=\"square\" />    To inventory <image set=\"playstation_buttons\" name=\"triangle\" />";
		}
		else if ( m_RightArea.IsActive() )
		{
			if ( m_RightArea.IsPlayerEquipmentActive() )
			{
				context_text = "Open/Close container <image set=\"playstation_buttons\" name=\"R3\" />    To hands/swap <image set=\"playstation_buttons\" name=\"cross\" />    Combine <image set=\"playstation_buttons\" name=\"circle\" />    To inventory <image set=\"playstation_buttons\" name=\"triangle\" />    Drop <image set=\"playstation_buttons\" name=\"triangle\" />(hold)";
			}
			else
			{
				context_text = "Open/Close container <image set=\"playstation_buttons\" name=\"R3\" />    To hands/swap <image set=\"playstation_buttons\" name=\"cross\" />    Combine <image set=\"playstation_buttons\" name=\"circle\" />    Equip <image set=\"playstation_buttons\" name=\"square\" />    Drop <image set=\"playstation_buttons\" name=\"triangle\" />(hold)";
			}
		}
		else if ( m_HandsArea.IsActive() )
		{
			context_text = "Equip <image set=\"playstation_buttons\" name=\"square\" />    To inventory <image set=\"playstation_buttons\" name=\"triangle\" />    Drop <image set=\"playstation_buttons\" name=\"triangle\" />(hold)";
		}
#endif
		
		//!!!!! Richtext string must end with character or white space (no tag elements)
		//set toolbar text
		RichTextWidget general_toolbar = RichTextWidget.Cast( m_ConsoleToolbar.FindAnyWidget( "GeneralToolbarText" ) );
		general_text = general_text + " ";							//richtext end-tag issue hotfix
		general_toolbar.SetText( general_text );
		
		RichTextWidget context_toolbar = RichTextWidget.Cast( m_ConsoleToolbar.FindAnyWidget( "ContextToolbarText" ) );
		context_text = context_text + " ";					//richtext end-tag issue hotfix
		context_toolbar.SetText( context_text );
	}
}
