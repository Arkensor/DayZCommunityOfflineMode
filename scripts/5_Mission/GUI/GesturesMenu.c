enum GestureCategories
{
	CATEGORIES,				//special category selection
	CATEGORY_1,
	CATEGORY_2,
	CATEGORY_3,
	CATEGORY_4,
	CATEGORY_5,
	CONSOLE_GESTURES
}

class GestureMenuItem
{
	protected int 					m_ID;
	protected string 				m_Name;
	protected GestureCategories 	m_Category;
	//radial menu
	protected Widget 				m_RadialMenuSelector;
	protected Widget				m_RadialMenuItemCard;
	
	void GestureMenuItem( int id, string name, GestureCategories category )
	{
		m_ID			= id;
		m_Name 			= name;
		m_Category 		= category;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	int GetID()
	{
		return m_ID;
	}
	
	GestureCategories GetCategory()
	{
		return m_Category;
	}
	
	Widget GetRadialItemCard()
	{
		return m_RadialMenuItemCard;
	}
	
	void SetRadialItemCard( Widget widget )
	{
		m_RadialMenuItemCard = widget;
	}
}

class GesturesMenu extends UIScriptedMenu
{
	protected Widget 							m_GestureItemCardPanel;
	protected ref array<ref GestureMenuItem> 	m_GestureItems;
	
	//
	const string 								RADIAL_TEXT		= "RadialText";
	//selections
	protected Widget 							m_SelectedItem;
	protected bool 								m_IsCategorySelected;
	
	//instance
	static GesturesMenu							instance;
	
	//============================================
	// GesturesMenu
	//============================================
	void GesturesMenu()
	{
		m_GestureItems = new ref array<ref GestureMenuItem>;
		
		if ( !instance )
		{
			instance = this;
		}
	}
	
	void ~GesturesMenu()
	{
	}

	//============================================
	// Menu Controls
	//============================================	
	static void OpenMenu()
	{
		GetGame().GetUIManager().EnterScriptedMenu( MENU_GESTURES, NULL );
	}
	
	static void CloseMenu()
	{
		instance.OnReleaseExecute();
		
		GetGame().GetUIManager().Back();
	}
		
	//============================================
	// Init & Widget Events
	//============================================
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/day_z_gestures.layout" );
		m_GestureItemCardPanel = layoutRoot.FindAnyWidget( RadialMenu.RADIAL_ITEM_CARD_CONTAINER );

		//register gestures menu
		RadialMenu.GetInstance().RegisterClass( this );
		
		//set radial menu properties
		RadialMenu.GetInstance().SetWidgetProperties( "gui/layouts/day_z_gesture_delimiter.layout" );
		
		//create content (widgets) for items
		RefreshGestures();
		
		return layoutRoot;
	}
	
	//============================================
	// Gestures
	//============================================
	protected void RefreshGestures( GestureCategories category_id = -1 )
	{
		//create gestures content (widgets) based on categories
		if ( category_id > -1 )
		{
			GetGestureItems( m_GestureItems, category_id );
		}
		else
		{
#ifdef PLATFORM_XBOX
		GetGestureItems( m_GestureItems, GestureCategories.CONSOLE_GESTURES );
#else		
#ifdef PLATFORM_PS4
		GetGestureItems( m_GestureItems, GestureCategories.CONSOLE_GESTURES );
#else
		GetGestureItems( m_GestureItems, GestureCategories.CATEGORIES );
#endif
#endif
		}		
		CreateGestureContent();
	}		
	
