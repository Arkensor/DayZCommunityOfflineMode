class HemolyticReaction: ModifierBase
{
	private float		m_RunningTime;
	
	override void Init()
	{
		m_TrackActivatedTime = true;
		m_ID 					= eModifiers.MDF_HEMOLYTIC_REACTION;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
		m_RunningTime = CalculateRunTime();
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}

	override void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_SICK);
	}

	override void OnReconnect(PlayerBase player)
	{
		OnActivate(player);
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		float attached_time = GetAttachedTime();
		
		if(attached_time > m_RunningTime )
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
		player.AddHealth("","Blood",-ModifierConstants.HEMOLYTIC_BLOOD_DRAIN_PER_SEC * deltaT);
	}
	
	float CalculateRunTime()
	{
		float time = ModifierConstants.HEMOLYTIC_BLOODLOSS_AMOUNT / ModifierConstants.SALINE_BLOOD_REGEN_PER_SEC;
		return time;
	}
};