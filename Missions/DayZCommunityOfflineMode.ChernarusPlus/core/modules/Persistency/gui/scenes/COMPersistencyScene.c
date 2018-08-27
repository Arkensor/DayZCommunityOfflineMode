enum CharGender
{
	MALE, FEMALE
}

class COMPersistencyScene: Managed
{
	CharGender 									m_Gender;
	int 										m_LastShavedSeconds;
	int											m_currentCharacterID;
	
	ref TStringArray 							m_CharAllCharacters;
	ref TStringAdvanceArray 					m_CharGenderList;
	ref TStringAdvanceArray 					m_CharPersonalityMaleList;
	ref TStringAdvanceArray 					m_CharPersonalityFemaleList;
	string										m_CharacterType;
	ref TStringAdvanceArray						m_CharShirtList;
	int 										m_CharShirtIndex;
	ref TStringAdvanceArray 					m_CharPantsList;
	int 										m_CharPantsIndex;
	ref TStringAdvanceArray 					m_CharShoesList;
	int 										m_CharShoesIndex;
	
	protected ref EntityAnimEndEventHandler 	m_anim_end_event_handler;
	protected ref PlayerNameHandler 			m_PlayerNameHandler;

	protected CameraTool 						m_CameraTool;
	protected PlayerBase 						m_DemoUnit;
	protected Weather							m_Weather;
	protected vector 							m_DemoPos;
	protected vector 							m_DemoRot;
	protected vector 							m_CameraTrans[4];
	protected vector 							m_Target;

	protected bool								m_EnableClick;
	protected bool 								m_RotatingCamera;
	protected bool 								m_RotatingCharacter;
	protected int 								m_RotatingCharacterMouseX;
	protected int 								m_RotatingCharacterMouseY;
	protected float 							m_RotatingCharacterRot;
	protected float 							m_Radius;
	protected float 							m_Radius_original;
	protected float 							m_DiffX;
	protected float 							m_DeltaX;
	protected float 							m_DeltaZ;

	void COMPersistencyScene()
	{
		m_currentCharacterID = -1;
		m_DemoPos = "0 0 0";
		m_DemoRot = "0 0 0";
		m_LastShavedSeconds = 0;
		
		SetClickEnable( true );

		World w = g_Game.GetWorld();

		string worldName;
		g_Game.GetWorldName(worldName);
	
		string root_path = "cfgCharacterScenes " + worldName;
		
		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count - 1);
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
	
