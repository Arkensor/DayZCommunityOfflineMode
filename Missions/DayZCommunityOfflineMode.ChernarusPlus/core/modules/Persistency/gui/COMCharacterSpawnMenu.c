class COMCharacterSpawnMenu extends UIScriptedMenu
{
	protected TextListboxWidget m_CharacterList;
	protected TextListboxWidget m_SaveList;

	protected ButtonWidget m_CloseButton;

	protected ButtonWidget m_SpawnButton;
	protected ButtonWidget m_CancelButton;

	protected ButtonWidget m_CreateNewButton;
	protected EditBoxWidget m_NewCharacterName;

	protected ref map< string, ref array<string> > m_Saves = new map< string, ref array<string> >;

	protected string m_sCharacterName = "default";
    protected string m_sSaveName = "latest";

	protected ref PersistencyModule m_oPersistencyModule;

	protected ref COMCharacterCreationMenu m_CharacterCreationMenu;

	void COMCharacterSpawnMenu(ref PersistencyModule oPersistencyModule)
	{
		m_oPersistencyModule = oPersistencyModule;

		string sName = "";
		FileAttr oFileAttr = FileAttr.INVALID;
		FindFileHandle oFileHandle = FindFile(BASE_PLAYER_SAVE_DIR + "\\*", sName, oFileAttr, FindFileFlags.ALL);

		Print("File Attributes: INVALID " + FileAttr.INVALID + " DIRECTORY " + FileAttr.DIRECTORY + " HIDDEN " + FileAttr.HIDDEN + " READONLY " + FileAttr.READONLY);

		Print("Character: " + sName + ", File Attr: " + oFileAttr);

		if (sName != "")
		{
			if (sName != m_sCharacterName)
			{
				m_Saves.Set(sName, new array<string>);
			}

			while (FindNextFile(oFileHandle, sName, oFileAttr))
			{
				if (sName != m_sCharacterName)
				{
					Print("Character: " + sName + ", File Attr: " + oFileAttr);
					m_Saves.Set(sName, new array<string>);
				}
			}
		}
	}

	void ~COMCharacterSpawnMenu()
	{
		Close();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\layouts\\COMCharacterSpawnMenu.layout" );

		m_CharacterList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_character_list") );
		m_SaveList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_save_list") );

		m_SpawnButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_spawn") );
		m_NewCharacterName = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("txt_new_character_name") );
		m_CreateNewButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_createNew") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_cancel") );

		m_CloseButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("close_button") );

		for ( int nIndex = 0; nIndex < m_Saves.Count(); nIndex++ )
        {
            m_CharacterList.AddItem( m_Saves.GetKey( nIndex ), NULL, 0 );
        }

		m_NewCharacterName.SetText("Enter New Name");

		return layoutRoot;
	}

	override void OnShow()
	{
	    super.OnShow();
	}

	override void OnHide()
	{
		super.OnHide();
	}

	override bool UseMouse()
	{
		return true;
	}

	override bool UseKeyboard()
	{
		return true;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
        if( key == KeyCode.KC_ESCAPE )
        {
			Close();
        }

		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_CreateNewButton)
		{
			if (!m_CharacterCreationMenu) {
				m_CharacterCreationMenu = new COMCharacterCreationMenu(m_oPersistencyModule, this);
			}

			if (!m_CharacterCreationMenu.IsVisible()) {
				GetGame().GetUIManager().ShowScriptedMenu( m_CharacterCreationMenu , NULL );
			}

			Close();

			return true;
		}
		if ( w == m_SpawnButton )
		{
			if (m_sCharacterName == "" || m_sSaveName == "") return false;

			Close();

			m_oPersistencyModule.SavePlayer();
			m_oPersistencyModule.LoadPlayer(m_sCharacterName, m_sSaveName);
			
			return true;
		}
		else if ( w == m_CancelButton || w == m_CloseButton )
		{
			Close();

			return true;
		}

		return false;
	}

	void PopulateSavedList(string sCharacter) {
		ref array<string> oSaves = m_Saves.Get(sCharacter);
		string sName = "";
		FileAttr oFileAttr = FileAttr.INVALID;
		FindFileHandle oFileHandle = FindFile(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\*.json", sName, oFileAttr, FindFileFlags.ALL);

		if (sName != "")
		{
			oSaves.Clear();

			if (sName != ".json")
			{
				oSaves.Insert(sName.Substring(0, sName.Length() - 5));
			}

			while (FindNextFile(oFileHandle, sName, oFileAttr))
			{
				if (sName != "" && sName != ".json")
				{
					oSaves.Insert(sName.Substring(0, sName.Length() - 5));
				}
			}
		}

		m_SaveList.ClearItems();

		for ( int nIndex = 0; nIndex < oSaves.Count(); nIndex++ )
        {
            m_SaveList.AddItem( oSaves.Get( nIndex ), NULL, 0 );
        }
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w == m_CharacterList )
		{
			if ( GetCurrentCharacterName() != m_sCharacterName)
			{
				if( m_Saves.Contains( GetCurrentCharacterName() ) )
				{
					m_sCharacterName = GetCurrentCharacterName();
				}

				PopulateSavedList(m_sCharacterName);
			}
		}

		if ( w == m_SaveList )
		{
			if (GetCurrentSaveName() != m_sSaveName && m_sCharacterName != "")
			{
				if( m_Saves.Get(m_sCharacterName).Find( GetCurrentSaveName() ) )
				{
					m_sSaveName = GetCurrentSaveName();
				}
			}
		}

		Print("TEST2: " + m_sCharacterName + " | " + m_sSaveName);

		return true;
	}

	string GetCurrentCharacterName()
	{
		if ( m_CharacterList.GetSelectedRow() != -1 )
		{
			string sCharacterName;
			m_CharacterList.GetItemText( m_CharacterList.GetSelectedRow(), 0, sCharacterName );
			return sCharacterName;
		}

		return "";
	}

    string GetCurrentSaveName()
	{
		if ( m_SaveList.GetSelectedRow() != -1 )
		{
			string sSaveName;
			m_SaveList.GetItemText( m_SaveList.GetSelectedRow(), 0, sSaveName );
			return sSaveName;
		}

		return "";
	}
}