class AgentBase extends MessageReceiverBase
{
	float			m_Type = 0;
	float			m_Invasibility;//increase the agent count by this number per second
	float 			m_TransferabilityIn;//to the player
	float			m_TransferabilityOut;//from the player
	int				m_MaxCount = 1;
	float 			m_AutoinfectProbability = 1000;//probability of autoinfect as percentage per hour (50 means 50% chance autoinfect with this agent will happen 1x times within an hour of gameplay)(also dependent on CanAutoinfectPlayer check)
	//int				m_ChanceOfInfection = 1;//chance of transmission from enviro sources like ponds etc. [0..1]
	
	void AgentBase()
	{
		Init();
	}
	
	void Init();
	
	int GetAgentType()
	{
		return m_Type;
	}
	float GetInvasibility()
	{
		return m_Invasibility;
	}

	bool AutoinfectCheck(float deltaT, PlayerBase player)
	{
		float probability = (m_AutoinfectProbability / 3600) * deltaT * 1000;
		float dice_throw = Math.RandomFloat(0,100 * 1000);
		if( dice_throw < probability )
		{
			return CanAutoinfectPlayer(player);
		}
		else 
		{
			return false;
		}
	}

	bool CanAutoinfectPlayer(PlayerBase player)
	{
		return false;
	}
	
	float GetTransferabilityIn()
	{
		return m_TransferabilityIn;
	}

	float GetTransferabilityOut()
	{
		return m_TransferabilityOut;
	}

	int GetMaxCount()
	{
		return m_MaxCount;
	}

	string GetName()
	{
		return ClassName();
	}

}