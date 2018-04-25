class ItemOptics extends InventoryItemSuper
{
	/**@fn		EnterOptics
	 * @brief	switches to optics mode if possible
	 * @return true if success, false otherwise
	 **/
	proto native bool EnterOptics ();
	/**@fn		IsInOptics
	 * @brief	is weapon in optics mode or not
	 * @return true if in optics mode, false otherwise
	 **/
	proto native bool IsInOptics ();
	/**@fn		ExitOptics
	 * @brief	switches out of optics mode (if possible)
	 * @return true if success, false otherwise
	 **/
	proto native bool ExitOptics ();

	/**@fn		GetStepFOVCount
	 * @brief	returns number of configured steps
	 **/
	proto native int GetStepFOVCount ();
	/**@fn		GetStepZoom
	 * @brief	returns position of currently used value in discreteFov config array
	 * @return	position of currently used value in discreteFov config array
	 **/
	proto native int GetStepFOVIndex ();
	/**@fn		SetStepZoom
	 * @brief sets zoom to fov value defined at given position in discreteFov config array
	 * @param[in] index of configured step, range [0..cfg_max]
	 * @return	true if zoom set
	 **/
	proto native bool SetStepFOVIndex (int step);
	/**@fn		StepZoomIn
	 * @brief sets zoom to next defined (respective to current) value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepFOVUp ();
	/**@fn		StepZoomOut
	 * @brief	sets zoom to previous (respective to current) defined value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepFOVDown ();

	/**@fn		GetCurrentStepFOV
	 * @brief	returns fov at current index, or _opticInfo._opticsZoomInit
	 **/
	proto native float GetCurrentStepFOV ();

	/**@fn		GetStepZeroing
	 * @brief	returns position of currently used value in zeroing config array
	 * @return	 position of currently used value in zeroing config array
	 **/
	proto native int GetStepZeroing ();
	/**@fn		SetZeroing
	 * @brief sets zeroing to value defined at given position in zeroing config array
	 * @param[in] index of zeroing to set [0..cfg_max]
	 * @return	true if zeroing set
	 **/
	proto native bool SetStepZeroing (int step);
	/**
	 * @fn		StepZeroingUp
	 * @brief sets zeroing to next defined (respective to current) value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingUp ();
	/**
	 * @fn		StepZeroingDown
	 * @brief	sets zeroing to previous (respective to current) defined value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingDown ();
	
	proto native void GetCameraPoint (out vector pos, out vector dir);
};	

typedef ItemOptics OpticBase;


