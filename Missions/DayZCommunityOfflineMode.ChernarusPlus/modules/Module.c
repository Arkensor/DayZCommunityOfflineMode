/*
	Extend this class for separate objects
	Only override mouse and key functions for extra utility
*/
class Module
{
	protected CommunityOfflineMode m_Mission;
	protected bool m_Enabled;
	protected ref set< ref KeyMouseBinding > m_KeyBindings;

	protected ref UIScriptedMenu m_GUIWindow;

	protected ref ModuleManager m_oManager;

	void Module( CommunityOfflineMode mission )
	{
		m_Mission = mission;

		m_Enabled = true;
		
		m_KeyBindings = new ref set< ref KeyMouseBinding >;
	}

	void Module( CommunityOfflineMode mission, UIScriptedMenu window )
	{
		m_Mission = mission;
		
		m_GUIWindow = window;

		m_Enabled = true;
		
		m_KeyBindings = new ref set< ref KeyMouseBinding >;
	}
	
	void ~Module()
	{
		m_Mission = NULL;
	}

	void SetModuleManager( ref ModuleManager oManager )
	{
		m_oManager = oManager;
	}
	
	void Init()
	{
		RegisterKeyMouseBindings();
	}

    void Toggle()
    {
        m_Enabled = !m_Enabled;
    }

	void ShowWindow( UIScriptedMenu oParent = NULL )
	{
		if (m_GUIWindow) {
			if (m_GUIWindow.IsVisible()) {
				return;
			}
		} else 
		{
			return;
		}

		GetGame().GetUIManager().ShowScriptedMenu( m_GUIWindow , oParent );
	}

    string GetModuleName()
    {
        return ClassName();
    }

    typename GetModuleType()
    {
        return GetModuleName().ToType();
    }

    bool IsEnabled()
    {
        return m_Enabled;
    }

	void RegisterKeyMouseBindings() 
	{
	}
	
	void RegisterKeyMouseBinding( KeyMouseBinding binding ) 
	{
		m_KeyBindings.Insert( binding );
	}
	
	set< ref KeyMouseBinding > GetBindings()
	{
		return m_KeyBindings;
	}

	// Override functions 
	
	void onUpdate( int timeslice ) 
	{
	}
	
	/* Mouse Functions */ 
	
	void onMouseMove() 
	{
	}
	
	void onMouseDragY ( int state ) 
	{
	}
	
	void onMouseDragX ( int state )
	{
	}
	
	void onMouseWheelUp( int state ) 
	{
	}
	
	void onMouseWheelDown( int state ) 
	{
	}
	
	void onMouseButtonPress( int button )
	{
	}
	
	void onMouseButtonRelease( int button )
	{
	}

	/* Key Functions */
	
	void onKeyPress( int key ) 
	{
	}
	
	void onKeyRelease( int key )
	{
	}
	
	/* Mission Functions */
	
	void onMissionStart()
	{
	}
	
	void onMissionFinish()
	{
	}
	
	void onMissionLoaded()
	{
	}
}
