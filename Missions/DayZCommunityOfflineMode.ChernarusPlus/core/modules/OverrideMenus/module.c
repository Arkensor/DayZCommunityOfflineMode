#ifndef COM_MODULES_OLDLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\BaseModuleInclude.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_OVERRIDEMENUS

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\OverrideMenus.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\CustomPauseButton.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\gui\\CustomInGameMenu.c"
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    GetModuleManager().RegisterModule( new OverrideMenus );
}
#endif