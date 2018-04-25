class StuffedStomach: ModifierBase
{
	protected const int	 	STUFFED_TRESHOLD 			= 1000;
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_STUFFED;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{

	}
	
	override bool ActivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomachSolid().Get() + player.GetStatStomachWater().Get();
		if ( stomachs >= STUFFED_TRESHOLD )
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
		if( player.GetNotifiersManager() )
		{
			player.GetNotifiersManager().AttachByType(NTF_STUFFED);
		}
	}

	override void OnDeactivate(PlayerBase player)
	{
		if( player.GetNotifiersManager() )
		{
			player.GetNotifiersManager().DetachByType(NTF_STUFFED);
		}
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomachSolid().Get() + player.GetStatStomachWater().Get();
		if ( stomachs < STUFFED_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};