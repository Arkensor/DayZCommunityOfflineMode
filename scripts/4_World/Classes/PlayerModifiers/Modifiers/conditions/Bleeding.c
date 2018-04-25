class BleedingCheck: ModifierBase
{
	protected const float		BLOOD_DECREMENT_PER_SEC = -10;
		
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BLEEDING;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		float blood = player.GetHealth("","Blood");
		
		if( blood < ModifierConstants.BLOOD_THRESHOLD_FATAL )
		{
			return true;
		}
		return false;
		
	}

	override void OnActivate(PlayerBase player)
	{
		player.SetHealth("","",-1000);
	}

	override void OnDeactivate(PlayerBase player)
	{
		
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		
	}
};