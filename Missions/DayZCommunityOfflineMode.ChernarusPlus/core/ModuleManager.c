#define COM_MODULES_OLDLOADING

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\Module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\KeyMouseBinding.c"

#ifdef COM_MODULES_OLDLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\UIExtender\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Admintool\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComKeyBinds\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComMenu\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\DebugMonitor\\module.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\BarrelCrosshair\\module.c"
#endif

#ifndef COM_MODULES_OLDLOADING
const string COM_MODULE_DIR = "$currentDir:";
const string COM_MODULE_FOLDER = "Missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\";
#endif

class ModuleManager
{
	protected const int CLICK_TIME			= 200; //ms
	protected const int HOLD_CLICK_TIME_MIN	= 200; //ms
	protected const int DOUBLE_CLICK_TIME	= 300; //ms

    protected ref array< ref Module > m_Modules;
	protected ref array< ref MouseButtonInfo > m_MouseButtons;

    protected ScriptModule m_ParentScriptModule;

    void ModuleManager()
    {
        Print( "ModuleManager::ModuleManager()" );

		m_Modules 	   = new ref array< ref Module >;
		m_MouseButtons = new ref array< ref MouseButtonInfo >;
		m_MouseButtons.Insert( new MouseButtonInfo( MouseState.LEFT ) );
		m_MouseButtons.Insert( new MouseButtonInfo( MouseState.RIGHT ) );
		m_MouseButtons.Insert( new MouseButtonInfo( MouseState.MIDDLE ) );

		RegisterModules();
    }

    void ~ModuleManager()
    {
        Print( "ModuleManager::~ModuleManager()" );
    }

#ifndef COM_MODULES_OLDLOADING
    bool IsValidModule( string name, FileAttr attributes )
    {
        Print( "Found: " + COM_MODULE_DIR + COM_MODULE_FOLDER + name + " as a " + FileAttributeToString( attributes ) );

        if ( ! (attributes & FileAttr.DIRECTORY ) ) return false;

        if ( name == "" ) return false;

        return true;
    }

    void LoadModule( string name )
    {
		ScriptModule script = ScriptModule.LoadScript( m_ParentScriptModule, COM_MODULE_FOLDER + name + "\\module.c", true );
        if ( script )
        {
            Param p = new Param;
            script.CallFunction( NULL, "RegisterModule", p, new Param );
        }
    }

    void RegisterModule( Module module )
    {
        m_Modules.Insert( module );
    }

    private ScriptModule LoadScriptModule( string path, bool listing = true )
    {
        Print( "Loading ScriptModule " + path );
        ScriptModule module = ScriptModule.LoadScript( m_ParentScriptModule, path, listing );
        if ( module )
        {
            Print( "Loaded ScriptModule " + path );
        }
        else
        {
            Print( "Failed to load ScriptModule " + path );
        }
        return module;
    }

    void RegisterModules()
    {
        Print( "ModuleManager::RegisterModules()" );


        //m_ParentScriptModule = NULL;
        // m_ParentScriptModule = GetGame().GameScript;
        m_ParentScriptModule = GetMission().MissionScript;

        //m_ParentScriptModule = LoadScriptModule( "Scripts/1_Core" );
        //m_ParentScriptModule = LoadScriptModule( "Scripts/2_GameLib" );
        //m_ParentScriptModule = LoadScriptModule( "Scripts/3_Game" );
        //m_ParentScriptModule = LoadScriptModule( "Scripts/4_World" );
        //m_ParentScriptModule = LoadScriptModule( "Scripts/5_Mission" );

		int index = 0;
        string module = "";
		FileAttr oFileAttr = FileAttr.INVALID;
		FindFileHandle oFileHandle = FindFile( COM_MODULE_DIR + COM_MODULE_FOLDER + "*", module, oFileAttr, FindFileFlags.DIRECTORIES );

		if ( module != "" )
		{
            if ( IsValidModule( module, oFileAttr ) )
            {
                LoadModule( module );
			    index++;
            }

			while (FindNextFile(oFileHandle, module, oFileAttr))
			{
				if ( IsValidModule( module, oFileAttr ) )
				{
                    LoadModule( module );
					index++;
				}
			}
		}
    }
#else
    void RegisterModule( ref Module module )
    {
        m_Modules.Insert( module );
    }

