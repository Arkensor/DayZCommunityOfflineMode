#ifdef GAME_TEMPLATE

[EditorAttribute("box", "GameLib/Scripted", "Script camera", "-0.25 -0.25 -0.25", "0.25 0.25 0.25", "255 0 0 255")]
class ScriptCameraClass
{

}

ScriptCameraClass ScriptCameraSource;

class ScriptCamera: GenericEntity
{
	[Attribute("60", "slider", "Field of view", "0 180 1")]
	float FOV;
	[Attribute("1", "editbox", "Near plane clip")]
	float NearPlane;
	[Attribute("4000", "editbox", "Far plane clip")]
	float FarPlane;
	
	[Attribute("1", "combobox", "Projection type", "", ParamEnumArray.FromEnum(CameraType) )]
	int Type;
	[Attribute("5", "slider", "Camera speed", "0 20 1")]
	float Speed;
	[Attribute("1", "combobox", "Free Fly", "", ParamEnumArray.FromEnum(EBool) )]
	bool FreeFly;
	[Attribute("0", "combobox", "Invert vertical", "", ParamEnumArray.FromEnum(EBool) )]
	bool Inverted;
	[Attribute("0", "slider", "Camera index", "0 31 1")]
	int Index;
	float m_MouseSensitivity = 0.001; // should be somewhere else.
	float m_GamepadSensitivity = 0.2; // should be somewhere else.
	int m_GamepadFreeFly;

	// debug variables
	int m_DbgListSelection = 0;
	ref array<string> m_DbgOptions = {"Perspective", "Orthographic"};

	void ScriptCamera(IEntitySource src, IEntity parent)
	{
		SetFlags(EntityFlags.ACTIVE, false);
		SetEventMask(EntityEvent.FRAME);

		SetCameraVerticalFOV(Index, FOV);
		SetCameraFarPlane(Index, FarPlane);
		SetCameraNearPlane(Index, NearPlane);
		SetCameraType(Index, Type);
		m_DbgListSelection = Type - 1;
		SetCamera(Index, GetOrigin(), GetYawPitchRoll());

		vector camMat[4];
		GetTransform(camMat);
		SetCameraEx(Index, camMat);
		m_GamepadFreeFly = FreeFly;
	}

	override private void EOnFrame(IEntity other, float timeSlice) //EntityEvent.FRAME
	{
		GetGame().GetInputManager().ActivateContext("ScriptCameraContext");
		
		if (GetGame().GetInputManager().GetActionTriggered("CamFreeFly"))
		{
			FreeFly = !FreeFly;
		}
		
		if (FreeFly)
		{
			FreeFly(timeSlice);
		}
		else
		{
			vector camMat[4]; // matrix can be set outside the class
			GetTransform(camMat);
			SetCameraEx(Index, camMat);
		}
		
		if (GameSettings.Debug)
		{
			DebugInfo();
		}
	}

	protected void FreeFly(float timeSlice)
	{
		vector camPosition = GetOrigin();
		vector angles = GetYawPitchRoll();
		vector camMat[4];
		GetTransform(camMat);	
		InputManager imanager = GetGame().GetInputManager();
		imanager.ActivateContext("ScriptCameraFreeFlyContext");
		
		// get input
		float turnX 		= imanager.GetActionValue("CamTurnRight") * 20.0 * timeSlice;
		float turnY 		= imanager.GetActionValue("CamTurnUp") * 20.0 * timeSlice;
		float turnZ			= imanager.GetActionValue("CamRotate") * 20.0 * timeSlice;
		float moveForward	= imanager.GetActionValue("CamForward");
		float moveRight 	= imanager.GetActionValue("CamRight");
		float moveAscend 	= imanager.GetActionValue("CamAscend");
		float speedDelta   	= imanager.GetActionValue("CamSpeedDelta") * timeSlice;
		bool speedBoostHigh   	= imanager.GetActionTriggered("CamSpeedBoostHigh");
		bool speedBoostLow  = imanager.GetActionTriggered("CamSpeedBoostLow");

		Speed = Math.Clamp(Speed + speedDelta * Speed * 0.25, 0.1, 1000.0);
		
		float finalSpeed = Speed;
		if (speedBoostLow)
			finalSpeed *= 25;
		else if (speedBoostHigh)
			finalSpeed *= 5;

		// rotation
		angles[0] = turnX + angles[0];
		if (Inverted)
			angles[1] = turnY + angles[1];
		else
			angles[1] = -turnY + angles[1];

		angles[2] = turnZ + angles[2];
		
		// movement
		vector move = vector.Zero;
		vector forward = camMat[2];
		vector up = camMat[1];
		vector side = camMat[0];
		
		move += forward * moveForward;
		move += side    * moveRight;
		move += up      * moveAscend;
		
		// ------------		
		camPosition = (move * timeSlice * finalSpeed) + camPosition;
		
		Math3D.YawPitchRollMatrix(angles, camMat);
		camMat[3] = camPosition;
		SetTransform(camMat);
		SetCameraEx(Index, camMat);
	}

	protected void DebugInfo()
	{
		InputManager imanager = GetGame().GetInputManager();
		DbgUI.Begin(String("Camera #" + Index.ToString()), 0, Index * 300);

		DbgUI.Text(String("Position : " + GetOrigin().ToString()));
		DbgUI.Text(String("Orientation (Y, P, R): " + GetYawPitchRoll().ToString()));
		DbgUI.Text(String("Speed : " + Speed.ToString()));
		DbgUI.Text(String("Mouse sensitivity : " + (2000 - (1 / m_MouseSensitivity)).ToString()));
		DbgUI.Check("Select Free fly", FreeFly);
		DbgUI.List("Camera type", m_DbgListSelection, m_DbgOptions);
		if (m_DbgListSelection + 1 != Type)
		{
			Type = m_DbgListSelection + 1;
			SetCameraType(Index, Type);
		}
		
		float sensitivity = 2000 - (1 / m_MouseSensitivity);
		DbgUI.SliderFloat("Mouse sensitivity", sensitivity, 1, 1999);
		m_MouseSensitivity = 1 / (2000 - sensitivity);
		
		DbgUI.Text("CamTurnRight: " + imanager.GetActionValue("CamTurnRight"));
		DbgUI.Text("CamTurnUp: " + imanager.GetActionValue("CamTurnUp"));
		DbgUI.Text("CamSpeedDelta: " + imanager.GetActionValue("CamSpeedDelta"));
		DbgUI.Text("CamForward: " + imanager.GetActionValue("CamForward"));
		DbgUI.Text("CamRight: " +imanager.GetActionValue("CamRight"));
		DbgUI.Text("CamAscend: " + imanager.GetActionValue("CamAscend"));
		DbgUI.Text("CamSpeedBoostHigh: " + imanager.GetActionTriggered("CamSpeedBoostHigh"));
		DbgUI.Text("CamSpeedBoostLow:" + imanager.GetActionTriggered("CamSpeedBoostLow"));		
		
		DbgUI.End();
	}
}

#endif
