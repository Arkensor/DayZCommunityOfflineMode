class BadgeFracture extends DisplayElement
{
	void BadgeFracture()
	{
		m_NumOfBits = 2;
		m_Key 	= 	NTFKEY_FRACTURE;
		m_Type	=	eDisplayElements.DELM_BADGE_FRACTURE;
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