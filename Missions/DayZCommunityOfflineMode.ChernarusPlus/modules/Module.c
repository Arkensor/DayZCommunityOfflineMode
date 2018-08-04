/**

	Extend this class for separate objects
	Only override mouse and key functions for extra utility
	
**/

class Module
{
	protected CommunityOfflineMode m_Mission;
	protected bool m_Enabled = true;
	
	ref set<ref KeyMouseBinding> k_m_Bindings;
	
	void Module( CommunityOfflineMode m_Mission )
	{
		this.m_Mission = m_Mission;
		
		k_m_Bindings = new set<ref KeyMouseBinding>;
	}
	
	void ~Module()
	{
		m_Mission = NULL;
	}
	
	void Init()
	{
		RegisterKeyMouseBindings();
	}
	
	void RegisterKeyMouseBindings() 
	{
		
	}
	
	void RegisterKeyMouseBinding( KeyMouseBinding binding ) 
	{
		k_m_Bindings.Insert( binding );
	}
	
	set<ref KeyMouseBinding> GetBindings() 
	{
		return k_m_Bindings;
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
	
	void Toggle() 
	{
		m_Enabled = !m_Enabled;
	}
	
	string GetModuleName()
	{
		return ClassName();
	}

	typename GetModuleType()
	{
		return GetModuleName().ToType();
	}
	
	PlayerBase GetPlayer() 
	{
		return m_Mission.m_oPlayer;
	}
	
	bool IsEnabled()
	{
		return m_Enabled;
	}
	
	set< Object > GetObjectsAtCursor( vector from, vector to, Object ignore = NULL )
	{
		vector contact_pos;
		vector contact_dir;
		int contact_component;

		set< Object > objects = new set< Object >;
		
		if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, objects, NULL, ignore, false, false, ObjIntersectView, 0.5 ) )
		{
			return objects;
		}

		return NULL;
	}
	
}
