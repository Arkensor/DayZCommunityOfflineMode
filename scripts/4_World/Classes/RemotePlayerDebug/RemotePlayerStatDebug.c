enum eRemoteStatType
{
	NONE,
	DAMAGE_SYSTEM = 1,
	PLAYER_STATS = 2,
	OTHER = 4,
}


class RemotePlayerStatDebug
{
	PlayerBase m_Player;
	ref array<ref StatDebugObject> m_Stats = new array<ref StatDebugObject>;
	
	void RemotePlayerStatDebug(PlayerBase player)
	{
		m_Player = player;
		Init();

	}
	
	void Init()
	{
		UpdatePlayerStatsValues();
		InjectDamageSystemValues();
	}
	
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	void UpdatePlayerStatsValues()
	{
		m_Player.GetPlayerStats().GetDebugInfo(m_Stats, 0);
	}
	
	void InjectDamageSystemValues()
	{
		m_Stats.Insert( new StatDebugObject("Health", m_Player.GetHealth("",""), eRemoteStatType.DAMAGE_SYSTEM) );
		m_Stats.Insert( new StatDebugObject("Blood", m_Player.GetHealth("","Blood"), eRemoteStatType.DAMAGE_SYSTEM) );
		m_Stats.Insert( new StatDebugObject("Shock", m_Player.GetHealth("","Shock"), eRemoteStatType.DAMAGE_SYSTEM) );
	}
	
	void SerializeNames(array<string> names, eRemoteDebugType type)
	{
		for(int i = 0; i < m_Stats.Count(); i++)
		{
			if( type ==  eRemoteDebugType.ALL )
			{
				names.Insert(m_Stats.Get(i).GetName());
			}
			else if( type ==  eRemoteDebugType.DAMAGE_ONLY )
			{
				StatDebugObject obj = m_Stats.Get(i);
				eRemoteStatType debug_type = obj.GetType();
				if(debug_type == eRemoteStatType.DAMAGE_SYSTEM)
				{
					names.Insert(m_Stats.Get(i).GetName());
				}
			}
		}
	}
	
	void SerializeValues(array<string> values, eRemoteDebugType type)
	{
		for(int i = 0; i < m_Stats.Count(); i++)
		{
			if( type ==  eRemoteDebugType.ALL )
			{
				values.Insert(m_Stats.Get(i).GetValue());
			}
			else if( type ==  eRemoteDebugType.DAMAGE_ONLY )
			{
				StatDebugObject obj = m_Stats.Get(i);
				eRemoteStatType debug_type = obj.GetType();
				if(debug_type == eRemoteStatType.DAMAGE_SYSTEM)
				{
					values.Insert(m_Stats.Get(i).GetValue());
				}
			}
		}
	}
	
	void Debug()
	{
		for(int i = 0; i < m_Stats.Count(); i++)
		{
			m_Stats.Get(i).Debug();
		}
	}

}