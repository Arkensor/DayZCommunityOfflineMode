class RemotePlayerMeta
{
	eRemoteDebugType 	m_DebugType;
	PlayerBase 			m_Player;
	
	void RemotePlayerMeta(PlayerBase player, eRemoteDebugType type )
	{
		m_Player = player;
		m_DebugType = type;
	}
	
	void SetDebugType(eRemoteDebugType type)
	{
		m_DebugType = type;
	}
	
	eRemoteDebugType GetDebugType(eRemoteDebugType type)
	{
		return m_DebugType;
	}
	
	void SetPlayer(PlayerBase player)
	{
		m_Player = player;
	}
	
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
}