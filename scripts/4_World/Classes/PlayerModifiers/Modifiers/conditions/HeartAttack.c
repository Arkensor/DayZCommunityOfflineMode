class HeartAttack: ModifierBase
{
	private const float 	HEALTH_DECREMENT_PER_SEC = -0.2;
	private const float 	SHOCK_DECREMENT_PER_SEC = -2;
	private const float 	SHOCK_LIMIT = 0;
		
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_HEART_ATTACK;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{	
		if ( player.GetHealth("GlobalHealth","Shock") <= SHOCK_LIMIT )
		{
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_DECREMENT_PER_SEC*deltaT) ) );
			
		}
		else
		{
			float currentshock =  player.GetHealth("GlobalHealth", "Shock");
			player.SetHealth("GlobalHealth", "Shock" , ( currentshock + (SHOCK_DECREMENT_PER_SEC*deltaT) ) );
		}
	}
};