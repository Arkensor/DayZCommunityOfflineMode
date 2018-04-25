//
// Keep this Object up-to-date with KeyCodes Enum in EnSystem.c
//
// KeysToUIElements is mapping of KeyCodes to its literal name or icon from ImageSet
// that will be displayed in floating widget for Default Actions - aka Action Target Selection

typedef Param2<string, bool> KeyToUIElement;

class KeysToUIElements
{
	static private ref map<int, ref KeyToUIElement> m_KeysToUIElements;
	
	static private void	RegisterKeyToUIElement(int key_code, string name, bool is_image_set )
	{
		if ( m_KeysToUIElements == NULL )
		{
			m_KeysToUIElements = new map<int, ref KeyToUIElement>;
		}
		
		if ( m_KeysToUIElements.Contains(key_code) )
		{
			Debug.Log("Template ID: "+string.ToString(key_code)+" is alredy exist!", "KeysToUIElements -> OnInit()", "System", "KeysToUIElements Registration", "None");
		}
		else
		{
			KeyToUIElement params = new KeyToUIElement(name, is_image_set);
			m_KeysToUIElements.Set(key_code, params);
		}
	}

	static KeyToUIElement GetKeyToUIElement(int key_code)
	{
		if ( m_KeysToUIElements && m_KeysToUIElements.Contains(key_code) )
		{
			return m_KeysToUIElements.Get(key_code);
		}
		
		Debug.Log("Template ID: "+string.ToString(key_code)+" does not exist!", "KeysToUIElements -> GetKeysToUIElement()", "System", "Get KeysToUIElements", "None");
		return NULL;
	}

