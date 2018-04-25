#ifdef GAME_TEMPLATE

/**
* \defgroup VRDeviceAPI generic and platform specific devices
* @{
*/

enum VRDeviceType
{
	DEVICE_TYPE_OCULUS,
	DEVICE_TYPE_PS4
}

enum VRStatus
{
	VR_STATUS_UNKNOWN,					//< Unknown state, probably uninitialized.
	VR_STATUS_VISIBLE,					//< The HMD is being used for rendering.
	VR_STATUS_PRESENT,					//< The HMD port is open.
	VR_STATUS_MOUNTED,					//<	The HMD is mounted on users head.
	VR_STATUS_DISPLAY_LOST,			//< The HMD was present and disappeared.
	VR_STATUS_SHOULD_QUIT,			//< Application requested exit.
	VR_STATUS_SHOULD_RECENTER,	//< The HMD Recenter request event was triggered. 
	VR_STATUS_TRACKED,					//< The tracking data for the HMD are up to date.
	VR_STATUS_CALIBRATING,			//< The HMD is being calibrated.
}

enum VRHandEnum
{
	VR_HAND_LEFT, //< HMD controller left hand.
	VR_HAND_RIGHT //< HMD controller right hand.
}

enum VREye
{
	VR_EYE_LEFT, // HMD left eye.
	VR_EYE_RIGHT // HMD right eye.
}

/*!
	VRDevice interface common for all VR implementations.

	DO NOT INHERIT FROM THIS CLASS!
*/
class VRDevice : Managed
{
	/*!
		\brief returns VRDevice instance.
		\return device instance.
	*/
	proto native static VRDevice GetInstance();

	/*!
		\brief returns status flags.

		\return current device state flags.
	*/
	proto native VRStatus 				GetStatusFlags();
	/*!
		\brief returns device name.
		\return device brand name.
	*/
	proto native owned string	GetName();
	/*!
		\brief returns device type. 
		\return device type enum.
	*/
	proto native VRDeviceType GetDeviceType();
	/*!
		\brief Forces immediate head recenter.

		Uses the current scene position and orientation
		as the new camera origin.
	*/
	proto native void 								RecenterHeadTracking();
	/*!
		\brief returns HMD head position .
		\return position in world space.
	*/
	proto native vector 						GetHeadPosition();
	/*!
		\brief returns HMD head orientation. 
		\return Euler angles in device space. (Yaw, Pitch, Roll)
	*/
	proto native vector 						GetHeadOrientation();
	/*!
		\brief returns HMD controller's position.

		\param hand left/right hand enum.
		\return position in world space.
	*/
	proto native vector 						GetHandPosition(VRHand hand);
	/*!
		\brief returns HMD controller's orientation.

		\param hand left/right hand enum.
		\return Euler angles in device space. (Yaw, Pitch, Roll)
	*/
	proto native vector 						GetHandOrientation(VRHand hand);
	/*!
		\brief returns HMD Eye's position.

		\param eye left/right eye enum.
		\return position in world space.
	*/
	proto native vector 						GetEyePosition(VREye eye);
	/*!
		\brief returns HMD Eye's orientation.
		
		\param eye left/right eye enum.
		\return Euler angles in device space. (Yaw, Pitch, Roll)
	*/
	proto native vector							GetEyeOrientation(VREye eye);
	/*!
		\brief Sets up current vr scale.
		
		All translations will get scaled by this value.
		With higher scale values will the world around look smaller.

		"Warning: The tracking errors will render more visible with high scale."
		\param scale the scale amount.
	*/
	proto native void 								SetScale(float scale);
	/*!
		\brief returns current vr scale.
		\return the scale amount. 
	*/
	proto native float 							GetScale();

	/*!
		\brief returns current FOV's tangent values.

		In VR mode the FOV should be asymmetric.
		\returns tangent values of asymetric FOV.
	*/
	proto external void 						GetEyeFOV(VREye eye, out float leftTan, out float rightTan, out float upTan, out float downTan);
};

enum PSVrHeadRecenterFlagsEnum
{
	VR_RECENTER_POSITION,
	VR_RECENTER_ORIENTATION
}

