class KeyMouseBinding
{
    const int KB_EVENT_PRESS			= 0;
    const int KB_EVENT_RELEASE			= 1;
    const int KB_EVENT_HOLD				= 2;

    const int MB_EVENT_PRESS			= 0;
    const int MB_EVENT_CLICK			= 1;
    const int MB_EVENT_RELEASE			= 2;
    const int MB_EVENT_DOUBLECLICK		= 3;
    const int MB_EVENT_DRAG				= 4;
    const int MB_EVENT_HOLD 			= 5;

	protected typename m_Object;
	protected ref map<int, int> m_KeyBinds;
	protected ref map<int, int> m_MouseBinds;
	protected string m_strCallbackFunction;
	protected string m_strShortcut;
	protected string m_strDescription;
	protected bool canUseInMenu;
	
	void KeyMouseBinding( typename object, string callback, string shortcut, string description, bool menu = false ) 
	{
		m_Object = object;
		m_KeyBinds = new map< int, int >;
		m_MouseBinds = new map< int, int >;
		
		m_strCallbackFunction = callback;
		m_strShortcut = shortcut;
		m_strDescription = description;
		
		canUseInMenu = menu;
	}
	
	bool canUseInMenu() 
	{
		return canUseInMenu;
	}
	
	bool Check()
	{
		bool k_m_Pressed = true;
		for ( int kb = 0; kb < m_KeyBinds.Count(); ++kb ) 
		{
			int keyCode = m_KeyBinds.GetKey(kb);
			int keyEvent = m_KeyBinds.Get(keyCode);
			
			if ( keyEvent == KB_EVENT_RELEASE ) 
			{ // Skip checking for release keys
				continue; 
			}
			if ( KeyState( keyCode ) == 0 )
			{
				k_m_Pressed = false;
			}
		}
		
		for ( int mb = 0; mb < m_MouseBinds.Count(); ++mb ) 
		{
			int mouseButton = m_MouseBinds.GetKey(mb);
			int mouseEvent = m_MouseBinds.Get(mouseButton);
			
			if ( mouseEvent == MB_EVENT_RELEASE || mouseEvent == MB_EVENT_CLICK || mouseEvent == MB_EVENT_DOUBLECLICK || mouseButton == MouseState.WHEEL || mouseEvent == MB_EVENT_DRAG ) 
			{
				continue; // Skip checking for release buttons, click or double click, or mouse drag/wheel (handled else where)
			}
			if ( !(GetMouseState( mouseButton ) & MB_PRESSED_MASK ) )
			{
				k_m_Pressed = false;
			}
		}
		return k_m_Pressed;
	}
	
	bool IsRecurring() // Recurring if both mouse or keys are hold, drag OR wheel
	{
		bool recurring = true;
	
		for ( int kb = 0; kb < m_KeyBinds.Count(); ++kb ) 
		{
			int keyCode = m_KeyBinds.GetKey(kb);
			int keyEvent = m_KeyBinds.Get(keyCode);
			
			
			if ( keyEvent != KB_EVENT_HOLD ) 
			{
				return false; // a key is found that is not hold or drag. so it should not be recurring
			} 
		}
		
		for ( int mb = 0; mb < m_MouseBinds.Count(); ++mb ) 
		{
			int mouseButton = m_MouseBinds.GetKey(mb);
			int mouseEvent = m_MouseBinds.Get(mouseButton);
			
			if ( mouseEvent != MB_EVENT_DRAG && mouseEvent != MB_EVENT_HOLD && mouseButton != MouseState.WHEEL ) 
			{
				return false;
			} 
		}
	
	
		return recurring;
	}
	
	bool IsHold() 
	{
		bool release = false;
		
		for ( int kb = 0; kb < GetKeyBinds().Count(); ++kb) 
		{
			int keyCode = m_KeyBinds.GetKey(kb);
			int keyEvent = m_KeyBinds.Get(keyCode);
			
			if ( keyEvent == KB_EVENT_RELEASE ) 
			{
				
			}
		}
		
		return ( m_KeyBinds.GetKeyByValue(KB_EVENT_RELEASE) || m_MouseBinds.GetKeyByValue(MB_EVENT_RELEASE) );
	}
	
	bool HasKeyEvent( int key_Event ) 
	{
		for ( int kb = 0; kb < GetKeyBinds().Count(); ++kb) 
		{
			int keyCode = m_KeyBinds.GetKey(kb);
			int keyEvent = m_KeyBinds.Get(keyCode);
			
			if ( keyEvent == key_Event ) 
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool ContainsKey( int key ) 
	{
		return m_KeyBinds.Contains( key );
	}
	
	bool ContainsButton( int button ) 
	{
		return m_MouseBinds.Contains( button );
	}
	
	bool ContainsKeyEvent( int key, int key_Event ) 
	{
		int kc = -1;
		for ( int kb = 0; kb < GetKeyBinds().Count(); ++kb) 
		{
			int keyCode = m_KeyBinds.GetKey(kb);
			int keyEvent = m_KeyBinds.Get(keyCode);
			
			if ( keyCode == key && keyEvent == key_Event ) 
			{
				kc = keyCode;
			}
		}
		
		return kc > -1; 
	}
	
	bool ContainsButtonEvent( int button, int button_Event ) 
	{
		int m = -1;
		for ( int mb = 0; mb < GetMouseBinds().Count(); ++mb) 
		{
			int mouseBind = m_MouseBinds.GetKey(mb);
			int mouseEvemt = m_MouseBinds.Get(mouseBind);
			
			if ( mouseBind == button && mouseEvemt == button_Event ) 
			{
				m = mouseBind;
			}
		}
		
		return m > -1; 
	}
	
	void AddKeyBind( int key, int key_event ) 
	{
		m_KeyBinds.Insert( key, key_event );
	}
	
	void AddMouseBind( int button, int mouse_event ) 
	{
		m_MouseBinds.Insert( button, mouse_event );
	}
	
	ref map<int, int> GetKeyBinds() 
	{
		return m_KeyBinds;
	}
	
	ref map<int, int> GetMouseBinds() 
	{
		return m_MouseBinds;
	}
	
	typename GetObject() 
	{
		return m_Object;
	}
	
	string GetShortcut() 
	{
		return m_strShortcut;
	}
	
	string GetDescription() 
	{
		return m_strDescription;
	}
	
	string GetCallBackFunction() 
	{
		return m_strCallbackFunction;
	}

}