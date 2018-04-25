class TendencyHunger extends DisplayElement
{
	void TendencyHunger()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_ENERGY;
		m_Key = NTFKEY_HUNGRY;
	}
	
	override void UpdateHUD()
	{
		int value_mask = 7;//first 3 bits turned ON
		int status = m_Value >> 3;
		status = status << 3;
		//PrintString("status:" + status.ToString());
		
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
		
		m_ModulePlayerStatus.DisplayTendency(m_Key, i,TranslateLevelToStatus(status) );
	}
	

}