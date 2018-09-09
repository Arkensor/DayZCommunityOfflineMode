/*
    Define used for optional compilations
*/
#define MODULE_PERSISTENCY

// #define MODULE_PERSITENCY_IGNORE_LOADING

int MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER            = 47842;
int MODULE_PERSISTENCY_WIDGET_LOAD_CHARACTER            = 47843;
int MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER_PANEL      = 47844;

string BASE_COM_DIR = "$saves:CommunityOfflineMode";
string BASE_PLAYER_SAVE_DIR = BASE_COM_DIR + "\\PlayerSaves";

/*
    Include of all .c files that belong to this module
*/
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\PersistencyModule.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Data\\CharacterData.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Data\\HandData.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Data\\InventoryData.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Data\\ItemData.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Load\\CharacterLoad.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Load\\HandLoad.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Load\\InventoryLoad.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Load\\ItemLoad.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Save\\CharacterSave.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Save\\HandSave.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Save\\InventorySave.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\Save\\ItemSave.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\COMCharacterMenu.c"
#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\COMCharacterSave.c"

#include "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\scenes\\COMPersistencyScene.c"