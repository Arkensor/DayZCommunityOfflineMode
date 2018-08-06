#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\ItemSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\HandSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\InventorySave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Savers\\CharacterSave.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\StaticFunctions.c"

const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class SaveModule extends Module
{
	private ref CharacterSpawnMenu m_CharacterMenu;

	private bool m_CanBeSaved = false;

	private string m_sCharacter = "";
	private string m_sSave = "";

	void SaveModule( CommunityOfflineMode mission )
	{
		KeyMouseBinding showCharacterMenu = new KeyMouseBinding( GetModuleType() , "ShowCharacterMenu"  , "[M]"    , "Shows the character menu."   );
		
		showCharacterMenu.AddKeyBind( KeyCode.KC_M, KB_EVENT_RELEASE );

		RegisterKeyMouseBinding( showCharacterMenu );

		MakeDirectory("$saves:CommunityOfflineMode");
		MakeDirectory(BASE_PLAYER_SAVE_DIR);
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

	void CreateNew(string sCharacter, string sSave = "latest")
	{
		m_CanBeSaved = false;

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

		m_CanBeSaved = true;
	}

	void SavePlayer(string sSave = "latest") 
	{
		if (m_CanBeSaved) {
			m_sSave = sSave;
			CharacterSave.SavePlayer(m_sCharacter, m_sSave, m_Mission.m_oPlayer);
		}
	}

	void LoadPlayer(string sCharacter, string sSave = "latest")
	{
		m_CanBeSaved = false;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.FinishLoadingPlayer, 100, true, sCharacter, sSave);
	}

	private void FinishLoadingPlayer(string sCharacter, string sSave)
	{
		if (m_Mission.m_oPlayer)
		{
			m_Mission.m_oPlayer.Delete();
		}

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		Print("SAVESTEST: Character: " + m_sCharacter + " Save: " + m_sSave);

		m_Mission.m_oPlayer = CharacterSave.LoadPlayer(m_sCharacter, m_sSave);
		GetGame().SelectPlayer( NULL, m_Mission.m_oPlayer );
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.FinishLoadingPlayer);

		m_CanBeSaved = true;
	}
}