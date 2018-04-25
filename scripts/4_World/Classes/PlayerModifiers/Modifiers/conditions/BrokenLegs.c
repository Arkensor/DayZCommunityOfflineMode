class BrokenLegs: ModifierBase
{
	private const 	float	HEALTHY_LEG = 80;

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BROKEN_LEGS;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}	

	override bool  ActivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightLeg", "Health") <= 0 || player.GetHealth("LeftLeg", "Health") <= 0 )
		{
			return true;
		}
		return false;
	}

	override void  OnActivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Legs",true);
	}

	override void OnDeactivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Legs",false);
	}

	override bool  DeactivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightLeg", "Health") >= HEALTHY_LEG && player.GetHealth("LeftLeg", "Health") >= HEALTHY_LEG )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};