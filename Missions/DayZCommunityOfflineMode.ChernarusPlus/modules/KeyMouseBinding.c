const int KB_EVENT_PRESS			= 0;
const int KB_EVENT_RELEASE			= 1;
const int KB_EVENT_HOLD				= 2;
const int MB_EVENT_HOLD 			= 5;

class KeyMouseBinding
{
	private typename object;
	private ref map<int, int> k_Bind;
	private ref map<int, int> m_Bind;
	private string k_m_CallbackFunction;
	private string k_m_Shortcut;
	private string k_m_Description;
	private bool canUseInMenu;
	
	void KeyMouseBinding( typename object, string callback, string shortcut, string description, bool menu = false ) 
	{
		this.object = object;
		k_Bind = new map<int, int>;
		m_Bind = new map<int, int>;
		
		k_m_CallbackFunction = callback;
		k_m_Shortcut = shortcut;
		k_m_Description = description;
		
		canUseInMenu = menu;
	}
	
	bool canUseInMenu() 
	{
		return canUseInMenu;
	}
	
	bool Check()
	{
		bool k_m_Pressed = true;
		for ( int kb = 0; kb < k_Bind.Count(); ++kb ) 
		{
			int keyCode = k_Bind.GetKey(kb);
			int keyEvent = k_Bind.Get(keyCode);
			
			if ( keyEvent == KB_EVENT_RELEASE ) 
			{ // Skip checking for release keys
				continue; 
			}
			if ( KeyState( keyCode ) == 0 )
			{
				k_m_Pressed = false;
			}
		}
		
		for ( int mb = 0; mb < m_Bind.Count(); ++mb ) 
		{
			int mouseButton = m_Bind.GetKey(mb);
			int mouseEvent = m_Bind.Get(mouseButton);
			
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
	
		for ( int kb = 0; kb < k_Bind.Count(); ++kb ) 
		{
			int keyCode = k_Bind.GetKey(kb);
			int keyEvent = k_Bind.Get(keyCode);
			
			
			if ( keyEvent != KB_EVENT_HOLD ) 
			{
				return false; // a key is found that is not hold or drag. so it should not be recurring
			} 
		}
		
		for ( int mb = 0; mb < m_Bind.Count(); ++mb ) 
		{
			int mouseButton = m_Bind.GetKey(mb);
			int mouseEvent = m_Bind.Get(mouseButton);
			
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
			int keyCode = k_Bind.GetKey(kb);
			int keyEvent = k_Bind.Get(keyCode);
			
			if ( keyEvent == KB_EVENT_RELEASE ) 
			{
				
			}
		}
		
		return ( k_Bind.GetKeyByValue(KB_EVENT_RELEASE) || m_Bind.GetKeyByValue(MB_EVENT_RELEASE) );
	}
	
	bool HasKeyEvent( int key_Event ) 
	{
		for ( int kb = 0; kb < GetKeyBinds().Count(); ++kb) 
		{
			int keyCode = k_Bind.GetKey(kb);
			int keyEvent = k_Bind.Get(keyCode);
			
			if ( keyEvent == key_Event ) 
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool ContainsKey( int key ) 
	{
		return k_Bind.Contains( key );
	}
	
	bool ContainsButton( int button ) 
	{
		return m_Bind.Contains( button );
	}
	
	bool ContainsKeyEvent( int key, int key_Event ) 
	{
		int kc = -1;
		for ( int kb = 0; kb < GetKeyBinds().Count(); ++kb) 
		{
			int keyCode = k_Bind.GetKey(kb);
			int keyEvent = k_Bind.Get(keyCode);
			
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
			int mouseBind = m_Bind.GetKey(mb);
			int mouseEvemt = m_Bind.Get(mouseBind);
			
			if ( mouseBind == button && mouseEvemt == button_Event ) 
			{
				m = mouseBind;
			}
		}
		
		return m > -1; 
	}
	
	void AddKeyBind( int key, int key_event ) 
	{
		k_Bind.Insert( key, key_event );
	}
	
	void AddMouseBind( int button, int mouse_event ) 
	{
		m_Bind.Insert( button, mouse_event );
	}
	
	ref map<int, int> GetKeyBinds() 
	{
		return k_Bind;
	}
	
	ref map<int, int> GetMouseBinds() 
	{
		return m_Bind;
	}
	
	typename GetObject() 
	{
		return object;
	}
	
	string GetShortcut() 
	{
		return k_m_Shortcut;
	}
	
	string GetDescription() 
	{
		return k_m_Description;
	}
	
	string GetCallBackFunction() 
	{
		return k_m_CallbackFunction;
	}

}