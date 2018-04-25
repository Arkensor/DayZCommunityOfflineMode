
// *************************************************************************************
// ! HumanItemBehaviorCfg - per item config of stances / movements   
// *************************************************************************************
class HumanItemBehaviorCfg
{

	// static int	DEFMOVEMENT				= MOVEMENT_IDLE | MOVEMENT_WALK | MOVEMENT_RUN | SPRINT;

	//! bit mask of ik settings 
	static int IKSETTING_AIMING = 0x1;
	static int IKSETTING_RHAND 	= 0x2;
	static int IKSETTING_LHAND  = 0x4;	

	int		m_iStanceMask;					//!< combinations of STANCEMASK_
	int		m_StanceMovements[6];			//! 6 stances -> all has movement mask, STANCEIDX_ ... is index
	int 	m_IKSettings[24];				//! [stance][movement] mask for ik 
	int 	m_iPerItemCameraUserData;		//! per item camera user data - can be obtained in runtime by DayZPlayer.GetPerItemCameraUD()

	//! movement caps
	float 	m_fMoveHeadingFilterSpan;		//! delay of alignment when moving
	float 	m_fMoveHeadingFilterSpeed;		//! max speed of alignment when moving

	//! evade caps
	float 	m_fMeleeEvadeHeadingFilterSpan;		//! delay of alignment when evading
	float 	m_fMeleeEvadeHeadingFilterSpeed;	//! max speed of alignment when evading


	//! per weapon lean forward 
	bool	m_bAttackLean;					//! default false

	

	void 	SetIK(int pStance, int pMovement, bool pAim, bool pRArm, bool pLArm)
	{
		int val = 0;
		if (pAim)	val |=  IKSETTING_AIMING;
		if (pRArm)	val |=  IKSETTING_RHAND;
		if (pLArm)	val |=  IKSETTING_LHAND;

		// Print("Setting ik " + pStance.ToString() + "," +   pMovement.ToString() );
		pStance = pStance*4;
		pStance = pStance + pMovement;

		// Print("Setting ik " + pStance.ToString() + " to value " +   val.ToString() );
		m_IKSettings[pStance] = val;
	}

	void 	SetIKStance(int pStance, bool pAim, bool pRArm, bool pLArm)
	{
		int val = 0;
		if (pAim)	val |=  IKSETTING_AIMING;
		if (pRArm)	val |=  IKSETTING_RHAND;
		if (pLArm)	val |=  IKSETTING_LHAND;

		int bo = pStance * 4;

		for (int i = 0; i < 4; i++)
		{
			m_IKSettings[bo+i] = val;
		}
	}

	void 	SetIKAll(bool pAim, bool pRArm, bool pLArm)
	{
		int val = 0;
		if (pAim)	val |=  IKSETTING_AIMING;
		if (pRArm)	val |=  IKSETTING_RHAND;
		if (pLArm)	val |=  IKSETTING_LHAND;

		for (int i = 0; i < 24; i++)
		{
			m_IKSettings[i] = val;
		}
	}

	
}



// *************************************************************************************
// ! HumanItemAccessor - item helpers
// *************************************************************************************
class HumanItemAccessor
{
	//! signalization from script to engine that item in hands changed
	proto native void 	OnItemInHandsChanged();

	//! returns true if item in hands is a weapon
	proto native bool	IsItemInHandsWeapon();

	//! returns true if weapon is item and it has camera point - local pos, dir in weapon space
	proto native bool 	WeaponGetCameraPoint(out vector pPos, out vector pRot);

	//! returns true if weapon is item and it has camera point - model space matrix - 
	proto native bool 	WeaponGetCameraPointMSTransform (vector pCamPoint, vector pCamDir, out vector pTm[4]);

	//! returns true if weapon is item and it has camera point - model space matrix - 
	proto native bool 	WeaponGetCameraPointBoneRelative(vector pCamPoint, vector pCamDir, int pBoneIndex, out vector pTm[4]);

	//! return current aiming point from aiming model (additive swaying applied - no recoil points)
	proto native bool 	WeaponGetAimingModelDirTm(out vector pTm[4]);


	private void HumanItemAccessor() 
	{
	}

	private void ~HumanItemAccessor() 
	{
	}
}
