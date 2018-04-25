class Thirst: ModifierBase
{
	float 			m_LastWaterLevel;
	ref HumanMovementState		m_MovementState	= new HumanMovementState();
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_THIRST;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 1;
	}		

	override bool ActivateCondition(PlayerBase player)
	{
		return true;
	}
	
	override void OnReconnect(PlayerBase player)
	{

	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		player.GetMovementState(m_MovementState);
		float metabolic_speed = MiscGameplayFunctions.GetWaterMetabolicSpeed(m_MovementState.m_iMovement);
		//PrintString("metabolic_speed:"+metabolic_speed.ToString());
		float water = player.GetStatWater().Get();
		player.GetStatWater().Add( (-metabolic_speed * deltaT) );
		
		if ( water <= ModifierConstants.LOW_WATER_TRESHOLD )
		{		
			player.AddHealth("GlobalHealth", "Health", -ModifierConstants.LOW_WATER_DAMAGE_PER_SEC * deltaT );
		}
	}
};