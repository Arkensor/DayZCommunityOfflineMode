
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Saving.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Reading.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Helper.c"

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