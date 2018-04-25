class Health: ModifierBase
{
	private float	m_LastHealthLevel;
	private float	m_LastBloodLevel;
	
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_HEALTH;
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

	override void OnTick(PlayerBase player, float deltaT)
	{	
		
		float blood =  player.GetHealth("GlobalHealth", "Blood");
		float health = player.GetHealth("GlobalHealth", "Health");	

		float healthdelta  = Math.AbsInt(health - m_LastHealthLevel);
		if (health <  m_LastHealthLevel) healthdelta = -healthdelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_LIVES).DisplayTendency(healthdelta);
		m_LastHealthLevel = health;
		
		float blooddelta  = Math.AbsInt(blood - m_LastBloodLevel);
		if (blood <  m_LastBloodLevel) blooddelta = -blooddelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_BLOOD).DisplayTendency(blooddelta);
		m_LastBloodLevel = blood;
		
	}
};