class PersistencyModule extends Module
{
	protected ref COMPersistencyScene	m_Scene;

	protected ref COMCharacterMenu		m_CharacterMenu;
	protected ref Widget				m_CharacterSaveWidget;
	protected ref COMCharacterSave		m_CharacterSave;
	protected ref UIScriptedMenu		m_InGameMenu;

	protected bool m_CanBeSaved			= false;
	protected bool m_CharacterIsLoaded 	= false;

	protected string m_sCharacter 		= "";
	protected string m_sSave			= "";

	void PersistencyModule()
	{
		Print("PersistencyModule::PersistencyModule");

		MakeDirectory(BASE_COM_DIR);
		MakeDirectory(BASE_PLAYER_SAVE_DIR);
	}

	void ~PersistencyModule()
	{
		Print("PersistencyModule::~PersistencyModule");

		CleanupCharacterSaving();
		CleanupCharacterMenu();
		CleanupScene();
	}

	string GetLoadedCharacter()
	{
		return m_sCharacter;
	}

	string GetLoadedSave()
	{
		return m_sSave;
	}
	
	override void Init() 
	{
		super.Init();

		#ifdef MODULE_OVERRIDEMENUS
		OverrideMenus om = GetModuleManager().GetModuleByName("OverrideMenus");

		if ( om )
		{
			om.AddPauseButton( new CustomPauseButton( "SAVE CHARACTER", MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER, GetModuleType(), "OpenCharacterSaving" ), new OverrideValid(true, true), 1 );
			om.AddPauseButton( new CustomPauseButton( "LOAD CHARACTER", MODULE_PERSISTENCY_WIDGET_LOAD_CHARACTER, GetModuleType(), "OpenCharacterLoading" ), new OverrideValid(true, true), 1 );
		}

		#endif
	}

	override void onMissionLoaded()
	{
		Print("PersistencyModule::onMissionLoaded");

		m_CanBeSaved = false;
		m_CharacterIsLoaded = false;

		#ifdef MODULE_PERSITENCY_IGNORE_LOADING
		GetGame().SelectPlayer( NULL, CreateCustomDefaultCharacter() );
		#else
		OpenCharacterLoading();
		#endif
	}
	
	override void onUpdate( int timeslice ) 
	{
	}

	override void onMissionFinish()
	{
		SavePlayer(m_sSave);
	}

	ref COMPersistencyScene GetScene()
	{
		return m_Scene;
	}

	void CleanupScene()
	{
		Print("PersistencyModule::CleanupScene");

		if ( m_Scene )
		{
			delete m_Scene;
		}
	}

	void CleanupCharacterMenu()
	{
		Print("PersistencyModule::CleanupCharacterMenu");

		if (m_CharacterMenu)
		{
			// m_CharacterMenu.Close();

			GetGame().GetUIManager().HideScriptedMenu( m_CharacterMenu );

			delete m_CharacterMenu;
		}

		GetClientMission().SetCanPause( true );
	}
	
	private void SetupCharacterLoading()
	{
		Print("PersistencyModule::SetupCharacterLoading");
		
		m_CharacterIsLoaded = false;
		m_CanBeSaved = false;
		
		GetClientMission().SetCanPause( false );

		if ( GetPlayer() )
		{
			GetPlayer().SimulateDeath( false );

			if ( m_sCharacter != "" && m_sSave != "" )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove( this.SavePlayer );
				
				CharacterSave.SavePlayer( m_sCharacter, m_sSave, GetPlayer() );
			}
		}

		GetGame().GetUIManager().CloseMenu( MENU_INGAME );

