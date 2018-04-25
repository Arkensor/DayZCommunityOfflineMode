class Camera extends Entity
{
	/**
	\brief Returns active Camera instance (note: player's camera is not Camera instance - thus it return null)
	\return Camera \p active Camera instance
	*/
	static proto native Camera GetCurrentCamera();

	/**
	\brief Interpolation between camera instances (current camera becomes targetCamera at the end of interpolation)
	\param targetCamera \p to which camera we want to interpolate
	\param time \p true interpolation time
	\param type \p 
		0 = without dynamics
		1 = first half - acceleration, second half deceleration
		2 = first third - acceleration, second third - linear movement, last third - deceleration
	*/
	static proto native void InterpolateTo(Camera targetCamera, float time, int type);

	/**
	\brief Sets this camera as active
	*/
	proto native void SetActive(bool active);

	/**
	\brief Is this camera active?
	\return bool \p true if this camera is active
	*/
	proto native bool IsActive();
	
	/**
	\brief Field of View settings
	\param fov \p FOV angle in radians
	*/
	proto native void SetFOV(float fov);

	/**
	\brief Depth of Field settings
	\param distance \p DOF distance parameter
	\param blur \p DOF blur parameter
	*/
	proto native void SetFocus(float distance, float blur);

	/**
	\brief Orientation change by lookAt point
	\param targetPos \p where to look at
	*/
	proto native void LookAt(vector targetPos);		
};

//-----------------------------------------------------------------------------
class FreeDebugCamera extends Camera
{
	/**
	\brief Returns reference to FreeDebugCamera (it's singleton, thus only one instance is present)
	\return FreeDebugCamera \p reference to FreeDebugCamera instance
	*/
	static proto native FreeDebugCamera GetInstance();
	
	/**
	\brief Sets cameras freeze state
	\param freezed \p true = don't allow camera movement
	*/
	proto native void SetFreezed(bool freezed);

	/**
	\brief Returns if camera is freezed (you can't move it)
	\return bool \p true if camera is freezed
	*/
	proto native bool IsFreezed();
};
