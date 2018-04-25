class Burning: ModifierBase
{
	private const float	 	BURNING_TRESHOLD = 199;
	private const float	 	HEALTH_DECREMENT_PER_SEC = -5;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BURNING;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		float hc = player.GetStatHeatComfort().Get();
		if ( hc > BURNING_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnActivate(PlayerBase player)
	{
		//Print("My face is melting!");
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatHeatComfort().Get() <= BURNING_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		float currenthealth = player.GetHealth("GlobalHealth", "Health");
		player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_DECREMENT_PER_SEC*deltaT) ) );	
		player.GetStatTemperature().Set( player.GetStatTemperature().GetMax() );
	}
};