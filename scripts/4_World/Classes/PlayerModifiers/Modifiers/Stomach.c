class Stomach: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_STOMACH;
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

	override void OnReconnect(PlayerBase player)
	{

	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{
		float stomach_volume = player.GetStatStomachSolid().Get();
		float stomach_water = player.GetStatStomachWater().Get();
		
		if ( player.GetStatStomachEnergy().Get() >= ModifierConstants.STOMACH_ENERGY_TRANSFERED_PER_SEC*deltaT )
		{
			player.GetStatEnergy().Add(ModifierConstants.STOMACH_ENERGY_TRANSFERED_PER_SEC*deltaT);
			player.GetStatStomachEnergy().Add(-ModifierConstants.STOMACH_ENERGY_TRANSFERED_PER_SEC*deltaT);
		}
		else
		{
			player.GetStatEnergy().Add(player.GetStatStomachEnergy().Get());
			player.GetStatStomachEnergy().Add(-player.GetStatStomachEnergy().Get());
		}
		
		if (  stomach_water >= ModifierConstants.STOMACH_WATER_TRANSFERED_PER_SEC*deltaT )
		{
			player.GetStatStomachWater().Add(-ModifierConstants.STOMACH_WATER_TRANSFERED_PER_SEC*deltaT);
			player.GetStatWater().Add(ModifierConstants.STOMACH_WATER_TRANSFERED_PER_SEC*deltaT);
		}
		else
		{
			player.GetStatStomachWater().Add(-stomach_water);
			player.GetStatWater().Add(stomach_water);
		}
		
		if ( stomach_volume > 0 )
		{
			player.GetStatStomachSolid().Add(-ModifierConstants.STOMACH_SOLID_EMPTIED_PER_SEC * deltaT);
			
			if( player.GetStatStomachSolid().Get() < 0 )
			{
				player.GetStatStomachSolid().Set(0);
			}
		}
	}
};