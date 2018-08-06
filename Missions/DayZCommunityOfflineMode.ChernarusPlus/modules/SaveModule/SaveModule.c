#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\ItemSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\HandSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\InventorySave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\CharacterSave.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\StaticFunctions.c"

const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class SaveModule extends Module
{
	private ref CharacterSpawnMenu m_CharacterMenu;

	private string m_sCharacter = "default";
	private string m_sSave = "latest";

	void SaveModule( CommunityOfflineMode mission )
	{
		KeyMouseBinding spawnDefault = new KeyMouseBinding( GetModuleType() , "SpawnDefault"  , "[K]"    , "Spawns the default player."   );
		KeyMouseBinding showCharacterMenu = new KeyMouseBinding( GetModuleType() , "ShowCharacterMenu"  , "[M]"    , "Shows the character menu."   );
		
		spawnDefault.AddKeyBind( KeyCode.KC_K, KB_EVENT_RELEASE );
		showCharacterMenu.AddKeyBind( KeyCode.KC_M, KB_EVENT_RELEASE );

		RegisterKeyMouseBinding( spawnDefault );
		RegisterKeyMouseBinding( showCharacterMenu );

		MakeDirectory("$saves:CommunityOfflineMode");
		MakeDirectory(BASE_PLAYER_SAVE_DIR);
		
		CreateNew("default");
	}

	void ~SaveModule()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}

	override void onMissionLoaded()
	{
	}
	
	override void onUpdate( int timeslice ) 
	{
	}

	override void onMissionFinish()
	{
		SavePlayer();
	}

	void ShowCharacterMenu()
	{
		if (m_CharacterMenu) {
			if (m_CharacterMenu.IsVisible()) {
				return;
			}
		}

		m_CharacterMenu = new CharacterSpawnMenu(this);
		GetGame().GetUIManager().ShowScriptedMenu( m_CharacterMenu , NULL );
	}

	void SpawnDefault() 
	{
		if (m_Mission.m_oPlayer)
		{
			m_Mission.m_oPlayer.Delete();
		}

		if (m_Mission.CTRL()) {
			m_Mission.m_oPlayer = CreateCustomDefaultCharacter();
		}

		GetGame().SelectPlayer( NULL, m_Mission.m_oPlayer );
	}

	void CreateNew(string sCharacter, string sSave = "latest")
	{
		MakeDirectory(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		if (m_Mission.m_oPlayer)
		{
			m_Mission.m_oPlayer.Delete();
		}
		
		m_Mission.m_oPlayer = CreateCustomDefaultCharacter();
		GetGame().SelectPlayer( NULL, m_Mission.m_oPlayer );
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);
	}

	void SavePlayer() 
	{
		CharacterSave.SavePlayer(m_sCharacter, "latest", m_Mission.m_oPlayer);
	}

	PlayerBase LoadPlayer(string sCharacter = "default", string sSave = "latest")
	{
		m_sCharacter = sCharacter;
		m_sSave = sSave;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		if (m_Mission.m_oPlayer)
		{
			m_Mission.m_oPlayer.Delete();
		}

		m_Mission.m_oPlayer = CharacterSave.LoadPlayer(sCharacter, sSave);
		GetGame().SelectPlayer( NULL, m_Mission.m_oPlayer );
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);

		return m_Mission.m_oPlayer;
	}

	
}