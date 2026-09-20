#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\Module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\KeyMouseBinding.c"

#ifdef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Admintool\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComKeyBinds\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComMenu\\module.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\module.c"
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

    void ModuleManager() { RegisterModules(); }
    void ~ModuleManager() { m_Modules.Clear(); delete m_Modules; }
    void RegisterModule( Module module ) { m_Modules.Insert( module ); }

    void RegisterModules()
    {
        m_Modules = new array< ref Module >;
#ifdef MODULE_COM_EDITOR
        RegisterModule( new ObjectEditor );
#endif
#ifdef MODULE_CAMERA_TOOL
        RegisterModule( new CameraTool );
#endif
#ifdef MODULE_COM_KEYBINDS
        RegisterModule( new COMKeyBinds );
#endif
#ifdef MODULE_PERSISTENCY
        RegisterModule( new PersistencyModule );
#endif
#ifdef MODULE_DEBUG_MONITOR
        RegisterModule( new CustomDebugMonitor );
#endif
    }

    void ReloadSettings() {}
    array< ref Module > GetModules() { return m_Modules; }

    void OnInit()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) m_Modules.Get(i).Init();
        GetUApi().UpdateControls();
    }
    void OnMissionStart()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) m_Modules.Get(i).onMissionStart();
    }
    void OnMissionFinish()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) m_Modules.Get(i).onMissionFinish();
    }
    void OnMissionLoaded()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) m_Modules.Get(i).onMissionLoaded();
    }
    void OnUpdate( float timeslice )
    {
        bool inputIsFocused = false;
        Widget focusedWidget = GetFocus();
        if ( focusedWidget && focusedWidget.ClassName().Contains( "EditBoxWidget" ) ) inputIsFocused = true;
        for ( int i = 0; i < m_Modules.Count(); ++i )
        {
            Module module = m_Modules.Get(i);
            if ( !module.IsPreventingInput() )
            {
                auto bindings = module.GetBindings();
                for ( int nBinding = 0; nBinding < bindings.Count(); ++nBinding )
                {
                    auto binding = bindings[nBinding];
                    if ( !binding.CanBeUsedInMenu() && GetGame().GetUIManager().GetMenu() ) continue;
                    if ( inputIsFocused ) continue;
                    UAInput input = GetUApi().GetInputByName( binding.GetUAInputName() );
                    int action = binding.GetActionType();
                    if ( action & KeyMouseActionType.PRESS && input.LocalPress() ) GetGame().GameScript.CallFunction( GetModule( binding.GetObject() ), binding.GetCallBackFunction(), NULL, 0 );
                    if ( action & KeyMouseActionType.RELEASE && input.LocalRelease() ) GetGame().GameScript.CallFunction( GetModule( binding.GetObject() ), binding.GetCallBackFunction(), NULL, 0 );
                    if ( action & KeyMouseActionType.HOLD && input.LocalHold() ) GetGame().GameScript.CallFunction( GetModule( binding.GetObject() ), binding.GetCallBackFunction(), NULL, 0 );
                    if ( action & KeyMouseActionType.DOUBLECLICK && input.LocalDoubleClick() ) GetGame().GameScript.CallFunction( GetModule( binding.GetObject() ), binding.GetCallBackFunction(), NULL, 0 );
                    if ( action & KeyMouseActionType.VALUE && input.LocalValue() != 0 ) GetGame().GameScript.CallFunction( GetModule( binding.GetObject() ), binding.GetCallBackFunction(), NULL, input.LocalValue() );
                }
            }
            module.onUpdate( timeslice );
        }
    }

    Module GetModule( typename module_Type )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) if ( m_Modules.Get(i).GetModuleType() == module_Type ) return m_Modules.Get(i);
        return NULL;
    }
    Module GetModuleByName( string module_name )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i ) if ( m_Modules.Get(i).GetModuleName() == module_name ) return m_Modules.Get(i);
        return NULL;
    }
}

ref ModuleManager g_com_ModuleManager;
ModuleManager COM_GetModuleManager() { if( !g_com_ModuleManager ) g_com_ModuleManager = new ModuleManager(); return g_com_ModuleManager; }
ModuleManager NewModuleManager() { if ( g_com_ModuleManager ) delete g_com_ModuleManager; g_com_ModuleManager = new ModuleManager(); return g_com_ModuleManager; }
