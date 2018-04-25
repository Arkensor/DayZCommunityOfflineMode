class Shock: ModifierBase
{
	private const float	 	UNCONSCIOUS_LIMIT = 0;
	private const float 	SHOCK_INCREMENT_PER_SEC = 1;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_SHOCK;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}	

	override bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{	
		if ( player.GetHealth("GlobalHealth","Shock") <= UNCONSCIOUS_LIMIT )
		{
			//player.setUnconscious();
		}
		else
		{
			player.AddHealth("GlobalHealth","Shock", (SHOCK_INCREMENT_PER_SEC * deltaT) );
		}
	}
	
	override void OnReconnect(PlayerBase player)
	{

	}
};