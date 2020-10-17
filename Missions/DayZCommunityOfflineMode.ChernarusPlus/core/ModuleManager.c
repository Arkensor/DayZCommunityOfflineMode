#define COM_MODULES_OLDLOADING

#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\Module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\KeyMouseBinding.c"

#ifdef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Admintool\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComKeyBinds\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComMenu\\module.c"
//#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\DebugMonitor\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\BarrelCrosshair\\module.c"
#endif

class KeyMouseActionType
{
    static int PRESS = 1;
    static int RELEASE = 2;
    static int HOLD = 4;
    static int DOUBLECLICK = 8;
    static int VALUE = 16;
}

class ModuleManager
{
    protected ref array< ref Module > m_Modules;

    void ModuleManager()
    {
        RegisterModules();
    }

    void ~ModuleManager()
    {
        m_Modules.Clear();

        delete m_Modules;
    }

    void RegisterModule( ref Module module )
    {
        m_Modules.Insert( module );
    }

    void RegisterModules()
    {
        m_Modules = new ref array< ref Module >;

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
        RegisterModule( new ref PersistencyModule );
        #endif
        #ifdef MODULE_DEBUG_MONITOR
        RegisterModule( new ref CustomDebugMonitor );
        #endif
    }

    void ReloadSettings()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            // m_Modules.Get(i).ReloadSettings();
        }
    }

    ref array< ref Module > GetModules()
    {
        return m_Modules;
    }

    void OnInit()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).Init();
        }

        GetUApi().UpdateControls();
    }

    void OnMissionStart()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionStart();
        }
    }

    void OnMissionFinish()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionFinish();
        }
    }

    void OnMissionLoaded()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionLoaded();
        }
    }

    void OnUpdate( float timeslice )
    {
        //if ( GetGame().IsServer() && GetGame().IsMultiplayer() ) return;

		bool inputIsFocused = false;

		ref Widget focusedWidget = GetFocus();
		if ( focusedWidget && focusedWidget.ClassName().Contains( "EditBoxWidget" ) )
		{
			inputIsFocused = true;
		}

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            Module module = m_Modules.Get(i);

            if ( !module.IsPreventingInput() )
            {
                auto bindings = module.GetBindings();

                for ( int nBinding = 0; nBinding < bindings.Count(); ++nBinding )
                {
                    auto k_m_Binding = bindings[ nBinding ];

                    if ( !k_m_Binding.CanBeUsedInMenu() && GetGame().GetUIManager().GetMenu())
                    {
                        continue;
                    }

                    if ( inputIsFocused )
                    {
                        continue;
                    }

                    UAInput input = GetUApi().GetInputByName( k_m_Binding.GetUAInputName() );

                    //COM_Message( k_m_Binding.GetUAInputName() + " -> " + input.LocalPress() );

                    //input.ForceEnable( true );

                    int action = k_m_Binding.GetActionType();

                    if ( action & KeyMouseActionType.PRESS && input.LocalPress() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.RELEASE && input.LocalRelease() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.HOLD && input.LocalHold() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.DOUBLECLICK && input.LocalDoubleClick() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.VALUE && input.LocalValue() != 0 ) 
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, input.LocalValue() );
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
}

ref ModuleManager g_com_ModuleManager;

ref ModuleManager COM_GetModuleManager()
{
    if( !g_com_ModuleManager )
    {
        g_com_ModuleManager = new ref ModuleManager();
    }

    return g_com_ModuleManager;
}

ref ModuleManager NewModuleManager()
{
    if ( g_com_ModuleManager )
    {
        delete g_com_ModuleManager;
    }

    g_com_ModuleManager = new ref ModuleManager();

    return g_com_ModuleManager;
}