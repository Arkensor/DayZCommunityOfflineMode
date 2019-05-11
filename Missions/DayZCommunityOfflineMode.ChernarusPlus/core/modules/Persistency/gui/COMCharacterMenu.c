class COMCharacterMenu extends UIScriptedMenu
{
	ref TStringArray						m_CharGenderList;
	ref TStringArray						m_CharPersonalityMaleList;
	ref TStringArray						m_CharPersonalityFemaleList;
	ref TStringArray						m_CharShirtList;
	ref TStringArray 						m_CharPantsList;
	ref TStringArray 						m_CharShoesList;
	
	ref TStringArray 						m_Characters;
	ref TStringArray 						m_Saves;

	CharGender 								m_CharGender;

	int										m_CharTypeIndex;
	int 									m_CharShirtIndex;
	int 									m_CharPantsIndex;
	int 									m_CharShoesIndex;

	protected ref PersistencyModule 		m_oPersistencyModule;

	protected Widget						m_CharacterRotationFrame;
	protected ButtonWidget					m_Apply;
	protected ButtonWidget					m_NewCharacter;
	protected ButtonWidget					m_Cancel;

	protected ButtonWidget					m_PrevCharacter;
	protected TextWidget					m_CharacterText;
	protected ButtonWidget					m_NextCharacter;

	protected EditBoxWidget					m_PlayerName;
	protected TextWidget					m_ActionTitle;
	protected TextWidget					m_Version;

	protected ref OptionSelectorMultistate	m_SaveSelector;

	protected ref OptionSelectorMultistate	m_GenderSelector;
	protected ref OptionSelectorMultistate	m_SkinSelector;
	protected ref OptionSelectorMultistate	m_TopSelector;
	protected ref OptionSelectorMultistate	m_BottomSelector;
	protected ref OptionSelectorMultistate	m_ShoesSelector;

    protected bool                          m_IsLoadingSave;
	protected bool 							m_CanLoadSave;

	protected int							m_Character;
    protected int 							m_Save;
	protected bool							m_NoSaves;

    void COMCharacterMenu( ref PersistencyModule oPersistencyModule, bool isLoadSave = true )
	{
		PersistencyPrint("COMCharacterMenu::COMCharacterMenu");
		m_oPersistencyModule = oPersistencyModule;

		m_Characters = new ref TStringArray;
		m_Saves = new ref TStringArray;

		m_CharGenderList = new ref TStringArray;
		m_CharPersonalityMaleList = new ref TStringArray;
		m_CharPersonalityFemaleList = new ref TStringArray;
		m_CharShirtList = new ref TStringArray;
		m_CharPantsList = new ref TStringArray;
		m_CharShoesList = new ref TStringArray;

		m_Saves.Insert("N/A");

		InitCharacterCreationData();

        m_IsLoadingSave = isLoadSave;
		m_NoSaves = true;
	}

    void ~COMCharacterMenu()
    {
		PersistencyPrint("COMCharacterMenu::~COMCharacterMenu");
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove( this.UpdateInterval );

		delete m_CharGenderList;
		delete m_CharPersonalityMaleList;
		delete m_CharPersonalityFemaleList;
		delete m_CharShirtList;
		delete m_CharPantsList;
		delete m_CharShoesList;

		delete m_Saves;
		delete m_Characters;

		delete m_SaveSelector;
		delete m_GenderSelector;
		delete m_SkinSelector;
		delete m_TopSelector;
		delete m_BottomSelector;
		delete m_ShoesSelector;

		g_Game.SetKeyboardHandle( NULL );
    }

	CharGender GenerateRandomGender()
	{
		PersistencyPrint("COMCharacterMenu::GenerateRandomGender");
		int chance = Math.RandomInt(0, 2);
		if (chance == 0)
		{
			m_CharGender = CharGender.MALE;
		} else {
			m_CharGender = CharGender.FEMALE;
		}
		return m_CharGender;
	}

	void GenerateRandomCharacterType()
	{
		PersistencyPrint("COMCharacterMenu::GenerateRandomCharacterType");
		if (m_CharGender == CharGender.FEMALE)
		{
			m_CharTypeIndex = m_CharPersonalityFemaleList.GetRandomIndex();
		} else {
			m_CharTypeIndex = m_CharPersonalityMaleList.GetRandomIndex();
		}
	}

	string CharacterTypeIndexToString()
	{
		PersistencyPrint("COMCharacterMenu::CharacterTypeIndexToString");
		if (m_CharGender == CharGender.FEMALE)
		{
			return m_CharPersonalityFemaleList.Get( m_CharTypeIndex );
		} else {
			return m_CharPersonalityMaleList.Get( m_CharTypeIndex );
		}
	}

	void GenerateRandomCharacter()
	{		
		PersistencyPrint("COMCharacterMenu::GenerateRandomCharacter");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		GenerateRandomGender();
		GenerateRandomCharacterType();

		m_oPersistencyModule.GetScene().SetCharacter( CharacterTypeIndexToString() );

		m_oPersistencyModule.GetScene().SetAttachment(m_CharShirtList.GetRandomElement(), InventorySlots.BODY);
		m_oPersistencyModule.GetScene().SetAttachment(m_CharPantsList.GetRandomElement(), InventorySlots.LEGS);
		m_oPersistencyModule.GetScene().SetAttachment(m_CharShoesList.GetRandomElement(), InventorySlots.FEET);

		m_oPersistencyModule.GetScene().ResetIntroCamera();
	}

	// Need to read from a list later on. 
	void InitCharacterCreationData()
	{
		PersistencyPrint("COMCharacterMenu::InitCharacterCreationData");
		string character_CfgName;
		string root_path = "cfgCharacterCreation";
		
		GetGame().ConfigGetTextArray(root_path + " gender", m_CharGenderList);
		GetGame().ConfigGetTextArray(root_path + " top", m_CharShirtList);
		GetGame().ConfigGetTextArray(root_path + " bottom", m_CharPantsList);
		GetGame().ConfigGetTextArray(root_path + " shoe", m_CharShoesList);

		TStringArray allTypes = GetGame().ListAvailableCharacters();
		for (int i = 0; i < allTypes.Count(); i++)
		{
			character_CfgName = allTypes.Get(i);
			if (GetGame().IsKindOf(character_CfgName, "SurvivorMale_Base"))
			{
				m_CharPersonalityMaleList.Insert(character_CfgName);
			}
			else
			{
				m_CharPersonalityFemaleList.Insert(character_CfgName);
			}
		}
	}
	
	override Widget Init()
	{
		PersistencyPrint("COMCharacterMenu::Init");
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\layouts\\COMCharacterMenu.layout" );

		m_CharacterRotationFrame = layoutRoot.FindAnyWidget( "character_rotation_frame" );

        m_Apply = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_NewCharacter = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "new_character" ) );
		m_Cancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel" ) );

		m_PrevCharacter = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "prev_character" ) );
		m_CharacterText = TextWidget.Cast( layoutRoot.FindAnyWidget( "character_text" ) );
		m_NextCharacter = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "next_character" ) );
        
        m_PlayerName = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "general_name_setting_text" ) );
		m_ActionTitle = TextWidget.Cast( layoutRoot.FindAnyWidget( "CharacterCreationTextWidget" ) );
		m_Version = TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
        string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );

		m_CharacterText.SetText( GetCharacter() );

		m_SaveSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_save_setting_option" ), 0, null, false, m_Saves );

		m_GenderSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_CharGenderList );
		m_GenderSelector.SetValue( m_CharGender );

		ref TStringArray typeList = NULL;
		if ( m_CharGender == CharGender.FEMALE )
		{
			typeList = m_CharPersonalityFemaleList;
		} else
		{
			typeList = m_CharPersonalityMaleList;
		}

		m_SkinSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, typeList);

		m_TopSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_top_setting_option" ), 0, null, false, m_CharShirtList );
		m_BottomSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_bottom_setting_option" ), 0, null, false, m_CharPantsList );
		m_ShoesSelector = new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_shoes_setting_option" ), 0, null, false, m_CharShoesList );

		m_SaveSelector.m_OptionChanged.Insert( SaveChanged );
		m_GenderSelector.m_OptionChanged.Insert( GenderChanged );
		m_SkinSelector.m_OptionChanged.Insert( SkinChanged );
		m_TopSelector.m_OptionChanged.Insert( TopChanged );
		m_BottomSelector.m_OptionChanged.Insert( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Insert( ShoesChanged );

		SetCharacter( 0 );

		return layoutRoot;
	}

    bool IsLoadingSave()
    {
		PersistencyPrint("COMCharacterMenu::IsLoadingSave");
        return m_IsLoadingSave;
    }

	void SetOptions()
	{
		PersistencyPrint("COMCharacterMenu::SetOptions");
		if (m_IsLoadingSave)
		{	
			m_ActionTitle.SetText( "SELECT SAVE" );
			m_NewCharacter.SetText( "NEW CHARACTER" );

			layoutRoot.FindAnyWidget( "prev_character" ).Show( true );
			layoutRoot.FindAnyWidget( "next_character" ).Show( true );

			layoutRoot.FindAnyWidget( "character_content" ).Show( false );
			layoutRoot.FindAnyWidget( "character_save" ).Show( true );

			if (m_NoSaves)
			{
				layoutRoot.FindAnyWidget( "character_save_setting" ).Show( false );
				layoutRoot.FindAnyWidget( "character_save_setting_disable" ).Show( true );
			} else 
			{
				layoutRoot.FindAnyWidget( "character_save_setting" ).Show( true );
				layoutRoot.FindAnyWidget( "character_save_setting_disable" ).Show( false );
			}
		} else 
		{
			m_ActionTitle.SetText( "CREATE NEW CHARACTER" );
			m_NewCharacter.SetText( "SELECT SAVE" );

			m_CharacterText.SetText( "CHARACTER DETAILS" );

			layoutRoot.FindAnyWidget( "prev_character" ).Show( false );
			layoutRoot.FindAnyWidget( "next_character" ).Show( false );

			layoutRoot.FindAnyWidget( "character_content" ).Show( true );
			layoutRoot.FindAnyWidget( "character_save" ).Show( false );
		}

		if ( m_oPersistencyModule.GetLoadedCharacter() == "" || m_oPersistencyModule.GetLoadedSave() == "" )
		{
			layoutRoot.FindAnyWidget( "cancel" ).Show( false );
		} else 
		{
			layoutRoot.FindAnyWidget( "cancel" ).Show( true );
		}

		if ( m_CanLoadSave )
		{
			layoutRoot.FindAnyWidget( "new_character" ).Show( true );
		} else 
		{
			layoutRoot.FindAnyWidget( "new_character" ).Show( false );
		}
	}
    
    override bool UseMouse()
	{
		return true;
	}

	override bool UseKeyboard()
	{
		return true;
	}

	void Cancel()
	{
        m_oPersistencyModule.LoadLast();
	}

	void Apply()
	{
		PersistencyPrint("COMCharacterMenu::Apply");

		if ( GetPlayer() && !GetGame().IsMultiplayer() )
		{
			// GetPlayer().Delete();
		}

		if ( m_IsLoadingSave )
		{
			LoadSave();
		} else 
		{
			CreateCharacter();
		}
	}

	void LoadSave()
	{
		PersistencyPrint("COMCharacterMenu::LoadSave");
		string character = GetCharacter();
		string save = GetSave();

		if ( m_NoSaves || character == "" || save == "" )
		{
			GetGame().GetUIManager().ShowDialog("FAILURE", "No save found for this character!", 0, DBT_OK, DBB_OK, DMT_WARNING, NULL);
			return
		}

		CameraTool.CAMERA_SMOOTH_BLUR = 0.4;

		SetLoadingText("Loading Character");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_oPersistencyModule.LoadPlayer, 200, false, character, save);
	}

	void CreateCharacter()
	{
		PersistencyPrint("COMCharacterMenu::CreateCharacter");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		string characterName = m_PlayerName.GetText();

		if ( characterName == "" )
		{
			GetGame().GetUIManager().ShowDialog("FAILURE", "You didn't give a character name!", 0, DBT_OK, DBB_OK, DMT_WARNING, NULL);
			return;
		}

		if ( FileExist(BASE_PLAYER_SAVE_DIR + "\\" + characterName) )
		{
			GetGame().GetUIManager().ShowDialog("FAILURE", "That character already exists!", 0, DBT_OK, DBB_OK, DMT_WARNING, NULL);
			return;
		}

		CameraTool.CAMERA_SMOOTH_BLUR = 0.4;

		SetLoadingText("Creating Character");

		m_oPersistencyModule.GetScene().GetPlayerUnit().SetPosition( GetSpawnPoints().GetRandomElement() );

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_oPersistencyModule.CreatePlayer, 200, false, characterName, m_oPersistencyModule.GetScene().GetPlayerUnit());
	}

	void NewCharacter()
	{
		PersistencyPrint("COMCharacterMenu::NewCharacter");
		m_IsLoadingSave = !m_IsLoadingSave;
		
		SetCharacter(m_Character);
	}

	void UpdateCreatorSelectionsFromScene()
	{
		PersistencyPrint("COMCharacterMenu::UpdateCreatorSelectionsFromScene");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		m_GenderSelector.SetValue( m_CharGender );

		m_SkinSelector.SetValue( CharacterTypeIndexToString(), true );
		m_TopSelector.SetValue( m_CharShirtIndex, true );
		m_BottomSelector.SetValue( m_CharPantsIndex, true );
		m_ShoesSelector.SetValue( m_CharShoesIndex, true );
	}

	void PreviousCharacter()
	{
		PersistencyPrint("COMCharacterMenu::PreviousCharacter");
		m_Character--;
		SetCharacter(m_Character);
	}

	void NextCharacter()
	{
		PersistencyPrint("COMCharacterMenu::NextCharacter");
		m_Character++;
		SetCharacter(m_Character);
	}

	void SetCharacter( int index )
	{
		PersistencyPrint("COMCharacterMenu::SetCharacter");
		if ( m_IsLoadingSave )
		{
			if ( index < 0 ) index = m_Characters.Count() - 1;
			if ( index >= m_Characters.Count() ) index = 0;
			m_Character = index;

			m_CharacterText.SetText( GetCharacter() );

			SetSaveList();

			if ( !m_Saves || m_Saves.Count() == 0 ) {
				m_NoSaves = true;
			} else {
				m_NoSaves = false;
			}

			if ( !m_NoSaves )
			{
				m_Save = 0;
				m_SaveSelector.LoadNewValues( m_Saves, m_Save );

				SetSave();
			}
		} else 
		{	
			UpdateCreatorSelectionsFromScene();
		}
	}

	void SetSave()
	{
		PersistencyPrint("COMCharacterMenu::SetSave");
		string character = GetCharacter();
		string save = GetSave();

		if ( character == "" || save == "" )
		{
			return;
		}

		if ( m_oPersistencyModule.GetScene() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_oPersistencyModule.GetScene().LoadFromSave, 100, false, character, save );
		}
	}

	bool needsFileLoading = true;
    
    void UpdateInterval()
	{
		if ( needsFileLoading )
		{
			SetCharacterList();
			SetSaveList();
			
			SetCharacter( 0 );

			if ( m_oPersistencyModule.GetScene() )
			{
				m_oPersistencyModule.GetScene().SetupScene();
			}

			needsFileLoading = false;
		}

		if ( m_oPersistencyModule.GetScene() )
		{
        	m_oPersistencyModule.GetScene().Update();
		}

		SetOptions();
	}
    
    override void OnShow()
	{
		PersistencyPrint( "COMCharacterMenu::OnShow" );

		// GetGame().GetUIManager().CloseMenu( MENU_INGAME );

		super.OnShow();

		if ( GetPlayer() && !GetGame().IsMultiplayer() )
		{
			GetGame().SelectPlayer( NULL, NULL );
		}

		g_Game.SetKeyboardHandle( this );

		needsFileLoading = true;

        GetGame().GetInput().ChangeGameFocus( 1, INPUT_DEVICE_MOUSE );
        GetGame().GetUIManager().ShowUICursor( true );

        GetMission().GetHud().Show(false);

		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.UpdateInterval);

		PersistencyPrint( "Finished COMCharacterMenu::OnShow" );
	}

	/*
	override bool CanCloseOnEscape()
	{
		return false;
	}
	*/
    
    override void OnHide()
	{
		PersistencyPrint("COMCharacterMenu::OnHide");
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetInput().ResetGameFocus( INPUT_DEVICE_MOUSE );
        
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);


		super.OnHide();
	}

	void TemporaryFix_ReloadCharacterMenu()
	{
		PersistencyPrint( "COMCharacterMenu::TemporaryFix_ReloadCharacterMenu" );
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_oPersistencyModule.TemporaryFix_ReloadCharacterMenu, 100, false);

		GetGame().GetInput().ChangeGameFocus( 1, INPUT_DEVICE_MOUSE );
        GetGame().GetUIManager().ShowUICursor( true );

        GetMission().GetHud().Show(false);
		PersistencyPrint( "Finished COMCharacterMenu::TemporaryFix_ReloadCharacterMenu" );
	}

