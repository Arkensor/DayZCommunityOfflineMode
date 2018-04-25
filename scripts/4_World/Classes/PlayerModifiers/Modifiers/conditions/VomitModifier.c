class VomitModifier: ModifierBase
{
	protected const int	 	VOMIT_THRESHOLD 			= 1500;
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_VOMITMODIFIER;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{

	}
	
	override bool ActivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomachSolid().Get() + player.GetStatStomachWater().Get();
		if ( stomachs >= VOMIT_THRESHOLD )
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
		player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_VOMIT );
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomachSolid().Get() + player.GetStatStomachWater().Get();
		if ( stomachs < VOMIT_THRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};