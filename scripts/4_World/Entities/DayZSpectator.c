class DayZSpectator : Camera
{
	private float m_SpeedMultiplier = 1.0;
	
	private float m_SendUpdateAcc = 0.0;
	
	void DayZSpectator()
	{
		SetEventMask(EntityEvent.FRAME);
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		Input input = GetGame().GetInput();
		
		if( input.GetActionDown(UACarShiftGearUp) )
			m_SpeedMultiplier = m_SpeedMultiplier + 2;
		if( input.GetActionDown(UACarShiftGearDown) )
			m_SpeedMultiplier = m_SpeedMultiplier - 2;
		
		float speed = 5.0 * m_SpeedMultiplier;
		if( input.GetAction(UATurbo) )
			speed *= 2;
		
		float forward = input.GetAction(UAMoveForward) - input.GetAction(UAMoveBack);
		float strafe = input.GetAction(UATurnRight) - input.GetAction(UATurnLeft);
		
		vector direction = GetDirection();
		vector directionAside = vector.Up * direction;
		
		vector oldPos = GetPosition();
		vector forwardChange = forward * timeSlice * direction * speed;
		vector strafeChange = strafe * timeSlice * directionAside * speed;
		vector newPos = oldPos + forwardChange + strafeChange;
		float yMin = GetGame().SurfaceRoadY(newPos[0], newPos[2]);
		if( newPos[1] < yMin )
			newPos[1] = yMin;
		
		SetPosition(newPos);
		
		float yawDiff = input.GetAction(UAAimHeadLeft) - input.GetAction(UAAimHeadRight);
		float pitchDiff = input.GetAction(UAAimHeadDown) - input.GetAction(UAAimHeadUp);
		vector oldOrient = GetOrientation();
		vector newOrient = oldOrient;
		newOrient[0] = newOrient[0] - Math.RAD2DEG * yawDiff * timeSlice;
		newOrient[1] = newOrient[1] - Math.RAD2DEG * pitchDiff * timeSlice;
		if( newOrient[1] < -89 )
			newOrient[1] = -89;
		if( newOrient[1] > 89 )
			newOrient[1] = 89;
		
		SetOrientation(newOrient);
		
		if( m_SendUpdateAcc > 0.5 )
		{
			GetGame().UpdateSpectatorPosition(newPos);
			m_SendUpdateAcc = 0;
		}
		
		m_SendUpdateAcc = m_SendUpdateAcc + timeSlice;
	}
};