		m_Scene = new COMPersistencyScene;
	}

	void OpenCharacterLoading()
	{
		Print("PersistencyModule::OpenCharacterLoading");

		CleanupCharacterMenu();
		
		CleanupScene();

		SetupCharacterLoading();
		
		m_CharacterMenu = new COMCharacterMenu( this, true );

		GetGame().GetUIManager().ShowScriptedMenu( m_CharacterMenu , NULL );
	}

	void TemporaryFix_ReloadCharacterMenu()
	{
		Print("PersistencyModule::TemporaryFix_ReloadCharacterMenu");

		bool isLoadingSave = true;

		if ( m_CharacterMenu )
		{
			isLoadingSave = m_CharacterMenu.IsLoadingSave();
		}

		CleanupCharacterMenu();

		m_CharacterMenu = new COMCharacterMenu( this, isLoadingSave );

		GetGame().GetUIManager().ShowScriptedMenu( m_CharacterMenu , NULL );
	}

	private void CleanupCharacterSaving()
	{
		if (m_CharacterSave)
		{
			m_CharacterSave.Close();
			
			GetGame().GetUIManager().HideScriptedMenu( m_CharacterSave );

			delete m_CharacterSave;
		}
	}
	
	#ifdef MODULE_OVERRIDEMENUS
	void OpenCharacterSaving()
	{
		bool foundWidget = false;

		m_InGameMenu = GetGame().GetUIManager().FindMenu(MENU_INGAME);

		if ( m_InGameMenu.IsInherited( CustomInGameMenu ) )
		{
			if (CustomInGameMenu.Cast( m_InGameMenu ).m_RightPanel.FindAnyWidgetById(MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER_PANEL))
			{
				foundWidget = true;
			}
		}

		if ( m_CharacterSave || m_CharacterSaveWidget || foundWidget )
		{
			if ( m_InGameMenu.IsInherited( CustomInGameMenu ) && m_CharacterSaveWidget )
			{
				m_CharacterSaveWidget.Unlink();
				delete m_CharacterSaveWidget;
			} else {
				GetGame().GetUIManager().HideScriptedMenu( m_CharacterSave );
			}

			delete m_CharacterSave;
		} else {
			m_CharacterSave = new COMCharacterSave( this );

			if ( m_InGameMenu.IsInherited( CustomInGameMenu ) )
			{
				m_CharacterSaveWidget = m_CharacterSave.InitWithParent( CustomInGameMenu.Cast( m_InGameMenu ).m_RightPanel );
				m_CharacterSaveWidget.SetUserID(MODULE_PERSISTENCY_WIDGET_SAVE_CHARACTER_PANEL);
			} else {
				GetGame().GetUIManager().ShowScriptedMenu( m_CharacterSave , m_InGameMenu );
			}
		}
	}
	#else
	void OpenCharacterSaving()
	{
		m_InGameMenu = GetGame().GetUIManager().FindMenu(MENU_INGAME);

		if ( m_CharacterSave || m_CharacterSaveWidget ) 
		{
			GetGame().GetUIManager().HideScriptedMenu( m_CharacterSave );

			delete m_CharacterSave;
		} else 
		{
			m_CharacterSave = new COMCharacterSave( this );

			GetGame().GetUIManager().ShowScriptedMenu( m_CharacterSave , m_InGameMenu );
		}
	}
	#endif
	
	void SavePlayer(string sSave) 
	{
		Print("PersistencyModule::SavePlayer");

		if ( m_CanBeSaved ) {
			if ( sSave != "latest" )
			{
				m_sSave = sSave;
			}
			CharacterSave.SavePlayer( m_sCharacter, m_sSave, GetPlayer() );
		}
	}

	void CreatePlayer(string sCharacter, PlayerBase oPlayer)
	{
		Print("PersistencyModule::CreatePlayer");

		m_CanBeSaved = false;

		MakeDirectory(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter);

		m_sCharacter = sCharacter;
		m_sSave = "latest";

		CharacterSave.CreatePlayer( m_sCharacter, oPlayer );
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CreatePlayerInt, 100, false);
	}

	void LoadLast()
	{
		Print("PersistencyModule::LoadLast");

		m_CanBeSaved = false;
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CreatePlayerInt, 100, false);
	}

	void LoadPlayer(string sCharacter, string sSave = "latest")
	{
		Print("PersistencyModule::LoadPlayer");

		m_CanBeSaved = false;

		m_sCharacter = sCharacter;
		m_sSave = sSave;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CreatePlayerInt, 100, false);
	}

	private void CreatePlayerInt()
	{
		Print("PersistencyModule::CreatePlayerInt");

		PlayerBase oPlayer;

		if ( m_sCharacter == "" )
		{
			oPlayer = CreateCustomDefaultCharacter();
		} else 
		{
			if ( m_sSave == "" )
			{
				m_sSave = "latest";
			}

			oPlayer = CharacterLoad.LoadPlayer( m_sCharacter, m_sSave, false );
		}

		GetGame().SelectPlayer( NULL, oPlayer );

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.FinishLoadingInt, 100, false);
	}

	private void FinishLoadingInt()
	{
		Print("PersistencyModule::FinishLoadingInt");

        GetMission().GetHud().Show(true);

		if ( m_sCharacter != "" && m_sSave != "" )
		{
			GetPlayer().MessageStatus("Loaded character \'" + m_sCharacter + "\' with save \'" + m_sSave + "\'");
		}
		
		m_CanBeSaved = true;
		m_CharacterIsLoaded = true;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.SavePlayer, 1000, true, "latest");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CleanupCharacterMenu, 100, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CleanupScene, 150, false);
		
		GetGame().GetUIManager().ScreenFadeOut( 0.5 );
	}
}