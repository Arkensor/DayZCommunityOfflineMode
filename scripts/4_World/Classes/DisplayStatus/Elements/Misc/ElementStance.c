class ElementStance extends DisplayElement
{
	void ElementStance()
	{
		m_NumOfBits = 2;
		m_Type		=	eDisplayElements.DELM_STANCE;
		m_IsClientOnly = true;
	}
	
	override void UpdateHUD()
	{
		m_ModulePlayerStatus.SetStance( m_Value );
	}
}