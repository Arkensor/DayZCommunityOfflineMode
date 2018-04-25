class ClutterCutterFireplace : Object
{
	ref Timer m_DestroyTimer;
	
	void ClutterCutterFireplace()
	{
		//m_DestroyTimer = new Timer ( CALL_CATEGORY_GAMEPLAY );
		//m_DestroyTimer.Run ( 1, this, "DestroyAfterTime", NULL, false );			
	}
	
	void ~ClutterCutterFireplace()
	{
	}
	
	void DestroyAfterTime()
	{
		GetGame().ObjectDelete( this );
	}
}