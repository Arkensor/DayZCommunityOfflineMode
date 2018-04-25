class UIScriptedWindow
{
	Widget	m_WgtRoot;
	int m_Id;

	//---MOVE TO UIMANAGER WHEN FIXED
	static ref map<int, UIScriptedWindow> m_ActiveWindows;
	
	static void AddToActiveWindows( int id, UIScriptedWindow window )
	{
		if ( m_ActiveWindows == NULL )
		{
			m_ActiveWindows = new map<int, UIScriptedWindow>;
		}
			
		m_ActiveWindows.Insert( id, window );
	}
	
	static void RemoveFromActiveWindows( int id )
	{
		if ( m_ActiveWindows )
		{
			m_ActiveWindows.Remove( id );	
		}
	}
	
	static UIScriptedWindow GetWindow( int id )
	{
		if ( m_ActiveWindows )
		{
			return m_ActiveWindows.Get( id );
		}
		
		return NULL;
	}
	
	static map<int, UIScriptedWindow> GetActiveWindows()
	{
		return m_ActiveWindows;
	}
	//---

	
	void UIScriptedWindow(int id)
	{
		m_Id = id;
	}
		
	void ~UIScriptedWindow()
	{
		GetWidgetRoot().Show(false);
		delete GetWidgetRoot();
	}
	
	Widget GetWidgetRoot()
	{
		return m_WgtRoot;
	}
	
	Widget Init()
	{
	}
	
	void ShowWindow()
	{
		GetWidgetRoot().Show( true );
	}
	
	void HideWindow()
	{
		GetWidgetRoot().Show( false );
	}
	
	void CloseWindow()
	{
		GetGame().GetUIManager().CloseWindow( m_Id );
	}
	
	//--- EVENTS
	bool OnClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		return false;
	}
	
	bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	bool OnSelect(Widget w, int x, int y)
	{
		return false;
	}
	
	bool OnItemSelected(Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn)
	{
		return false;
	}
	
	bool OnFocus(Widget w, int x, int y)
	{
		return false;
	}
	
	bool OnFocusLost(Widget w, int x, int y)
	{
		return false;
	}
	
	bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}
	
	bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return false;
	}
	
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		return false;
	}
	
	bool OnController(Widget w, int control, int value)
	{
		return false;
	}
	
	bool OnKeyDown(Widget w, int x, int y, int key)
	{
		return false;
	}
	
	bool OnKeyUp(Widget w, int x, int y, int key)
	{
		return false;
	}
	
	bool OnKeyPress(Widget w, int x, int y, int key)
	{
		return false;
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		return false;
	}
	
	bool OnDrag(Widget w, int x, int y)
	{
		return false;
	}
	
	bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	
	bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	
	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	
	bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		return false;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}