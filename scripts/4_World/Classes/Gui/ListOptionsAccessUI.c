class ListOptionsAccessUI extends OptionsAccessUI
{
	void ~ListOptionsAccessUI()
	{
		Deattach();
	}
	
	//! Assigns widget handlers in order in which the widgets are arranged in the layout
	void Attach(OptionsAccess optionAccess, array<Widget> widget_array)
	{
		m_optionAccess = optionAccess;
		m_widget_array = new array<Widget>;
		m_widget_array = widget_array;
			
		for (int i = 0; i < m_widget_array.Count(); i++)
		{
			m_widget = m_widget_array.Get(i);
			if (m_widget)
			{
				m_widget.SetHandler(this);
				
				//Init();
				Update();
			}
		}
	}
	
	override void Deattach()
	{
		super.Deattach();
		
		m_optionAccess = NULL;
		if (m_widget_array.Count() > 0)
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
	
	/*override void Init()
	{
		ListOptionsAccess list = ListOptionsAccess.Cast( m_optionAccess );
		ButtonWidget button = ButtonWidget.Cast( m_widget );

		if (list && button)
		{
			string text;
			int c = list.GetItemsCount();
			for (int i = 0; i < c; i++)
			{
				list.GetItemText(i, text);
				combo.AddItem(text);
			}
		}
	}*/
	
	//TODO rework
	override void Update()
	{
		ListOptionsAccess list;
		ButtonWidget button;

		if (Class.CastTo(list, m_optionAccess) && Class.CastTo(button, m_widget))
		{
			button.SetState( list.GetIndex() == m_widget_array.Find(m_widget) );
		}
	}

	override void SetValue()
	{
		ListOptionsAccess list;
		ButtonWidget button;
		int index;

		if (Class.CastTo(list, m_optionAccess) && Class.CastTo(button, m_widget))
		{
			index = m_widget_array.Find(m_widget);
			list.SetIndex( index );
			SetButtonStates( index );
			Update();
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
		ButtonWidget temp;
		for (int i = 0; i < m_widget_array.Count(); i++)
		{
			temp = ButtonWidget.Cast( m_widget_array.Get(i) );
			if (temp)
			{
				temp.SetState( i == index );
			}
		}
	}
}