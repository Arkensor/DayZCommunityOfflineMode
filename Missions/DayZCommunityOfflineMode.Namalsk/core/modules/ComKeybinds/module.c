#ifndef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Namalsk\\core\\BaseModuleInclude.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_KEYBINDS

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Namalsk\\core\\modules\\ComKeybinds\\COMKeyBinds.c"
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new COMKeyBinds );
}
#endif