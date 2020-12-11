class COMCharacterSaveEventHandler extends ScriptedWidgetEventHandler
{
	ref COMCharacterSave m_CharacterSave;

	void COMCharacterSaveEventHandler( ref COMCharacterSave characterSave )
	{
		Print( "COMCharacterSaveEventHandler::COMCharacterSaveEventHandler" );
		m_CharacterSave = characterSave;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		Print( "COMCharacterSaveEventHandler::OnClick" );
		return m_CharacterSave.OnClick( w, x, y, button );
	}

	override bool OnMouseEnter( Widget w, int x, int y )
	{
		Print( "COMCharacterSaveEventHandler::OnMouseEnter" );
		return m_CharacterSave.OnMouseEnter( w, x, y );
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		Print( "COMCharacterSaveEventHandler::OnMouseLeave" );
		return m_CharacterSave.OnMouseLeave( w, enterW, x, y );
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		Print( "COMCharacterSaveEventHandler::OnFocus" );
		return m_CharacterSave.OnFocus( w, x, y );
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		Print( "COMCharacterSaveEventHandler::OnFocusLost" );
		return m_CharacterSave.OnFocusLost( w, x, y );
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		Print( "COMCharacterSaveEventHandler::OnModalResult" );
		return m_CharacterSave.OnModalResult( w, x, y, code, result );
	}
}

class COMCharacterSave extends UIScriptedMenu
{
	protected ref PersistencyModule 			m_oPersistencyModule;
	protected ref COMCharacterSaveEventHandler 	m_COMCharacterSaveEventHandler;

	protected ButtonWidget						m_Apply;
	protected EditBoxWidget						m_SaveText;

    void COMCharacterSave( ref PersistencyModule oPersistencyModule )
	{
		m_oPersistencyModule = oPersistencyModule;

		m_COMCharacterSaveEventHandler = new COMCharacterSaveEventHandler(this);
	}

    void ~COMCharacterSave()
    {
    }

	Widget InitWithParent( Widget parent )
	{
		Print( "COMCharacterSave::InitWithParent" );
		string layoutFile = "$CurrentDir:missions\\DayZCommunityOfflineMode.Namalsk\\core\\modules\\Persistency\\gui\\layouts\\COMCharacterSave.layout";
		if ( parent )
		{
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( layoutFile, parent );
		}
		else {
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( layoutFile );
		}
		layoutRoot.SetHandler( m_COMCharacterSaveEventHandler );

        m_Apply								= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "character_save_button_button" ) );

		m_SaveText 							= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "general_save_setting_text" ) );

        return layoutRoot;
	}
	
	override Widget Init()
	{
		Print( "COMCharacterSave::Init" );
		return InitWithParent( NULL );
    }

	void Save()
	{
		Print( "COMCharacterSave::Save" );

		bool canSave = false;

		string saveName = m_SaveText.GetText();
		string characterName = m_oPersistencyModule.GetLoadedCharacter();

		if ( saveName != "" )
		{
			if ( !FileExist(BASE_PLAYER_SAVE_DIR + "\\" + characterName + "\\" + saveName + ".json") )
			{
				canSave = true;
			} else {
				GetGame().GetUIManager().ShowDialog("WARNING", "Do you wish to overwrite your current save \"" + saveName + "\"?", 10, DBT_YESNO, DBB_NO, DMT_WARNING, this);
				canSave = false;
			}
		} else {
			canSave = false;
		}

		if ( canSave )
		{
			m_oPersistencyModule.SavePlayer(saveName);
			COM_GetMission().Continue();
		}
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);

		if ( code == 10 && result == DBB_YES )
		{
			m_oPersistencyModule.SavePlayer(m_SaveText.GetText());
			COM_GetMission().Continue();
			return true;
		}

		return false;
	}

	bool IsFocusable( Widget w )
	{
		return ( w == m_Apply );
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( w == m_Apply )
		{
			Save();
			return true;
		}
		return false;
	}

	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		return false;
	}

	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
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

}