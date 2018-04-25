class Light extends ItemBase
{
	ref Timer m_DeleteTimer;
	
	void Light()
	{
		SetPilotLight(true);
		
		m_DeleteTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_DeleteTimer.Run( 0.05 , this, "OnDeleteThis", NULL, false);
	}
	
	void OnDeleteThis()
	{
		GetGame().ObjectDelete(this);
	}
}