#ifndef COM_MODULES_OLDLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\CommunityOfflineMode.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_EDITOR

/*
    Include of all .c files that belong to this module
*/
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\ObjectEditor.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\EditorMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\ObjectMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\WeatherMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\GameMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\PopupMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\OverrideVerticalSpacer.c"

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    GetModuleManager().RegisterModule( new ObjectEditor );
}
#endif