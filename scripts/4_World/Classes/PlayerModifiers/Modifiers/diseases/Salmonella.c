class Salmonella: ModifierBase
{
	static const int SALMONELLA_AGENT_THRESHOLD_ACTIVATE = 100;
	static const int SALMONELLA_AGENT_THRESHOLD_DEACTIVATE = 20;
	override void Init()
	{
		m_TrackActivatedTime			= false;
		m_ID 					= eModifiers.MDF_SALMONELLA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override private bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_CHOLERA) > SALMONELLA_AGENT_THRESHOLD_ACTIVATE) 
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
		if(player.GetSingleAgentCount(AGT_CHOLERA) < SALMONELLA_AGENT_THRESHOLD_DEACTIVATE) 
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
		//Log("Ticking OnTick influenza modifier "+ToString(player.GetSingleAgentCount(AGT_INFLUENZA)));
		float chance_of_vomit = player.GetStatStomachSolid().GetNormalized() / 10;
		if( Math.RandomFloat01() < chance_of_vomit )
		{
			StateBase state = player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_VOMIT);
			state.SetDuration(5);
		}
	}
};