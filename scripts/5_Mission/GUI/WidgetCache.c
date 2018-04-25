class WidgetCache
{
	private ref static WidgetCache s_instance;

	protected ref map<string, ref array<ref WidgetCacheObject>> m_widgets_cache;

	const int INVENTORY_CACHES = 5;
	const int INVENTORY_ICON_CACHES = 5;
	const int INVENTORY_ICON_CONTAINER_CACHES = 5;
	const int COLLAPSIBLE_HEADER_CACHES = 5;
	ref Inventory m_Inventory;

	static WidgetCache GetInstance()
	{
		return s_instance;
	}

	Inventory GetInv()
	{
		return m_Inventory;
	}

	void WidgetCache()
	{
		s_instance = this;
		m_widgets_cache = new map<string, ref array<ref WidgetCacheObject>>;

		CreateWidgetsCache();
		/*m_Inventory = new Inventory( NULL );
		m_Inventory.Reset();
		m_Inventory.OnHide();*/
	}

	void Reset()
	{
		//m_Inventory.Reset();
	}

	void CreateWidgetsCache()
	{
		CreateWidgetCache( WidgetLayoutName.Inventory, INVENTORY_CACHES );
		CreateWidgetCache( WidgetLayoutName.InventoryIcon, INVENTORY_ICON_CACHES );
		CreateWidgetCache( WidgetLayoutName.InventoryIconContainer, 5 );
		CreateWidgetCache( WidgetLayoutName.CollapsibleHeader, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.LeftArea, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.CollapsibleContainer, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.ClosableHeader, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.HandsHeader, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.HandsArea, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.ClosableContainer, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.GridContainer, COLLAPSIBLE_HEADER_CACHES );
	}

	void CreateWidgetCache( string layoutName, int numberOfCaches )
	{
		ref array<ref WidgetCacheObject> widgets = new array<ref WidgetCacheObject>;

		for( int i = 0; i < numberOfCaches; i++)
		{
			ref Widget w = GetGame().GetWorkspace().CreateWidgets( layoutName, NULL );
			w.Show( false );
			if( layoutName == WidgetLayoutName.Inventory )
			w.SetSort(50);
			ref WidgetCacheObject w_cache_object = new WidgetCacheObject( w );
			widgets.Insert( w_cache_object );
		}

		m_widgets_cache.Insert( layoutName, widgets );
	}

	WidgetCacheObject Get( string layoutName )
	{
		array<ref WidgetCacheObject> objects = m_widgets_cache.Get( layoutName );
		if( objects && objects.Count() > 0 )
		{
			WidgetCacheObject w = objects.Get( 0 );
			objects.Remove(0);
	
			w.SaveOriginalValeus();
		}
		return w;
	}

	void Return( string layoutName, ref WidgetCacheObject w )
	{
		w.RestoreOriginalValeus();
		m_widgets_cache.Get( layoutName ).Insert( w );
	}
}
