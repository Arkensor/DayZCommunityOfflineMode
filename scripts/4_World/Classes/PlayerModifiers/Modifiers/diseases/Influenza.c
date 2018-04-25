class Influenza: ModifierBase
{
	const int INFLUENZA_AGENT_THRESHOLD_ACTIVATE = 300;
	const int INFLUENZA_AGENT_THRESHOLD_DEACTIVATE = 200;

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_INFLUENZA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override private bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_INFLUENZA) > INFLUENZA_AGENT_THRESHOLD_ACTIVATE) 
		{
			return true;
		}
		return false;
	}

	override private void OnActivate(PlayerBase player)
	{
		
	}

	override private void OnDeactivate(PlayerBase player)
	{
	}

	override private bool DeactivateCondition(PlayerBase player)
	{
		return (player.GetSingleAgentCount(AGT_INFLUENZA) < INFLUENZA_AGENT_THRESHOLD_DEACTIVATE);
	}

	override private void OnTick(PlayerBase player, float deltaT)
	{
		//player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );

		float chance_of_cough = player.GetSingleAgentCountNormalized(AGT_INFLUENZA);
		
		if( Math.RandomFloat01() < chance_of_cough / Math.RandomInt(1,10) )
		{
			StateBase state = player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_COUGH);
		}
	}
};