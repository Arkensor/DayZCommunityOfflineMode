class Grenade_Base extends ItemBase
{
	ref Timer m_UpdateTimer;
	
	override bool CanExplodeInFire()
	{
		return true;
	}
	
	// Temporal function for activation of the grenade (This is placeholder system for particle creation)
	void StartActivation()
	{
		if (!m_UpdateTimer)
			m_UpdateTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		if ( !m_UpdateTimer.IsRunning() ) // Makes sure the timer is NOT running already
		{
			m_UpdateTimer.Run(5, this, "OnActivate", NULL, true);
		}
	}
	
	void OnActivate()
	{
		
	}
}