		string scene_path = root_path + " " + childName;
		m_Target = SwapYZ(g_Game.ConfigGetVector(scene_path + " target"));
		vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + " position"));
		TIntArray date = new TIntArray;
		TFloatArray storm = new TFloatArray;
		g_Game.ConfigGetIntArray(scene_path + " date", date);
		float fov = g_Game.ConfigGetFloat(scene_path + " fov");
		float overcast = g_Game.ConfigGetFloat(scene_path + " overcast");
		float rain = g_Game.ConfigGetFloat(scene_path + " rain");
		float fog = g_Game.ConfigGetFloat(scene_path + " fog");
		float windspeed = -1;
		if ( g_Game.ConfigIsExisting(scene_path + " windspeed") ) 	windspeed = g_Game.ConfigGetFloat(scene_path + " windspeed");
		g_Game.ConfigGetFloatArray(scene_path + " storm", storm);
		
		World world = g_Game.GetWorld();
	
		if (world && date.Count() >= 5)
		{
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
		}
		
		m_CameraTool = GetModuleManager().GetModuleByName("CameraTool");
		m_CameraTool.EnableCamera( true );

		m_CameraTool.GetCamera().SetPosition( SnapToGround( position ) );

		Math3D.MatrixIdentity4(m_CameraTrans);
		
		if ( m_CameraTool.GetCamera() )
		{
			m_CameraTool.GetCamera().LookAt(m_Target);
			m_CameraTool.GetCamera().SetFOV(fov);
			m_CameraTool.GetCamera().SetFocus(5.0, 0.0); //5.0, 1.0
			
			m_CameraTool.GetCamera().SetActive(true);
			
			Math3D.DirectionAndUpMatrix(m_Target - SnapToGround(position), "0 1 0", m_CameraTrans);
			m_CameraTrans[3] = m_CameraTool.GetCamera().GetPosition();
			m_DemoPos = Vector(0.685547, -0.988281, 3.68823).Multiply4(m_CameraTrans);

			float pos_x = m_DemoPos[0];
			float pos_z = m_DemoPos[2];
			float pos_y = GetGame().SurfaceY(pos_x, pos_z);
			vector ground_demo_pos = Vector(pos_x, pos_y, pos_z);
			m_DemoPos = ground_demo_pos;

			m_DemoRot = "0 0 0";
			vector to_cam_dir = m_CameraTool.GetCamera().GetPosition() - m_DemoPos;
			m_DemoRot[0] = Math.Atan2(to_cam_dir[0], to_cam_dir[2]) * Math.RAD2DEG;
		}
		
		m_DeltaX = Math.AbsFloat(m_DemoPos[0] - m_CameraTool.GetCamera().GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_DemoPos[2] - m_CameraTool.GetCamera().GetPosition()[2]);
		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}
		
		m_Weather = g_Game.GetWeather();		

		m_Weather.GetOvercast().Set( overcast, 0, 1);
		m_Weather.GetRain().Set( rain, 0, 1);
		m_Weather.GetFog().Set( fog, 0, 1);
		
		if ( storm.Count() == 3 )
		{
			m_Weather.SetStorm(storm.Get(0),storm.Get(1),storm.Get(2));
		}
		
		if ( windspeed != -1 )
		{
			m_Weather.SetWindSpeed(windspeed);
			m_Weather.SetWindMaximumSpeed(windspeed);
			m_Weather.SetWindFunctionParams(1,1,1);
		}
		
		Init();
	}

	void ~COMPersistencyScene()
	{
		if ( m_DemoUnit )
		{
			GetGame().ObjectDelete(m_DemoUnit);
		}

		ref CameraTool ct = GetModuleManager().GetModuleByName("CameraTool");

		if ( ct )
		{
			ct.DisableCamera();
			ct.EnableCamera();
			
			ct.DisableCamera();
		}
	}
	
	void Init()
	{
		m_CharGenderList = new TStringAdvanceArray;
		m_CharPersonalityMaleList = new TStringAdvanceArray;
		m_CharPersonalityFemaleList = new TStringAdvanceArray;
		m_CharAllCharacters = new TStringArray;
		m_CharShirtList = new TStringAdvanceArray;
		m_CharPantsList = new TStringAdvanceArray;
		m_CharShoesList = new TStringAdvanceArray;
		
		string character_CfgName;
		string root_path = "cfgCharacterCreation";
		
		g_Game.ConfigGetTextArray(root_path + " gender", m_CharGenderList);
		g_Game.ConfigGetTextArray(root_path + " top", m_CharShirtList);
		g_Game.ConfigGetTextArray(root_path + " bottom", m_CharPantsList);
		g_Game.ConfigGetTextArray(root_path + " shoe", m_CharShoesList);

		m_CharAllCharacters = GetGame().ListAvailableCharacters();
		for (int i = 0; i < m_CharAllCharacters.Count(); i++)
		{
			character_CfgName = m_CharAllCharacters.Get(i);
			if (GetGame().IsKindOf(character_CfgName, "SurvivorMale_Base"))
			{
				m_CharPersonalityMaleList.Insert(character_CfgName);
			}
			else
			{
				m_CharPersonalityFemaleList.Insert(character_CfgName);
			}
		}
		
		CreateRandomCharacter();
	}
	
	void SetCharacterGender(CharGender gender)
	{
		m_Gender = gender;
	}
	
	CharGender GetCharacterGender()
	{
		return m_Gender;
	}
	
	void SetClickEnable( bool enable )
	{
		m_EnableClick = enable;
	}
	
	bool IsClickEnabled()
	{
		return m_EnableClick;
	}
	
	void ResetIntroCamera()
	{
		m_CameraTool.SetTarget(m_DemoUnit);
		m_CameraTool.FollowTarget();

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( this.SceneCharacterSetPos, 250 );
	}
	
	void RandomSelectGender()
	{
		int chance = Math.RandomInt(0, 2);
		if (chance == 0)
		{
			m_Gender = CharGender.MALE;
		} else {
			m_Gender = CharGender.FEMALE;
		}
	}
	
	void CreateRandomCharacter()
	{
		string params[2];
		
		RandomSelectGender();
		
		if (m_Gender == CharGender.FEMALE)
		{
			m_CharacterType = m_CharPersonalityFemaleList.GetRandomElement();
		}
		else
		{
			m_CharacterType = m_CharPersonalityMaleList.GetRandomElement();
		}

		CreateNewCharacter(m_CharacterType);
		
		if (m_DemoUnit)
		{
			SetAttachment(m_CharShirtList.GetRandomElement(), InventorySlots.BODY);
			SetAttachment(m_CharPantsList.GetRandomElement(), InventorySlots.LEGS);
			SetAttachment(m_CharShoesList.GetRandomElement(), InventorySlots.FEET);
		}

		ResetIntroCamera();
	}
	
	int RandomSelectIndex(TStringAdvanceArray list)
	{
		if (list)
		{
			int rnd = Math.RandomInt(0, list.Count());
			return rnd;
		}
		return -1;
	}
	
	void SetAttachment(string type, int slot)
	{
		if (!m_DemoUnit) return;

		g_Game.ObjectDelete(m_DemoUnit.GetInventory().FindAttachment(slot));
		EntityAI entity;
		Class.CastTo(entity, g_Game.CreateObject(type, "0 2000 0", true));
		if (entity)
		{
			m_DemoUnit.LocalTakeEntityAsAttachmentEx(entity, slot);
		}
	}

	void CharChangePart( Direction dir, int inv_slot )
	{
		TStringAdvanceArray list;
		int list_index;
		
		switch ( inv_slot )
		{
			case InventorySlots.BODY:
			{
				list = m_CharShirtList;
				m_CharShirtIndex = AdjustListIndex(list, dir, m_CharShirtIndex);
				list_index = m_CharShirtIndex;
				break;
			}
			case InventorySlots.LEGS:
			{
				list = m_CharPantsList;
				m_CharPantsIndex = AdjustListIndex(list, dir, m_CharPantsIndex);
				list_index = m_CharPantsIndex;
				break;
			}
			case InventorySlots.FEET:
			{
				list = m_CharShoesList;
				m_CharShoesIndex = AdjustListIndex(list, dir, m_CharShoesIndex);
				list_index = m_CharShoesIndex;
				break;
			}
		}		
		
		SetAttachment(list.Get(list_index), inv_slot);
	}
	
	int AdjustListIndex(TStringAdvanceArray list, Direction dir, int index)
	{
		if ( dir == Direction.RIGHT )
		{
			index++;
			
			if (index >= list.Count() )
			{
				index = 0;
			}
		}
		else if ( dir == Direction.LEFT )
		{
			index--;
			
			if (index < 0)
			{
				index = list.Count() - 1;
			}
		}
		
		return index;
	}

	void SetCharacter(string character, string save)
	{
		if ( m_DemoUnit )
		{
			GetGame().ObjectDelete(m_DemoUnit);
			m_DemoUnit = NULL;
		}

		m_DemoUnit = CharacterLoad.LoadPlayer( character, save, true );
		m_DemoUnit.SetPosition( SnapToGround( Vector( m_DemoPos[0], m_DemoPos[1], m_DemoPos[2] )) );

		if (m_DemoUnit)
		{
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetOrientation(m_DemoRot);
		}

		ResetIntroCamera();
	}
	
	void CreateNewCharacter(string type)
	{
		if (m_DemoUnit)
		{
			GetGame().ObjectDelete(m_DemoUnit);
			m_DemoUnit = NULL;
		}

		g_Game.PreloadObject(type, 1.0);

		m_DemoUnit = PlayerBase.Cast( g_Game.CreateObject( type, SnapToGround( Vector( m_DemoPos[0], m_DemoPos[1], m_DemoPos[2] )), true ) );
		
		if (m_DemoUnit)
		{
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetOrientation(m_DemoRot);
			m_DemoUnit.SetEventHandler(m_anim_end_event_handler);
			m_DemoUnit.SetLastShavedSeconds(m_LastShavedSeconds);

			string lvalue = "";
			m_DemoUnit.StatGetCounter("playtime", lvalue);

			PluginLifespan module_lifespan;
			Class.CastTo(module_lifespan, PluginLifespan.Cast( GetPlugin( PluginLifespan ) ));
			module_lifespan.UpdateLifespanParam( m_DemoUnit, lvalue, true );
		}

		ResetIntroCamera();
	}
	
	void SceneCharacterSetPos()
	{
		if (m_DemoUnit)
		{
			m_DemoUnit.SetPosition(m_DemoPos);
			m_DemoUnit.SetOrientation(m_DemoRot);
		}
		SetClickEnable( true );
	}

	bool IsRotatingCharacter() {
		return m_RotatingCharacter;
	}
	
	void CharacterRotationStart()
	{
		m_RotatingCharacter = true;
		g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);
		if (m_DemoUnit) 
		{
			m_RotatingCharacterRot = m_DemoRot[0];
		}
	}
	
	void CharacterRotationStop()
	{
		if (m_RotatingCharacter)
		{
			m_RotatingCharacter = false;
		}
	}
	
	void CharacterRotate()
	{
		int actual_mouse_x;
		int actual_mouse_y;
		float coef;
		g_Game.GetMousePos(actual_mouse_x, actual_mouse_y);
	
		m_DiffX = m_RotatingCharacterMouseX - actual_mouse_x;
		
		if (m_DemoUnit)
		{
			coef = ( m_RotatingCharacterRot + (m_DiffX * 0.5) ) / 360;
			coef = coef - Math.Floor(coef);

			m_DemoRot[0] = coef * 360;
			
			m_DemoUnit.SetOrientation(m_DemoRot);
		}
	}
	
	void Update()
	{
		// m_CameraTool.UpdateCamera();
		
		m_CameraTool.GetCamera().LookAt(m_Target);

		if (m_DemoUnit && m_RotatingCharacter)
		{
			CharacterRotate();
		}
	}
	
	vector SwapYZ(vector vec)
	{
		vector tmp;
		tmp[0] = vec[0];
		tmp[1] = vec[2];
		tmp[2] = vec[1];
	
		return tmp;
	}
	
	vector SnapToGround(vector pos)
	{
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}
	
	string GetCharacterName()
	{
		return g_Game.GetPlayerGameName();
	}
	
	PlayerBase GetPlayerUnit()
	{
		return m_DemoUnit;
	}
};