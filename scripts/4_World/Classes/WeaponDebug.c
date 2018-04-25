enum eDebugMode
{
    NORMAL,
	MUZZLE_FIRE,
	CAMERA_MUZZLE_HYBRID,
	COUNT
};



class WeaponDebug
{
	const int BUFFER_SIZE = 1000;
	const float COLLISIONS_DISTANCE_TOLERANCE = 0.01;
	const float MAX_MUZZLE_DISTANCE_TOLERANCE = 7;
	
	Weapon m_WeaponInHands;
	int m_BufferIndex;
	bool m_IsDrawKeyHeldDown;
	bool m_IsLMBPressed;
	bool m_IsToggleKeyPressed;
	bool m_IsFireKeyPressed;
	float m_TargetDistance;
	eDebugMode m_CurrentMode;
	vector m_AimTrailCyclic[BUFFER_SIZE];	
	vector m_AimTrailOrdered[BUFFER_SIZE];
	
	ref map<int, string> m_DebugModesNames = new map<int, string>;
	
	//ref array<Selection> m_Selections = new array<Selection>();
	
	Shape m_Shape_usti;
	Shape m_Shape_konec;
	Shape m_ShapeFireDirection1;
	Shape m_ShapeFireDirection2;
	Shape m_HitShape;
	Shape m_ShapeEye;
	Shape m_ShapeTrailLines;
	Shape m_ShapeFireDirCamera;
	Shape m_HitShape2;
	Shape m_HitShape3;
	Shape m_HitShape4;
	//Shape temp_shape;
	Shape m_PermanentLine1;
	Shape m_PermanentLine2;
	
	Weapon GetWeaponInHands()
	{
		Weapon weapon_in_hands;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if( player && player.GetItemInHands() ) Class.CastTo(weapon_in_hands,  player.GetItemInHands());
		
		return weapon_in_hands;
	}
	
	void WeaponDebug()
	{
		m_DebugModesNames.Insert(eDebugMode.NORMAL, "Mode "+eDebugMode.NORMAL.ToString()+": shoot along the weapon barrel");
		m_DebugModesNames.Insert(eDebugMode.MUZZLE_FIRE,"Mode "+eDebugMode.MUZZLE_FIRE.ToString()+": shoot from muzzle end to camera trace hit point");
		m_DebugModesNames.Insert(eDebugMode.CAMERA_MUZZLE_HYBRID, "Mode "+eDebugMode.CAMERA_MUZZLE_HYBRID.ToString()+": shoot either from camera lens or from muzzle \n depending on the situation");
	}
	
	void ~WeaponDebug()
	{
		RemoveAllShapes(true);
		
	}

	void RemoveAllShapes(bool is_exit = false)
	{
		Debug.RemoveShape(m_Shape_usti);
		Debug.RemoveShape(m_Shape_konec);
		Debug.RemoveShape(m_ShapeFireDirection1);
		Debug.RemoveShape(m_ShapeFireDirection2);
		Debug.RemoveShape(m_HitShape);
		Debug.RemoveShape(m_ShapeEye);
		Debug.RemoveShape(m_ShapeTrailLines);
		Debug.RemoveShape(m_ShapeFireDirCamera);
		Debug.RemoveShape(m_HitShape2);
		Debug.RemoveShape(m_HitShape3);
		
		if( is_exit )
		{
			Debug.RemoveShape(m_PermanentLine1);
			Debug.RemoveShape(m_PermanentLine2);
		}
	}


	void OnCommandHandlerUpdate()
	{
			
		m_IsLMBPressed = false;
		m_IsFireKeyPressed = false;
		
		if (KeyState(KeyCode.KC_LWIN) == 1)
		{
			if(!m_IsDrawKeyHeldDown)
			{
				OnKeyDown(KeyCode.KC_LWIN);
			}
			m_IsDrawKeyHeldDown = true;
		}
		else
		{
			if( m_IsDrawKeyHeldDown )
			{
				//OnKeyUp();
			}
			m_IsDrawKeyHeldDown = false;
		}
		
		if (KeyState(KeyCode.KC_Z) == 1)
		{
			ClearKey(KeyCode.KC_Z);
			CycleDebugMode();
		}
		
		if (KeyState(KeyCode.KC_X) == 1)
		{
			m_IsFireKeyPressed = true;
			ClearKey(KeyCode.KC_X);
		}

		if(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)
		{
			m_IsLMBPressed = true;
		}
	}

