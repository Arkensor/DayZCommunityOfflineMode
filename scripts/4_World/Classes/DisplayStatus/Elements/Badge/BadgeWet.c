class BadgeWet extends DisplayElement
{
	void BadgeWet()
	{
		m_NumOfBits = 2;
		m_Key 	= 	NTFKEY_WETNESS;
		m_Type	=	eDisplayElements.DELM_BADGE_WET;
	}
	
	override void UpdateHUD()
	{
		if( IsBlinking() )
		{
			//make it blink
		}
		else
		{
			if(m_Value == 1) 
			{
				m_ModulePlayerStatus.SetBadge(m_Key, true);
			}
			else
			{
				m_ModulePlayerStatus.SetBadge(m_Key, false);
			}
		}
	}
}