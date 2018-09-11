#ifdef COM_TEST_NEWLOADING
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\CommunityOfflineMode.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_KEYBINDS

/*
    Include of all .c files that belong to this module
*/
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComKeybinds\\COMKeyBinds.c"

#ifdef COM_TEST_NEWLOADING
void RegisterModule()
{
    GetModuleManager().RegisterModule( new COMKeyBinds );
}
#endif