class SalmonellaAgent extends AgentBase
{
	override void Init()
	{
		m_Type 					= AGT_SALMONELLA;
		m_Invasibility 			= 0.5;//increase the agent count by this number per second
		m_TransferabilityIn		= 0.1;//to the player
		m_TransferabilityOut	= 0.1;//from the player
		m_MaxCount 				= 1000;
	}
}