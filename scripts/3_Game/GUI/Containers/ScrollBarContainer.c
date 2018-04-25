// -----------------------------------------------------------
class ScrollBarContainer : ScriptedWidgetEventHandler
{
	reference bool Invert;
	protected Widget Content;
	protected Widget ScrollBar;
	protected Widget Scroller;
	protected Widget m_root;
	
	const int WHEEL_STEP = 20;
	protected float		m_root_height;
	protected float		m_content_height;
	protected float		m_position;
	protected bool		m_scrolling;
	protected float		m_scrolling_start_pos;
	protected int			m_scrolling_mouse_pos;
		
	void ~ScrollBarContainer()
	{
		//if(GetGame() != NULL)
		//GetGame().GetDragQueue().RemoveCalls(this);
	}
	
	void ScrollFixedAmount( bool down, float amount )
	{
		float diff = m_content_height - m_root_height;
		float one_percent = diff / 100;
		float percents = amount / one_percent;
		//float step = (1.0 / (m_content_height - m_root_height)) * WHEEL_STEP;
		float step = (percents/100) + 0.05;
		if(down)
		m_position += 1 * step;
		else
		m_position -= 1 * step;
		
		if (m_position < 0) m_position = 0;
		if (m_position > 1) m_position = 1;
		 Update();
	}
	
	void ScrollToBottom()
	{
		m_position = 1;
		Update();
	}
	
	void ScrollToTop()
	{
		m_position = 0;
		Update();
	}
	
	float GetRootHeight()
	{
		return m_root_height;
	}
	
	void Update()
	{
		float width;
		float height;
		float diff;
		float scroller_height;
	
		m_root.GetScreenSize(width, m_root_height);
		Content.GetScreenSize(width, m_content_height);
	
		diff = m_content_height - m_root_height;
		if (diff <= 0)
		{
			Content.SetPos(0,0);
			Scroller.Show(false);
			ScrollBar.Show(false);
			m_position = 0;
			return;
		}
		
		scroller_height = (m_root_height / m_content_height) * m_root_height;

		ScrollBar.Show(true);
		Scroller.Show(true);
		Scroller.GetSize(width, height);
		Scroller.SetSize(width, scroller_height);
		Scroller.SetPos(0, m_position * (m_root_height - scroller_height));
		if(Invert)
		Content.SetPos(0, -(diff + (-diff * m_position)));
		else
		Content.SetPos(0, (-diff * m_position));
	}

	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.SetHandler(this);
		m_root.SetFlags(WidgetFlags.VEXACTPOS);
		m_scrolling = false;
		Update();
	}
	
	protected void StopScrolling()
	{
		if (m_scrolling)
		{
			GetGame().GetDragQueue().RemoveCalls(this);
			m_scrolling = false;
		}
	}
	
	protected void UpdateScroll(int mouse_x, int mouse_y, bool is_dragging)
	{
		if (m_scrolling)
		{
			if (is_dragging)
			{
				float diff = (mouse_y - m_scrolling_mouse_pos);
				float scroller_height = (m_root_height / m_content_height) * m_root_height; 
				m_position = m_scrolling_start_pos + (diff / (m_root_height - scroller_height));
				if (m_position < 0) m_position = 0;
				if (m_position > 1) m_position = 1;
			}
			else
			{
				m_scrolling = false;
				StopScrolling();
			}
		}
		
		Update();
	}

	
	
	// ScriptedWidgetEventHandler override
	//--------------------------------------------------------------------------
	override bool OnMouseButtonDown(Widget  w, int  x, int  y, int  button)
	{
		if (button == MouseState.LEFT && w == Scroller && !m_scrolling)
		{
			m_scrolling = true;
			m_scrolling_start_pos = m_position;
			int mouse_x;
			GetMousePos(mouse_x, m_scrolling_mouse_pos);
			GetGame().GetDragQueue().Call(this, "UpdateScroll");
			return true;
		}
	
		return false;
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseButtonUp(Widget  w, int  x, int  y, int  button)
	{
		StopScrolling();
		return false;
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseWheel(Widget  w, int  x, int  y, int  wheel)
	{
		if (m_scrolling || m_content_height <= m_root_height) return false;
	
		float step = (1.0 / (m_content_height - m_root_height)) * WHEEL_STEP;
		m_position -= wheel * step;
	
		if (m_position < 0) m_position = 0;
		if (m_position > 1) m_position = 1;
		Update();
		return true;
	}
	
	override bool OnResize( Widget  w, int x, int y) {
		if (w == m_root || w == Content) 
		{
			Update();
		}
		return false;
	}
	
	override bool OnUpdate(Widget w)
	{
		if (w == m_root)
		{
			Update();
		}
		return false;
	}	
};
