class SceneData
{
	protected string	m_NameScene;
	protected string	m_NameMission;
	protected float 	m_InitTime;
	protected int 		m_DateInitYear;
	protected int		m_DateInitMonth;
	protected int		m_DateInitDay;
	protected int		m_DateInitHour;
	protected int		m_DateInitMinute;
	protected float 	m_WeaterInitOvercast;
	protected float 	m_WeaterInitRain;
	protected float 	m_WeaterInitFog;
	protected float 	m_WeaterInitWindForce;
	
	protected ref array<ref SceneObject>	m_Objects;
	protected ref array<ref ScenePlayer>	m_Players;
	protected ref array<ref SceneObject>	m_AllObjs;
	
	//========================================
	// Constructor -> SceneData
	//========================================
	void SceneData()
	{
		m_Objects = new array<ref SceneObject>;
		m_Players = new array<ref ScenePlayer>;
		m_AllObjs = new array<ref SceneObject>;
	}
	
	//========================================
	// GetSceneObjects
	//========================================
	array<ref SceneObject> GetSceneObjects()
	{
		return m_Objects;
	}
	
	//========================================
	// GetScenePlayers
	//========================================
	array<ref ScenePlayer> GetScenePlayers()
	{
		return m_Players;
	}
	
	//========================================
	// GetSceneObjectsAll
	//========================================
	array<ref SceneObject> GetSceneObjectsAll()
	{
		array<ref ScenePlayer> scene_players = GetScenePlayers();
		array<ref SceneObject> scene_object = GetSceneObjects();
		
		m_AllObjs.Clear();
		
		foreach (auto p: scene_players)
		{
			m_AllObjs.Insert(p);
		}
		
		foreach (auto o: scene_object)
		{
			m_AllObjs.Insert(o);
		}
		
		
		
		return m_AllObjs;
	}
	
	//========================================
	// GetNameScene
	//========================================
	string GetNameScene()
	{
		return m_NameScene;
	}
	
	//========================================
	// SetNameScene
	//========================================
	void SetNameScene(string name)
	{
		m_NameScene = name;
	}
	
	//========================================
	// GetNameMission
	//========================================
	string GetNameMission()
	{
		return m_NameMission;
	}
	
	//========================================
	// SetNameMission
	//========================================
	void SetNameMission(string name)
	{
		m_NameMission = name;
	}
	
	//========================================
	// GetInitTime
	//========================================
	float GetInitTime()
	{
		return m_InitTime;
	}
	
	//========================================
	// SetInitTime
	//========================================
	void SetInitTime(float time)
	{
		m_InitTime = time;
	}
	
	
	//========================================
	// GetInitYear
	//========================================
	int GetInitYear()
	{
		return m_DateInitYear;
	}
	
