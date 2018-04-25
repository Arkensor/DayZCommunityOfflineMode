class Hypothermia: ModifierBase
{
	private const float	 	HYPOTHERMIC_TEMPERATURE_TRESHOLD = 35.8;
	private const float	 	TREMOR_TRESHOLD = 35.5;
	private const float	 	FATAL_TEMPERATURE_TRESHOLD = 34.5;
	private const float	 	HEALTH_DECREMENT_PER_SEC = -0.2;
	private const float	 	IMMUNITY_DECREMENT_PER_SEC = -1;

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_HYPOTHERMIA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}	

	override bool ActivateCondition(PlayerBase player)
	{
		return false;
		/*
		if ( player.GetStatTemperature().Get() <= HYPOTHERMIC_TEMPERATURE_TRESHOLD )
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
		if ( player.GetStatTemperature().Get() > HYPOTHERMIC_TEMPERATURE_TRESHOLD )
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
	//	player.GetStatEnergy().Add( (((HYPOTHERMIC_TEMPERATURE_TRESHOLD+1 - player.GetStatTemperature().Get())*(HYPOTHERMIC_TEMPERATURE_TRESHOLD+1 - player.GetStatTemperature().Get()))*-1*deltaT) ); //lower is you temperature, faster is your energy drained
//		m_Immunity.Add( IMMUNITY_DECREMENT_PER_SEC*deltaT );
		if ( player.GetStatTemperature().Get() < TREMOR_TRESHOLD )
		{
			player.GetStatTremor().Set( TREMOR_TRESHOLD - player.GetStatTemperature().Get() );
		}
		if ( player.GetStatTemperature().Get() < FATAL_TEMPERATURE_TRESHOLD )
		{
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_DECREMENT_PER_SEC*deltaT) ) );
		}	
	}
};
