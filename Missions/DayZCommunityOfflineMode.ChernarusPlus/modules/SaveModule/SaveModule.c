#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\ItemSave.c"
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

	void SavePlayer() 
	{
		CharacterSave.SavePlayer("a", "b", m_Mission.m_oPlayer);
	}

	PlayerBase LoadPlayer()
	{
		return CharacterSave.LoadPlayer("a", "b");
	}

    // This is where we will show the Character Select GUI. 
	override void onMissionLoaded()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);
	}
	
	override void onUpdate( int timeslice ) 
	{
	}

	override void onMissionFinish()
	{
		SavePlayer();
	}
}