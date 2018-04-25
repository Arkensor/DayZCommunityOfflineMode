class OptionsAccessUI extends ScriptedWidgetEventHandler
{
	void ~OptionsAccessUI()
	{
		Deattach();
	}

	void Attach(OptionsAccess optionAccess, Widget ctrl)
	{
		m_optionAccess = optionAccess;
		m_widget = ctrl;
		
		if (m_widget)
		{
			m_widget.SetHandler(this);
			
			Init();
			Update();
		}
	}
	
	
	void Deattach()
	{
		m_optionAccess = NULL;
		if (m_widget)
		{
			m_widget.SetHandler(NULL);
			m_widget = NULL;
		}
	}
	
	void Update() 
	{
	}

	void Init() 
	{
	}

	void SetValue() 
	{
	}

	OptionsAccess GetOptionAccess()
	{
		return m_optionAccess;
	}
	
	Widget GetWidget()
	{
		return m_widget;
	}
	
	array<Widget> GetWidgetArray()
	{
		return m_widget_array;
	}

	OptionsAccess m_optionAccess;
	Widget m_widget;
	ref array<Widget> m_widget_array;
}