	void OnKeyDown(KeyCode key)
	{
		if(key == KeyCode.KC_X)
		{
		}
	}
	
	void CycleDebugMode()
	{
		m_CurrentMode++;
		if( m_CurrentMode == eDebugMode.COUNT )
		{
			m_CurrentMode = 0;
		}
	
	}
	
	void OnPostFrameUpdate()
	{
		if( GetWeaponInHands() )
		{
			RemoveAllShapes();
			Weapon weapon = GetWeaponInHands();
			vector cameraDirection = GetGame().GetCurrentCameraDirection();
			vector cameraPosition = GetGame().GetCurrentCameraPosition();
			vector usti_hlavne_position = weapon.GetSelectionPosition( "usti hlavne" );//usti hlavne
			vector konec_hlavne_position = weapon.GetSelectionPosition( "konec hlavne" );//konec hlavne
			usti_hlavne_position = weapon.ModelToWorld(usti_hlavne_position);
			konec_hlavne_position = weapon.ModelToWorld(konec_hlavne_position);
			
			if( m_CurrentMode == eDebugMode.NORMAL ) 
			{
				DrawLineOfFire(konec_hlavne_position,usti_hlavne_position );
			}
			
			if( m_CurrentMode == eDebugMode.MUZZLE_FIRE ) 
			{
				DrawLineOfFireMuzzleToHit(usti_hlavne_position, cameraDirection, cameraPosition);
			}
			
			if( m_CurrentMode == eDebugMode.CAMERA_MUZZLE_HYBRID ) 
			{
				DrawLineOfFireCameraHybrid(usti_hlavne_position, cameraDirection, cameraPosition);
			}
			
			if(m_IsDrawKeyHeldDown)
			{
				AddPosToCyclicBuffer(usti_hlavne_position);
				OrderTrailArray();
			}

			m_ShapeTrailLines = Debug.DrawLines( m_AimTrailOrdered,BUFFER_SIZE, COLOR_YELLOW, ShapeFlags.NOZBUFFER );
			
		
			DrawEyePoint(weapon);
			DrawBarrelMemoryPoints(konec_hlavne_position,usti_hlavne_position );
			DisplayGeneralInfo();
			DisplayTargetInfo();
			/*
			vector pos;
			weapon.GetProjectedCursorPos3d(pos);
			Debug.RemoveShape(temp_shape);
			temp_shape = Debug.DrawSphere(pos, 0.1, Colors.GREEN);
			*/
		}
	}

