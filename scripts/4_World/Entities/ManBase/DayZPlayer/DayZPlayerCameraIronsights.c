
//! this is main camera class 
class DayZPlayerCameraIronsights extends DayZPlayerCameraBase
{
	static const float 	CONST_UD_MIN	= -85.0;		//!< down limit
	static const float 	CONST_UD_MAX	= 85.0;			//!< up limit

	static const float 	CONST_LR_MIN	= -160.0;		//!< down limit
	static const float 	CONST_LR_MAX	= 160.0;			//!< up limit


	void 	DayZPlayerCameraIronsights(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_iBoneIndex = pPlayer.GetBoneIndexByName("RightHand_Dummy");
		if (m_iBoneIndex == -1)
		{
			Print("DayZPlayerCamera1stPerson: main bone not found");
		}

		HumanItemAccessor hia = m_pPlayer.GetItemAccessor();

		vector camPos, camDir;
		GetCurrentSightInfo(camPos, camDir);
		
		// get model space transform matrix of the gun's eye vector 
		hia.WeaponGetCameraPointBoneRelative(camPos, camDir, m_iBoneIndex, m_CachedWeaponOffset);
	}
	

	//
	override void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
			vector 	f = pPrevCamera.GetBaseAngles();
			m_fUpDownAngle		= f[0]; 
			m_fLeftRightAngle	= f[1]; 
			m_fUpDownAngleAdd	= f[2];
	}

	EntityAI GetCurrentSightEntity ()
	{
		EntityAI inHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
		if (inHands)
			return Weapon.Cast(inHands);
		return null;
	}

	bool GetCurrentSightInfo (out vector camPos, out vector camDir)
	{
		EntityAI e = GetCurrentSightEntity();
		if (e)
		{
			Weapon weapon = Weapon.Cast(e);
			weapon.GetCameraPoint(weapon.GetCurrentMuzzle(), camPos, camDir);
			return true;
		}
		return false;
	}

	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		float 	udAngle 	= UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
		m_fLeftRightAngle	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		//! get aiming model TM - direction 
		HumanItemAccessor hia = m_pPlayer.GetItemAccessor();
		hia.WeaponGetAimingModelDirTm(pOutResult.m_CameraTM);
		
	
		// pOutResult.m_CameraTM[0]		= m_CachedWeaponOffset[0];
		// pOutResult.m_CameraTM[1]		= m_CachedWeaponOffset[1];
		// pOutResult.m_CameraTM[2]		= m_CachedWeaponOffset[2];
		pOutResult.m_CameraTM[3]		= m_CachedWeaponOffset[3];

		pOutResult.m_iDirectBone 			= m_iBoneIndex;
		pOutResult.m_iDirectBoneMode 		= 1; // 4;		// keep position 

		pOutResult.m_fFovAbsolute 			= DZPLAYER_CAMERA_FOV_IRONSIGHTS;
	
		pOutResult.m_bUpdateWhenBlendOut	= false;
		pOutResult.m_fDistance 				= 0;		
		pOutResult.m_fUseHeading 			= 0;
		pOutResult.m_fInsideCamera 			= 1.0;
		pOutResult.m_fShootFromCamera		= 0.0;
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


	//! runtime values 
	protected	int 	m_iBoneIndex;				//!< right hand dummy bone index
	protected	vector 	m_CachedWeaponOffset[4];	//!< cached weapon offset
	protected 	float 	m_fUpDownAngle;				//!< up down angle in rad
	protected 	float 	m_fUpDownAngleAdd;			//!< up down angle in rad
	protected 	float 	m_fLeftRightAngle;			//!< left right angle in rad (in freelook only)

}

class DayZPlayerCameraOptics : DayZPlayerCameraIronsights
{
	override EntityAI GetCurrentSightEntity ()
	{
		EntityAI inHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
		if (inHands)
		{
			ItemOptics optics = ItemOptics.Cast(inHands);
			if (optics)
				return optics;
			else
			{
				Weapon weapon = Weapon.Cast(inHands);
				if (weapon)
				{
					optics = weapon.GetAttachedOptics();
					return optics;
				}
			}
		}
		return null;
	}

	override bool GetCurrentSightInfo (out vector camPos, out vector camDir)
	{
		EntityAI e = GetCurrentSightEntity();
		if (e)
		{
			ItemOptics optics = ItemOptics.Cast(e);
			optics.GetCameraPoint(camPos, camDir);
			return true;
		}
		return false;
	}

	override void OnUpdate (float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		ItemOptics optics = ItemOptics.Cast( GetCurrentSightEntity() );
		if (optics)
			pOutResult.m_fFovAbsolute = optics.GetCurrentStepFOV();
		else
			Error("optic camera, but there is no optic item or optic on weapon");
	}
};

