class SHumanGlobalSettings
{
	float		m_fPhysMoveMaxTracedDistance;		//!<	when actual phys step is lower than this -> distance is traced, otherwise it's just set 
	
}


class SHumanCommandMoveSettings 
{
	float 		m_fRunSpringTimeout;		//!< filter span value		[s]
	float		m_fRunSpringMaxChange;		//!< filter speed value		[val/s]
	float		m_fDirFilterTimeout;		//!< direction filter timeout [s]
	float		m_fDirFilterSpeed;			//!< direction filter max rot speed [rad/s]
	float		m_fMaxSprintAngle;			//!< max sprint angle [rad]

	float 		m_fTurnAngle;					//!< when character starts to turn (45 deg default)
	float 		m_fTurnEndUpdateNTime;			//!< when turn values are not updated anymore (normalized time)
	float 		m_fTurnNextNTime;				//!< when next turn is started from previous turn

	float		m_fSlidingPoseAngle;		//!< angle (0.707 * PI)	- when angle is bigger - sliding pose is done (rad)
	float		m_fSlidingPoseTrackTime;	//!< time change of angle is tracked in the past (sec)
	float		m_fSlidingPoseRepTime;		//!< not to do sliding pose after another sliding pose (sec)

	float		m_fHeadingChangeLimiterIdle;
	float		m_fHeadingChangeLimiterWalk;
	float		m_fHeadingChangeLimiterRun;
}

class SHumanCommandSwimSettings 
{		
	float 		m_fAlignIdleTimeout;				//! align filters in idle, slow, fast
	float 		m_fAlignIdleMaxChanged;	
	float 		m_fAlignSlowTimeout;		
	float 		m_fAlignSlowMaxChanged;	
	float 		m_fAlignFastTimeout;		
	float 		m_fAlignFastMaxChanged;	

	float		m_fMovementSpeed;			
	float		m_fMovementSpeedFltTime;
	float		m_fMovementSpeedFltMaxChange;

	float		m_fWaterLevelSwim;					//!< when swimming - entity position depth (1.4 m)
	float		m_fWaterLevelIn;					//!< when entering water - what level cases swimming (1.5m)
	float		m_fWaterLevelOut;					//!< what water level causes to stop swimming (1.2m)


	float 		m_fToCrouchLevel;					//!< when to crouch
	float 		m_fToErectLevel;					//!< when to stand
}