	//========================================
	// SetInitYear
	//========================================
	void SetInitYear(int value)
	{
		m_DateInitYear = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitMonth
	//========================================
	int GetInitMonth()
	{
		return m_DateInitMonth;
	}
	
	//========================================
	// SetInitMonth
	//========================================
	void SetInitMonth(int value)
	{
		m_DateInitMonth = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitDay
	//========================================
	int GetInitDay()
	{
		return m_DateInitDay;
	}
	
	//========================================
	// SetInitDay
	//========================================
	void SetInitDay(int value)
	{
		m_DateInitDay = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitHour
	//========================================
	int GetInitHour()
	{
		return m_DateInitHour;
	}
	
	//========================================
	// SetInitHour
	//========================================
	void SetInitHour(int value)
	{
		m_DateInitHour = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitMinute
	//========================================
	int GetInitMinute()
	{
		return m_DateInitMinute;
	}
	
	//========================================
	// SetInitMinute
	//========================================
	void SetInitMinute(int value)
	{
		m_DateInitMinute = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitOvercast
	//========================================
	float GetInitOvercast()
	{
		return m_WeaterInitOvercast;
	}
	
	//========================================
	// SetInitOvercast
	//========================================
	void SetInitOvercast(float value)
	{
		m_WeaterInitOvercast = value;
		GetGame().GetWeather().GetOvercast().Set( value, 0, 1000 );
	}
	
	//========================================
	// GetInitRain
	//========================================
	float GetInitRain()
	{
		return m_WeaterInitRain;
	}
	
	//========================================
	// SetInitRain
	//========================================
	void SetInitRain(float value)
	{
		m_WeaterInitRain = value;
		GetGame().GetWeather().GetRain().Set( value, 0, 1000 );
	}
	
	//========================================
	// GetInitFog
	//========================================
	float GetInitFog()
	{
		return m_WeaterInitFog;
	}
	
	//========================================
	// SetInitFog
	//========================================
	void SetInitFog(float value)
	{
		m_WeaterInitFog = value;
		GetGame().GetWeather().GetFog().Set( value, 0, 1000 );
	}
	
	//========================================
	// GetInitWindForce
	//========================================
	float GetInitWindForce()
	{
		return m_WeaterInitWindForce;
	}
	
	//========================================
	// SetInitWindForce
	//========================================
	void SetInitWindForce(float value)
	{
		// The following method SetWindSpeed does not support smaller values than 0.1!
		if (value > 0.1)
		{
			m_WeaterInitWindForce = value;
		}
		else
		{
			m_WeaterInitWindForce = 0.1;
		}
		
		GetGame().GetWeather().SetWindSpeed( m_WeaterInitWindForce );
	}
	
	//========================================
	// CreateSceneObject
	//========================================
	SceneObject CreateSceneObject(string obj_name, vector pos)
	{
		SceneObject sc_obj = new SceneObject();
		sc_obj.Init(obj_name, pos);
		
		if ( sc_obj != NULL )
		{
			m_Objects.Insert(sc_obj);
				
			return sc_obj;
		}
		
		return NULL;
	}
	
	//========================================
	// CreateScenePlayer
	//========================================
	ScenePlayer CreateScenePlayer()
	{
		ScenePlayer sc_ply = new ScenePlayer();
		sc_ply.Init(STRING_EMPTY, Vector(0,0,0));
		
		m_Players.Insert(sc_ply);
//PrintString("CreateScenePlayer Add => "+ m_Players.Count().ToString() );
		return sc_ply;
	}
	
	//========================================
	// Clear
	//========================================
	void Clear()
	{
		m_Objects.Clear();
		m_Players.Clear();
		m_AllObjs.Clear();
	}
	
	//========================================
	// AddObject
	//========================================
	SceneObject AddObject(EntityAI e)
	{
		SceneObject sc_obj = new SceneObject();
		sc_obj.Init("player", e.GetPosition());
		
		sc_obj.LinkEntityAI( e );
		
		if ( sc_obj.IsPlayer() )
		{
			m_Players.Insert( ScenePlayer.Cast( sc_obj ) );
		}
		else
		{
			m_Objects.Insert( sc_obj );
		}		
		
		return sc_obj;
	}
	
	//========================================
	// GetSceneObjectByEntityAI
	//========================================
	SceneObject GetSceneObjectByEntityAI(EntityAI e)
	{
		for ( int i = 0; i < m_Players.Count(); ++i )
		{
			EntityAI ply = m_Players.Get(i).GetObject();
			
			if ( ply != NULL && ply == e )
			{
				return m_Players.Get(i);
			}	
		}
		
		for ( int j = 0; j < m_Objects.Count(); ++j )
		{
			EntityAI obj = m_Objects.Get(j).GetObject();
			
			if ( obj != NULL && obj == e )
			{
				return m_Objects.Get(j);
			}	
		}
		
		return NULL;	
	}
	
	//========================================
	// DeleteSceneObject
	//========================================
	void DeleteSceneObject(SceneObject scene_object)
	{
		int index_p = m_Players.Find( ScenePlayer.Cast( scene_object ) );
		
		if ( index_p != -1 )
		{
			m_Players.Remove(index_p);
		}
		
		
		int index_o = m_Objects.Find(scene_object);
		
		if ( index_o != -1 )
		{
			m_Objects.Remove(index_o);
		}
	}
}