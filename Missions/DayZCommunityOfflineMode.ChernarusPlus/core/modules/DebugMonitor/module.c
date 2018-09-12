#ifndef COM_MODULES_OLDLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\CommunityOfflineMode.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_DEBUG_MONITOR

/*
    Include of all .c files that belong to this module
*/
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\DebugMonitor\\CustomDebugMonitor.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\DebugMonitor\\gui\\DebugMonitor.c"

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    GetModuleManager().RegisterModule( new CustomDebugMonitor );
}
#endif