class Cholera: ModifierBase
{
	static const int CHOLERA_AGENT_THRESHOLD_ACTIVATE = 100;
	static const int CHOLERA_AGENT_THRESHOLD_DEACTIVATE = 20;
	
	private const float	 	TEMPERATURE_INCREMENT_PER_SEC = 0.1;
	float m_Time = 0;
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_CHOLERA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override private bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_CHOLERA) > CHOLERA_AGENT_THRESHOLD_ACTIVATE) 
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
		if(player.GetSingleAgentCount(AGT_CHOLERA) < CHOLERA_AGENT_THRESHOLD_DEACTIVATE) 
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
		player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
		//Log("Ticking OnTick influenza modifier "+ToString(player.GetSingleAgentCount(AGT_INFLUENZA)));
		float chance_of_vomit = player.GetStatStomachSolid().GetNormalized() / 10;
		if( Math.RandomFloat01() < chance_of_vomit )
		{
			StateBase state = player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_VOMIT);
			state.SetDuration(5);
		}
	}
};