	void DrawBarrelMemoryPoints(vector begin_point, vector end_point)
	{
		if(!m_IsDrawKeyHeldDown)
		{
			m_Shape_usti = Debug.DrawSphere(end_point, 0.011, Colors.GREEN, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
			m_Shape_konec = Debug.DrawSphere(begin_point, 0.011, Colors.GREEN, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
		}
	}


	void DrawLineOfFire(vector begin_point, vector end_point)
	{
		vector contact_point;
		vector contact_dir;
		vector aim_point = end_point - begin_point;
		int contact_component;
		
		aim_point = aim_point.Normalized() * 100;
		aim_point = aim_point + end_point;
		
		m_ShapeFireDirection1 = Debug.DrawLine(end_point, aim_point);
		m_ShapeFireDirection2 = Debug.DrawLine(begin_point, end_point, ShapeFlags.NOZBUFFER );
		
		if( DayZPhysics.RaycastRV(end_point, aim_point, contact_point, contact_dir, contact_component, null, null, null, false, false, ObjIntersectFire) )
		{	
			m_HitShape = Debug.DrawSphere(contact_point, 0.04, COLOR_RED);
		}
		
		if( m_IsFireKeyPressed )
		{
			Debug.RemoveShape(m_PermanentLine1);
			m_PermanentLine1 = Debug.DrawLine(end_point, contact_point, Colors.RED, ShapeFlags.NOZBUFFER );
		}
	}

	void AddPosToCyclicBuffer(vector pos)
	{
		m_AimTrailCyclic[m_BufferIndex] = pos;
		m_BufferIndex++;
		if(m_BufferIndex == BUFFER_SIZE)
		{
			m_BufferIndex = 0;			
		}
	}	
	
	void OrderTrailArray()
	{
		int unordered_index;
		
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			unordered_index = m_BufferIndex + i;
			if( unordered_index >= BUFFER_SIZE )
			{
				unordered_index = unordered_index - BUFFER_SIZE;
			}
			m_AimTrailOrdered[i] = m_AimTrailCyclic[unordered_index];
		}
	}	
	
	void DrawEyePoint(Weapon weapon)
	{
		vector position = GetEyePointPosition(weapon);
		m_ShapeEye = Debug.DrawSphere(position, 0.009, COLOR_BLUE, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
	}

	vector GetEyePointPosition(Weapon weapon)
	{
		string memory_point_name = weapon.ConfigGetString("memoryPointCamera");
		ItemBase optics = weapon.GetAttachedOptics();
		if(optics)
		{
			memory_point_name = optics.ConfigGetString("memoryPointCamera");
			return optics.ModelToWorld(optics.GetSelectionPosition( memory_point_name ));
		}
		return weapon.ModelToWorld(weapon.GetSelectionPosition( memory_point_name ));
		
	}

	void DisplayGeneralInfo()
	{
		DbgUI.Begin("sway weight", 50, 50);
		DayZPlayerImplement player = DayZPlayerImplement.Cast(GetGame().GetPlayer());
		float sway_weight = player.GetAimingModel().GetSwayWeight();
		DbgUI.Text("value: " + sway_weight.ToString());
		DbgUI.Text("Hold LWIN to draw debug line");
		DbgUI.Text("Press X to simulate fire");
		DbgUI.Text("Press Z to cycle debug modes");
		DbgUI.Text("Debug " +m_DebugModesNames.Get(m_CurrentMode) );
		DbgUI.Text("");
		DbgUI.End();
	}

	void DisplayTargetInfo()
	{
		DbgUI.Begin("target distance", 50,200);
		DbgUI.Text("value: " + m_TargetDistance.ToString());
		DbgUI.End();
	}

		
	void DrawLineOfFireMuzzleToHit(vector begin_point, vector camera_dir, vector camera_pos)
	{
		vector contact_point_cam_trace;
		vector contact_point_muzzle_trace;
		
		vector contact_dir_muzzle;
		vector contact_dir_camera;
		
		vector contact_point;
		vector contact_dir;
		
		int contact_component;
		int contact_component_muzzle;
		
		vector end_point = camera_pos + camera_dir * 1000;
		Man player = GetGame().GetPlayer();
		Object player_o;
		Class.CastTo(player_o, player);
		
		if( DayZPhysics.RaycastRV(camera_pos, end_point, contact_point_cam_trace, contact_dir_camera, contact_component,null, null, player_o , false, false, ObjIntersectFire, 0.1) )
		{
			m_ShapeFireDirCamera = Debug.DrawLine(begin_point, contact_point_cam_trace, Colors.RED, ShapeFlags.NOZBUFFER );
			m_HitShape2 = Debug.DrawSphere(contact_point_cam_trace, 0.03, Colors.GREEN);
			m_TargetDistance =  vector.Distance( player.GetPosition(), contact_point_cam_trace);
			
			if( m_IsFireKeyPressed )
			{
				Debug.RemoveShape(m_PermanentLine1);
				Debug.RemoveShape(m_PermanentLine2);
				m_PermanentLine1 = Debug.DrawLine(begin_point, contact_point_cam_trace, Colors.RED, ShapeFlags.NOZBUFFER );
				m_PermanentLine2 = Debug.DrawLine(camera_pos, contact_point_cam_trace, Colors.GREEN, ShapeFlags.NOZBUFFER );
			}
			
		}
		
		else
		{
			m_ShapeFireDirCamera = Debug.DrawLine(begin_point, end_point, Colors.GREEN, ShapeFlags.NOZBUFFER );
			m_TargetDistance = -1;
		}
		
		if( DayZPhysics.RaycastRV(begin_point, contact_point_cam_trace, contact_point_muzzle_trace, contact_dir_muzzle, contact_component_muzzle, null, null, null, false, false, ObjIntersectFire, 0.0) )
		{
			m_HitShape3 = Debug.DrawSphere(contact_point_muzzle_trace, 0.03, COLOR_RED);
		}
	}

	void DrawLineOfFireCameraHybrid(vector usti_hlavne_position, vector camera_dir, vector camera_pos)
	{
		bool muzzle_ray_colliding;
		vector contact_point_cam_trace;
		vector contact_point_muzzle_trace;
		
		vector aim_at_position;
		
		vector contact_dir_muzzle;
		vector contact_dir;
		
		vector contact_point;
		
		int contact_component;
		int contact_component_muzzle;
		
		float distance_to_aim_at;
		vector end_point = camera_pos + camera_dir * 1000;
		
		Man player = GetGame().GetPlayer();
		Object player_o;
		Class.CastTo(player_o, player);

		if( DayZPhysics.RaycastRV(camera_pos, end_point, contact_point_cam_trace, contact_dir, contact_component,null, null, player_o , false, false, ObjIntersectFire) )
		{
			float collision_distance_camera = vector.Distance(usti_hlavne_position, contact_point_muzzle_trace);
			aim_at_position = contact_point_cam_trace;
		}
		
		if( DayZPhysics.RaycastRV(usti_hlavne_position, contact_point_cam_trace, contact_point_muzzle_trace, contact_dir, contact_component,null, null, player_o , false, false, ObjIntersectFire, 0.05) )
		{
			float collision_distance = vector.Distance(contact_point_cam_trace, contact_point_muzzle_trace);
			float muzzle_collision_distance = vector.Distance(usti_hlavne_position, contact_point_muzzle_trace);
			
			if(collision_distance > COLLISIONS_DISTANCE_TOLERANCE && muzzle_collision_distance < MAX_MUZZLE_DISTANCE_TOLERANCE)
			{
				muzzle_ray_colliding = true;
				aim_at_position = contact_point_muzzle_trace;
			}
		}
		
		distance_to_aim_at = vector.Distance(camera_pos, aim_at_position);
		
		if( m_IsFireKeyPressed )
		{
			Debug.RemoveShape(m_PermanentLine1);
			Debug.RemoveShape(m_PermanentLine2);
			
			if(muzzle_ray_colliding)
			{

				m_PermanentLine1 = Debug.DrawLine(usti_hlavne_position, contact_point_muzzle_trace, Colors.RED, ShapeFlags.NOZBUFFER );
				m_PermanentLine2 = Debug.DrawLine(camera_pos, contact_point_cam_trace, Colors.GREEN, ShapeFlags.NOZBUFFER );
			}
			else
			{
				m_PermanentLine1 = Debug.DrawLine(camera_pos, contact_point_cam_trace, Colors.RED, ShapeFlags.NOZBUFFER );
			}
		}
		
		float clamped_distance = Math.Clamp(distance_to_aim_at, 0 , 100);
		float distance_normalized = Math.InverseLerp(0, 100, clamped_distance);
		float hit_sphere_size = Math.Lerp(0.025, 0.75, distance_normalized);
		
		Debug.RemoveShape(m_HitShape4);
		m_HitShape4 = Debug.DrawSphere(aim_at_position, hit_sphere_size);
		
	}
	
}