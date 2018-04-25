class Hyperthermia: ModifierBase
{
	private const float	 	HYPERTHERMIC_TEMPERATURE_TRESHOLD = 37.5;
	private const float		FATAL_TEMPERATURE_TRESHOLD = 40;
	private const float		HEALTH_DECREMENT_PER_SEC = -0.2;

	override void Init()
	{
		m_TrackActivatedTime			= false;
		m_ID 					= eModifiers.MDF_HYPERTHERMIA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		return false;
		/*
		if ( player.GetStatTemperature().Get() > HYPERTHERMIC_TEMPERATURE_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
		*/
	}

	override void OnActivate(PlayerBase player)
	{
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatTemperature().Get() <= HYPERTHERMIC_TEMPERATURE_TRESHOLD )
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
		float waterreduction = (player.GetStatTemperature().Get() - HYPERTHERMIC_TEMPERATURE_TRESHOLD + 1);
		if (waterreduction > 0)
		{
			player.GetStatWater().Add( (Math.Sqrt(waterreduction)*-1*deltaT) );
			if ( player.GetStatTemperature().Get() > FATAL_TEMPERATURE_TRESHOLD )
			{
				float currenthealth = player.GetHealth("GlobalHealth", "Health");
				player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_DECREMENT_PER_SEC*deltaT) ) );
			}
		}
	}
};