class Hunger: ModifierBase
{
	protected float	m_EnergyDelta;
	protected float	m_LastEnergyLevel;
	ref HumanMovementState		m_MovementState	= new HumanMovementState();
	
	
		
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_HUNGER;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 1;
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
		player.GetMovementState(m_MovementState);
		float energy = player.GetStatEnergy().Get();
		float metabolic_speed = MiscGameplayFunctions.GetEnergyMetabolicSpeed(m_MovementState.m_iMovement);
		//PrintString(metabolic_speed.ToString());
		
		float energy_delta  = Math.AbsInt(player.GetStatEnergy().Get() - m_LastEnergyLevel);
		if (energy <  m_LastEnergyLevel) energy_delta = -energy_delta;
		m_LastEnergyLevel = player.GetStatEnergy().Get();
		
		player.GetStatEnergy().Add( -metabolic_speed * deltaT );
		if ( energy <= ModifierConstants.LOW_ENERGY_TRESHOLD )
		{
			/*
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			float currentblood = player.GetHealth("GlobalHealth", "Blood");
			
			float health_delta = ( 1 - Math.InverseLerp(0,ModifierConstants.LOW_ENERGY_TRESHOLD, player.GetStatEnergy().Get()) ) * -ModifierConstants.DAMAGE_PER_SEC * deltaT;
			
			player.AddHealth("GlobalHealth", "", health_delta);
			*/
			player.AddHealth("GlobalHealth", "Health", -ModifierConstants.LOW_ENERGY_DAMAGE_PER_SEC * deltaT );
		}
	}
	
	
};