class KeyBinding
{
	private int		m_ActiveUIMenuID;
	private int		m_Key1;
	private int		m_Key2;	
	private string	m_CallbackTarget;
	private string	m_CallbackFunction;
	private string	m_InfoKeys;
	private string	m_InfoDescription
	
	void KeyBinding( int ui_id, int key1, int key2, string call_target, string call_function, string info_bind, string info_description )
	{
		m_ActiveUIMenuID	= ui_id;
		m_Key1				= key1;
		m_Key2				= key2;
		m_CallbackTarget	= call_target;
		m_CallbackFunction	= call_function;
		m_InfoKeys			= info_bind;
		m_InfoDescription	= info_description;
	}
	
	int GetUIMenuID()
	{
		return m_ActiveUIMenuID;
	}
	
	int GetKey1()
	{
		return m_Key1;
	}
	
	int GetKey2()
	{
		return m_Key2;
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