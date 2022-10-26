#ifndef COM_MODULES_OLDLOADING
#include "$CurrentDir:missions\\DayZCommunityOfflineMode." + worldMap + "\\core\\BaseModuleInclude.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_EDITOR

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
#include "$mission:core\\modules\\ComEditor\\ObjectEditor.c"
#include "$mission:core\\modules\\ComEditor\\gui\\EditorMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\ObjectMenu.c"
#include "$mission:core\\modules\\AdminTool\\gui\\PositionMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\WeatherMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\GameMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\PopupMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\ObjectInfoMenu.c"
#include "$mission:core\\modules\\ComEditor\\gui\\OverrideVerticalSpacer.c"
#include "$mission:core\\modules\\ComEditor\\scene\\Scene.c"
#include "$mission:core\\modules\\ComEditor\\scene\\SceneManager.c"
#include "$mission:core\\modules\\ComEditor\\scene\\SceneInfo.c"
#include "$mission:core\\modules\\ComEditor\\scene\\ObjectData.c"
#include "$mission:core\\modules\\ComEditor\\scene\\LootSpot.c"
#include "$mission:core\\modules\\ComEditor\\scene\\ObjectSave.c"
#include "$mission:core\\modules\\ComEditor\\scene\\PositionData.c"
#include "$mission:core\\modules\\ComEditor\\scene\\PositionSave.c"
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new ObjectEditor );
}
#endif