class StatDebugObject
{
	string m_Name;
	float m_Value;
	eRemoteDebugType m_Type;
	
	void StatDebugObject(string name, float value, eRemoteDebugType type)
	{
		m_Name = name;
		m_Value = value;
		m_Type = type;
	}
	
	void Debug()
	{
		PrintString("stat_type:"+ m_Name + "|stat_value:"+ m_Value.ToString());
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	string GetValue()
	{
		return m_Value.ToString();
	}
	
	void SetType(eRemoteDebugType type)
	{
		m_Type = type;
	}
	
	eRemoteDebugType GetType()
	{
		return m_Type;
	}
	
};
