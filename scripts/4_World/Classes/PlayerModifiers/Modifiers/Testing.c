class Testing: ModifierBase
{
	float stuff;
	bool swch;
	override void Init()
	{
		m_TrackActivatedTime			= true;
		m_ID 					= eModifiers.MDF_TESTING;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override void OnActivate(PlayerBase player)
	{
		//m_Timer1.Run(1, this, "BadaBang");
		player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_COUGH );
		
	}

	override void OnReconnect(PlayerBase player)
	{

	}

	// ------------------------------------------------------------------------------

	// ------------------------------------------------------------------------------

	override bool ActivateCondition(PlayerBase player)
	{
		return swch;
		if(stuff < 40)
		{
			return true;
		}
		else return false;
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		if (GetAttachedTime() > 100) {swch = true; return true;}
		else return false;
		if(stuff > 40)
		{
			return true;
		}
		else return false;
	}
	// ------------------------------------------------------------------------------

	override void OnTick(PlayerBase player, float deltaT)
	{
		stuff += deltaT;
	}
};