    void RegisterModules()
    { 
        Print( "ModuleManager::RegisterModules()" );
        #ifdef MODULE_COM_EDITOR
        RegisterModule( new ref ObjectEditor );
        #endif
        #ifdef MODULE_CAMERA_TOOL
        RegisterModule( new ref CameraTool );
        #endif
        #ifdef MODULE_COM_KEYBINDS
        RegisterModule( new ref COMKeyBinds );
        #endif
        #ifdef MODULE_PERSISTENCY
        #ifndef DISABLE_PERSISTENCY
        RegisterModule( new ref PersistencyModule );
        #endif
        #endif
        #ifdef MODULE_DEBUG_MONITOR
        RegisterModule( new ref CustomDebugMonitor );
        #endif
        #ifdef MODULE_UIEXTENDER
        RegisterModule( new ref UIExtender );
        #endif
    }
#endif

	void OnInit()
	{
	    Print( "ModuleManager::OnInit()" );

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).Init();
        }
	}

    void OnMissionStart()
    {
        Print( "ModuleManager::OnMissionStart()" );

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionStart();
        }
    }

    void OnMissionFinish()
    {
        Print( "ModuleManager::OnMissionFinish()" );

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionFinish();
        }
    }

    void OnMissionLoaded()
    {
        Print( "ModuleManager::OnMissionLoaded()" );

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionLoaded();
		}
    }

	void OnMouseButtonRelease(int button)
	{
		moduleMouseCheck( button, KeyMouseBinding.MB_EVENT_RELEASE );
	}
    
    void OnMouseButtonHold( int button )
	{
		moduleMouseCheck( button, KeyMouseBinding.MB_EVENT_HOLD );
	}

	void OnMouseButtonPress( int button )
	{
		moduleMouseCheck( button, KeyMouseBinding.MB_EVENT_PRESS );
	}

    void OnKeyPress( int key )
    {
        moduleKeyCheck( key, KeyMouseBinding.KB_EVENT_PRESS );
    }

	void OnKeyRelease(int key)
	{
		moduleKeyCheck( key, KeyMouseBinding.KB_EVENT_RELEASE );
    }

    void OnUpdate( float timeslice )
    {
		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			Module module = m_Modules.Get(i);

            if ( !module.IsPreventingInput() )
            {
                for ( int kb = 0; kb < module.GetBindings().Count(); ++kb )
                {
                    KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);

                    if ( GetGame().GetUIManager().GetMenu() )
                    {
                        if ( !k_m_Binding.canUseInMenu() )
                        {
                            continue;
                        }
                    }

                    if ( k_m_Binding.IsRecurring() )
                    {
                        if (k_m_Binding.Check())
                        {
                            int mouseButton = -1;

                            bool hasDrag = false;
                            for ( int mb = 0; mb < k_m_Binding.GetMouseBinds().Count(); ++mb)
                            {
                                int mouseBind = k_m_Binding.GetMouseBinds().GetKey(mb);
                                int mouseEvent = k_m_Binding.GetMouseBinds().Get(mouseBind);

                                if ( mouseEvent == KeyMouseBinding.MB_EVENT_DRAG )
                                {
                                    hasDrag = true;
                                    mouseButton = mouseBind;
                                }
                                else if ( mouseEvent == KeyMouseBinding.MB_EVENT_HOLD )
                                {
                                    mouseButton = mouseBind;
                                }
                            }

                            if ( mouseButton > -1 )
                            {
                                MouseButtonInfo info = m_MouseButtons.Get( mouseButton );
                                if ( info )
                                {
                                    if ( info.IsButtonDown() )
                                    {
                                        int time_curr = GetGame().GetTime();
                                        int time_hold = info.GetTimeLastPress() + HOLD_CLICK_TIME_MIN;

                                        if ( time_hold < time_curr )
                                        {
                                            if ( hasDrag )
                                            {
                                                GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                                            }
                                            else if ( k_m_Binding.ContainsButtonEvent( mouseButton, KeyMouseBinding.MB_EVENT_HOLD) )
                                            {
                                                GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                                            }
                                        }
                                    }
                                }
                            }

                            if ( k_m_Binding.ContainsButton( MouseState.WHEEL ) )
                            {
                                if ( GetMouseState (MouseState.WHEEL) != 0 )
                                {
                                    GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, GetMouseState( MouseState.WHEEL ) );
                                }
                            }
                        }
                    }
                }
            }
			module.onUpdate( timeslice );
		}
    }

    ref Module GetModule( typename module_Type )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i )
        {
            ref Module module = m_Modules.Get(i);

            if ( module.GetModuleType() == module_Type)
            {
                return module;
            }
        }

        return NULL;
    }

    ref Module GetModuleByName( string module_name )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i )
        {
            ref Module module = m_Modules.Get( i );

            if (module.GetModuleName() == module_name)
            {
                return module;
            }
        }

        return NULL;
    }

    MouseButtonInfo GetMouseButtonInfo( int button )
    {
        for ( int i = 0; i < m_MouseButtons.Count(); ++i )
        {
            MouseButtonInfo info = m_MouseButtons.Get(i);

            if ( info.GetButtonID() == button )
            {
                return info;
            }
        }
        return NULL;
    }

    void moduleMouseCheck( int button, int mouseEvent )
    {
        MouseButtonInfo button_info = GetMouseButtonInfo( button );

        if ( button_info == NULL ) return;

        if ( mouseEvent == KeyMouseBinding.MB_EVENT_PRESS ) button_info.Press();

        int time_curr			= GetGame().GetTime();
        int time_last_press		= button_info.GetTimeLastPress();
        int time_last_release	= button_info.GetTimeLastRelease();
        int time_delta_press	= time_curr - time_last_press;
        int time_delta_release	= time_curr - time_last_release;

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            Module module = m_Modules.Get(i);

            if ( module.IsPreventingInput() )
            {
                continue;
            }

            for ( int kb = 0; kb < module.GetBindings().Count(); ++kb )
            {
                KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);

                if ( GetGame().GetUIManager().GetMenu() )
                {
                    if ( !k_m_Binding.canUseInMenu() )
                    {
                        continue;
                    }
                }

                if ( k_m_Binding.ContainsButton( button ) )
                {
                    if ( k_m_Binding.Check() )
                    {
                        if ( mouseEvent == KeyMouseBinding.MB_EVENT_RELEASE )
                        {
                            if ( time_delta_release < DOUBLE_CLICK_TIME )
                            {
                                if ( k_m_Binding.ContainsButtonEvent( button, KeyMouseBinding.MB_EVENT_DOUBLECLICK ) )
                                {
                                    GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                                }
                            }
                            else if ( time_delta_press < CLICK_TIME )
                            {
                                if ( k_m_Binding.ContainsButtonEvent( button, KeyMouseBinding.MB_EVENT_CLICK ) )
                                {
                                    GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                                }
                            }
                            else
                            {
                                if ( k_m_Binding.ContainsButtonEvent( button, KeyMouseBinding.MB_EVENT_RELEASE ) )
                                {
                                    GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                                }
                            }
                        }
                        else if ( k_m_Binding.ContainsButtonEvent( button, KeyMouseBinding.MB_EVENT_PRESS ) )
                        {
                                GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                        }
                    }
                }
            }

            switch ( mouseEvent )
            {
                case KeyMouseBinding.MB_EVENT_PRESS:
                    module.onMouseButtonPress( button );
                    break;
                case KeyMouseBinding.MB_EVENT_RELEASE:
                    module.onMouseButtonRelease( button );
                    break;
            }
        }

        if ( mouseEvent == KeyMouseBinding.MB_EVENT_RELEASE ) button_info.Release();
    }

    void moduleKeyCheck( int key, int keyEvent )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            Module module = m_Modules.Get(i);

            if ( module.IsPreventingInput() )
            {
                continue;
            }

            for ( int kb = 0; kb < module.GetBindings().Count(); ++kb )
            {
                KeyMouseBinding k_m_Binding = module.GetBindings().Get(kb);

                if ( GetGame().GetUIManager().GetMenu() )
                {
                    if ( !k_m_Binding.canUseInMenu() )
                    {
                        continue;
                    }
                }

                if ( k_m_Binding.ContainsKeyEvent( key, keyEvent ) )
                {
                    if ( k_m_Binding.Check() )
                    {
                        GetGame().GameScript.CallFunction(GetModule(k_m_Binding.GetObject()), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }
                }
            }

            switch ( keyEvent )
            {
                case KeyMouseBinding.KB_EVENT_PRESS:
                    module.onKeyPress( key ); //extra utility
                    break;
                case KeyMouseBinding.KB_EVENT_RELEASE:
                    module.onKeyRelease( key );
                    break;
            }
        }
    }
}

ref ModuleManager g_com_ModuleManager;

ref ModuleManager GetModuleManager()
{
    if( !g_com_ModuleManager )
    {
        g_com_ModuleManager = new ref ModuleManager();
    }

    return g_com_ModuleManager;
}