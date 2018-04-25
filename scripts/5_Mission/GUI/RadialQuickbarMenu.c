class RadialQuickbarItem
{
	protected int 					m_Id;
	protected EntityAI 				m_Item;
	protected string 				m_ItemName;
	
	//radial menu
	protected Widget 			m_RadialMenuSelector;
	protected Widget			m_RadialMenuItemCard;
	
	void RadialQuickbarItem( int id, EntityAI item, string item_name )
	{
		m_Id			= id;
		m_Item 			= item;
		m_ItemName		= item_name;
	}
	
	EntityAI GetItem()
	{
		return m_Item;
	}
	
	int GetId()
	{
		return m_Id;
	}
		
	Widget GetRadialItemCard()
	{
		return m_RadialMenuItemCard;
	}
	
	void SetRadialItemCard( Widget widget )
	{
		m_RadialMenuItemCard = widget;
	}
	
	string GetItemName()
	{
		return m_ItemName;
	}
}

class RadialQuickbarMenu extends UIScriptedMenu
{
	protected Widget 								m_ItemCardPanel;
	protected ref array<ref RadialQuickbarItem> 	m_Items;
	
	//
	const string 									TEXT_ITEM_NAME	= "ItemName";
	const string 									TEXT_ITEM_TITLE	= "ItemTitle";
	//selections
	protected Widget 								m_SelectedItem;
	static EntityAI									m_ItemToAssign;
	
	//instance
	static RadialQuickbarMenu						instance;
	
	//============================================
	// RadialQuickbarMenu
	//============================================
	void RadialQuickbarMenu()
	{
		m_Items = new ref array<ref RadialQuickbarItem>;
		
		if ( !instance )
		{
			instance = this;
		}
	}
	
	void ~RadialQuickbarMenu()
	{
	}
	
	static void SetItemToAssign( EntityAI item )
	{
		m_ItemToAssign = item;
	}
	
	static EntityAI GetItemToAssign()
	{
		return m_ItemToAssign;
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		//reset item to assign
		RadialQuickbarMenu.SetItemToAssign( NULL );
	}

	//============================================
	// Menu Controls
	//============================================	
	static void OpenMenu( UIScriptedMenu parent = NULL )
	{
		GetGame().GetUIManager().EnterScriptedMenu( MENU_RADIAL_QUICKBAR, parent );
	}
	
	static void CloseMenu()
	{
		GetGame().GetUIManager().Back();
	}
		
	//============================================
	// Init & Widget Events
	//============================================
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/radial_quickbar/radial_quickbar_menu.layout" );
		m_ItemCardPanel = layoutRoot.FindAnyWidget( RadialMenu.RADIAL_ITEM_CARD_CONTAINER );

		//register gestures menu
		RadialMenu.GetInstance().RegisterClass( this );
		
		//set radial menu properties
		RadialMenu.GetInstance().SetWidgetProperties( "gui/layouts/radial_quickbar/radial_quickbar_delimiter.layout" );

		//create content (widgets) for items
		RefreshQuickbar();
		
