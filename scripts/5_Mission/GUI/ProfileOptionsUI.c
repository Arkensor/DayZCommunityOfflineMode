class ProfileOptionsUI extends ScriptedWidgetEventHandler
{
	void ~ProfileOptionsUI()
	{
		Deattach();
	}

	void Attach(int profileOption, array<Widget> widget_array)
	{
		m_profileOption = profileOption;
		m_widget_array = new array<Widget>;
		m_widget_array = widget_array;
			
		for (int i = 0; i < m_widget_array.Count(); i++)
		{
			m_widget = m_widget_array.Get(i);
			if (m_widget)
			{
				m_widget.SetHandler(this);
				
				Init();
			}
		}
	}
	
	void Deattach()
	{
		//m_profileOption = NULL;
		if (m_widget_array && m_widget_array.Count() > 0)
		{
			for (int i = 0; i < m_widget_array.Count(); i++)
			{
				m_widget = m_widget_array.Get(i);
				if (m_widget)
				{
					m_widget.SetHandler(NULL);
					m_widget = NULL;
				}
			}
		}
	}
	
	void Init()
	{
		int state;
		ButtonWidget button;
		
		if ( Class.CastTo(button, m_widget) )
		{
			state = m_widget_array.Find(m_widget);
			button.SetState( g_Game.GetProfileOption(m_profileOption) == state ); //works for bool values only!!!
		}
	}

	void SetValue()
	{
		int state;
		ButtonWidget button;
		
		if ( Class.CastTo(button, m_widget) )
		{
			state = m_widget_array.Find(m_widget);
			SetButtonStates( state );
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		m_widget = w;
		SetValue();
		return false;
	}
	
	//TODO check for unnescessary duplicate methods
	void SetButtonStates(int index)
	{
		ButtonWidget button;
		for (int i = 0; i < m_widget_array.Count(); i++)
		{
			button = ButtonWidget.Cast( m_widget_array.Get(i) );
			if (button)
			{
				button.SetState( i == index );
			}
		}
	}
	
	array<Widget> GetWidgetArray()
	{
		return m_widget_array;
	}
	
	int GetProfileOption()
	{
		return m_profileOption;
	}

	int m_profileOption;
	Widget m_widget;
	ref array<Widget> m_widget_array;
}
