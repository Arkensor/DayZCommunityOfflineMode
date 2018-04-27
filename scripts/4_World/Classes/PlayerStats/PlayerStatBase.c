class PlayerStatBase
{
	EPlayerStats	m_Type;
	void 	OnStoreSave( ParamsWriteContext ctx );
	void 	OnStoreLoad( ParamsReadContext ctx );
	void 	OnRPC(ParamsReadContext ctx);
	float 	Get();
	string 	GetLabel();
	void 	SetByFloat(float value);
	bool	IsSynced();
	array<PlayerStatRecord> GetRecords();
	void 	Init(int id, PlayerStats manager);
	void 	SerializeValue(array<ref StatDebugObject> objects, int flags);
	int		GetType()
	{
		return m_Type;
	}
};

class PlayerStat<Class T> extends PlayerStatBase
{
	protected T 		m_MinValue;
	protected T 		m_MaxValue;
	protected T 		m_Value;
	protected string 	m_ValueLabel;
	protected int		m_Flags;
	
	ref array<PlayerStatRecord> m_Records;
	PlayerStats 		m_Manager;
	
	
	void PlayerStat(T min, T max,T init, string label, int flags)
	{
		m_MinValue 		= min;
		m_MaxValue 		= max;
		m_Value			= init;
		m_ValueLabel 	= label;
		m_Flags			= flags;
		
		m_Records = new array<PlayerStatRecord>;
	}
		
	override void Init(int id, PlayerStats manager)
	{
		m_Type = id;
		m_Manager = manager;	
	}
	
	override void SerializeValue(array<ref StatDebugObject> objects, int flags)
	{
		objects.Insert( new StatDebugObject(GetLabel(), Get(), eRemoteStatType.PLAYER_STATS) );
	}
	
	PlayerStats GetManager()
	{
		return m_Manager;
	}	
	
	void Set( T value, string system = "" )
	{
		if( value > m_MaxValue )
		{
			m_Value = m_MaxValue;
		}
		else if(value < m_MinValue)
		{
			m_Value = m_MinValue;
		}
		else
		{
			m_Value = value;
		}
		if( GetManager().GetAllowLogs() ) CreateRecord(value, system);
	}
	
	void SetByFloat(float value, string system = "" )
	{
		T f = value;
		Set(f,system);
		
	}
	/*
	void SetByParam(Param param, string system = "" )
	{
		Class.CastTo(p1, param);
		T v = p1.param1;
		Set(v, system);
	}
	*/
	void Add( T value, string system = "" )
	{
		Set(m_Value+value, system);
	}

	override float Get()
	{
		return m_Value;
	}
	
	override string GetLabel()
	{
		return m_ValueLabel;
	}

	float GetMax()
	{
		return m_MaxValue;
	}

	float GetMin()
	{
		return m_MinValue;
	}
	
	float GetNormalized()
	{
		return ((Get()- GetMin()) / (GetMax() - GetMin()));
	}
	
	override array<PlayerStatRecord> GetRecords()
	{
		return m_Records;
	}
	
	void CreateRecord(float value, string system)
	{
		m_Records.Insert( new PlayerStatRecord(value, GetGame().GetTime(), system ) );
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		//ctx.Write(m_ValueLabel);
		//PrintString("saving " + ClassName()+" value:" +m_Value);
		ctx.Write(m_Value);
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		//string name;
		
		//ctx.Read(name);
		
		ctx.Read(m_Value);
		//PrintString("loading " + ClassName()+" value:" +m_Value);

	}
}