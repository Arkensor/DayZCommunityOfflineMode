class PluginRemotePlayerDebugServer extends PluginBase
{
	
	ref set<PlayerBase> m_ClientList = new set<PlayerBase>;
	
	ref array<ref RemotePlayerStatDebug> m_PlayerDebugStats = new array<ref RemotePlayerStatDebug>;
	ref map<PlayerBase, ref RemotePlayerDamageDebug> m_PlayerDebugDamage = new map<PlayerBase, ref RemotePlayerDamageDebug>;
	float m_AccuTime;
	const int INTERVAL = 1;
	
	eRemoteDebugType m_DebugType;
	bool m_Watching;
	
	override void OnUpdate(float delta_time)
	{
		if( m_ClientList.Count()!=0 )
		{
			m_AccuTime+=delta_time;
			
			if( m_AccuTime > INTERVAL )
			{
				m_AccuTime = 0;
				SendDebug();
			}
		}
	}
	
	void SetWatching(bool enable)
	{
		m_Watching = enable;
	}
	
	bool GetWatching()
	{
		return m_Watching;
	}
	
	void GatherPlayerInfo()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		m_PlayerDebugStats.Clear();
		
		for(int i = 0; i < players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(players.Get(i));
			RemotePlayerStatDebug rpd = new RemotePlayerStatDebug(player);
			m_PlayerDebugStats.Insert(rpd);
		}
		
	}
	
	
	void SendDebug()
	{
		GatherPlayerInfo();
		array<ref RemotePlayerDamageDebug> player_damage = new array<ref RemotePlayerDamageDebug>;
		
		for(int i = 0; i < m_ClientList.Count(); i++)
		{
			PlayerBase player = m_ClientList.Get(i);
			if(player)
			{
				ScriptRPC rpc = new ScriptRPC();
				rpc.Write(m_PlayerDebugStats);
				
				
				for(int x = 0; x < m_PlayerDebugDamage.Count(); x++)
				{
					PlayerBase player2 = m_PlayerDebugDamage.GetKey(x);
					RemotePlayerDamageDebug value = m_PlayerDebugDamage.GetElement(x);
					if(player2)
					{
						player_damage.Insert(value);
						
					}
					//int xs = 1 + 1;
				}
				
				rpc.Write(player_damage);
				
				rpc.Send( player, ERPCs.DEV_PLAYER_DEBUG_DATA, true, player.GetIdentity() );
				m_PlayerDebugDamage.Clear();
			}
			else
			{
				m_ClientList.Remove(i);
			}
			
		}
	}
	

	void OnDamageEvent(PlayerBase player, TotalDamageResult damageResult)
	{
		if( !GetWatching() || !damageResult ) return;
		float damage_global = damageResult.GetDamage("","");
		float damage_blood = damageResult.GetDamage("","Blood");
		float damage_shock = damageResult.GetDamage("","Shock");
		
		if( m_PlayerDebugDamage.Contains(player) )
		{
			m_PlayerDebugDamage.Get(player).AddDamage( damage_global, damage_blood, damage_shock );
		}
		else
		{
			RemotePlayerDamageDebug rpdd = new RemotePlayerDamageDebug(player);
			rpdd.AddDamage(damage_global, damage_blood, damage_shock);
			m_PlayerDebugDamage.Insert(player, rpdd);
		}
	}
	

	void OnRequestReceived(PlayerBase player, bool enable)
	{
		int index = m_ClientList.Find(player);
		
		if(enable)
		{
			m_ClientList.Insert(player);
			SetWatching(true);
		}
		else
		{
			m_ClientList.Remove(index);
			if( m_ClientList.Count() == 0 )
			{
				SetWatching(false);
			}
		}
	}
	
	void OnRPC(ParamsReadContext ctx, PlayerBase player)
	{
		bool enable;
		ctx.Read(enable);
		OnRequestReceived(player, enable);
	}
}