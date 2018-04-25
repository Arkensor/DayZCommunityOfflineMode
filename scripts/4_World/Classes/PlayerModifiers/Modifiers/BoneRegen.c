class BoneRegen: ModifierBase
{
	private const 	float	MINIMAL_WATER_TO_REGENRATE = 2500;
	private const 	float	MINIMAL_ENERGY_TO_REGENRATE = 4000;
	private const 	float	MINIMAL_BLOOD_TO_REGENRATE = 5000;
	private const 	float	MINIMAL_BLOOD_REQUIRED_TO_REGENERATE_HEALTH = 5000;
	private const	float 	BONE_HEALTH_INCREMENT_PER_SEC = 0.25;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BONE_REGEN;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		if (  player.GetHealth("GlobalHealth", "Blood") >= MINIMAL_BLOOD_TO_REGENRATE && player.GetStatWater().Get() >= MINIMAL_WATER_TO_REGENRATE && player.GetStatEnergy().Get() >= MINIMAL_ENERGY_TO_REGENRATE )
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
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		if (  player.GetHealth("GlobalHealth", "Blood") < MINIMAL_BLOOD_TO_REGENRATE && player.GetStatWater().Get() < MINIMAL_WATER_TO_REGENRATE && player.GetStatEnergy().Get() < MINIMAL_ENERGY_TO_REGENRATE )
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
		/*
		player.AddHealth("RightArm","Health",BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("RightHand","Health",BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("LeftArm","Health",BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("LeftHand","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		
		player.AddHealth("RightLeg","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("RightFoot","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("LeftLeg","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		player.AddHealth("LeftFoot","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		
		player.AddHealth("Torso","Health",+BONE_HEALTH_INCREMENT_PER_SEC * deltaT);
		*/
	}
};