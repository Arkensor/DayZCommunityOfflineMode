/* 
DayZPlayerImplement

this file is implemenation of dayzPlayer in script 
- logic of movement 
- camera switching logic

*/

class DayZPlayerImplementHeading
{

	//-------------------------------------------------------------
	//!
	//! This HeadingModel - Clamps heading
	//! 

	//!
	static bool	ClampHeading (float pDt, SDayZPlayerHeadingModel pModel, out float pLastHeadingDiff)
	{
		float		aDiff = pModel.m_fHeadingAngle - pModel.m_fOrientationAngle;
		if (aDiff < -Math.PI)
		{
			aDiff += Math.PI2;
		}
		else if (aDiff > Math.PI)
		{
			aDiff -= Math.PI2;
		}

		// Print("Heading model: or: " + pModel.m_fOrientationAngle.ToString() + " head:" + pModel.m_fHeadingAngle.ToString() + " dif:" + aDiff.ToString());

		if (pLastHeadingDiff < -Math.PI_HALF && aDiff > 0)
		{
			aDiff					= -Math.PI + 0.01;
			pLastHeadingDiff		= aDiff;
			pModel.m_fHeadingAngle 	= pModel.m_fOrientationAngle + aDiff;
		
        	// Print("-APA- : or: " + pModel.m_fOrientationAngle.ToString() + " head:" + pModel.m_fHeadingAngle.ToString() + " dif:" + aDiff.ToString());

			return true;		// modify heading
		}
		else if (pLastHeadingDiff > Math.PI_HALF && aDiff < 0)
		{
			aDiff 					= Math.PI - 0.01;
			pLastHeadingDiff		= aDiff;
			pModel.m_fHeadingAngle 	= pModel.m_fOrientationAngle + aDiff;

        	// Print("-APA- : or: " + pModel.m_fOrientationAngle.ToString() + " head:" + pModel.m_fHeadingAngle.ToString() + " dif:" + aDiff.ToString());

			return true;		// modify heading
		}

		pLastHeadingDiff	= aDiff;
		// Print("Heading model diff " + aDiff.ToString());
		return false;
	}

	//-------------------------------------------------------------
	//!
	//! This HeadingModel - Rotates orientations - so player slides 
	//! 

	static float 	CONST_ROTLIMIT = Math.PI * 0.95;

	//!
	static bool	RotateOrient (float pDt, SDayZPlayerHeadingModel pModel, out float pLastHeadingDiff)
	{
		float		aDiff = pModel.m_fHeadingAngle - pModel.m_fOrientationAngle;

		if (aDiff < -Math.PI)
		{
			aDiff += Math.PI2;
		}
		else if (aDiff > Math.PI)
		{
			aDiff -= Math.PI2;
		}

		// Print("Heading model: or: " + pModel.m_fOrientationAngle.ToString() + " head:" + pModel.m_fHeadingAngle.ToString() + " dif:" + aDiff.ToString());

		if (pLastHeadingDiff < -Math.PI_HALF && aDiff > 0)
		{
			aDiff -= Math.PI2;
		}
		else if (pLastHeadingDiff > Math.PI_HALF && aDiff < 0)
		{
			aDiff += Math.PI2;
		}

		pLastHeadingDiff	= aDiff;		

		if (aDiff < -CONST_ROTLIMIT)
		{
			pModel.m_fOrientationAngle += aDiff +  CONST_ROTLIMIT;
			return true;
		}
		else if (aDiff > CONST_ROTLIMIT)
		{
			pModel.m_fOrientationAngle += aDiff - CONST_ROTLIMIT;
			return true;
		}

		// Print("Heading model diff " + aDiff.ToString());
		return false;

	}



}

