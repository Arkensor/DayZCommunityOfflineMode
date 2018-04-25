class Tremor: ModifierBase
{
	private const float	 	TREMOR_DECREMENT_PER_SEC = 0.008;

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_TREMOR;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		if ( player.GetStatTremor().Get() > player.GetStatTremor().GetMin() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatTremor().Get() == player.GetStatTremor().GetMin() )
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
		player.GetStatTremor().Add( (TREMOR_DECREMENT_PER_SEC*deltaT) );
		
		//Mirek: SetShakeBodyFactor is removed now, because it worked only on legacy animation system
		//player.SetShakeBodyFactor(player.GetStatTremor().Get());
		//_person SetBodyShaking tremor; ASK GAMEPLAY PROGRAMMERS TO EXPOSE THIS ENGINE FUNCTION IN ENSCRIPT
		//PrintString( "Tremor:" + ToString(tremor) );
	}
};