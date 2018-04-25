// *************************************************************************************
// ! DayZPlayerCamera3rdPerson - base for 3rd person cameras
// *************************************************************************************
class DayZPlayerCamera3rdPerson extends DayZPlayerCameraBase
{
	static const float 	CONST_UD_MIN	= -85.0;		//!< down limit
	static const float 	CONST_UD_MAX	= 85.0;			//!< up limit

	static const float 	CONST_LR_MIN	= -160.0;		//!< down limit
	static const float 	CONST_LR_MAX	= 160.0;		//!< up limit

	
	void 	DayZPlayerCamera3rdPerson(DayZPlayer pPlayer, HumanInputController pInput)
	{
		if (pInput.Camera3rdIsRightShoulder())
		{
			m_fCameraLRShoulder		= 1.0;
		}
		else
		{
			m_fCameraLRShoulder		= -1.0;
		}

		m_fCameraLRShoulderVel[0]	= 0.0;
		m_fRoll						= 0.0;
	}
		

	//
	override void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		if (pPrevCamera)
		{
			vector 	f = pPrevCamera.GetBaseAngles();
			m_fUpDownAngle		= f[0]; 
			m_fLeftRightAngle	= f[1]; 
			m_fUpDownAngleAdd	= f[2];

		}
	}

	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		float 	udAngle 	= UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
		m_fLeftRightAngle	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		// update l/r offsets and set it as 
		if (m_pInput.Camera3rdIsRightShoulder())
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, 1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}
		else
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, -1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}

		float 	shoulderDist = m_fCameraLRShoulder * m_fShoulderWidth;
		
		//! 
		vector rot;
		rot[0] = m_fLeftRightAngle;
		rot[1] = udAngle;
		rot[2] = m_fRoll;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);
		//! Print(m_fLeftRightAngle);

		//! base bone pos 
		if (m_iBoneIndex != -1)
		{
			pOutResult.m_CameraTM[3]	= m_pPlayer.GetBonePositionMS(m_iBoneIndex);
		}
		else
		{
			pOutResult.m_CameraTM[3]	= "0 0 0";
		}

		//! apply shoulder dist
		vector msOffset = m_CameraOffsetMS;
		vector lsOffset = m_CameraOffsetLS;

		if (m_bShoulderInLS)
		{
			lsOffset[0] = lsOffset[0] + shoulderDist;
		}
		else
		{
			msOffset[0] = msOffset[0] + shoulderDist;
		}

		// ls offset + ms offset + shoulder width			
		pOutResult.m_CameraTM[3]	= pOutResult.m_CameraTM[3] + pOutResult.m_CameraTM[0] * lsOffset[0] + pOutResult.m_CameraTM[1] * lsOffset[1] + pOutResult.m_CameraTM[2] * lsOffset[2] + msOffset;

		//! store distance 
		pOutResult.m_fDistance 		= m_fDistance;
		pOutResult.m_fUseHeading 	= 1.0;
		pOutResult.m_fInsideCamera 	= 0.0;

		super.OnUpdate(pDt, pOutResult);
	}

	//
	override vector GetBaseAngles()
	{
		vector a;
		a[0] = m_fUpDownAngle;
		a[1] = m_fLeftRightAngle;
		a[2] = m_fUpDownAngleAdd;
		return a;
	}

	//! runtime config
	protected	int 	m_iBoneIndex = -1;	//!< main bone 
	protected 	vector  m_CameraOffsetMS;	//!< model space offset
	protected 	vector  m_CameraOffsetLS;	//!< local space offset
	protected 	float 	m_fDistance;		//!< distance from start
	protected 	float 	m_fShoulderWidth;	//!< shoulder camera widths
	protected 	bool	m_bShoulderInLS;	//!< true - shoulder is in local space
	protected 	float 	m_fRoll;			//!< camera roll


	//! runtime values 
	protected 	float 	m_fUpDownAngle;		//!< up down angle in rad
	protected 	float 	m_fUpDownAngleAdd;	//!< up down angle in rad
	protected 	float 	m_fLeftRightAngle;	//!< left right angle in rad (in freelook only)

	//! shoulder offsets
	protected float 	m_fCameraLRShoulder;			// -1..1 shoulderness :)
	protected float 	m_fCameraLRShoulderVel[1];		// 0
}




// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected
// *************************************************************************************
class DayZPlayerCamera3rdPersonErc extends DayZPlayerCamera3rdPerson
{
	void DayZPlayerCamera3rdPersonErc(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonErc");
		
		m_fDistance 		= 1.6;
		m_CameraOffsetMS	= "0.0 1.4 0.0";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.3;
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected sprint
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcSpr extends DayZPlayerCamera3rdPersonErc
{
	void DayZPlayerCamera3rdPersonErcSpr(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonErcSpr");
		
		m_fDistance 		= 2.0;
		m_CameraOffsetMS    = "0.0 1.3 0.0";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.15;
				
		m_iPelvisBone	= pPlayer.GetBoneIndexByName("Pelvis");
		if (m_iPelvisBone == -1)
		{
			Print("DayZPlayerCamera3rdPersonErcSpr: hips bone not found");
		}

		m_fTime = 0.0;
	}



	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		m_fTime += pDt * 5;
		
		vector 	tm[4];

		m_pPlayer.GetBoneTransformLS(m_iPelvisBone, tm);

		//! basically -> transform up vector (0,1,0) and read x coord -> and set is as roll
		float 	xShift = tm[1][0];
		m_fRoll = xShift * 3.0;	// 3 is just made up value i like :)

		// m_fRoll = (sin(1.5 * m_fTime)); //  + 0.3 * sin(3.6 * m_fTime) + 0.2 * sin(0.7 * m_fTime)) / 1.3 * 3;

		//! prev update 
		super.OnUpdate(pDt, pOutResult);
		
		//pOutResult.m_fFovMultiplier = 1.2;
	}
	

	protected int 	m_iPelvisBone;
	protected float m_fTime;	

}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErcRaised - 3rd person erected raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcRaised extends DayZPlayerCamera3rdPersonErc
{
	void DayZPlayerCamera3rdPersonErcRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonErcRaised");

		if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_EMPTYHANDED )
		{
			// no item in hands 
			m_fDistance 		= 1.2;
			m_fShoulderWidth	= 0.4;
			m_CameraOffsetMS    = "0.0 1.3 0.2";
			m_CameraOffsetLS	= "0.0 0.3 0.0";
			
		}		
				else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_TWOHANDED )
				{
					// two handed
					m_fDistance 		= 1.2;
					m_fShoulderWidth	= 0.4;
					m_CameraOffsetMS    = "0.0 1.3 0.2";
					m_CameraOffsetLS	= "0.0 0.3 0.0";
				}
		
				else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_ONEHANDED )
				{
					// one handed
					m_fDistance 		= 1.2;
					m_fShoulderWidth	= 0.4;
					m_CameraOffsetMS    = "0.0 1.3 0.2";
					m_CameraOffsetLS	= "0.0 0.3 0.0";
				}
		
				
		else
		{
			// weapons in hands			
			m_fDistance 		= 0.8;
			m_CameraOffsetMS    = "0.0 1.3 0.2";
			m_CameraOffsetLS	= "0.0 0.3 0.0";
			m_fShoulderWidth	= 0.5;
		}
		
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErcRaised - 3rd person erected raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcRaisedMelee extends DayZPlayerCamera3rdPersonErcRaised
{
	void DayZPlayerCamera3rdPersonErcRaisedMelee(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonErcRaisedMelee");

		//! shift on override - stay at current state
		pInput.Override3rdIsRightShoulder(true, pInput.Camera3rdIsRightShoulder());
	}

	void ~DayZPlayerCamera3rdPersonErcRaisedMelee()
	{
		//! shift off override
		m_pInput.Override3rdIsRightShoulder(false, false);
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouch - 3rd person crouch
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouch extends DayZPlayerCamera3rdPerson
{
	static const float 	CONST_CAMERAMOVEMENTHEIGH = 0.4;	// 0 idle, 0.15 walk 0.3 run/sprint 

	void DayZPlayerCamera3rdPersonCrouch (DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonCrouch");
		
		m_fDistance 		= 1.4;
		m_CameraOffsetMS	= "0.0 0.7 0.1";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.3;
	}

	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		ref HumanMovementState		state 	= new HumanMovementState();		
		m_pPlayer.GetMovementState(state);

		//! movement height 
		float 	movement 	= (Limit(state.m_iMovement, 1, 3) - 1.0) * 0.5;
		movement 			*= CONST_CAMERAMOVEMENTHEIGH;
		m_fCameraHeight		= Math.SmoothCD(m_fCameraHeight, movement, m_fCameraHeightVel, 0.2, 1000, pDt);
		m_CameraOffsetMS[1] = 0.7 + m_fCameraHeight;		
		
		//! prev update 
		super.OnUpdate(pDt, pOutResult);
	}

	protected float 	m_fCameraHeight;			// -1..1 shoulderness :)
	protected float 	m_fCameraHeightVel[1];		// 0
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouchRaised- 3rd person crouch raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouchRaised extends DayZPlayerCamera3rdPersonCrouch
{
	void DayZPlayerCamera3rdPersonCrouchRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonCrouchRaised");
		
		m_fDistance 		= 0.8;
		m_CameraOffsetMS    = "0.0 0.8 0.1"; // being overwritten by m_CameraOffsetMS[1] from DayZPlayerCamera3rdPersonCrouch 
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.5;
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProneRaised - 3rd person prone raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonProneBase extends DayZPlayerCamera3rdPerson
{
    protected 	float 	m_LookAtLRAngle;
	
	float UpdateLRAngleLookAt(float pMin, float pMax, float pDt)
	{
		//! lr angle
		if (m_pInput.CameraIsFreeLook() || m_bForceFreeLook)	
		{
			m_LookAtLRAngle	+= m_pInput.GetAimChange()[0] * Math.RAD2DEG;
			m_LookAtLRAngle	= Limit(m_LookAtLRAngle, pMin, pMax);

			m_fLRAngleVel[0]	= 0;	// reset filter
		}
		else
		{
			// smooth value back to 0 
			m_LookAtLRAngle = Math.SmoothCD(m_LookAtLRAngle, 0, m_fLRAngleVel, 0.14, 1000, pDt);
			// m_fLeftRightAngle	= 0.9 * (1.0 - pDt);			
		}

		return m_LookAtLRAngle;
	}


	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		float 	udAngle 	= UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);

		float 	orientYaw	= m_pPlayer.GetOrientation()[0];
		float 	headYaw 	= m_pInput.GetHeadingAngle() * Math.RAD2DEG;	//! this is actually negative to yaw
		float 	lrAngle		= -fixAngle_180_180(orientYaw + headYaw);		

		// Print("OY: " + orientYaw.ToString() + " HY: " + headYaw.ToString());
	
		// update l/r offsets and set it as 
		if (m_pInput.Camera3rdIsRightShoulder())
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, 1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}
		else
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, -1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}

		float 	shoulderDist = m_fCameraLRShoulder * m_fShoulderWidth;
		
		//! 
		vector rot;
		rot[0] = lrAngle;
		rot[1] = udAngle;
		rot[2] = m_fRoll;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);
		// Print(m_fLeftRightAngle);


		//! base bone pos 
		if (m_iBoneIndex != -1)
		{
			pOutResult.m_CameraTM[3]	= m_pPlayer.GetBonePositionMS(m_iBoneIndex);
		}
		else
		{
			pOutResult.m_CameraTM[3]	= "0 0 0";
		}

		//! apply shoulder dist
		vector msOffset = m_CameraOffsetMS;
		vector lsOffset = m_CameraOffsetLS;
		
		lsOffset[0] = lsOffset[0] + shoulderDist;

		// ls offset + ms offset + shoulder width			
		pOutResult.m_CameraTM[3]	= pOutResult.m_CameraTM[3] + pOutResult.m_CameraTM[0] * lsOffset[0] + pOutResult.m_CameraTM[1] * lsOffset[1] + pOutResult.m_CameraTM[2] * lsOffset[2] + msOffset;

		float 	lookAtLR 	= UpdateLRAngleLookAt(-180, 180, pDt);

		rot[0] = lookAtLR;
		rot[1] = udAngle;
		rot[2] = m_fRoll;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);

		//! store distance 
		pOutResult.m_fDistance 		= m_fDistance;
		pOutResult.m_fUseHeading 	= 1.0;
		pOutResult.m_fInsideCamera 	= 0.0;
		pOutResult.m_fPositionModelSpace = 1.0;
		pOutResult.m_bUpdateWhenBlendOut	= false;
		
		super.OnUpdate(pDt, pOutResult);
	}
}

// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProne - 3rd person prone
// *************************************************************************************
class DayZPlayerCamera3rdPersonProne extends DayZPlayerCamera3rdPersonProneBase
{
	void DayZPlayerCamera3rdPersonProne(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonProne");
		m_iBoneIndex		= pPlayer.GetBoneIndexByName ("Camera3rd_Helper");
		m_fDistance 		= 1.0;
		m_CameraOffsetMS	= "0.0 0.4 0.0";
		m_CameraOffsetLS	= "0.0 0.2 0.0";
		m_fShoulderWidth	= 0.5;
	}

}

// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProneRaised - 3rd person prone raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonProneRaised extends DayZPlayerCamera3rdPersonProneBase
{
	void DayZPlayerCamera3rdPersonProneRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Print("new camera: DayZPlayerCamera3rdPersonProneRaised");
		m_iBoneIndex		= pPlayer.GetBoneIndexByName ("Camera3rd_Helper");
		m_fDistance 		= 1.0;
		m_CameraOffsetMS	= "0.0 0.4 0.0";
		m_CameraOffsetLS	= "0.0 0.2 0.0";
		m_fShoulderWidth	= 0.5;
	}
}
