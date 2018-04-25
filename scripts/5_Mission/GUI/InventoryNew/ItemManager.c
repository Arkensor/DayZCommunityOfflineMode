class ItemManager
{
	private ref static ItemManager m_Instance;
	protected bool m_IsDragging;
	protected EntityAI m_DraggedItem;
	protected Icon m_DraggedIcon;
	protected ref Widget m_TooltipWidget;
	protected ItemPreviewWidget m_ItemPreviewWidget;
	protected Widget m_RootWidget;
	protected ref map<string, bool> m_DefautOpenStates;
	protected ref map<string, bool> m_DefautHeaderOpenStates;
 	protected int m_HandsDefaultOpenState;
	protected ref Timer m_ToolTipTimer;

	protected EntityAI m_SelectedItem;
	protected Widget m_SelectedWidget;
	protected ItemPreviewWidget m_Ipw;
	
	protected HandsPreview m_HandsPreview;
	
	protected const float TOOLTIP_DELAY = 0.75; // in seconds
	
	void ItemManager( Widget root )
	{
		m_RootWidget = root;
		m_DefautOpenStates = new map<string, bool>;
		m_DefautHeaderOpenStates = new map<string, bool>;
		m_TooltipWidget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory_new/day_z_inventory_new_tooltip.layout", NULL );
		m_TooltipWidget.Show( false );
		m_Instance = this;
	}
	
	HandsPreview GetHandsPreview()
	{
		return m_HandsPreview;
	}
	
	void SetHandsPreview( HandsPreview hansd_preview )
	{
		m_HandsPreview = hansd_preview;
	}

	EntityAI GetSelectedItem()
	{
		return m_SelectedItem;
	}

	Widget GetSelectedWidget()
	{
		return m_SelectedWidget;
	}

	void SetSelectedItem( EntityAI selected_item, Widget selected_widget )
	{
		if( m_SelectedWidget != NULL )
		{
			m_SelectedWidget.Show( false );
		}

		m_SelectedItem = selected_item;
		m_SelectedWidget = selected_widget;
	}

	void SetSelectedVicinityItem( ItemPreviewWidget ipw )
	{
		m_Ipw = ipw;
	}

	void ClearDefaultOpenStates()
	{
		m_DefautOpenStates.Clear();
	}

	void ClearDefaultHeaderOpenStates()
	{
		m_DefautHeaderOpenStates.Clear();
	}

	void SetDefaultOpenState( string type, bool is_opened )
	{
		m_DefautOpenStates.Set( type, is_opened );
	}

	void SetDefaultHeaderOpenState( string type, bool is_opened )
	{
		m_DefautHeaderOpenStates.Set( type, is_opened );
	}

	void SetDefaultOpenStateHands( bool is_opened )
	{
		m_HandsDefaultOpenState = is_opened;
		int hands_default_open_state = m_HandsDefaultOpenState; //from bool to int for easier parsing
		GetGame().SetProfileString( "defaultOpenStateHands", hands_default_open_state.ToString() );
		GetGame().SaveProfile();
	}

	bool GetDefaultOpenStateHands( )
	{
		string value;
		GetGame().GetProfileString( "defaultOpenStateHands", value );
		m_HandsDefaultOpenState = value.ToInt();
		return m_HandsDefaultOpenState;
	}

	void SerializeDefaultHeaderOpenStates()
	{
		TStringArray serialized_types = new TStringArray;

		for ( int i = 0; i < m_DefautHeaderOpenStates.Count(); i++ )
		{
			int is_opened = m_DefautHeaderOpenStates.GetElement( i ); //from bool to int for easier parsing
			serialized_types.Insert( m_DefautHeaderOpenStates.GetKey( i ) + "," + is_opened );
		}

		if( serialized_types.Count() > 0 )
		{
			GetGame().SetProfileStringList( "defaultHeaderOpenStates", serialized_types );
		}
		GetGame().SaveProfile();
	}

	void DeserializeDefaultHeaderOpenStates()
	{
		ItemManager.GetInstance().ClearDefaultHeaderOpenStates();

		TStringArray serialized_types = new TStringArray;
		GetGame().GetProfileStringList( "defaultHeaderOpenStates", serialized_types );

		for( int i = 0; i < serialized_types.Count(); i++ )
		{
			TStringArray strs = new TStringArray;
			serialized_types.Get( i ).Split( ",", strs );
			bool is_opened = strs.Get( 1 ).ToInt();
			SetDefaultHeaderOpenState( strs.Get( 0 ), is_opened );
		}
	}

	void SerializeDefaultOpenStates()
	{
		TStringArray serialized_types = new TStringArray;

		for ( int i = 0; i < m_DefautOpenStates.Count(); i++ )
		{
			int is_opened = m_DefautOpenStates.GetElement( i ); //from bool to int for easier parsing
			serialized_types.Insert( m_DefautOpenStates.GetKey( i ) + "," + is_opened );
		}

		if( serialized_types.Count() > 0 )
		{
			GetGame().SetProfileStringList( "defaultOpenStates", serialized_types );
		}
		GetGame().SaveProfile();
	}

	void DeserializeDefaultOpenStates()
	{
		ItemManager.GetInstance().ClearDefaultOpenStates();

		TStringArray serialized_types = new TStringArray;
		GetGame().GetProfileStringList( "defaultOpenStates", serialized_types );

		for( int i = 0; i < serialized_types.Count(); i++ )
		{
			TStringArray strs = new TStringArray;
			serialized_types.Get( i ).Split( ",", strs );
			bool is_opened = strs.Get( 1 ).ToInt();
			SetDefaultOpenState( strs.Get( 0 ), is_opened );
		}
	}

	bool GetDefaultOpenState( string type )
	{
		if( m_DefautOpenStates.Contains( type ) )
		{
			return m_DefautOpenStates.Get( type );
		}
		else
		{
			return true;
		}
	}

	bool GetDefaultHeaderOpenState( string type )
	{
		if( m_DefautHeaderOpenStates.Contains( type ) )
		{
			return m_DefautHeaderOpenStates.Get( type );
		}
		else
		{
			return true;
		}
	}

	static ItemManager GetInstance()
	{
		return m_Instance;
	}

	void HideDropzones()
	{
		GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( false );
		GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( false );
		GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( false );
	}

	void ShowSourceDropzone( EntityAI item )
	{
		EntityAI owner = item.GetHierarchyParent();
		if( owner && owner != GetGame().GetPlayer() )
		{
			ShowSourceDropzone( owner );
		}
		else
		{
			InventoryLocation inv_loc_src = new InventoryLocation;
			item.GetInventory().GetCurrentInventoryLocation( inv_loc_src );
			int loc_type = inv_loc_src.GetType();
			HideDropzones();
			if( loc_type == InventoryLocationType.GROUND )
			{
				GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else if( loc_type == InventoryLocationType.HANDS )
			{
				GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
			else
			{
				GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			}
		}
	}

	Widget GetRootWidget()
	{
		return m_RootWidget;
	}

	void SetDraggedItem( EntityAI dragged_item )
	{
		m_DraggedItem = dragged_item;
	}

	EntityAI GetDraggedItem()
	{
		return m_DraggedItem;
	}

	void SetDraggedIcon( Icon dragged_icon )
	{
		m_DraggedIcon = dragged_icon;
	}

	Icon GetDraggedIcon()
	{
		return m_DraggedIcon;
	}

	void SetIsDragging( bool is_dragging )
	{
		m_IsDragging = is_dragging;
	}

	bool IsDragging()
	{
		return m_IsDragging;
	}

	void HideTooltip()
	{
		m_TooltipWidget.Show( false );
		delete m_ItemPreviewWidget;
		delete m_ToolTipTimer;
	}

	void SetTemperature( EntityAI item, Widget item_w )
	{
		if ( item_w )
		{
			if ( true/*show_temperature */ )
			{
				if ( item && item.IsInherited( InventoryItem ) )
				{
					int color = ColorManager.GetInstance().GetItemColor( InventoryItem.Cast( item ) ); // !!!!!
					if ( color )
					{
						string name = item_w.GetName();
						name.Replace("Render", "Temperature");
						Widget temperature_widget = item_w.GetParent().FindAnyWidget( name );
						if( color != -1 )
						{
							temperature_widget.Show( true );
							temperature_widget.SetColor( color );
						}
						else
						{
							temperature_widget.Show( false );
						}

						temperature_widget.SetAlpha( 0.3 );
					}
				}
			}
		}
	}

	void SetIconTemperature( EntityAI item, Widget item_w )
	{
		if ( item_w )
		{
			if ( true/*show_temperature */ )
			{
				if ( item && item.IsInherited( InventoryItem ) )
				{
					int color = ColorManager.GetInstance().GetItemColor( InventoryItem.Cast( item ) ); // !!!!!

					if ( color )
					{
						Widget color_widget = item_w.FindAnyWidget("Color");
						if( color != -1 )
						{
							color_widget.SetColor( color );
						}
						else
						{
							color_widget.SetColor( ColorManager.BASE_COLOR );
						}

						//color_widget.SetAlpha( 0.3 );
					}
				}
			}
		}
	}

	void PrepareTooltip( EntityAI item )
	{
		if( IsDragging() )
		{
			return;
		}

		if ( item.IsInherited( InventoryItem) )
		{
			UpdateItemInfo( m_TooltipWidget, item );
			int x, y, screen_w, screen_h;
			float w, h;

			GetMousePos(x,y);
			GetScreenSize(screen_w, screen_h);
			m_TooltipWidget.GetScreenSize(w,h);

			int m_normal_item_size = 32;
			screen_w -= 10;
			screen_h -= 10;
			x += m_normal_item_size;
			y += m_normal_item_size;

			int right_edge = x + w;
			if (right_edge > screen_w)
			{
				x = screen_w - w;
			}

			int bottom_edge = y + h;
			if (bottom_edge > screen_h)
			{
				y = y - h - (2*m_normal_item_size);
			}

			m_TooltipWidget.SetPos(x, y);

			m_ToolTipTimer = new Timer();
			
			m_ToolTipTimer.Run( TOOLTIP_DELAY, this, "ShowTooltip" );
			
			// item preview
			delete m_ItemPreviewWidget;

			Widget preview_frame = m_TooltipWidget.FindAnyWidget("ItemFrameWidget");
			if (preview_frame)
			{
				float ww;
				float hh;
				preview_frame.GetSize(ww, hh);
				m_ItemPreviewWidget = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 210, preview_frame) );
				m_ItemPreviewWidget.SetItem(item);
			}
		}
	}
	
	void SetWidgetDraggable( Widget w, bool draggable )
	{
		if( draggable )
		{
			w.SetFlags( WidgetFlags.DRAGGABLE );
		}
		else
		{
			w.ClearFlags( WidgetFlags.DRAGGABLE );
		}
	}
	
	void ShowTooltip()
	{	
		if( m_ItemPreviewWidget )
		{
			m_TooltipWidget.Show( true );
		}
	}
}