	void GetGestureItems( out ref array<ref GestureMenuItem> gesture_items, GestureCategories category )
	{
		gesture_items.Clear();
		
		//PC PLATFORM
		//All categories
		if ( category == GestureCategories.CATEGORIES )
		{
			gesture_items.Insert( new GestureMenuItem( GestureCategories.CATEGORY_1, "Category 1", GestureCategories.CATEGORIES ) );
			gesture_items.Insert( new GestureMenuItem( GestureCategories.CATEGORY_2, "Category 2", GestureCategories.CATEGORIES ) );
			gesture_items.Insert( new GestureMenuItem( GestureCategories.CATEGORY_3, "Category 3", GestureCategories.CATEGORIES ) );
			gesture_items.Insert( new GestureMenuItem( GestureCategories.CATEGORY_4, "Category 4", GestureCategories.CATEGORIES ) );
		}
		//Category 1
		else if ( category == GestureCategories.CATEGORY_1 )
		{
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_LYINGDOWN, 	"Lay down", GestureCategories.CATEGORY_1 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_DABBING, 	"Dabbing", 	GestureCategories.CATEGORY_1 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_THUMB, 		"Thumb", 	GestureCategories.CATEGORY_1 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_TAUNT, 		"Taunt", 	GestureCategories.CATEGORY_1 ) );
		}
		//Category 2
		else if ( category == GestureCategories.CATEGORY_2 )
		{
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_TIMEOUT,	"Timeout", 	GestureCategories.CATEGORY_2 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_HEART, 		"Heart", 	GestureCategories.CATEGORY_2 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_FACEPALM, 	"Facepalm", GestureCategories.CATEGORY_2 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_CLAP, 		"Clap", 	GestureCategories.CATEGORY_2 ) );			
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_SILENT, 	"Silent", 	GestureCategories.CATEGORY_2 ) );
		}
		//Category 3
		else if ( category == GestureCategories.CATEGORY_3 )
		{
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_GREETING, 	"Greeting", GestureCategories.CATEGORY_3 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_SALUTE, 	"Salute", 	GestureCategories.CATEGORY_3 ) );
			//gesture_items.Insert( new GestureMenuItem( ID_EMOTE_SOS, 		"SOS", 		GestureCategories.CATEGORY_3 ) );			
		}
		//Category 4
		else if ( category == GestureCategories.CATEGORY_4 )
		{
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_DANCE, 	"Dance", 	GestureCategories.CATEGORY_4 ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_RPS, 	"RPS", 		GestureCategories.CATEGORY_4 ) );
		}
		else
		{
		}
		
		//CONSOLE PLATFORM ONLY
		//Only 1 category
		if ( category == GestureCategories.CONSOLE_GESTURES )
		{
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_GREETING, 	"Hello", 		GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_HEART, 		"Heart", 		GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_SOS, 		"Wave", 		GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_TAUNTKISS,	"Magnifique", 	GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_THROAT, 	"Die", 			GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_TAUNT, 		"Frig off", 	GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_DANCE, 		"Dance", 		GestureCategories.CONSOLE_GESTURES ) );
			gesture_items.Insert( new GestureMenuItem( ID_EMOTE_THUMB, 		"Thumbs up", 	GestureCategories.CONSOLE_GESTURES ) );
		}
	}
	
	void CreateGestureContent()
	{
		//delete existing gesture widgets
		DeleteGestureItems();
		
		for ( int i = 0; i < m_GestureItems.Count(); ++i )
		{
			GestureMenuItem gesture_item = m_GestureItems.Get( i );
			
			//create item card
			Widget gesture_item_card_widget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "gui/layouts/day_z_gesture_item_card.layout", m_GestureItemCardPanel ) );
			gesture_item.SetRadialItemCard( gesture_item_card_widget );
			
			//set text
			TextWidget text_widget = TextWidget.Cast( gesture_item_card_widget.FindAnyWidget( RADIAL_TEXT ) );
			text_widget.SetText( gesture_item.GetName() );
			
			//set data
			gesture_item_card_widget.SetUserData( gesture_item );
		}
		
		//adjust radial parameters for content
		if ( m_GestureItems.Count() > 0 ) 
		{
			RadialMenu.GetInstance().AdjustRadialMenu( 0, 0.5, 0, 0.25, true );
		}		
		
		//refresh radial menu
		RadialMenu.GetInstance().Refresh();
	}
	
	void DeleteGestureItems()
	{
		Widget child;
		Widget child_to_destroy;
		
		child = m_GestureItemCardPanel.GetChildren();
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
		instance.m_SelectedItem = w;
		
		if ( w )
		{
			GestureMenuItem gesture_item;
			w.GetUserData( gesture_item );

			//is not category
			if ( gesture_item && gesture_item.GetCategory() != GestureCategories.CATEGORIES )
			{	
				//alter item visual
				TextWidget text_widget = TextWidget.Cast( gesture_item.GetRadialItemCard().FindAnyWidget( RADIAL_TEXT ) );
				text_widget.SetColor( ARGB( 255, 66, 175, 95 ) );
			}

			//Print("GesturesMenu->OnSelectionSelect");			
		}
	}
	
	void OnSelectionDeselect( Widget w )
	{
		instance.m_SelectedItem = NULL;
		
		if ( w )
		{
			GestureMenuItem gesture_item;
			w.GetUserData( gesture_item );

			//is not category
			if ( gesture_item && gesture_item.GetCategory() != GestureCategories.CATEGORIES )
			{			
				//alter item visual
				TextWidget text_widget = TextWidget.Cast( gesture_item.GetRadialItemCard().FindAnyWidget( RADIAL_TEXT ) );
				text_widget.SetColor( ARGB( 255, 255, 255, 255 ) );
			}
			
			//Print("GesturesMenu->OnSelectionDeselect");			
		}
	}	
	
	void OnSelectionExecute( Widget w )
	{
		//only when category is not picked yet
		if ( w && !instance.m_IsCategorySelected )
		{
			GestureMenuItem gesture_item;
			w.GetUserData( gesture_item );
			
			//is category
			if ( gesture_item.GetCategory() == GestureCategories.CATEGORIES )
			{
				//show selected category gestures
				GetGestureItems( m_GestureItems, gesture_item.GetID() );
				CreateGestureContent();
				RefreshGestures( gesture_item.GetID() );
				
				//set category selected
				instance.m_IsCategorySelected = true;
			}
			
			//Print("GesturesMenu->OnSelectionExecuted");			
		}
	}
	
	void OnReleaseExecute()
	{
		if ( instance.m_SelectedItem )
		{
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				
				GestureMenuItem gesture_item;
				instance.m_SelectedItem.GetUserData( gesture_item );
			
				if ( gesture_item ) 
				{
					if( player.GetEmoteManager() ) 
					{
						player.GetEmoteManager().SetGesture( gesture_item.GetID() );
						player.GetEmoteManager().PlayEmote( gesture_item.GetID() );
					}
				}
			}
		}
	}
}
