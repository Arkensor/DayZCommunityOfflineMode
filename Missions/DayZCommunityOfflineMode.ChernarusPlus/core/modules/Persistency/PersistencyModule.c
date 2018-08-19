const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class PersistencyModule extends Module
{
	protected ref COMPersistencyScene	m_Scene;

	protected ref COMCharacterMenu 	m_CharacterMenu;

	protected bool m_CanBeSaved = false;

	protected string m_sCharacter = "";
	protected string m_sSave = "";

	protected string m_sLastPlayer = "";
	protected string m_sLastSave = "";

	protected bool m_bExit;

	void PersistencyModule()
	{
		Print("PersistencyModule::PersistencyModule");

		KeyMouseBinding showCharacterMenu = new KeyMouseBinding( GetModuleType() , "ShowCharacterMenu"  , "[M]"    , "Shows the character menu."   );
		//KeyMouseBinding refreshCharacterMenu = new KeyMouseBinding( GetModuleType() , "RefreshCharacterMenu"  , "[O]"    , "Refreshes the character menu."   );
		
		showCharacterMenu.AddKeyBind( KeyCode.KC_M, KeyMouseBinding.KB_EVENT_RELEASE );
		//refreshCharacterMenu.AddKeyBind( KeyCode.KC_O, KeyMouseBinding.KB_EVENT_RELEASE );

		RegisterKeyMouseBinding( showCharacterMenu );
		//RegisterKeyMouseBinding( refreshCharacterMenu );

		MakeDirectory("$saves:CommunityOfflineMode");
		MakeDirectory(BASE_PLAYER_SAVE_DIR);
	}

	void ~PersistencyModule()
	{
		Print("PersistencyModule::~PersistencyModule");
		m_CharacterMenu = NULL;
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

	ref COMPersistencyScene GetScene()
	{
		return m_Scene;
	}

	void CloseCharacterMenu()
	{
		Print("PersistencyModule::CloseCharacterMenu");
		if (m_CharacterMenu)
		{
			m_CharacterMenu.Close();

			delete m_CharacterMenu;
		}

		if (m_Scene)
		{
			delete m_Scene;
		}

		m_CanPause = true;
	}

	void RefreshCharacterMenu()
	{
		Print("PersistencyModule::RefreshCharacterMenu");
		CloseCharacterMenu();
		ShowCharacterMenu();
	}

	void ShowCharacterMenu()
	{
		Print("PersistencyModule::ShowCharacterMenu");

		m_sCharacter = "temp";
		m_sSave = "latest";
		m_sLastPlayer = m_sCharacter;
		m_sLastSave = m_sSave;

		SavePlayer(m_sLastSave);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		CloseCharacterMenu();
		
		m_CanBeSaved = false;

		m_bExit = true;

		m_Scene = new COMPersistencyScene;

		if ( !m_CharacterMenu ) {
			m_CharacterMenu = new COMCharacterMenu( this );
		} 

		if (m_CharacterMenu.IsVisible()) {
			GetGame().GetUIManager().HideScriptedMenu( m_CharacterMenu );
		}

		m_CanPause = false;

		GetGame().GetUIManager().ShowScriptedMenu( m_CharacterMenu , NULL );
	}
	
	void SavePlayer(string sSave = "latest") 
	{
		Print("PersistencyModule::SavePlayer");

		if ( m_CanBeSaved ) {
			m_sSave = sSave;
			CharacterSave.SavePlayer( m_sCharacter, m_sSave, GetPlayer() );
		}
	}

	void CreatePlayer(string sCharacter, PlayerBase oPlayer, string sSave = "latest")
	{
		Print("PersistencyModule::CreatePlayer");

		m_CanBeSaved = false;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		MakeDirectory(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter);

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		m_sLastPlayer = sCharacter;
		m_sLastSave = sSave;

		oPlayer.SetPosition( GetSpawnPoints().GetRandomElement() );

		CharacterSave.SavePlayer( m_sCharacter, m_sSave, oPlayer );

		oPlayer.Delete();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.LoadPlayer, 100, true, m_sCharacter, m_sSave);
	
		m_CanBeSaved = true;
	}

	void LoadLast()
	{
		Print("PersistencyModule::LoadLast");

		m_CanBeSaved = false;

		m_bExit = true;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CreatePlayerInt, 100, true, m_sLastPlayer, m_sLastSave);
	}

	void LoadPlayer(string sCharacter, string sSave = "latest")
	{
		Print("PersistencyModule::LoadPlayer");

		m_CanBeSaved = false;

		m_sLastPlayer = sCharacter;
		m_sLastSave = sSave;

		m_bExit = false;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SavePlayer);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.LoadPlayer);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CreatePlayerInt, 100, true, sCharacter, sSave);
	}

	private void CreatePlayerInt(string sCharacter, string sSave)
	{
		Print("PersistencyModule::CreatePlayerInt");

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		PlayerBase oPlayer;

		if ( !m_bExit )
		{
			if ( m_sLastPlayer == "" || m_sLastSave == "" )
			{
				oPlayer = CreateCustomDefaultCharacter();
			} else 
			{
				oPlayer = CharacterLoad.LoadPlayer( m_sCharacter, m_sSave );
			}
		} else {
			oPlayer = CharacterLoad.LoadPlayer( m_sLastPlayer, m_sLastSave );
		}

		m_bExit = false;

		GetGame().SelectPlayer( NULL, oPlayer );

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.CreatePlayerInt);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.FinishLoadingInt, 100, true);
	}

	private void FinishLoadingInt()
	{
		Print("PersistencyModule::FinishLoadingInt");

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.FinishLoadingInt);

        GetGame().GetMission().GetHud().Show(true);

		GetPlayer().MessageStatus("Loaded character \'" + m_sCharacter + "\' with save \'" + m_sSave + "\'");

		m_CanBeSaved = true;

		CloseCharacterMenu();
	}
}