/*
	// This is all temporary! Waiting on a feature from the developers which should be implemented sometime soon!
    override bool OnKeyDown( Widget w, int x, int y, int key )
	{
		super.OnKeyDown( w, x, y, key);
		
		switch(key)
		{
			case KeyCode.KC_ESCAPE:
			{
				// TemporaryFix_ReloadCharacterMenu();
				return true;
			}
		}
		
		return false;
	}
*/

	override bool OnClick( Widget w, int x, int y, int button )
	{
		PersistencyPrint("COMCharacterMenu::OnClick");
		if( w == m_Apply )
		{
			Apply();
			return true;
		}
		else if ( w == m_NewCharacter )
		{
			NewCharacter();
			return true;
		}
		else if ( w == m_Cancel )
		{
			Cancel();
			return true;
		}
		else if ( w == m_NextCharacter )
		{
			NextCharacter();
			return true;
		}
		else if ( w == m_PrevCharacter )
		{
			PreviousCharacter();
			return true;
		}
		return false;
	}
    
    override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		PersistencyPrint("COMCharacterMenu::OnMouseButtonDown");
		if ( w == m_CharacterRotationFrame )
		{
			if ( m_oPersistencyModule.GetScene() )
				m_oPersistencyModule.GetScene().CharacterRotationStart();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		PersistencyPrint("COMCharacterMenu::OnMouseButtonUp");
		if ( m_oPersistencyModule.GetScene() )
			m_oPersistencyModule.GetScene().CharacterRotationStop();
		return false;
	}

	override bool OnMouseEnter( Widget w, int x, int y )
	{
		PersistencyPrint("COMCharacterMenu::OnMouseEnter");
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		PersistencyPrint("COMCharacterMenu::OnMouseLeave");
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		PersistencyPrint("COMCharacterMenu::OnFocus");
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		PersistencyPrint("COMCharacterMenu::OnFocusLost");
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		PersistencyPrint("COMCharacterMenu::IsFocusable");
		return ( w == m_Apply || w == m_NewCharacter || w == m_Cancel || w == m_PrevCharacter || w == m_NextCharacter );
	}

	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		PersistencyPrint("COMCharacterMenu::ColorRed");
		SetFocus( w );

		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		PersistencyPrint("COMCharacterMenu::ColorWhite");
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}

	bool IsValidSave( string name, FileAttr attributes )
    {
        PersistencyPrint( "Found: " + BASE_PLAYER_SAVE_DIR  + "\\" + GetCharacter() + "\\" + name + " as a " + FileAttributeToString( attributes ) );

		string extenstion = ".json";
		int strLength = name.Length();

		if ( name == extenstion ) return false;

        if ( (attributes & FileAttr.DIRECTORY ) ) return false;

        if ( name == "" ) return false;

		PersistencyPrint( "Loaded" );

        return true;
    }

	bool IsValidCharacter( string name, FileAttr attributes )
    {
        PersistencyPrint( "Found: " + BASE_PLAYER_SAVE_DIR  + "\\" + name + " as a " + FileAttributeToString( attributes ) );

        if ( ! (attributes & FileAttr.DIRECTORY ) ) return false;

        if ( name == "" ) return false;

        return true;
    }

	void SetSaveList()
	{
		PersistencyPrint("COMCharacterMenu::SetSaveList");
		m_Saves.Clear();

		if ( !IsValidCharacter( GetCharacter(), FileAttr.DIRECTORY ) || !m_CanLoadSave ) return;

		string sName = "";
		FileAttr oFileAttr = FileAttr.INVALID;
		FindFileHandle oFileHandle = FindFile(BASE_PLAYER_SAVE_DIR + "\\" + GetCharacter() + "\\*.json", sName, oFileAttr, FindFileFlags.ALL);

		if (sName != "")
		{
			if ( IsValidSave( sName, oFileAttr ) )
			{
				m_Saves.Insert(sName.Substring(0, sName.Length() - 5));
			}

			while (FindNextFile(oFileHandle, sName, oFileAttr))
			{
				if ( IsValidSave( sName, oFileAttr ))
				{
					m_Saves.Insert(sName.Substring(0, sName.Length() - 5));
				}
			}
		}
	}

	void SetCharacterList()
	{
		PersistencyPrint("COMCharacterMenu::SetCharacterList");
		m_Characters.Clear();

		string sName = "";
		FileAttr oFileAttr = FileAttr.INVALID;
		FindFileHandle oFileHandle = FindFile(BASE_PLAYER_SAVE_DIR + "\\*", sName, oFileAttr, FindFileFlags.ALL);

		int index = 0;

		if (sName != "")
		{
			if ( IsValidCharacter( sName, oFileAttr ))
			{
				m_Characters.Insert(sName);
				index++;
			}

			while (FindNextFile(oFileHandle, sName, oFileAttr))
			{
				if ( IsValidCharacter( sName, oFileAttr ) )
				{
					m_Characters.Insert(sName);
					index++;
				}
			}
		}

		if ( index == 0 ) 
		{
			m_IsLoadingSave = false;
			m_CanLoadSave = false;
		} else {
			m_CanLoadSave = true;
		}
	}

	string GetCharacter()
	{
		PersistencyPrint("COMCharacterMenu::GetCharacter");
		if ( !m_Characters || m_Characters.Count() == 0 ) return "";

		return m_Characters.Get( m_Character );
	}

	string GetSave()
	{
		PersistencyPrint("COMCharacterMenu::GetSave");
		if ( !m_Saves || m_Saves.Count() == 0 ) return "";

		return m_Saves.Get( m_Save );
	}

	void SaveChanged()
	{
		PersistencyPrint("COMCharacterMenu::SaveChanged");
		m_Save = m_SaveSelector.GetValue();

		SetSave();
	}

	void SetGender( string gender )
	{
		PersistencyPrint("COMCharacterMenu::SetGender");
		if ( m_oPersistencyModule.GetScene() )
		{
			switch ( gender )
			{
				case "Female":
				{
					m_CharGender = CharGender.FEMALE;
					m_SkinSelector.LoadNewValues( m_CharPersonalityFemaleList, 0 );
					break;
				}
				case "Male":
				{
					m_CharGender = CharGender.MALE;
					m_SkinSelector.LoadNewValues( m_CharPersonalityMaleList, 0 );
					break;
				}
			}
		}
		m_SkinSelector.SetRandomValue();
	}

	void GenderChanged()
	{
		PersistencyPrint("COMCharacterMenu::GenderChanged");
		SetGender( m_GenderSelector.GetStringValue() );
	}
	
	void SkinChanged()
	{
		PersistencyPrint("COMCharacterMenu::SkinChanged");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		m_oPersistencyModule.GetScene().SetCharacter( m_SkinSelector.GetStringValue() );
		
		TopChanged();
		BottomChanged();
		ShoesChanged();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_oPersistencyModule.GetScene().ResetIntroCamera, 100 );
	}
	
	void TopChanged()
	{		
		PersistencyPrint("COMCharacterMenu::TopChanged");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		m_oPersistencyModule.GetScene().SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
	}
	
	void BottomChanged()
	{		
		PersistencyPrint("COMCharacterMenu::BottomChanged");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		m_oPersistencyModule.GetScene().SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
	}
	
	void ShoesChanged()
	{
		PersistencyPrint("COMCharacterMenu::ShoesChanged");
		if ( !m_oPersistencyModule.GetScene() )
		{
			return;
		}

		m_oPersistencyModule.GetScene().SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
	}

	override int GetID()
	{
		return MENU_MAIN;
	}
}