	static void Init()
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////// Register KeysToUIElements ///////////////////////////////////////////////////////////////////////
		//					  | KeyCode					| Name									| is_image_set  //
		RegisterKeyToUIElement( KeyCode.KC_ESCAPE		, "ESC"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_1			, "1"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_2			, "2"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_3			, "3"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_4			, "4"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_5			, "5"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_6			, "6"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_7			, "7"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_8			, "8"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_9			, "9"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_0			, "0"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_MINUS		, "-"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_EQUALS		, "="									, false			);
		RegisterKeyToUIElement( KeyCode.KC_BACK			, "BKSPC"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_TAB			, "TAB"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_Q			, "Q"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_W			, "W"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_E			, "E"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_R			, "R"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_T			, "T"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_Y			, "Y"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_U			, "U"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_I			, "I"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_O			, "O"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_P			, "P"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_LBRACKET		, "("									, false			);
		RegisterKeyToUIElement( KeyCode.KC_RBRACKET		, ")"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_RETURN		, "ENTER"								, false			);
		RegisterKeyToUIElement(	KeyCode.KC_LCONTROL		, "LCTRL"								, false			);
		RegisterKeyToUIElement(	KeyCode.KC_A			, "A"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_S			, "S"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_D			, "D"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_F			, "F"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_G			, "G"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_H			, "H"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_J			, "J"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_K			, "K"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_L			, "L"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_SEMICOLON	, ";"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_APOSTROPHE	, "'"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_GRAVE		, "`"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_LSHIFT		, "LSHIFT"								, false			);
		RegisterKeyToUIElement(	KeyCode.KC_BACKSLASH	, "\\"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_Z			, "Z"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_X			, "X"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_C			, "C"									, false			);
		RegisterKeyToUIElement(	KeyCode.KC_V			, "V"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_B			, "B"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_N			, "N"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_M			, "M"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_COMMA		, ","									, false			);
		RegisterKeyToUIElement( KeyCode.KC_PERIOD		, "."									, false			);
		RegisterKeyToUIElement( KeyCode.KC_SLASH		, "/"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_RSHIFT		, "RSHIFT"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_MULTIPLY		, "*"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_LMENU		, "LALT"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_SPACE		, "SPACE"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_CAPITAL		, "CPSLOCK"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_F1			, "F1"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F2			, "F2"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F3			, "F3"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F4			, "F4"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F5			, "F5"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F6			, "F6"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F7			, "F7"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F8			, "F8"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F9			, "F9"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F10			, "F10"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMLOCK		, "NMLOCK"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_SCROLL		, "SCRLOCK"								, false			); 
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD7		, "Num7"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD8		, "Num8"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD9		, "Num9"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_SUBTRACT		, "Num-"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD4		, "Num4"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD5		, "Num5"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD6		, "Num6"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_ADD			, "Num+"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD1		, "Num1"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD2		, "Num2"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD3		, "Num3"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPAD0		, "Num0"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_DECIMAL		, "Num."								, false			);
		//RegisterKeyToUIElement( KeyCode.KC_OEM_102,    ///< < > | on UK/Germany keyboards 
		RegisterKeyToUIElement( KeyCode.KC_F11			, "F11"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_F12			, "F12"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPADEQUALS	, "Num="								, false			);
		RegisterKeyToUIElement( KeyCode.KC_PREVTRACK	, "PrevTrack"							, false			);
		//RegisterKeyToUIElement( KeyCode.KC_AT,    ///<                     (NEC PC98) 
		RegisterKeyToUIElement( KeyCode.KC_COLON		, ":"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_UNDERLINE	, "_"									, false			);
		//RegisterKeyToUIElement( KeyCode.KC_STOP,    ///<                     (NEC PC98) 
		//RegisterKeyToUIElement( KeyCode.KC_AX,    ///<                     (Japan AX) 
		//RegisterKeyToUIElement( KeyCode.KC_UNLABELED,    ///<                        (J3100) 
		RegisterKeyToUIElement( KeyCode.KC_NEXTTRACK	, "NextTrack"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPADENTER	, "NumEnter"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_RCONTROL		, "RCTRL"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_MUTE			, "Mute"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_CALCULATOR	, "Calc"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_PLAYPAUSE	, "Play"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_MEDIASTOP	, "Stop"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_VOLUMEDOWN	, "Vol-"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_VOLUMEUP		, "Vol+"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_WEBHOME		, "Web Home"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_NUMPADCOMMA	, "Num,"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_DIVIDE		, "Num/"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_SYSRQ		, "SysRq"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_RMENU		, "RALT"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_PAUSE		, "Pause"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_HOME			, "Home"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_UP			, "ArrowUp"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_PRIOR		, "PageUp"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_LEFT			, "ArrowLeft"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_RIGHT		, "ArrowRight"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_END			, "End"									, false			);
		RegisterKeyToUIElement( KeyCode.KC_DOWN			, "ArrowDown"							, false			);
		RegisterKeyToUIElement( KeyCode.KC_NEXT			, "PgDn"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_INSERT		, "Insert"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_DELETE		, "Delete"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_LWIN			, "LWIN"								, false			);
		RegisterKeyToUIElement( KeyCode.KC_RWIN			, "RWIN"							    , false			);
		//RegisterKeyToUIElement( KeyCode.KC_APPS,    ///< AppMenu key 
		//RegisterKeyToUIElement( KeyCode.KC_POWER,    ///< System Power 
		//RegisterKeyToUIElement( KeyCode.KC_SLEEP,    ///< System Sleep 
		//RegisterKeyToUIElement( KeyCode.KC_WAKE,    ///< System Wake 
		//RegisterKeyToUIElement( KeyCode.KC_MEDIASELECT   ///< Media Selection
		// Mouse buttons
		RegisterKeyToUIElement( 1048576					, "set:dayz_gui image:left_mouse"		, true			);
		RegisterKeyToUIElement( 1048577					, "set:dayz_gui image:right_mouse"		, true			);
		RegisterKeyToUIElement( 1048578			 		, "set:dayz_gui image:middle_mouse"		, true			);
	}
}