class Poisoning: ModifierBase
{
	private const float 	WATER_DECREMENT_PER_SEC = -50;
	private const float 	BLOOD_DECREMENT_PER_SEC = -5;
	private const float 	POISONING_LASTS_SEC	 = 15;
	private float			m_Time;
	
	override void Init()
	{
		m_TrackActivatedTime			= true;
		m_ID 					= eModifiers.MDF_POISONING;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_SICK);
	}

	override void OnTick(PlayerBase player, float deltaT)
	{	
		player.GetStatWater().Add((WATER_DECREMENT_PER_SEC*deltaT));
		
		float currentblood = player.GetHealth("GlobalHealth", "Blood");
		player.SetHealth("GlobalHealth", "Blood", ( currentblood + (BLOOD_DECREMENT_PER_SEC*deltaT) ) );

		if ( (GetAttachedTime() / 1000) > POISONING_LASTS_SEC )
		{
			player.GetModifiersManager().DeactivateModifier(eModifiers.MDF_POISONING);
		}
	}
};