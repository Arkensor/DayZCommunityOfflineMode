class BrokenArms: ModifierBase
{
	private const 	float	HEALTHY_ARM = 80;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_BROKEN_ARMS;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override bool ActivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightArm", "Health") <= 0 || player.GetHealth("LeftArm", "Health") <= 0 )
		{
			return true;
		}
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Arms",true);
	}

	override void OnDeactivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Arms",false);
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightArm", "Health") >= HEALTHY_ARM && player.GetHealth("LeftArm", "Health") >= HEALTHY_ARM )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};