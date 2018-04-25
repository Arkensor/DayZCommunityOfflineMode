class BrainDisease: ModifierBase
{
	static const int BRAIN_AGENT_THRESHOLD_ACTIVATE = 100;
	static const int BRAIN_AGENT_THRESHOLD_DEACTIVATE = 0;
	override void Init()
	{
		m_TrackActivatedTime			= false;
		m_ID 					= eModifiers.MDF_BRAIN;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override private bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_BRAIN) > BRAIN_AGENT_THRESHOLD_ACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override private void OnActivate(PlayerBase player)
	{
	}

	override private void OnDeactivate(PlayerBase player)
	{
	}

	override private bool DeactivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_BRAIN) < BRAIN_AGENT_THRESHOLD_DEACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override private void OnTick(PlayerBase player, float deltaT)
	{
		float chance_of_laughter = player.GetSingleAgentCountNormalized(AGT_BRAIN);
		
		if( Math.RandomFloat01() < chance_of_laughter / Math.RandomInt(1,10) )
		{
			StateBase state = player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_LAUGHTER);
		}
	}
};