		return layoutRoot;
	}
	
	//============================================
	// Content
	//============================================
	protected void RefreshQuickbar()
	{
		GetItems( m_Items );
		CreateContent();
	}	
	
	//
	//	ITEMS
	//
	void GetItems( out ref array<ref RadialQuickbarItem> items )
	{
		items.Clear();
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		int size = player.GetQuickBarSize();
		EntityAI entity;
		
		for ( int i = 0; i < size; ++i )
		{
			entity = player.GetQuickBarEntity( i );
			
			items.Insert( new RadialQuickbarItem( i, entity, "" ) );
		}
	}
	
	void CreateContent( bool hide_selector = true )
	{
		//delete existing content
		DeleteItems();
		
		InventoryManager inv_manager = InventoryManager.GetInstance();
		
		for ( int i = 0; i < m_Items.Count(); ++i )
		{
			RadialQuickbarItem quickbar_item = m_Items.Get( i );
			
			//create item card
			Widget item_card_widget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "gui/layouts/radial_quickbar/radial_quickbar_item_card.layout", m_ItemCardPanel ) );
			quickbar_item.SetRadialItemCard( item_card_widget );
			
			//get content panels
			Widget item_details = item_card_widget.FindAnyWidget( "ItemDetails" );
			TextWidget item_title = TextWidget.Cast( item_card_widget.FindAnyWidget( "ItemTitle" ) );
			
			//set text
			TextWidget text_widget = TextWidget.Cast( item_card_widget.FindAnyWidget( TEXT_ITEM_NAME ) );
			EntityAI item = quickbar_item.GetItem();
			
			Widget quantity_panel = item_card_widget.FindAnyWidget( "QuantityPanel" );
			if ( item ) 
			{
				//item text
				text_widget.SetText( quickbar_item.GetItem().GetDisplayName() );
				
				//item preview
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( item_card_widget.FindAnyWidget( "ItemPreview" ) );
				item_preview.SetItem( item );
				item_preview.SetModelOrientation( Vector( 0,0,0 ) );
				
				//item quantity
				Widget quantity_stack = quantity_panel.FindAnyWidget( "QuantityStackPanel" );
				ProgressBarWidget quantity_bar = ProgressBarWidget.Cast( quantity_panel.FindAnyWidget( "QuantityBar" ) );
				int has_quantity = inv_manager.HasItemQuantity( item );
				//calculate and set quantity
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					quantity_panel.Show( false );
				}
				else if ( has_quantity == QUANTITY_COUNT )
				{
					//hide bar
					quantity_bar.Show( false );
					
					//show stack
					TextWidget quantity_text = TextWidget.Cast( quantity_stack.FindAnyWidget( "Quantity" ) );
					quantity_text.SetText( inv_manager.GetItemQuantityText( item ) );
					quantity_stack.Show( true );
				}
				else if ( has_quantity == QUANTITY_PROGRESS )
				{
					//hide stack
					quantity_stack.Show( false );
					
					//show bar
					float progress_max = quantity_bar.GetMax();
					int max = item.ConfigGetInt( "varQuantityMax" );
					int count = item.ConfigGetInt( "count" );
					float quantity = inv_manager.GetItemQuantity( ItemBase.Cast( item ) );
					
					if ( count > 0 )
					{
						max = count;
					}
					if ( max > 0 )
					{

						float value = Math.Round( ( quantity / max ) * 100 );
						quantity_bar.SetCurrent( value );
					}
					
					quantity_bar.Show( true );
				}
				
				//display content panels
				item_details.Show( true );
				item_title.Show( false );
			}
			else
			{
				item_title.SetText( "Empty" );
				
				//display content panels
				item_details.Show( false );
				item_title.Show( true );			
			}
			
			//set data
			item_card_widget.SetUserData( quickbar_item );
		}
		
		//adjust radial parameters for content
		if ( m_Items.Count() > 0 ) 
		{
			RadialMenu.GetInstance().AdjustRadialMenu( 0, 0.5, 0, 0.25, false );
		}		
		
		//refresh radial menu
		RadialMenu.GetInstance().Refresh( false );
	}
	
	//Common
	void DeleteItems()
	{
		Widget child;
		Widget child_to_destroy;
		
		child = m_ItemCardPanel.GetChildren();
		while ( child )
		{
			child_to_destroy = child;
			child = child.GetSibling();
			
			delete child_to_destroy;
		}		
	}
	
	//============================================
	// Radial Menu Events
	//============================================
	void OnSelectionSelect( Widget w )
	{
		m_SelectedItem = w;
		
		if ( w )
		{
			RadialQuickbarItem quickbar_item;
			w.GetUserData( quickbar_item );

			//is not category
			if ( quickbar_item )
			{	
				if ( quickbar_item.GetItem() )
				{	
					//alter item visual
					TextWidget text_widget = TextWidget.Cast( quickbar_item.GetRadialItemCard().FindAnyWidget( TEXT_ITEM_NAME ) );
					text_widget.SetColor( ARGB( 255, 66, 175, 95 ) );
				}
				else
				{
					//alter item visual
					TextWidget title_widget = TextWidget.Cast( quickbar_item.GetRadialItemCard().FindAnyWidget( TEXT_ITEM_TITLE ) );
					title_widget.SetColor( ARGB( 255, 66, 175, 95 ) );					
				}				
			}

			//Print("RadialQuickbarMenu->OnSelectionSelect");			
		}
	}
	
	void OnSelectionDeselect( Widget w )
	{
		m_SelectedItem = NULL;
		
		if ( w )
		{
			RadialQuickbarItem quickbar_item;
			w.GetUserData( quickbar_item );

			//is not category
			if ( quickbar_item )
			{			
				if ( quickbar_item.GetItem() )
				{
					//alter item visual
					TextWidget text_widget = TextWidget.Cast( quickbar_item.GetRadialItemCard().FindAnyWidget( TEXT_ITEM_NAME ) );
					text_widget.SetColor( ARGB( 255, 255, 255, 255 ) );
				}
				else
				{
					//alter item visual
					TextWidget title_widget = TextWidget.Cast( quickbar_item.GetRadialItemCard().FindAnyWidget( TEXT_ITEM_TITLE ) );
					title_widget.SetColor( ARGB( 255, 255, 255, 255 ) );					
				}
			}
			
			//Print("RadialQuickbarMenu->OnSelectionDeselect");
		}
	}	
	
	void OnControllerPressA( Widget w )
	{
		if ( instance.m_SelectedItem )
		{
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				RadialQuickbarItem quickbar_item;
				instance.m_SelectedItem.GetUserData( quickbar_item );
			
				if ( quickbar_item ) 
				{
					PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
					
					//ASSIGN ACTION
					if ( GetItemToAssign() )
					{
						//assign item to slot
						player.SetQuickBarEntityShortcut( GetItemToAssign(), quickbar_item.GetId() );
						RefreshQuickbar();
					}
					//SWAP
					else
					{
						EntityAI item = quickbar_item.GetItem();
						
						if ( item )
						{
							//swap
							player.RadialQuickBarSingleUse( quickbar_item.GetId() + 1 );				//id must begin with 1 (simulating key press 1-9)
							RefreshQuickbar();
						}	
					}
				}
			}
		}
		
		//Print("RadialQuickbarMenu->OnControllerPressA");
	}
	
	void OnControllerPressB( Widget w )
	{
		if ( instance.m_SelectedItem )
		{
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				RadialQuickbarItem quickbar_item;
				instance.m_SelectedItem.GetUserData( quickbar_item );
			
				if ( quickbar_item ) 
				{
					PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
					EntityAI item = quickbar_item.GetItem();
					
					//COMBINE					
					if ( item )
					{
						player.RadialQuickBarCombine( quickbar_item.GetId() + 1 );					//id must begin with 1 (simulating key press 1-9)
						RefreshQuickbar();
					}	
				}
			}
		}
		
		//Print("RadialQuickbarMenu->OnControllerPressB");
	}	
}
