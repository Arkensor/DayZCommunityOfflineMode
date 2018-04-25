class ContainerBase: ScriptedWidgetEventHandler
{
	protected Widget			m_MainPanel;
	protected Widget			m_ParentWidget;
	protected ContainerBase		m_Parent;
	protected string			m_LayoutName;
	int							m_FocusedRow = 0;
	int							m_FocusedColumn = 0;
	
	protected WidgetCacheObject	m_CacheObject;

	void ContainerBase( ContainerBase parent )
	{		
		m_Parent = parent;
		
		this.SetLayoutName();
		this.SetParentWidget();
		
		if( m_LayoutName )
		{
			m_CacheObject = GetWidgetCache().Get( m_LayoutName );
			if( m_CacheObject )
			{
				m_MainPanel = m_CacheObject.GetWidget();
				m_MainPanel.Show( true );
			}
			else
			{
				m_MainPanel = GetGame().GetWorkspace().CreateWidgets( m_LayoutName );
				m_MainPanel.Show( true );
			}
		}
		
		if( m_ParentWidget )
		{
			m_ParentWidget.Show( true );
			if( m_MainPanel )
			{
				m_ParentWidget.AddChild( m_MainPanel );
			}
		}
	}
	
	void ~ContainerBase()
	{
		if( m_CacheObject )
		{
			GetWidgetCache().Return( m_LayoutName, m_CacheObject );
		}
		else
		{
			delete m_MainPanel;
		}
	}
	
	void UpdateInterval() {}

	void SetLayoutName() {}

	void SetParentWidget()
	{
		if( m_Parent != NULL )
		{
			m_ParentWidget = m_Parent.GetMainPanel();
		}
	}
	
	ContainerBase GetParent()
	{
		return m_Parent;
	}

	void SetActive( bool active )
	{
	}

	bool IsActive()
	{
		if( m_MainPanel.FindAnyWidget("SelectedContainer") )
			return m_MainPanel.FindAnyWidget("SelectedContainer").IsVisible();
		return true;
	}

	Widget GetMainPanel()
	{
		return m_MainPanel;
	}

	void OnShow()
	{
		m_MainPanel.Show( true );
		if( m_ParentWidget )
		{
			m_ParentWidget.Show( true );
		}
	}

	void OnHide()
	{
		m_MainPanel.Show( false );
	}

	void Refresh()
	{
		m_MainPanel.Update();
	}
	
	void InspectItem(InventoryItem item)
	{
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		InspectMenuNew inspect_menu = InspectMenuNew.Cast( menu.EnterScriptedMenu(MENU_INSPECT) );
		if ( inspect_menu )
		{
			inspect_menu.SetItem(item);
		}
	}
	
	WidgetCache GetWidgetCache()
	{
		MissionGameplay mission = MissionGameplay.Cast( GetGame().GetMission() );
		WidgetCache cache = mission.GetWidgetCache();
		return cache;
	}
}
