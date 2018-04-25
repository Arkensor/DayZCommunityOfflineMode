class BloodRegen: ModifierBase
{
	private const float	BLOOD_INCREMENT_PER_SEC = 5;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BLOOD_REGEN;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
			m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		float m_MaxBlood = player.GetMaxHealth("GlobalHealth", "Blood");
		if ( player.GetHealth("GlobalHealth", "Blood") <= m_MaxBlood )
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
		float m_MaxBlood = player.GetMaxHealth("GlobalHealth", "Blood");
		if ( player.GetHealth("GlobalHealth", "Blood") >= m_MaxBlood )
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
		float regen_modifier_water = GetRegenModifierWater( player.GetStatWater().Get() );
		float regen_modifier_energy = GetRegenModifierWater( player.GetStatEnergy().Get() );
		float blood_regen_speed = ModifierConstants.BLOOD_REGEN_SPEED * regen_modifier_water * regen_modifier_energy;
		/*
		PrintString("regen_modifier_water" + regen_modifier_water);
		PrintString("regen_modifier_energy" + regen_modifier_energy);
		PrintString("blood_regen_speed" + blood_regen_speed);
		*/
		player.AddHealth("","Blood", blood_regen_speed * deltaT );
	}
	
	float GetRegenModifierWater(float water)
	{
		float modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_WATER_HIGH;
		if( water < ModifierConstants.BLOOD_REGEN_THRESHOLD_WATER_MID ) {modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_WATER_MID}
		if( water < ModifierConstants.BLOOD_REGEN_THRESHOLD_WATER_LOW ) {modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_WATER_LOW}
		return modifier;
	}
	
	float GetRegenModifierEnergy(float energy)
	{
		float modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_ENERGY_HIGH;
		if( energy < ModifierConstants.BLOOD_REGEN_THRESHOLD_ENERGY_MID ) {modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_ENERGY_MID}
		if( energy < ModifierConstants.BLOOD_REGEN_THRESHOLD_ENERGY_LOW ) {modifier = ModifierConstants.BLOOD_REGEN_MODIFIER_WATER_LOW}
		return modifier;
	}
	
	
};