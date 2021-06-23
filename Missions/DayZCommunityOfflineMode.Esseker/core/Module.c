/*
	Extend this class for separate objects
	Only override mouse and key functions for extra utility
*/
class Module
{
	protected bool m_Enabled;
	protected bool m_PreventInput;
	protected ref set< ref KeyMouseBinding > m_KeyBindings;
	
	void Module()
	{
		m_Enabled = true;
		m_PreventInput = false;
		m_KeyBindings = new ref set< ref KeyMouseBinding >;
	}
	
	void ~Module()
	{
	}
	
	void Init()
	{
		RegisterKeyMouseBindings();
	}

    void Toggle()
    {
        m_Enabled = !m_Enabled;
    }

	void PreventInput(bool prevent)
	{
		m_PreventInput = prevent;
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

	bool IsPreventingInput()
	{
		return m_PreventInput;
	}

	void RegisterKeyMouseBindings() 
	{
	}
	
	void RegisterKeyMouseBinding( KeyMouseBinding binding ) 
	{
        m_KeyBindings.Insert( binding );

        UAInput input = GetUApi().RegisterInput( binding.GetUAInputName(), binding.GetDescription(), "core" );

        if ( input == NULL ) return;

        input.AddAlternative();

        ref array< string > bindings = binding.GetBindings();
        for ( int i = 0; i < bindings.Count(); i++ )
        {
            input.BindCombo( bindings[i] );
        }
	}
	
	set< ref KeyMouseBinding > GetBindings()
	{
		return m_KeyBindings;
	}

	// Override functions 
	
	void onUpdate( float timeslice ) 
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
