class HealthRegen: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_HEALTH_REGEN;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	override void OnActivate(PlayerBase player)
	{
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
		float blood =  player.GetHealth("GlobalHealth", "Blood");

		float blood_scale_normalized = Math.InverseLerp(ModifierConstants.BLOOD_THRESHOLD_FATAL, player.GetMaxHealth("", "Blood"), blood);
		float regen_speed = Math.Lerp(ModifierConstants.HEALTH_REGEN_MIN, ModifierConstants.HEALTH_REGEN_MAX, blood_scale_normalized);
		regen_speed = regen_speed * deltaT;
		player.AddHealth("GlobalHealth", "Health" ,  regen_speed );
		
		player.AddHealth("RightArm","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER );
		player.AddHealth("RightHand","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		player.AddHealth("LeftArm","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		player.AddHealth("LeftHand","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		
		player.AddHealth("RightLeg","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		player.AddHealth("RightFoot","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		player.AddHealth("LeftLeg","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		player.AddHealth("LeftFoot","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
		
		player.AddHealth("Torso","Health",regen_speed * ModifierConstants.DAMAGE_ZONE_BLOOD_REGEN_MODIFIER);
	}
};
