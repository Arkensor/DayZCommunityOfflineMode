class Blinded: ModifierBase
{
	private const float		BLIND_TIME = 7;
	private float	 		m_Timer;

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BLINDED;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnActivate(PlayerBase player)
	{	
		m_Timer = 0;
		
	}

	override void OnDeactivate(PlayerBase player)
	{	
		g_Game.GetWorld().SetAperture(0);
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		if ( m_Timer > BLIND_TIME )
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
		m_Timer++;
		g_Game.GetWorld().SetAperture(m_Timer);
	}
};