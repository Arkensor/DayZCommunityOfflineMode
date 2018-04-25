class BrainAgent extends AgentBase
{
	override void Init()
	{
		m_Type 					= AGT_BRAIN;
		m_Invasibility 			= 1;//increase the agent count by this number per second
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_MaxCount 				= 5000;
	}
}