enum CharGender
{
	MALE, FEMALE
}

class COMPersistencyScene: Managed
{
	CharGender 									m_Gender;
	int 										m_LastShavedSeconds;
	int											m_currentCharacterID;
	
//	protected ref EntityAnimEndEventHandler 	m_anim_end_event_handler;

	protected CameraTool 						m_CameraTool;
	protected PlayerBase 						m_DemoUnit;
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
		PersistencyPrint("COMPersistencyScene::COMPersistencyScene");
		SetClickEnable( true );
	}

	void ~COMPersistencyScene()
	{
		PersistencyPrint("COMPersistencyScene::~COMPersistencyScene");
		if ( m_DemoUnit )
		{
			GetGame().ObjectDelete(m_DemoUnit);
		}

		ref CameraTool ct = COM_GetModuleManager().GetModuleByName("CameraTool");

		if ( ct )
		{
			ct.DisableCamera();
			ct.EnableCamera();
			
			ct.DisableCamera();
		}
	}

	void SetupScene()
	{
		PersistencyPrint("COMPersistencyScene::SetupScene");
		m_currentCharacterID = -1;
		m_DemoPos = "0 0 0";
		m_DemoRot = "0 0 0";
		m_LastShavedSeconds = 0;

		string worldName;
		g_Game.GetWorldName(worldName);
		string root_path = "cfgCharacterScenes " + worldName;

		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count - 1);
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
	
		string scene_path = root_path + " " + childName;
		
		SetupWorld( root_path, scene_path );

		// Don't like it, removing it.
		// SetupWeather( root_path, scene_path );
		
		SetupCamera( root_path, scene_path );
	}

	void SetupCamera( string root_path, string scene_path )
	{
		PersistencyPrint("COMPersistencyScene::SetupCamera");
		float fov = GetGame().ConfigGetFloat(scene_path + " fov");
		
		m_Target = SwapYZ(g_Game.ConfigGetVector(scene_path + " target"));
		vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + " position"));

		Module possibleModule = COM_GetModuleManager().GetModuleByName("CameraTool");

		m_CameraTool = CameraTool.Cast( possibleModule );

		if ( !m_CameraTool ) {
			PersistencyPrint( "WTF, the camera doesn't exist shitbag! You dun fucked up in here." );
		}

		m_CameraTool.EnableCamera( true );

		m_CameraTool.GetCamera().SetPosition( COM_SnapToGround( position ) );

		Math3D.MatrixIdentity4( m_CameraTrans );
		
		if ( m_CameraTool.GetCamera() )
		{
			m_CameraTool.GetCamera().LookAt( m_Target );
			m_CameraTool.GetCamera().SetFOV( fov );
			m_CameraTool.GetCamera().SetFocus(5.0, 0.0); //5.0, 1.0
			
			m_CameraTool.GetCamera().SetActive(true);
			
			Math3D.DirectionAndUpMatrix( m_Target - COM_SnapToGround(position), "0 1 0", m_CameraTrans );

			m_CameraTrans[3] = m_CameraTool.GetCamera().GetPosition();

			m_DemoPos = Vector( 0.685547, -0.988281, 3.68823 ).Multiply4( m_CameraTrans );
			
			m_DemoPos = Vector( m_DemoPos[0], GetGame().SurfaceY( m_DemoPos[0], m_DemoPos[2] ), m_DemoPos[2] );

			m_DemoRot = "0 0 0";

			vector camDirection = m_CameraTool.GetCamera().GetPosition() - m_DemoPos;
			m_DemoRot[0] = Math.Atan2( camDirection[0], camDirection[2] ) * Math.RAD2DEG;
		}
		
		m_DeltaX = Math.AbsFloat(m_DemoPos[0] - m_CameraTool.GetCamera().GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_DemoPos[2] - m_CameraTool.GetCamera().GetPosition()[2]);
		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}
	}

	void SetupWorld( string root_path, string scene_path )
	{
		PersistencyPrint("COMPersistencyScene::SetupWorld");
		TIntArray date = new TIntArray;
		GetGame().ConfigGetIntArray(scene_path + " date", date);

		World world = GetGame().GetWorld();
	
		if (world && date.Count() >= 5)
		{
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
		}
	}

	void SetupWeather( string root_path, string scene_path )
	{
		PersistencyPrint("COMPersistencyScene::SetupWeather");
		float overcast = GetGame().ConfigGetFloat(scene_path + " overcast");
		float rain = GetGame().ConfigGetFloat(scene_path + " rain");
		float fog = GetGame().ConfigGetFloat(scene_path + " fog");

		float windspeed = -1;
		if ( GetGame().ConfigIsExisting(scene_path + " windspeed") )
			windspeed = GetGame().ConfigGetFloat(scene_path + " windspeed");

		TFloatArray storm = new TFloatArray;
		GetGame().ConfigGetFloatArray(scene_path + " storm", storm);

		Weather weather = g_Game.GetWeather();
		weather.GetOvercast().SetLimits( overcast, overcast );
		weather.GetRain().SetLimits( rain, rain );
		weather.GetFog().SetLimits( fog, fog );
		
		weather.GetOvercast().Set( overcast, 0, 0);
		weather.GetRain().Set( rain, 0, 0);
		weather.GetFog().Set( fog, 0, 0);
		
		if ( storm.Count() == 3 )
		{
			weather.SetStorm(storm.Get(0),storm.Get(1),storm.Get(2));
		}
		
		if ( windspeed != -1 )
		{
			weather.SetWindSpeed(windspeed);
			weather.SetWindMaximumSpeed(windspeed);
			weather.SetWindFunctionParams(1,1,1);
		}
	}
	
	void SetClickEnable( bool enable )
	{
		PersistencyPrint("COMPersistencyScene::SetClickEnable");
		m_EnableClick = enable;
	}
	
	bool IsClickEnabled()
	{
		PersistencyPrint("COMPersistencyScene::IsClickEnabled");
		return m_EnableClick;
	}
	
	void ResetIntroCamera()
	{
		PersistencyPrint("COMPersistencyScene::ResetIntroCamera");
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( this.ResetPosition, 250 );

		if ( m_DemoUnit )
		{
			m_CameraTool.SetTarget(m_DemoUnit);
			m_CameraTool.FollowTarget();
		}
	}
	
	void SetAttachment(string type, int slot)
	{
		PersistencyPrint("COMPersistencyScene::SetAttachment");
		if (!m_DemoUnit) return;

		g_Game.ObjectDelete(m_DemoUnit.GetInventory().FindAttachment(slot));
		EntityAI entity;
		Class.CastTo(entity, g_Game.CreateObject(type, "0 2000 0", true));
		if (entity)
		{
			m_DemoUnit.LocalTakeEntityAsAttachmentEx(entity, slot);
		}
	}

	void LoadFromSave(string character, string save)
	{
		PersistencyPrint("COMPersistencyScene::LoadFromSave");
		if ( m_DemoUnit )
		{
			GetGame().ObjectDelete(m_DemoUnit);
			m_DemoUnit = NULL;
		}

		m_DemoUnit = CharacterLoad.LoadPlayer( character, save, true );
		
		if (m_DemoUnit)
		{
			m_DemoUnit.SetPosition( COM_SnapToGround( Vector( m_DemoPos[0], m_DemoPos[1], m_DemoPos[2] )) );
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetOrientation(m_DemoRot);
		}

		ResetIntroCamera();
	}
	
	void SetCharacter(string type)
	{
		PersistencyPrint("COMPersistencyScene::SetCharacter");
		if ( m_DemoUnit )
		{
			GetGame().ObjectDelete(m_DemoUnit);
			// m_DemoUnit = NULL;
		}

		g_Game.PreloadObject(type, 1.0);

		m_DemoUnit = PlayerBase.Cast( GetGame().CreateObject( type, "0 0 0", true, true, true ) );
		
		if (m_DemoUnit)
		{
			m_DemoUnit.SetPosition( COM_SnapToGround( Vector( m_DemoPos[0], m_DemoPos[1], m_DemoPos[2] )) );
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetOrientation(m_DemoRot);
//			m_DemoUnit.SetEventHandler(m_anim_end_event_handler);
			m_DemoUnit.SetLastShavedSeconds(m_LastShavedSeconds);

			string lvalue = "";
			m_DemoUnit.StatGetCounter("playtime", lvalue);

			PluginLifespan module_lifespan;
			Class.CastTo(module_lifespan, PluginLifespan.Cast( GetPlugin( PluginLifespan ) ));
			module_lifespan.UpdateLifespanParam( m_DemoUnit, lvalue, true );
		}

		ResetIntroCamera();
	}
	
	void ResetPosition()
	{
		PersistencyPrint("COMPersistencyScene::ResetPosition");
		if (m_DemoUnit)
		{
			m_DemoUnit.SetPosition(m_DemoPos);
			m_DemoUnit.SetOrientation(m_DemoRot);
		}
		SetClickEnable( true );
	}

	bool IsRotatingCharacter()
	{
		PersistencyPrint("COMPersistencyScene::IsRotatingCharacter");
		return m_RotatingCharacter;
	}
	
	void CharacterRotationStart()
	{
		PersistencyPrint("COMPersistencyScene::CharacterRotationStart");
		m_RotatingCharacter = true;
		g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);
		if (m_DemoUnit) 
		{
			m_RotatingCharacterRot = m_DemoRot[0];
		}
	}
	
	void CharacterRotationStop()
	{
		PersistencyPrint("COMPersistencyScene::CharacterRotationStop");
		if (m_RotatingCharacter)
		{
			m_RotatingCharacter = false;
		}
	}
	
	void CharacterRotate()
	{
		PersistencyPrint("COMPersistencyScene::CharacterRotate");
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
		PersistencyPrint("COMPersistencyScene::SwapYZ");
		vector tmp;
		tmp[0] = vec[0];
		tmp[1] = vec[2];
		tmp[2] = vec[1];
	
		return tmp;
	}
	
	vector COM_SnapToGround(vector pos)
	{
		PersistencyPrint("COMPersistencyScene::SnapToGround");
		
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}
	
	PlayerBase GetPlayerUnit()
	{
		PersistencyPrint("COMPersistencyScene::GetPlayerUnit");
		return m_DemoUnit;
	}
};