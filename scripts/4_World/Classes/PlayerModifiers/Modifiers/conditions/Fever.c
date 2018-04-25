class Fever: ModifierBase
{
	private const float	 	TEMPERATURE_INCREMENT_PER_SEC = 0.1;
		
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_FEVER;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		//Print(GetGame().GetTime());
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		//Print(GetGame().GetTime());
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_FEVERISH);
		
		player.GetStateManager().QueueUpPrimaryState(StateIDs.STATE_FEVERBLUR);
		
	}

	override void OnReconnect(PlayerBase player)
	{
		this.OnActivate(player);
	}


	override void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_FEVERISH);
		player.GetStateManager().RemoveSecondaryState(StateIDs.STATE_FEVERBLUR);
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
	}
};