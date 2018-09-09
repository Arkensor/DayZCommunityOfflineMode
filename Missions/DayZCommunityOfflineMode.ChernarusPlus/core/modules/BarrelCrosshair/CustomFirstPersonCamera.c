modded class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{	
	void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		float 	udAngle 	= UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
		m_CurrentCameraPitch = udAngle;
		m_fLeftRightAngle 	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		vector rot;
		rot[0] = m_fLeftRightAngle;
		rot[1] = udAngle;
		rot[2] = 0;
		
        vector rotationMat[];
        Math3D.MatrixIdentity4(rotationMat);
		Math3D.YawPitchRollMatrix(rot, rotationMat);

        vector positionMat[];
        Math3D.MatrixIdentity4(positionMat);
        positionMat[3] = Vector(0, 1, 0);

        Math3D.MatrixMultiply4(rotationMat, positionMat, pOutResult.m_CameraTM);

		pOutResult.m_iDirectBone 		= m_iBoneIndex;
		pOutResult.m_iDirectBoneMode 	= 1;
		pOutResult.m_fUseHeading 		= 1.0;
		pOutResult.m_fInsideCamera 		= 1.0;

		super.OnUpdate(pDt, pOutResult);
		
		pOutResult.m_fNearPlane = 0.005;

        Print("Is this even called LOL?");
	}
}