#ifndef COM_MODULES_OLDLOADING
#include "$mission:core\\BaseModuleInclude.c"
#endif
/*
    Define used for optional compilations
*/
#define MODULE_PERSISTENCY

// #define MODULE_PERSISTENCY_IGNORE_LOADING

int MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER            = 4842;
int MODULE_PERSISTENCY_WIDGET_LOAD_CHARACTER            = 4843;
int MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER_PANEL      = 4844;

string BASE_PLAYER_SAVE_DIR = BASE_COM_DIR + "PlayerSaves";

/*
    Include of all .c files that belong to this module
*/

void PersistencyPrint( string var )
{
    if ( true )
    {
        Print( var );
    }
}

#ifdef COM_MODULES_OLDLOADING
#include "$mission:core\\modules\\Persistency\\PersistencyModule.c"

#include "$mission:core\\modules\\Persistency\\Data\\CharacterData.c"
#include "$mission:core\\modules\\Persistency\\Data\\HandData.c"
#include "$mission:core\\modules\\Persistency\\Data\\InventoryData.c"
#include "$mission:core\\modules\\Persistency\\Data\\ItemData.c"

#include "$mission:core\\modules\\Persistency\\Load\\CharacterLoad.c"
#include "$mission:core\\modules\\Persistency\\Load\\HandLoad.c"
#include "$mission:core\\modules\\Persistency\\Load\\InventoryLoad.c"
#include "$mission:core\\modules\\Persistency\\Load\\ItemLoad.c"

#include "$mission:core\\modules\\Persistency\\Save\\CharacterSave.c"
#include "$mission:core\\modules\\Persistency\\Save\\HandSave.c"
#include "$mission:core\\modules\\Persistency\\Save\\InventorySave.c"
#include "$mission:core\\modules\\Persistency\\Save\\ItemSave.c"

#include "$mission:core\\modules\\Persistency\\gui\\COMCharacterMenu.c"
#include "$mission:core\\modules\\Persistency\\gui\\COMCharacterSave.c"
#include "$mission:core\\modules\\Persistency\\gui\\LoadText.c"

#include "$mission:core\\modules\\Persistency\\gui\\scenes\\COMPersistencyScene.c"
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new PersistencyModule );
}
#endif