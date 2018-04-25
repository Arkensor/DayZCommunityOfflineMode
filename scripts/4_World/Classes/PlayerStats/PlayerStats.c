enum EPSstatsFlags
{
	EMPTY,
};


enum EPlayerStats
{
	SHOCK,
	UNCONSCIOUS,
	TEMPERATURE,
	HEATCOMFORT,
	HEATISOLATION,
	TREMOR,
	WET,
	ENERGY,
	WATER,
	STOMACH_SOLID,
	STOMACH_ENERGY,
	STOMACH_WATER,
	DIET,
	STAMINA,
	SPECIALTY,
	BLOODTYPE	
};


class PlayerStats
{

	ref array<ref PlayerStatBase> m_PlayerStats;
	ref array<ref StatDebugObject> m_PlayerStatsDebug;
	
	ref Timer m_SyncTimer;
	Man m_Player;
	bool m_AllowLogs;
	string m_System = "Stats"; //debuging tag
	
	void PlayerStats(Man player)
	{
		m_PlayerStats 	= new array<ref PlayerStatBase>;
		m_Player 		= player;
		
		/*	VARIABLES TO BE ADDED
		"feet",
		"restrainedwith", 
		"kzombies",
		"kplayers",	
		"bleedingsources",	
		"myNotifiers",
		"damageArray",
		"musclecramp",
		"penalties",	
		"totalWeight"
	*/
		//																	min						max						initial					name				flag
		RegisterStat(EPlayerStats.SHOCK,	  		new PlayerStat<float>	(0,						5000, 					0, 						"Shock",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.UNCONSCIOUS,  	new PlayerStat<float>	(0,						1,						0,						"Unconscious",		EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.TEMPERATURE,  	new PlayerStat<float>	(30,					45,						36.3,					"Temperature",		EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.HEATCOMFORT,  	new PlayerStat<float>	(-100,					300,					-20,					"HeatComfort",		EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.HEATISOLATION,  	new PlayerStat<float>	(0,						5000,					0,						"HeatIsolation",	EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.TREMOR,		  	new PlayerStat<float>	(0,						1,						0,						"Tremor",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.WET,  			new PlayerStat<float>	(0,						1,						0,						"Wet",				EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.ENERGY,  			new PlayerStat<float>	(0,						20000,					600,					"Energy",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.WATER,  			new PlayerStat<float>	(0,						5000,					500,					"Water",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.STOMACH_SOLID,  	new PlayerStat<float>	(0,						5000,					0,						"StomachSolid",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.STOMACH_ENERGY,  	new PlayerStat<float>	(0,						40000,					0,						"StomachEnergy",	EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.STOMACH_WATER,  	new PlayerStat<float>	(0,						5000,					0,						"StomachWater",		EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.DIET,  			new PlayerStat<float>	(0,						5000,					2500,					"Diet",				EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.STAMINA,  		new PlayerStat<float>	(0,						STAMINA_MAX,			100,					"Stamina",			EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.SPECIALTY,  		new PlayerStat<float>	(-1,					1,						0,						"Specialty",		EPSstatsFlags.EMPTY) );
		RegisterStat(EPlayerStats.BLOODTYPE,  		new PlayerStat<int>		(0,						7,				BloodTypes.GenerateBloodType(),	"BloodType",		EPSstatsFlags.EMPTY) );
	}


	void ~PlayerStats()
	{
		//if( GetGame() && GetGame().IsDebugActions() ) GatherAllRecords();
	}
	
	void RegisterStat(int id, PlayerStatBase stat)
	{
		m_PlayerStats.InsertAt(stat, id);
		stat.Init(id, this);
	}

	PlayerStatBase GetStatObject(int id)
	{
		return m_PlayerStats.Get(id);
	}

	array<ref PlayerStatBase> Get()
	{
		return m_PlayerStats;
	}

	void SetAllowLogs(bool enable)
	{
		m_AllowLogs = enable;
	}

	bool GetAllowLogs()
	{
		return 	m_AllowLogs;	
	}
	
	void GetDebugInfo( array<ref StatDebugObject> objects, int flags )
	{
		for(int i = 0; i < m_PlayerStats.Count(); i++)
		{
			m_PlayerStats.Get(i).SerializeValue(objects, flags);
		}
	}
	
	void GatherAllRecords()
	{
		FileHandle file = OpenFile("$profile:StatRecords.log", FileMode.WRITE);
		
		FPrintln(file, "================================================================");
		FPrintln(file," ================== " + m_Player.ToString() +" ================== ");
		FPrintln(file, "================================================================");
		
		for(int i = 0; i < m_PlayerStats.Count(); i++)
		{
			array<PlayerStatRecord> records =  m_PlayerStats.Get(i).GetRecords();
			
			FPrintln(file, m_PlayerStats.Get(i).GetLabel() );
			
			for(int z = 0; z < records.Count(); z++)
			{
				PlayerStatRecord record = records.Get(z);
				string output = record.GetStringOutput();
				FPrintln(file, output);
			}
		}
	}
	
	void SaveStats ( ParamsWriteContext ctx )
	{
		for ( int i = 0; i < m_PlayerStats.Count(); i++ )
		{
			m_PlayerStats.Get(i).OnStoreSave(ctx);
		}
	}

	void LoadStats ( ParamsReadContext ctx )
	{
		for ( int i = 0; i < m_PlayerStats.Count(); i++ )
		{
			m_PlayerStats.Get(i).OnStoreLoad(ctx);
		}
	}

}