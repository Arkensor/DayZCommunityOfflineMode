class ColorManager
{
	private ref static ColorManager m_Instance;
	
	static int ITEM_BACKGROUND_COLOR = ARGB( 50, 255, 255, 255 );
	static int BASE_COLOR = ARGB( 200, 95, 95, 95 );
	static int RED_COLOR = ARGB( 150, 255, 1, 1 );
	static int GREEN_COLOR = ARGB( 150, 1, 255, 1 );
	static int SWAP_COLOR = ARGB( 150, 135, 206, 250 );
	static int COMBINE_COLOR = ARGB( 150, 255, 165, 0 );
	
	void ColorManager()
	{
		m_Instance = this;
	}
	
	void Test()
	{
		Print("teset");
	}
	
	static ColorManager GetInstance()
	{
		return m_Instance;
	}
	
	void SetColor( Widget w, int color )
	{
		if( w.FindAnyWidget( "Color" ) )
		{
			w.FindAnyWidget( "Color" ).SetColor( color );
		}
		else
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Col" );
			Widget w2 = w.FindAnyWidget( name );
			if( w2 )
			{
				w2.SetColor( color );
			}
		}
	}
	
	int GetItemColor( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( dragged_item ) return 0;
		// int color = InventoryGrid.ITEM_COLOR_NORMAL;
		int color = -1;

		// if item icon is main icon (view, viewgrid )
		if ( item.IsInherited( InventoryItem ) && item.GetHierarchyParent() && item.GetHierarchyParent() != player )
		{
			color = -1;
		}

		if( item.IsInherited( InventoryItem )  ) 
		{
			ItemBase item_base = ItemBase.Cast( item );
			if( item_base )
			{
				float temperature = item_base.GetTemperature();
				if ( temperature )
				{
					color = GetTemperatureColor( temperature );
				}
			}
		}
		return color;
	}
}
