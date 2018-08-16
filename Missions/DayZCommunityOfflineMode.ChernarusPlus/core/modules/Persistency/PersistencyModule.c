const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class PersistencyModule extends Module
{
	private ref COMCharacterSpawnMenu m_CharacterMenu;

	private bool m_CanBeSaved = false;

	private string m_sCharacter = "";
	private string m_sSave = "";

	private string m_sLastPlayer = "";
	private string m_sLastSave = "";

	void PersistencyModule()
	{
		Print("PersistencyModule::PersistencyModule()");
		KeyMouseBinding showCharacterMenu = new KeyMouseBinding( GetModuleType() , "ShowCharacterMenu"  , "[M]"    , "Shows the character menu."   );
		
		showCharacterMenu.AddKeyBind( KeyCode.KC_M, KeyMouseBinding.KB_EVENT_RELEASE );

		RegisterKeyMouseBinding( showCharacterMenu );

		MakeDirectory("$saves:CommunityOfflineMode");
		MakeDirectory(BASE_PLAYER_SAVE_DIR);
	}

	void ~PersistencyModule()
	{
		Print("PersistencyModule::~PersistencyModule()");
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
		Print("PersistencyModule::ShowCharacterMenu()");
		if (!m_CharacterMenu) {
			m_CharacterMenu = new COMCharacterSpawnMenu(this);
		}

		if (m_CharacterMenu.IsVisible()) {
			return;
		}

		GetGame().GetUIManager().ShowScriptedMenu( m_CharacterMenu , NULL );
	}

	void CreateNew(string sCharacter, PlayerBase oPlayer, string sSave = "latest")
	{
		Print("PersistencyModule::CreateNew(string sCharacter, PlayerBase oPlayer, string sSave = \"latest\")");
		m_CanBeSaved = false;

		MakeDirectory(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove( this.SavePlayer );

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		if ( GetPlayer() )
		{
			GetPlayer().Delete();
		}

		GetGame().SelectPlayer( NULL, oPlayer );
		
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY).CallLater( this.SavePlayer, 1000, true );

		m_CanBeSaved = true;
	}

	void SavePlayer(string sSave = "latest") 
	{
		Print("PersistencyModule::SavePlayer(string sSave = \"latest\")");
		if (m_CanBeSaved) {
			m_sSave = sSave;
			CharacterSave.SavePlayer( m_sCharacter, m_sSave, GetPlayer() );
		}
	}

	void LoadLast()
	{
		Print("PersistencyModule::LoadLast()");
		if (m_sLastPlayer == "" || m_sLastSave == "")
		{
			PlayerBase oPlayer = CommunityOfflineMode.CreateCustomDefaultCharacter();
			GetGame().SelectPlayer( NULL, oPlayer );

			m_CanBeSaved = true;
		} else 
		{
			m_CanBeSaved = false;

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.FinishLoadingPlayer, 100, true, m_sLastPlayer, m_sLastSave);
		}
	}

	void LoadPlayer(string sCharacter, string sSave = "latest")
	{
		Print("PersistencyModule::LoadPlayer(string sCharacter, string sSave = \"latest\")");
		m_CanBeSaved = false;

		m_sLastPlayer = sCharacter;
		m_sLastSave = sSave;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.FinishLoadingPlayer, 100, true, sCharacter, sSave);
	}

	private void FinishLoadingPlayer(string sCharacter, string sSave)
	{
		Print("PersistencyModule::FinishLoadingPlayer(string sCharacter, string sSave)");
		if ( GetPlayer() )
		{
			GetPlayer().Delete();
		}

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		PlayerBase oPlayer = CharacterLoad.LoadPlayer(m_sCharacter, m_sSave);
		GetGame().SelectPlayer( NULL, oPlayer );
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.FinishLoadingPlayer);
		
		oPlayer.MessageStatus("Loaded character \'" + m_sCharacter + "\' with save \'" + m_sSave + "\'");

		m_CanBeSaved = true;
	}
}