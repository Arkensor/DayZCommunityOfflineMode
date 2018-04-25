class Temperature: ModifierBase
{
	private const float	NONAFFECTING_HEAT_COMFORT_MIN_TRESHOLD = -10;
	private const float	NONAFFECTING_HEAT_COMFORT_MAX_TRESHOLD = 18;
	private const float	HEALTHY_TEMPERATURE = 36.8;
	private const float	TEMPERATURE_INCREMENT_PER_SEC = 0.008;
	private float		m_LastTemperatureLevel;
		
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_TEMPERATURE;
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

	override void OnReconnect(PlayerBase player)
	{

	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{	
		
		float heat_comfort = player.GetStatHeatComfort().Get();
		float temperature = player.GetStatTemperature().Get();
		
		//float temperaturedelta  = Math.AbsInt(temperature - m_LastTemperatureLevel);
		//if (temperature <  m_LastTemperatureLevel) temperaturedelta = -temperaturedelta;

		//m_LastTemperatureLevel = temperature;
		
		if ( heat_comfort > NONAFFECTING_HEAT_COMFORT_MAX_TRESHOLD || heat_comfort < NONAFFECTING_HEAT_COMFORT_MIN_TRESHOLD )
		{
			//  // temp decresase rate calculation
			float temperature_increment = 0.001 * heat_comfort * deltaT; //extremer the heatcomfort value, the bigger effect on temperature growth/decrease ,
			
			player.GetStatTemperature().Add( temperature_increment, this.GetName() ); 
		}
		else
		{
			if ( temperature < HEALTHY_TEMPERATURE )
			{
				player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT),this.GetName() );
			}
		}	
	}
};

