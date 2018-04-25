class MouseBinding
{
	private int		m_ActiveUIMenuID;
	private int		m_MouseButton;
	private int		m_MouseEvent;
	private string	m_CallbackTarget;
	private string	m_CallbackFunction;
	private string	m_InfoKeys;
	private string	m_InfoDescription
	
	void MouseBinding( int ui_id, int mouse_button, int mouse_event, string call_target, string call_function, string info_bind, string info_description )
	{
		m_ActiveUIMenuID	= ui_id;
		m_MouseButton		= mouse_button;
		m_MouseEvent		= mouse_event;
		m_CallbackTarget	= call_target;
		m_CallbackFunction	= call_function;
		m_InfoKeys			= info_bind;
		m_InfoDescription	= info_description;
	}
	
	int GetUIMenuID()
	{
		return m_ActiveUIMenuID;
	}
	
	int GetButtonID()
	{
		return m_MouseButton;
	}
	
	int GetMouseEvent()
	{
		return m_MouseEvent;
	}
	
	string GetCallbackTarget()
	{
		return m_CallbackTarget;
	}
	
	string GetCallbackFunction()
	{
		return m_CallbackFunction;
	}
	
	string GetInfoBind()
	{
		return m_InfoKeys;
	}
	
	string GetInfoDescription()
	{
		return m_InfoDescription;
	}
}