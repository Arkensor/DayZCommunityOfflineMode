class PlayerAgentPool
{
	ref map<int,float> m_VirusPool = new map<int,float>;
	ref array<int> m_VirusPoolArray = new array<int>;
	float m_LastTicked = 0;
	float m_TotalAgentCount;
	PlayerBase m_Player;
	
	PluginTransmissionAgents m_PluginTransmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
	
	void PlayerAgentPool(PlayerBase player)
	{
		m_Player = player;
	}

	void ~PlayerAgentPool()
	{
		
	}
	
	void PrintAgents()
	{
		if( m_VirusPool )
		{
			for(int i = 0; i < m_VirusPool.Count(); i++)
			{
				Debug.Log("Agent: "+ m_VirusPool.GetKey(i).ToString(), "Agents");
				Debug.Log("Count: "+ m_VirusPool.GetElement(i).ToString(), "Agents");
			}
		
		}
	}
	
	void ImmuneSystemTick(float value, float deltaT)//this is a regular tick induced in the the player's immune system
	{
		SpawnAgents(deltaT);
		GrowAgents(deltaT);
		AttackAgents(value);
	}

	void OnStoreSave(ParamsWriteContext ctx)
	{
		//Debug.Log("PlayerAgentPool OnStoreSave called", "Agents");
		
		ctx.Write( m_VirusPool.Count() );
		for(int i = 0; i < m_VirusPool.Count();i++)
		{
			int key = m_VirusPool.GetKey(i);
			int value = m_VirusPool.GetElement(i);
			ctx.Write( key );
			ctx.Write( value );
		}
	}
	
	void OnStoreLoad(ParamsReadContext ctx)
	{
		//Debug.Log("PlayerAgentPool OnStoreLoad called", "Agents");
		int count;
		ctx.Read(count);
	
		for(int i = 0; i < count;i++)
		{
			int key;
			int value;
			ctx.Read(key);
			ctx.Read(value);
			m_VirusPool.Insert( key,value );
		}
	
	}
		
	void AddAgent(int agent_id, int count)
	{
		int max_count = m_PluginTransmissionAgents.GetAgentMaxCount(agent_id);

		if(	!m_VirusPool.Contains(agent_id) )//if it contains, maybe add count only ?
		{
			m_VirusPool.Insert( agent_id, Math.Clamp(count,0,max_count) );
		}
		else
		{
			int new_value = m_VirusPool.Get(agent_id) + count;
			m_VirusPool.Set(agent_id,Math.Clamp(new_value,0,max_count) );
		}
	}


	void RemoveAgent(int agent_id)
	{
		if(	m_VirusPool.Contains(agent_id) )
		{
			m_VirusPool.Remove( agent_id );
		}
	}
	
	void RemoveAllAgents()
	{
		m_VirusPool.Clear();
	}
	
	array<int> GetAgents()
	{
		m_VirusPoolArray.Clear();
		
		for(int i = 0; i < m_VirusPool.Count();i++)
		{
			m_VirusPoolArray.Insert( m_VirusPool.GetKey(i) );
		}
		
		return m_VirusPoolArray;
	}

	int GetSingleAgentCount(int agent_id)
	{
		if(	m_VirusPool.Contains(agent_id) )//if it contains, maybe add count only ?
		{
			return m_VirusPool.Get( agent_id );
		}
		else return 0;
	}
	
	int GetTotalAgentCount()
	{
		int agent_count;
		for(int i = 0; i < m_VirusPool.Count(); i++)
		{
			agent_count += m_VirusPool.GetElement(i);
		}
		return agent_count;
	}
	
	void SpawnAgents(float deltaT)
	{
		int count = m_PluginTransmissionAgents.GetAgentList().Count();
		for(int i = 0; i < count;i++)
		{
			AgentBase agent = m_PluginTransmissionAgents.GetAgentList().GetElement(i);
			int agent_id = agent.GetAgentType();
			
			if( GetSingleAgentCount(agent_id)==0 && agent.AutoinfectCheck(deltaT, m_Player) )
			{
				AddAgent(agent_id,100);
			}
		}
	}
	

	void SetAgentCount(int agent_id, int count)
	{
		if(count > 0)
		{
			Debug.Log("+ growing agent"+ agent_id.ToString() +"to count: "+count.ToString(), "Agents");
			m_VirusPool.Set( agent_id, count);
		}
		else
		{
			Debug.Log("- REMOVING agent"+ agent_id.ToString(), "Agents");
			m_VirusPool.Remove( agent_id );
		}			
		
	}
	
	void GrowAgents(float deltaT)
	{
		if ( !IsPluginManagerExists() )//check if modules are running
		{
			return;
		}

		//float m_DeltaT = (GetGame().GetTime() - m_LastTicked) / 1000;
		m_LastTicked = GetGame().GetTime();
		m_TotalAgentCount = 0;
		for(int i = 0; i < m_VirusPool.Count(); i++)
		{	
			int agent_id = m_VirusPool.GetKey(i);
			float invasibility = m_PluginTransmissionAgents.GetAgentInvasibility( agent_id );
			float grow_delta = invasibility * deltaT;
			float old_count = m_VirusPool.Get( agent_id );
			float new_count = old_count + grow_delta;
			m_TotalAgentCount += new_count;//count the overall num of agents
			Debug.Log("old_count"+ old_count.ToString(), "Agents");
			Debug.Log("new_count"+ new_count.ToString(), "Agents");
			PrintString(new_count.ToString());
			SetAgentCount(agent_id, new_count);
		}
	}
	

	void AttackAgents(float immune_attack_value)
	{
		for(int i = 0; i < m_VirusPool.Count(); i++)
		{
			int agent_id = m_VirusPool.GetKey(i);
			float new_count = m_VirusPool.Get( agent_id ) - immune_attack_value;

			SetAgentCount(agent_id, new_count);
		}
	}
	
	void GetDebugObject(array<ref Param> object_out)
	{
	
		int total_items = 0;
		for(int i = 0;i < m_VirusPool.Count(); i++)
		{
			int 	item_key 	= m_VirusPool.GetKey(i);
			float 	item_value 	= m_VirusPool.Get(item_key);
			
			string agent_name = m_PluginTransmissionAgents.GetNameByID(item_key);
			
			object_out.Insert( new Param2<string,string>(agent_name, item_value.ToString()) );
			total_items++;
		}
		object_out.InsertAt(new Param1<int>(total_items) ,0);
	}
}