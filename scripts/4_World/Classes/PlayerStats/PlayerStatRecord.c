class PlayerStatRecord
{
	float 		m_Value;
	float 		m_Time;
	string		m_System;
	
	void PlayerStatRecord(float value, float time, string system)
	{
		m_Value = value;
		m_Time = time;
		m_System = system;
	}
	
	string GetStringOutput()
	{
		return m_Time.ToString()+", " +m_Value.ToString() + ", " +m_System;
		
	}

};
