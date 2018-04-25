//--------------------------------------------------------------------------
class ContextMenu extends ScriptedWidgetEventHandler
{
	const int ITEMS_COUNT = 27;
	private Widget m_context_menu_root_widget;
	private Widget m_context_menu_panel_widget;
	private ref array<ref CallQueueContext> m_commands;
	private int m_max_item_width;
	private int m_count;
	//--------------------------------------------------------------------------
	void ContextMenu()
	{
		m_commands = new array<ref CallQueueContext>;
		m_count = 0;
	}
	//--------------------------------------------------------------------------
	void ~ContextMenu()
	{
		Clear();
	
		delete m_context_menu_root_widget;
	}
	//--------------------------------------------------------------------------
	void Init(Widget layoutRoot)
	{
		m_context_menu_root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_context_menu.layout", layoutRoot);
		m_context_menu_panel_widget = m_context_menu_root_widget.FindAnyWidget("PanelWidget");
		m_context_menu_root_widget.Show(false);
		m_context_menu_root_widget.SetHandler(this);
	}
	
	//--------------------------------------------------------------------------
	void Show(int x, int y)
	{
		if ( m_count == 0) return;
		int screen_w, screen_h;
		float w, h;
		float sx, sy;
		int offset_x;// = -20;
		int offset_y;// = -10;
	
		GetScreenSize(screen_w, screen_h);
	
		// align buttons
		float button_height_percent = 0.04;  // button height is 4% of screen height
		float button_height = screen_h * button_height_percent;
		for ( int i = 0; i < m_count; i++)
		{
			ButtonWidget menu_button = ButtonWidget.Cast( m_context_menu_root_widget.FindAnyWidget( String( "Button" + (i+1).ToString() ) ) );
			menu_button.SetSize(0.92, button_height);
		}
	
	
		AutoHeightSpacer spacer;
		m_context_menu_panel_widget.GetScript(spacer);
		if ( spacer )
		{
			spacer.Update();	
		}
	
		m_context_menu_root_widget.GetSize(w, h);
		m_context_menu_panel_widget.GetSize(sx, sy);
		m_context_menu_root_widget.SetSize(w, sy);
	
		// set position
		m_context_menu_root_widget.GetScreenSize(w,h);
		screen_w -= 10;
		screen_h -= 10;
	
		int right_edge = x + w - offset_x;
		if (right_edge > screen_w) 
		{
			x = screen_w - w - offset_x;
		}
		else
		{
			x = x + offset_x;
		}
	
		int bottom_edge = y + h - offset_y;
		if (bottom_edge > screen_h)
		{
			y = y - h - offset_y;
		}
		else
		{
			y = y + offset_y;
		}
	
		m_context_menu_root_widget.SetPos(x, y);
		m_context_menu_root_widget.Show(true);
	}	
	
	//--------------------------------------------------------------------------
	void ShowBackdrop(bool show)
	{
		if (show == true)
		{
			m_context_menu_root_widget.FindAnyWidget("BackdropImageWidget").Show(true);
		}
		else
		{
			m_context_menu_root_widget.FindAnyWidget("BackdropImageWidget").Show(false);
		}
	}
	
	//--------------------------------------------------------------------------
	void Hide()
	{
		m_context_menu_root_widget.Show(false);
		Clear();
	}
	
	//--------------------------------------------------------------------------
	bool IsVisible()
	{
		return m_context_menu_root_widget.IsVisible();
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		if ( enterW && m_context_menu_panel_widget && enterW != m_context_menu_panel_widget && enterW.GetParent() != m_context_menu_panel_widget )
		{
			Hide();
			return true;
		}
		return false;
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (button == MouseState.LEFT && w.GetUserID() > -1 && w.GetUserID() < m_commands.Count())
		{
			CallQueueContext ctx = m_commands.Get( w.GetUserID() ); 
			ctx.Call();
			Hide();
	
			UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
			if (menu) GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(menu.Refresh);
			return true;
		}
		return false;
	}
	
	//--------------------------------------------------------------------------
	void Add(string label, Class obj, string fn_name, Param params /*= NULL*/)
	{
		int count = Count();
		ButtonWidget menu_button = ButtonWidget.Cast( m_context_menu_root_widget.FindAnyWidget( String( "Button" + (count+1).ToString() ) ) );
		if ( menu_button )
		{
			label.ToUpper();
			menu_button.SetText( label );
			menu_button.Show( true );
	
			int item_width = label.Length();
			if (m_max_item_width < item_width)
			{
				m_max_item_width = item_width;
			}
		}
		m_count = m_count + 1;
		
		m_commands.Insert(new CallQueueContext(obj, fn_name, params));
	}
	
	//--------------------------------------------------------------------------
	void Remove(int index)
	{
		if (index < m_commands.Count())
		{
			m_commands.RemoveOrdered(index);
			ButtonWidget menu_button = ButtonWidget.Cast( m_context_menu_root_widget.FindAnyWidget( String( "Button" + ( index + 1 ).ToString() ) ) );
			menu_button.Show( false );
			menu_button.SetText( "" );
			m_count = m_count - 1;
		}
	}
	
	//--------------------------------------------------------------------------
	int Count()
	{
		return m_commands.Count();
	}
	
	//--------------------------------------------------------------------------
	void Clear()
	{	
		int i;
		
		m_commands.Clear();
	
		for ( i = 0; i < ITEMS_COUNT; i++)
		{
			ButtonWidget menu_button = ButtonWidget.Cast( m_context_menu_root_widget.FindAnyWidget( String( "Button" + (i+1).ToString() ) ) );
			menu_button.Show( false );
			menu_button.SetText( "" );
		}
		m_count = 0;
		m_max_item_width = 0;
	}
};