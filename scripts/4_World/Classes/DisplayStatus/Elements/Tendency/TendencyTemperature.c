class TendencyTemperature extends DisplayElement
{

	void TendencyTemperature()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_TEMPERATURE;
		m_Key = NTFKEY_FEVERISH;
	}
	
	override void UpdateHUD()
	{
		return;
		int value_mask = 7;
		int color = m_Value >> 3;
		color = color << 3;
		//PrintString("color:" + color.ToString());
		
		int value = value_mask & m_Value;
		int i;
		if(value > 3) 
		{
			i = 0 - (value - 3);
		}
		else
		{
			i = value;
		}
		//PrintString("value:" + i.ToString());
		m_ModulePlayerStatus.DisplayTendency(m_Key, i);
	}
}