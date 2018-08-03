#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\ItemSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\CargoSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\InventorySave.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\CharacterSave.c"

class SaveModule extends Module
{

	void SaveModule( CommunityOfflineMode mission )
	{
	}

	void ~SaveModule()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}

    // This is where we will show the Character Select GUI. 
	override void onMissionLoaded()
	{
	}

	override void onMissionFinish()
	{
	}
}