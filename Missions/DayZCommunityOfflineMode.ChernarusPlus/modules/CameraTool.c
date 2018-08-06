class CameraTool extends Module
{
	protected Camera m_oCamera;
	protected float m_CamFOV = 1.0; // default FOV
	protected float m_DistanceToObject;
	protected bool m_FollowTarget = false;
	protected bool m_FreezePlayer = false;
	protected bool m_OrbitalCam = false;
	protected bool m_FreezeCam = false;
	
	static bool  CAMERA_DOF = true;
	static bool  CAMERA_AFOCUS = true;
	static float CAMERA_BLUR = 0.0; // modified via ui
	static float CAMERA_FLENGTH = 50.0; // modified via ui
	static float CAMERA_FNEAR = 50.0; // modified via ui
	static float CAMERA_FDIST = 0.0;
	static float CAMERA_DOFFSET = 0.0;
	
	protected vector m_CamOffset;
	
	protected Object m_Target;
	protected vector m_TargetPos; // Static position
	
	void CameraTool( CommunityOfflineMode mission )
	{
	}

	void ~CameraTool()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( int timeslice )
	{
		UpdateCamera();
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCamera  = new KeyMouseBinding( GetModuleType(), "ToggleCamera" , "[Insert]"    , "Toggle camera."  );
		KeyMouseBinding freezeCamera  = new KeyMouseBinding( GetModuleType(), "FreezeCamera" , "[BackSlash]" , "Freezes camera." );
		KeyMouseBinding freezePlayer  = new KeyMouseBinding( GetModuleType(), "FreezePlayer" , "[Capslock]"  , "Freezes player." );
		KeyMouseBinding followTarget  = new KeyMouseBinding( GetModuleType(), "FollowTarget" , "[LBracket]"  , "Follows target." );
		KeyMouseBinding toggleOrbit   = new KeyMouseBinding( GetModuleType(), "ToggleOrbital", "[RBracket]"  , "Toggle orbital mode" );
		KeyMouseBinding targetCamera  = new KeyMouseBinding( GetModuleType(), "TargetCamera" , "[Return]"	 , "Targets objects or positions" );
		KeyMouseBinding zoomCamera    = new KeyMouseBinding( GetModuleType(), "ZoomCamera"   , "(RMB)+(Drag)", "Zooms camera"	 );
		
		toggleCamera.AddKeyBind( KeyCode.KC_INSERT    , KB_EVENT_RELEASE );
		freezeCamera.AddKeyBind( KeyCode.KC_BACKSLASH , KB_EVENT_RELEASE );
		freezePlayer.AddKeyBind( KeyCode.KC_CAPITAL   , KB_EVENT_RELEASE );
		followTarget.AddKeyBind( KeyCode.KC_LBRACKET  , KB_EVENT_RELEASE );
		toggleOrbit .AddKeyBind( KeyCode.KC_RBRACKET  , KB_EVENT_RELEASE );
		
		targetCamera.AddMouseBind( MouseState.MIDDLE , MB_EVENT_CLICK );
		zoomCamera	.AddMouseBind( MouseState.RIGHT  , MB_EVENT_DRAG  );
		
		RegisterKeyMouseBinding( toggleCamera );
		RegisterKeyMouseBinding( freezeCamera );
		RegisterKeyMouseBinding( freezePlayer );
		RegisterKeyMouseBinding( followTarget );
		RegisterKeyMouseBinding( toggleOrbit  );
		RegisterKeyMouseBinding( targetCamera );
		RegisterKeyMouseBinding( zoomCamera   );
		
	}
	
	void ToggleCamera() 
	{
		if ( m_oCamera )
		{
			SetFreezePlayer(false);
			SetFreezeMouse(false);

			if( m_Mission.CTRL() || m_Mission.SHIFT() ) // Extra
			{
				vector oCamPos = m_oCamera.GetPosition();
				oCamPos[1] = GetGame().SurfaceY( oCamPos[0], oCamPos[2] );

				GetPlayer().SetPosition( oCamPos );
			}
			else
			{
				GetPlayer().SetPosition( m_Mission.GetCursorPos() );
			}

			m_oCamera.SetActive( false );

			GetGame().ObjectDelete( m_oCamera );

			m_oCamera = NULL;
			
			m_CamFOV = 1.0;
			
			m_FollowTarget = false;
			m_OrbitalCam = false;
			
			m_Target = NULL;
			m_TargetPos = vector.Zero;
			
			PPEffects.ResetDOFOverride();
		}
		else
		{
			m_oCamera = g_Game.CreateObject( "FreeDebugCamera", GetPlayer().GetPosition(), true );

			m_oCamera.SetActive( true );
			
			SetFreezePlayer(true);
			
			m_DistanceToObject = 0.0;
		}
	}
	
	void TargetCamera() 
	{
		if ( m_oCamera ) 
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				// Stop targetting
				m_Target = NULL;
				m_TargetPos = vector.Zero;
			
				return;
			}
			
			vector dir = GetGame().GetPointerDirection();
			vector from = GetGame().GetCurrentCameraPosition();
			vector to = from + ( dir * 10000 );
			
			set< Object > objects = GetObjectsAtCursor(from, to);
			
			if ( objects.Count() > 0) 
			{
				Object object = objects.Get(0);
				if ( object.IsInherited( EntityAI ) && !object.IsBuilding() )
				{
					m_Target = object;
					GetPlayer().MessageStatus("Selected Target: " + object.GetType());
					return;
				} 
			}
			m_TargetPos = m_Mission.GetCursorPos();
		}
	}
	
	void UpdateCamera() 
	{
		if ( m_oCamera ) 
		{
			float dist = 0.0;
			vector from = GetGame().GetCurrentCameraPosition();
	
			if ( m_Target ) 
			{
				vector targetPos;
				
				if ( m_Target.IsInherited( SurvivorBase ) ) 
				{
					targetPos = GetTargetCenter();
				}
				else 
				{
					vector pos = m_Target.GetPosition();
					pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
					
					vector clippingInfo;
					vector objectBBOX;
					
					m_Target.GetCollisionBox(objectBBOX);
					
					pos[1] = (pos[1] - objectBBOX[1] + clippingInfo[1] - objectBBOX[1]) + 1.5;
					
					targetPos = pos;
				}
				
				if ( m_OrbitalCam ) 
				{
					m_oCamera.LookAt( targetPos );
				}
				
				dist = vector.Distance( from, targetPos );
				
				if ( m_FollowTarget ) 
				{
					if ( m_DistanceToObject == 0.0 )
					{
						m_DistanceToObject = vector.Distance(GetTargetCenter(), m_oCamera.GetPosition());
						m_CamOffset = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						m_CamOffset.Normalize();
					}
					
					vector newPos;
					
					if ( m_OrbitalCam ) 
					{
						vector direction = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						direction.Normalize();
						newPos = GetTargetCenter() + ( direction * m_DistanceToObject );
					} 
					else 
					{
						newPos = GetTargetCenter() + ( m_CamOffset * m_DistanceToObject );
					}
					
					m_oCamera.SetPosition( newPos );
					dist = m_DistanceToObject;
				}
			}
			else if ( m_TargetPos != vector.Zero ) 
			{
				m_oCamera.LookAt( m_TargetPos ); // auto orbital
				dist = vector.Distance( from, m_TargetPos );
			}
			
			if ( CAMERA_DOF ) // DOF enabled
			{
				if ( CAMERA_AFOCUS && !m_Target ) //auto focus
				{
					vector to = from + (GetGame().GetCurrentCameraDirection() * 9999);
					vector contact_pos;
					
					DayZPhysics.RaycastRV( from, to, contact_pos, NULL, NULL, NULL , NULL, NULL, false, false, ObjIntersectIFire);
					dist = vector.Distance( from, contact_pos );
				}
				if ( dist > 0 ) CAMERA_FDIST = dist;
				
				PPEffects.OverrideDOF(true, CAMERA_FDIST, CAMERA_FLENGTH, CAMERA_FNEAR, CAMERA_BLUR, CAMERA_DOFFSET);
			}
		}
	}
	
	void ZoomCamera() 
	{
		if ( m_oCamera ) 
		{
			int i = GetMouseState(MouseState.Y);
		
			if ( i != 0 ) 
			{
				m_CamFOV+=i*0.000008; // zoom speed
				
				if ( m_CamFOV < 0.01 ) 
				{
					m_CamFOV = 0.01;
				}
				m_oCamera.SetFOV(m_CamFOV);
			}
		}
	}
	
	void FreezeCamera() 
	{
		if ( m_oCamera ) 
		{
			SetFreezeCam(!m_FreezeCam);
			if ( m_OrbitalCam && m_FreezeCam ) 
			{
				SetFreezeMouse(false);
			}
		}		
	}
	
	void FollowTarget()
	{
		if ( m_oCamera ) 
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				m_FollowTarget = !m_FollowTarget;
				
				if ( !m_OrbitalCam ) 
				{
					SetFreezeCam( m_FollowTarget );
				}
				
				if ( !m_FollowTarget ) 
				{
					m_DistanceToObject = 0.0;
					m_CamOffset = vector.Zero;
				}
			}
		}		
	}
	
	void FreezePlayer()
	{
		if ( m_oCamera )
		{
			m_FreezePlayer = !m_FreezePlayer;
			SetFreezePlayer( m_FreezePlayer );
		}	
	}
	
	void ToggleOrbital() 
	{
		if ( m_oCamera )
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				m_OrbitalCam = !m_OrbitalCam;
				SetFreezeMouse(m_OrbitalCam);
				if (! m_OrbitalCam ) 
				{
					SetFreezeCam(false);
				}
			}
		}		
	}
	
	override void onKeyRelease( int key ) // Fix
	{
		switch( key ) 
		{			
			case KeyCode.KC_W:
			case KeyCode.KC_A:
			case KeyCode.KC_S:
			case KeyCode.KC_D:
			case KeyCode.KC_Q:
			case KeyCode.KC_Z:
			{
				if ( m_oCamera )
				{
					if ( m_Target ) 
					{
						m_CamOffset = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						m_CamOffset.Normalize();
						break;
					}
				}
			}
		}
	}
	
	override void onMouseButtonRelease( int button ) 
	{
		if ( m_oCamera ) 
		{
			if ( button == MouseState.RIGHT ) 
			{
				if ( !m_OrbitalCam ) 
				{
					SetFreezeMouse( false );
				}
			}
		}
	}
	
	override void onMouseButtonPress( int button ) 
	{
		if ( m_oCamera ) 
		{
			if ( button == MouseState.RIGHT ) 
			{
				SetFreezeMouse( true );
			}
		}
	}
	
	bool IsUsingCamera() 
	{
		return m_oCamera != NULL;
	}
	
	vector GetTargetCenter() 
	{
		vector targetPosition;
		
		if ( m_Target.IsInherited( SurvivorBase )) 
		{
			targetPosition = GetPlayer().GetPosition();
			targetPosition[1] = targetPosition[1] + 1.5;
		}
		else 
		{
			targetPosition = m_Target.GetPosition();
			targetPosition[1] = GetGame().SurfaceY(targetPosition[0], targetPosition[2]);
			
			vector clippingInfo;
			vector objectBBOX;
			
			m_Target.GetCollisionBox(objectBBOX);
			
			targetPosition[1] = (targetPosition[1] - objectBBOX[1] + clippingInfo[1] - objectBBOX[1]) + 1.5;
		}
				
		return targetPosition;
	}
	
	void SetFreezePlayer( bool freeze ) 
	{
		GetPlayer().GetInputController().OverrideMovementSpeed( freeze, 0 );
		GetPlayer().GetInputController().OverrideAimChangeX( freeze, 0 );
        GetPlayer().GetInputController().OverrideAimChangeY( freeze, 0 );
	}
	
	void SetFreezeCam( bool freeze ) 
	{
		if ( m_FollowTarget && !freeze && !m_OrbitalCam ) 
		{
			m_FreezeCam = true;
			FreeDebugCamera.Cast( m_oCamera ).SetFreezed( true );
			return; // cannot un freeze camera during follow mode
		}

		m_FreezeCam = freeze;
		FreeDebugCamera.Cast( m_oCamera ).SetFreezed( freeze );
	}
	
	void SetFreezeMouse( bool freeze ) 
	{
		if ( freeze ) 
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
		}
		else 
		{
			GetGame().GetInput().ResetGameFocus(INPUT_DEVICE_MOUSE);
		}
	}
}