
//-----------------------------------------------------------------------------
class Input
{
	// input locking 
	
	/**
	\brief Change game focus number
	@param add number to add to focus number
	@param input_device if equals -1, works globally on all devices, see INPUT_DEVICE_* values in constants.h 
	@see HasGameFocus()
	*/
	proto native void		ChangeGameFocus(int add, int input_device = -1);
	
	/**
	\brief Reset game focus number (set to 0)
	@param input_device if equals -1, works globally on all devices, see INPUT_DEVICE_* values in constants.h 
	@see HasGameFocus()
	*/
	proto native void		ResetGameFocus(int input_device = -1);
	
	/**
	\brief Check if game should have focus
	@param input_device if equals -1, checks all devices, see INPUT_DEVICE_* values in constants.h 
	@return true if focus number == 0, false otherwise
	*/
	proto native bool		HasGameFocus(int input_device = -1);

	// actions
	proto native int		GetActionGroupsCount();
	proto native int		GetActionGroupSize(int group_index);
	proto int				GetActionGroupName(int group_index, out string name);
	proto native void		GetActionGroupItems(int group_index, out TIntArray items);
	proto int				GetActionDesc(int action_index, out string desc);
	proto native void		GetActionKeys(int action_index, out TIntArray keys);
	proto native void		GetActionDefaultKeys(int action_index, out TIntArray keys);
	proto native void		SetActionKeys(int action_index, TIntArray keys);
	proto int				GetActionKeyName(int key_index, out string name);
	proto native int		GetActionsCount();
	proto native void		GetSpecialKeys(out TIntArray special_keys);
	proto native int		IsKeyReserved(int dikCode);
	proto void				SeparateComboAndKey(int dik, out int dikCombo, out int dikKey);
	proto native int		IsActionGroupCollision(int actionIndexA, int actionIndexB);
	int						CheckKeyCollision(int actionIndex, int key)
	{
		ref TIntArray keys = new TIntArray;
		int basicKey = 0;
		int sepraratedKey = 0;
		SeparateComboAndKey(key, sepraratedKey, basicKey);
	
		bool is_combo;
		if (sepraratedKey != 0)
		{
			is_combo = true;
		}
		else
		{
			is_combo = false;
		}
	
		for (int ai = 0; ai < GetActionsCount(); ai++)
		{
			if (ai == actionIndex) continue;
			if (IsActionGroupCollision(ai, actionIndex) == 0) continue;
	
			GetActionKeys(ai, keys);
	
			for (int ki = 0; ki < keys.Count(); ki++)
			{
				int otherKey = keys.Get(ki);
				if (key == otherKey)
				{
					return ai;
				}
	
				int otherBasicKey = 0;
				int otherSepraratedKey = 0;
				bool other_is_combo;
	
				SeparateComboAndKey(otherKey, otherSepraratedKey, otherBasicKey);
	
				if (otherSepraratedKey != 0)
				{
					other_is_combo = true;
				}
				else
				{
					other_is_combo = false;
				}
	
				if (is_combo != other_is_combo)
				{
					if (otherBasicKey == sepraratedKey || otherSepraratedKey == basicKey)
					{
						return ai;
					}
				}
			}
		}
	
		return -1;
	}

	// getting action state
	/**
	\brief Get action state
	@param action id of action, defined in \ref 4_World/Classes/UserActionsComponent/_constants.c
	@param check_focus if true and game is unfocused, returns 0; otherwise returns actual value
	@return actual action state as float, for regular two state buttons returns 0 or 1, for analog buttons/axes returns value in interval <0, 1> with consideration of defined deadzones
	*/
	proto native float	GetAction(int action, bool check_focus = true);
		
	/**  
	\brief Returns true just in frame, when action was invoked (button was pressed)
	@param action id of action, defined in \ref 4_World/Classes/UserActionsComponent/_constants.c
	@param check_focus if true and game is unfocused, returns 0; otherwise returns actual value
	@return true if action was invoked in that frame, false otherwise
	*/
	proto native bool	GetActionDown(int action, bool check_focus = true);
	
	/**  
	\brief Returns true just in frame, when action was released (button was releases)
	@param action id of action, defined in \ref 4_World/Classes/UserActionsComponent/_constants.c
	@param check_focus if true and game is unfocused, returns 0; otherwise returns actual value
	@return true if action was released in that frame, false otherwise
	@see GetActionDown()
	*/
	proto native bool	GetActionUp(int action, bool check_focus = true);

	proto native void	BlockInput(int key);
	
	/**  
	\brief Disable key until end of frame
	@param key id of key, defined in \ref KeyCode
	\code
	GetGame().GetInput().DisableKey(KeyCode.KC_RETURN);
	\endcode
	*/
	proto native void	DisableKey(int key);
	
	/**  
	\brief Disable action until end of frame
	@param action id of action, defined in \ref 4_World/Classes/UserActionsComponent/_constants.c
	*/
	proto native void	DisableAction(int action);
	
	proto native int	GetSpecialKeyPressed(); // special key - gamepad, joystick, etc

	//! gets currently selected profile
	proto native int	GetCurrentProfile();
	// gets currently selected profile keys for action
	proto native void	GetCurrentProfileActionKeys(int action_index, out TIntArray keys);
	//! gets profile by index
	proto int			GetProfileName(int profile_index, out string name);
	//! gets profile by name
	proto native int	GetProfilesCount();
	//! setting active profile
	proto native int	SetProfile(int index);


	// devices
	proto native int		GetDevicesCount();
	proto int				GetDeviceName(int device_index, out string name);
	proto native int		IsDeviceXInput(int device_index);
	proto native int		IsDeviceEnabled(int device_index);
	proto native void		SetDeviceEnabled(int device_index, bool enabled);
	proto native int		IsCustomSchemeEnabled();
	proto native void		SetCustomSchemeEnabled(bool enabled);
	
	//! return true if was deflected button.
	proto bool				GetGamepadThumbDirection(GamepadButton thumbButton, out float angle, out float value);
};

