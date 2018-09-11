#ifdef COM_TEST_NEWLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\CommunityOfflineMode.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_OVERRIDEMENUS

/*
    Include of all .c files that belong to this module
*/
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\OverrideMenus.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\CustomPauseButton.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\gui\\CustomInGameMenu.c"

#ifdef COM_TEST_NEWLOADING
void RegisterModule()
{
    GetModuleManager().RegisterModule( new OverrideMenus );
}
#endif