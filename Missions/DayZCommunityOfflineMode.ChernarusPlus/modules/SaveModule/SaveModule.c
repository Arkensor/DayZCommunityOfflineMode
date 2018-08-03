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

	void SavePlayer() 
	{
		if (!m_Mission.m_oPlayer) return;

		CharacterSave.SavePlayer("a", "b", m_Mission.m_oPlayer);
	}

    // This is where we will show the Character Select GUI. 
	override void onMissionLoaded()
	{
		m_Mission.m_oPlayer = CharacterSave.LoadPlayer("a", "b", m_Mission.m_oPlayer);
		GetGame().SelectPlayer( NULL, m_Mission.m_oPlayer );

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 10000, true);
			
	}
	
	override void onUpdate( int timeslice ) 
	{
	}

	override void onMissionFinish()
	{
	}
}