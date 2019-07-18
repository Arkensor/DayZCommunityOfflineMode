class CameraTool extends Module
{
	protected Camera m_oCamera; // active static camera "staticcamera"

	protected ref array<vector> m_cKeyframes = new array<vector>;

	protected vector velocity; // canera velocity

	protected float yawVelocity;
	protected float pitchVelocity;

	protected float m_CamFOV = 1.0; // default FOV
	protected float m_TargetFOV = 1.0;
	protected float m_TargetRoll;
	protected float m_DistanceToObject;
	protected bool m_FollowTarget = false;
	protected bool m_FreezePlayer = false;
	protected bool m_OrbitalCam = false;
	protected bool m_FreezeCam = false;

	protected bool m_FreezeMouse = false;
	
	static float CAMERA_FOV = 1.0;
	static float CAMERA_TARGETFOV = 1.0;
	static float CAMERA_FOV_SPEED_MODIFIER = 5.0;
	static float CAMERA_SPEED = 5.0;
	static float CAMERA_VELDRAG = 0.9; // 0.9 - 1.0 0.9 == no smoothing
	static float CAMERA_MSENS = 0.8; // acceleration
	static float CAMERA_SMOOTH = 0.65; // drag

	static bool  CAMERA_DOF = false;
	static bool  CAMERA_AFOCUS = true;
	static float CAMERA_BLUR = 0.0; // modified via ui
	static float CAMERA_FLENGTH = 50.0; // modified via ui
	static float CAMERA_FNEAR = 50.0; // modified via ui
	static float CAMERA_FDIST = 0.0;
	static float CAMERA_DOFFSET = 0.0;

	static float CAMERA_SMOOTH_BLUR = 0.0;

	static float EXPOSURE = 0.0;
	static float CHROMABERX = 0.0; // these need to go somewhere else. not where this object is GC'd
	static float CHROMABERY = 0.0;
	static float HUESHIFT = 0.0;

	static float ROTBLUR = 0.0;
	static float MINDEPTH = 2.5;
	static float MAXDEPTH = 4.5;

	static float RADBLURX = 0.0;
	static float RADBLURY = 0.0;
	static float RADBLUROFFX = 0.0;
	static float RADBLUROFFY = 0.0;

	static float VIGNETTE = 0.0;
	static float VARGB[4] = { 0, 0, 0, 0 };

	static float CARGB[4] = { 0, 0, 0, 1 }; // color overlay

	static int VIEWDISTANCE = 1600; // move later

	protected vector m_CamOffset;
	
	protected Object m_Target;
	protected vector m_TargetPos; // Static position
	
	protected float m_CurrentSmoothBlur;

	void CameraTool()
	{
	}

	void ~CameraTool()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
		float speed = 0.2;
		m_CurrentSmoothBlur = Math.Lerp( m_CurrentSmoothBlur, CAMERA_SMOOTH_BLUR, speed );
		PPEffects.SetBlur( m_CurrentSmoothBlur );

		UpdateCamera( timeslice );
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCamera  = new KeyMouseBinding( GetModuleType(), "ToggleCamera" , "Toggle camera."  );
		KeyMouseBinding freezeCamera  = new KeyMouseBinding( GetModuleType(), "FreezeCamera" , "Freezes camera." );
		KeyMouseBinding freezePlayer  = new KeyMouseBinding( GetModuleType(), "FreezePlayer" , "Freezes player.", true);
		KeyMouseBinding followTarget  = new KeyMouseBinding( GetModuleType(), "FollowTarget" , "Follows target.", true );
		KeyMouseBinding toggleOrbit   = new KeyMouseBinding( GetModuleType(), "ToggleOrbital", "Toggle orbital mode", true );
		KeyMouseBinding targetCamera  = new KeyMouseBinding( GetModuleType(), "TargetCamera" , "Targets objects or positions", true );
		KeyMouseBinding zoomCamera    = new KeyMouseBinding( GetModuleType(), "ZoomCamera"   , "Zooms camera" );
		KeyMouseBinding incCamSpeed   = new KeyMouseBinding( GetModuleType(), "IncCamSpeed"  , "Increase camera speed" );
		KeyMouseBinding decCamSpeed   = new KeyMouseBinding( GetModuleType(), "DecCamSpeed"  , "Decrease camera speed" );

		KeyMouseBinding release       = new KeyMouseBinding( GetModuleType(), "Release"		 , "Release mouse", true);

		toggleCamera.AddBinding( "kInsert" );
		freezeCamera.AddBinding( "kBackslash" );
		freezePlayer.AddBinding( "kCapital" );
		followTarget.AddBinding( "kLBracket" );
		toggleOrbit .AddBinding( "kRBracket" );
		
		targetCamera.AddBinding( "mBMiddle" );
		
		zoomCamera.AddBinding( "mBRight", KeyMouseActionType.HOLD );
		zoomCamera.AddBinding( "kLControl", KeyMouseActionType.HOLD );

		incCamSpeed.AddBinding( "mWheelUp" );
		decCamSpeed.AddBinding( "mWheelDown" );

		release.AddBinding( "mBRight", KeyMouseActionType.RELEASE );

//		zoomCamera    .AddBinding( MouseState.WHEEL, 0 );
		
		RegisterKeyMouseBinding( toggleCamera );
		RegisterKeyMouseBinding( freezeCamera );
		RegisterKeyMouseBinding( freezePlayer );
		RegisterKeyMouseBinding( followTarget );
		RegisterKeyMouseBinding( toggleOrbit  );
		RegisterKeyMouseBinding( targetCamera );
		RegisterKeyMouseBinding( zoomCamera   );
		RegisterKeyMouseBinding( incCamSpeed  );
		RegisterKeyMouseBinding( decCamSpeed  );
		RegisterKeyMouseBinding( release );
	}

	Camera GetCamera()
	{
		return m_oCamera;
	}

	void EnableCamera( bool staticCam = false )
	{
		if ( m_oCamera )
		{
			return;
		}

		vector position = "0 0 0";

		if ( GetPlayer() )
		{
			position = GetPlayer().GetPosition();
			position[ 1 ] = position[ 1 ] + 2;
		}

		m_oCamera = g_Game.CreateObject( "staticcamera", position, false );
		m_oCamera.SetActive( true );

		if ( !staticCam ) 
		{
			SetFreezePlayer( true );
		}
		
		m_DistanceToObject = 0.0;
	}

	void DisableCamera()
	{
		if ( m_oCamera )
		{
		    //Close menu to give back control to player
            if( GetGame().GetUIManager().GetMenu() && ( GetGame().GetUIManager().GetMenu().GetID() == 133742 ) )
            {
                GetGame().GetUIManager().Back();
            }

			SetFreezePlayer( false );
			SetFreezeMouse( false );

			vector position;

			if( CTRL() || SHIFT() ) // Extra
			{
				position = m_oCamera.GetPosition();
				position[ 1 ] = GetGame().SurfaceY( position[ 0 ], position[ 2 ] );
			}
			else
			{
				position = GetCursorPos();
			}

			if ( GetPlayer() )
			{
				GetPlayer().SetPosition( position );
			}

			m_oCamera.SetActive( false );

			GetGame().SelectPlayer( NULL, GetPlayer() );

			GetGame().ObjectDelete( m_oCamera );

			m_oCamera = NULL;

			m_CamFOV = 1.0;
			m_TargetFOV = 1.0;
			m_TargetRoll = 0;

			m_FollowTarget = false;
			m_OrbitalCam = false;

			m_Target = NULL;
			m_TargetPos = vector.Zero;

			PPEffects.ResetDOFOverride();
		}
	}
	
	void ToggleCamera() 
	{
		if ( m_oCamera )
		{
			DisableCamera();
		}
		else
		{
			EnableCamera();
		}
	}
	
	void TargetCamera() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				// Stop targetting
				m_Target = NULL;
				m_TargetPos = vector.Zero;
			
				SetFreezeMouse(false);

				return;
			}
			
			vector dir = GetGame().GetPointerDirection();
			vector from = GetGame().GetCurrentCameraPosition();
			vector to = from + ( dir * 1000 );
			
			set< Object > objects = GetObjectsAt(from, to);
			
			if ( objects ) 
			{
				if ( objects.Count() > 0) 
				{
					Object object = objects.Get(0);
					if ( object.IsInherited( EntityAI ) && !object.IsBuilding() )
					{
						m_Target = object;
						return;
					} 
				}
			}
			m_TargetPos = GetCursorPos();
		}
	}

	void SetTarget(Object oObject)
	{
		if (oObject)
		{
			m_Target = oObject;
			m_TargetPos = oObject.GetPosition();
		} else
		{
			m_Target = NULL;
		}
	}
	
	void UpdateCamera( float timeslice) 
	{
		if ( m_oCamera ) 
		{

			if ( m_CamFOV != m_TargetFOV ) 
			{
				m_CamFOV = Math.Lerp( m_CamFOV, m_TargetFOV, timeslice*CAMERA_FOV_SPEED_MODIFIER );
				m_oCamera.SetFOV( m_CamFOV );
			}

			vector oldOrient = m_oCamera.GetOrientation();
			if ( oldOrient[2] != m_TargetRoll ) 
			{
				//oldOrient[2] = Math.Lerp( oldOrient[2], m_TargetRoll, timeslice*CAMERA_FOV_SPEED_MODIFIER );
				//m_oCamera.SetOrientation( oldOrient );
			}

			// Camera movement
			Input input = GetGame().GetInput();

			if ( !m_FreezeCam ) 
			{

				float forward = KeyState(KeyCode.KC_W) - KeyState(KeyCode.KC_S); // -1, 0, 1
				float strafe = KeyState(KeyCode.KC_D) - KeyState(KeyCode.KC_A);
				float altitude = KeyState(KeyCode.KC_Q) - KeyState(KeyCode.KC_Z); // change to hardcode keys? these actions can be rebinded via vanilla keybind menu

				if( KeyState(KeyCode.KC_LSHIFT) ) 
				{
					forward *= 10.0;
					strafe *= 10.0;
					altitude *= 10.0;
				}
				float cam_speed = CAMERA_SPEED;
				float drag = CAMERA_VELDRAG;

				if ( CAMERA_VELDRAG == 0.9 ) 
				{
					cam_speed *= 15.0;
					drag = 0;
				}

				vector up = vector.Up;
				vector direction = m_oCamera.GetDirection();
				vector directionAside = vector.Up * direction;

				up = up * altitude * cam_speed * timeslice;
				direction = direction * forward * cam_speed * timeslice;
				directionAside = directionAside * strafe * cam_speed * timeslice;

				velocity = velocity * drag;
				velocity = ( velocity + direction + directionAside + up );

				vector oldPos = m_oCamera.GetPosition();
				vector newPos = oldPos + velocity;

				float surfaceY = GetGame().SurfaceY( newPos[0], newPos[2] ) + 0.25;
				if ( newPos[1] < surfaceY ) 
				{
					newPos[1] = surfaceY;
				}

				m_oCamera.SetPosition(newPos);
			}

			if ( !m_FreezeMouse ) 
			{
				float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
				float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");

				yawVelocity = yawVelocity + yawDiff * CAMERA_MSENS;
				pitchVelocity = pitchVelocity + pitchDiff * CAMERA_MSENS; // 0.8

				vector newOrient = oldOrient;

				newOrient[0] = oldOrient[0] - Math.RAD2DEG * yawVelocity * timeslice;
				newOrient[1] = oldOrient[1] - Math.RAD2DEG * pitchVelocity * timeslice;

				yawVelocity *= CAMERA_SMOOTH; // drag 0.9
				pitchVelocity *= CAMERA_SMOOTH;

				if( newOrient[1] < -89 )
					newOrient[1] = -89;
				if( newOrient[1] > 89 )
					newOrient[1] = 89;

				m_oCamera.SetOrientation( newOrient );
			}

			// Camera targetting
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
					
					if ( m_OrbitalCam ) 
					{
						direction = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
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
				// m_oCamera.LookAt( m_TargetPos ); // auto orbital
				vector lookDir = m_TargetPos - m_oCamera.GetPosition();
				float roll = m_oCamera.GetOrientation()[2];
				m_oCamera.SetDirection( lookDir );

				vector newRoll = m_oCamera.GetOrientation();
				newRoll[2] = roll;
				m_oCamera.SetOrientation(newRoll);
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
				
				m_oCamera.SetFocus(CAMERA_FDIST, CAMERA_BLUR);
				//m_oCamera.SetNearPlane(CAMERA_FNEAR);
				// PPEffects.OverrideDOF(true, CAMERA_FDIST, CAMERA_FLENGTH, CAMERA_FNEAR, CAMERA_BLUR, CAMERA_DOFFSET); broken :(
			}
		}
	}
	
	void IncCamSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			CAMERA_SPEED *= 1.2;
		}
	}
	
	void DecCamSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			
			CAMERA_SPEED *= 0.8;
			if ( CAMERA_SPEED < 0.001 ) 
			{
				CAMERA_SPEED = 0.001;
			}
		}
	}

	void CameraSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			int i = GetMouseState( MouseState.WHEEL );

			if ( ALT() ) 
			{
				vector ori = m_oCamera.GetOrientation();
				ori[2] = ori[2] - i*5;
				m_oCamera.SetOrientation( ori );
				//m_TargetRoll = ori[2] - i*5; // redo this
				//Message(m_TargetRoll.ToString());
			}
			else 
			{
				float value = 1.2;
				if ( i < 0 ) 
				{
					value = 0.8;
				}

				CAMERA_SPEED *= value;
				if ( CAMERA_SPEED < 0.001 ) 
				{
					CAMERA_SPEED = 0.001;
				}
			}
		}
	}

	void Release() 
	{
		if ( m_oCamera ) 
		{
			if ( !m_OrbitalCam ) 
			{
				SetFreezeMouse( false );
			}
		}
	}

	void ZoomCamera() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			int i = GetMouseState(MouseState.Y);
		
			if ( i != 0 ) 
			{
				SetFreezeMouse(true);
				m_TargetFOV+=i*0.000006; // zoom speed
				
				if ( m_TargetFOV < 0.01 ) 
				{
					m_TargetFOV = 0.01;
				}
				//m_oCamera.SetFOV(m_CamFOV);
			}
		}
		
		/*
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			int i = GetMouseState( MouseState.WHEEL );

			ObjectEditor objEditor = GetModuleManager().GetModule( ObjectEditor );

			if ( objEditor.m_SelectedObject ) 
			{	
				return;
			}

			if ( CTRL() ) 
			{
				vector ori = m_oCamera.GetOrientation();
				m_TargetRoll = ori[2] - Math.RAD2DEG * i*0.09;
			}
			else 
			{
				m_TargetFOV-=i*0.09; // invert
				if ( m_TargetFOV < 0.01 ) 
				{
					m_TargetFOV = 0.01;
				}
			}
			//m_oCamera.SetFOV(m_CamFOV);	
		}
		*/
	}
	
	void FreezeCamera() 
	{
		if ( m_oCamera ) 
		{
			SetFreezeCam(!m_FreezeCam);
		}		
	}
	
	void FollowTarget()
	{
		if ( m_oCamera ) 
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				m_FollowTarget = !m_FollowTarget;
				SetFreezeMouse( m_FollowTarget );
				SetFreezeCam( m_FollowTarget );
				
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
			SetFreezePlayer( !m_FreezePlayer );
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
			targetPosition = m_Target.GetPosition();
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

	Object GetTargetObject() 
	{
		return m_Target;
	}

	vector GetTargetPos() 
	{
		return m_TargetPos;
	}
	
	void SetFreezePlayer( bool freeze ) 
	{
		m_FreezePlayer = freeze;
		GetPlayer().GetInputController().OverrideMovementSpeed( freeze, 0 );
		GetPlayer().GetInputController().OverrideAimChangeX( freeze, 0 );
        GetPlayer().GetInputController().OverrideAimChangeY( freeze, 0 );
	}
	
	void SetFreezeCam( bool freeze ) 
	{
		m_FreezeCam = freeze;
	}
	
	void SetFreezeMouse( bool freeze ) 
	{
		m_FreezeMouse = freeze;
	}
};