enum PSVrBrightnessRiskEnum
{
	VR_BRIGHTNESS_RISK_LOW,
	VR_BRIGHTNESS_RISK_HIGH,
	VR_BRIGHTNESS_RISK_MAX
}

enum PSVrTrackingQualityEnum
{
	VR_TRACKING_QUALITY_NONE,
	VR_TRACKING_QUALITY_NOT_VISIBLE,
	VR_TRACKING_QUALITY_PARTIAL,
	VR_TRACKING_QUALITY_FULL
}

enum PSVrDialogStatusEnum
{
	VR_DIALOG_UNKNOWN,
	VR_DIALOG_OK,
	VR_DIALOG_CANCELED,
	VR_DIALOG_RUNNING	
}

/*!
	PS4 specific VR interface.
*/
class VRDevice_PS4 : VRDevice
{
	/*!
		\brief Switches VR device into 2D "theater" mode.

		Don't forget to set appropriate FOV.

		At the moment the device uses the render targets of the
		left eye. These render targets are 8:9 and we are still rendering
		both eyes. If the feature gets used in a actual gameplay we can
		extend it to separate set of 16:9 render targets.

		\param enabled enable/disable flag.
	*/
	proto native void Set2D(bool enabled);
	/*!
		\brief Pops up HMD service dialog.
		\return true when dialog was opened and his state was resolved as OK.
	*/
	proto native bool ShowHmdServiceDialog();
	/*!
		\brief Pops up HMD setup dialog.
		\return true when dialog was opened and closed successfully.
	*/
	proto native bool ShowHmdSetupDialog();
	/*!
		\brief Returns current setup dialog state.
		\return dialog state enum.
	*/
	proto native PSVrDialogStatusEnum GetSetupDialogStatus();
	/*!
		\brief Returns current service dialog state.
		\return dialog state enum.
	*/
	proto native PSVrDialogStatusEnum GetServiceDialogStatus();

	/*!
		\brief HMD recentering hint flags setup.
	*/
	proto native void SetRecenterHeadFlags(PSVrHeadRecenterFlagsEnum flags);
	/*!
		\brief Returns HMD recentering flags.
		\return recentering enum.
	*/
	proto native PSVrHeadRecenterFlagsEnum GetRecenterHeadFlags();
	/*!
		\brief Returns the eye offset in device space.
		\return 3D position centered around device space center.
	*/
	proto native vector GetEyeOffset(VREye eye);
	/*!
		\brief Sets up the minimal displayable color.

		Setup this value when smears start appearing in 
		dark environments.
	*/
	proto native void SetMinOutputColor(int color);
	/*!
		\brief Per eye FOV setup.

		Keep in mind that changing the values from device presets
		may cause user discomfort.
	*/
	proto native void SetEyeFOV(VREye eye, float leftTan, float rightTan, float upTan, float downTan);
	/*!
		\brief Resets the FOV settings to device preset value.
	*/
	proto native void ResetToDeviceFOV(VREye eye);
	/*!
		\brief Sets up the render target size multiplier.

		Set this value before the device gets initialized.
	*/
	proto native void SetOversamplingFactor(float factor);
	/*!
		\brief Returns HMD tracking status.
		\param bRisk tracked scene brightness risk.
		\param posQuality precision of the HMD position.
		\param orQuality precision of tthe HMD orientation.
		\return false when the HMD port is not open.
	*/
	proto bool GetHmdTrackingStatus(	out PSVrBrightnessRiskEnum bRisk, 
																		out PSVrTrackingQualityEnum posQuality, 
																		out PSVrTrackingQualityEnum orQuality);
	/*!
		\brief Returns Move controller tracking status.

		\param hand identifier of the contoller.
		\param bRisk tracked scene brightness risk.
		\param posQuality precision of the controllers position.
		\param orQuality precision of the controllers orientation.
		\return false when the Move port is not open.
	*/
	proto bool GetMoveTrackingStatus(	VRHand hand, 
																						out PSVrBrightnessRiskEnum bRisk, 
																						out PSVrTrackingQualityEnum posQuality, 
																						out PSVrTrackingQualityEnum orQuality);
}				

/*!
	Oculus specific VR interface.
*/
class Oculus : VRDevice
{
	// TODO:
}

/*
* @}
*/

#endif