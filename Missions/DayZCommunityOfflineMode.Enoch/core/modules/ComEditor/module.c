#ifndef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\BaseModuleInclude.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_EDITOR

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\ObjectEditor.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\EditorMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\ObjectMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\AdminTool\\gui\\PositionMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\WeatherMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\GameMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\PopupMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\ObjectInfoMenu.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\OverrideVerticalSpacer.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\Scene.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\SceneManager.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\SceneInfo.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\ObjectData.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\LootSpot.c"
#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\scene\\SceneSaveST.c"
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new ObjectEditor );
}
#endif