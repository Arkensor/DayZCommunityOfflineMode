class SyncedValue
{
	string m_Name;
	float m_Value;
	bool m_State;
	
	void SyncedValue( string name, float value, bool state )
	{
		m_Name = name;
		m_Value = value;
		m_State = state;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	float GetValue()
	{
		return m_Value;
	}
	
	bool GetState()
	{
		return m_State;
	}
}
