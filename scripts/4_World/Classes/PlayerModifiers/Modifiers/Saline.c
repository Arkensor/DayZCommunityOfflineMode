class Saline: ModifierBase
{
	float m_RegenTime;
	override void Init()
	{
		m_TrackActivatedTime = true;
		m_ID 					= eModifiers.MDF_SALINE;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
		m_RegenTime = CalculateRegenTime();
	}

	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		float attached_time = GetAttachedTime();
		
		if(attached_time > m_RegenTime )
		{
			return true;	
		}
		else
		{
			return false;	
		}
	}

	override void OnReconnect(PlayerBase player)
	{

	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{
		player.AddHealth("","Blood",ModifierConstants.SALINE_BLOOD_REGEN_PER_SEC * deltaT);
	}
	
	float CalculateRegenTime()
	{
		float regen_time = ModifierConstants.SALINE_LIQUID_AMOUNT / ModifierConstants.SALINE_BLOOD_REGEN_PER_SEC;
		return regen_time;
	}
};