class InfluenzaAgent extends AgentBase
{
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_TEMP = 35;//degrees celsius
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_IMMUNITY = 0.5;//[0..1]

	override void Init()
	{
		m_Type 					= AGT_INFLUENZA;
		m_Invasibility 			= 0.1;//increase the agent count by this number per second
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 1;//from the player
		m_MaxCount 				= 1000;
		m_AutoinfectProbability = 0.5;//
	}

	override bool CanAutoinfectPlayer(PlayerBase player)
	{
		if( player.GetStatTemperature().Get() < INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_TEMP && player.GetImmunity() < INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_IMMUNITY )
		{
			return true;			
		}
		else
		{
			return false;	
		}
	}

}