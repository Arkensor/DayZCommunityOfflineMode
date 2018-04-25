class RemotePlayerDamageDebug
{
	const int MAX_DAMAGE_RECORDS = 5;
	PlayerBase m_Player;
	bool m_ChangedSinceSerialization;
	
	ref array<ref DamageData> m_DamageList = new array<ref DamageData>;
	
	void RemotePlayerDamageDebug(PlayerBase player)
	{
		m_Player = player;
	}
	
	void AddDamage(float value_global, float value_blood, float value_shock)
	{
		m_ChangedSinceSerialization = true;
		DamageData damage_data = new DamageData( value_global, value_blood, value_shock );
		m_DamageList.InsertAt(damage_data,0);
		if( m_DamageList.Count() > MAX_DAMAGE_RECORDS )
		{
			m_DamageList.RemoveOrdered(MAX_DAMAGE_RECORDS);
		}
	}
	
	void InsertDamageObject(DamageData damage_object)
	{
		m_DamageList.Insert(damage_object);
	}
	

	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	
	void Get(array<ref DamageData> damage_list)
	{
		for(int i = 0; i < m_DamageList.Count(); i++)
		{
			damage_list.Insert(m_DamageList.Get(i));
		}
	}
	
	void GetReversed(array<ref DamageData> damage_list)
	{
		int index = m_DamageList.Count() - 1;
		for(; index >= 0; index--)
		{
			damage_list.Insert(m_DamageList.Get(index));
		}
	}
	
	void Serialize(array<ref RemotePlayerDamageDebug> list)
	{
		if( m_ChangedSinceSerialization )
		{
			list.Insert(this);
		}
		m_ChangedSinceSerialization = false;
	}
	
	void Debug()
	{
		string output;
		for(int i = 0; i < m_DamageList.Count(); i++)
		{
			output = output + m_DamageList.Get(i).ToString() + ", ";
		}
		PrintString("damage values for player " + m_Player.ToString()+":" + output);
		
	}
	
}