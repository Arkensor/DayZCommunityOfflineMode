class CommonCold: ModifierBase
{
	const int INFLUENZA_AGENT_THRESHOLD_ACTIVATE = 200;
	const int INFLUENZA_AGENT_THRESHOLD_DEACTIVATE = 100;
	override void Init()
	{
		m_TrackActivatedTime			= true;
		m_ID 					= eModifiers.MDF_COMMON_COLD;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_INFLUENZA) > INFLUENZA_AGENT_THRESHOLD_ACTIVATE) 
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
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}


	override private bool DeactivateCondition(PlayerBase player)
	{
		return (player.GetSingleAgentCount(AGT_INFLUENZA) < INFLUENZA_AGENT_THRESHOLD_DEACTIVATE);
	}

	override private void OnTick(PlayerBase player, float deltaT)
	{
		float chance_of_sneeze = player.GetSingleAgentCountNormalized(AGT_INFLUENZA);
		
		if( Math.RandomFloat01() < chance_of_sneeze / Math.RandomInt(4,10) )
		{
			StateBase state = player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_SNEEZE);
		}
	}
};