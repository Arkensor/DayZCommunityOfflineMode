class MainMenu extends UIScriptedMenu
{
	protected ref WidgetFadeTimer 		m_fade_timer;
	protected DayZIntroScene 			m_scene;
	ref TextWidget 						m_name_widget;
	ref PlayerNameHandler 				m_player_name_handler;

	protected TextListboxWidget 		m_GlobalStatsTextListBox;
	protected ref Timer 				m_statisticsUpdateTimer;
	protected Widget 					m_connection_warning_widget;
	protected bool 						m_statisticsLoadedSuccessfully;
	
	const string 						NEWS_FEED_DATA = "Scripts/Data/news_feed.txt";
	
	const int 							ID_FEEDBACK_LINK 	= 150;
	const int 							ID_FORUMS_LINK 		= 151;
	const int 							ID_TWITTER_LINK 	= 152;
	const int 							ID_YOUTUBE_LINK 	= 153;
	
	//============================================
	// MainMenu
	//============================================
	void MainMenu()
	{
		m_fade_timer = new WidgetFadeTimer;
		m_statisticsUpdateTimer = new Timer(CALL_CATEGORY_GUI);
		m_statisticsLoadedSuccessfully = false;
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_MainMenu.layout");
		m_player_name_handler = new PlayerNameHandler;
		
		string version;
		GetGame().GetVersion( version );
		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("VersionText") );
		if (version_widget)		version_widget.SetText("#main_menu_version " + version);

		m_GlobalStatsTextListBox = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("GlobalStatisticsTextListbox") );

		MissionMainMenu mission = MissionMainMenu.Cast( g_Game.GetMission() );
		m_scene = mission.GetIntroScene();
		
		m_name_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("CharacterNameText") );
		m_name_widget.SetText(g_Game.GetPlayerGameName());
		
		//news feed
		string news_feed_text;
		GetNewsFeedData( news_feed_text );
		RichTextWidget news_feed_widget = RichTextWidget.Cast( layoutRoot.FindAnyWidget( "NewsFeedRichtext" ) );
		news_feed_widget.SetText( news_feed_text );
		
		GetGame().GetUIManager().ScreenFadeOut(0);
		PPEffects.ResetAll();
		// NotificationMessage m_notification_widget = new NotificationMessage( layoutRoot ); 
		return layoutRoot;
	}
	
	//============================================
	// Events
	//============================================	
	override void OnShow()
	{
		//super.OnShow();
		
		//g_Game.EarlyAccessDialog(this);

		g_Game.PlayMission( "$CurrentDir:\\missions\\Arkensor_DayZSP.ChernarusPlus" );
		
		//if (!m_statisticsLoadedSuccessfully && m_statisticsUpdateTimer.IsRunning() == false) UpdateStats(); // hotfix: on steamversion calling of StatInvokeUpdate freeze the game for 15+ seconds everytime player enters main menu
		
		//refresh player name
		m_name_widget.SetText(g_Game.GetPlayerGameName());
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick( w, x, y, button);
		
		if (m_scene && m_scene.m_disableClick)
		{
			return false;
		}
		
		if (w.GetName() == "RandomCharacter")
		{
			m_scene.m_disableClick = true;
			m_scene.ChangeCharacter(-1);
			
			//GetGame().GetPlayerName(m_scene.m_player_name);
			m_name_widget.SetText(g_Game.GetPlayerGameName());
		}
		
		if (w.GetUserID() != IDC_MAIN_PLAY)
		{
			switch (w.GetUserID())
			{
				case IDC_MAIN_NEXT_CHARACTER:
					m_scene.SaveCharName();
					m_scene.ChangeCharacter(m_scene.NextCharacterID());
					if (m_scene.m_demoUnit)
					{
						m_scene.m_disableClick = true;
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_scene.SceneCharacterSetPos, 250);
						//GetGame().GetPlayerName(m_scene.m_player_name);
						m_name_widget.SetText(g_Game.GetPlayerGameName());
					}
					return true;
				break;
				
				case IDC_MAIN_PREV_CHARACTER:
					m_scene.SaveCharName();
					m_scene.ChangeCharacter(m_scene.PrevCharacterID());
					if (m_scene.m_demoUnit) 
					{
						m_scene.m_disableClick = true;
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_scene.SceneCharacterSetPos, 250);
						//GetGame().GetPlayerName(m_scene.m_player_name);
						m_name_widget.SetText(g_Game.GetPlayerGameName());
					}
					return true;
				break;
				
				case IDC_MAIN_CHARACTER:
					EnterScriptedMenu(MENU_CHARACTER);
					return true;
				break;
				
				case IDC_MAIN_MULTIPLAYER:
					m_scene.SaveCharName();
					g_Game.GetUIManager().EnterServerBrowser(this);
					//saves demounit for further use
					if (m_scene && m_scene.m_demoUnit && m_scene.m_demoUnit.GetInventory().FindAttachment(InventorySlots.BODY) && m_scene.CurrentCharacterID() == -1)		m_scene.SetCharacterInfo();
					return true;
				break;
		
				case IDC_MAIN_OPTIONS:
					EnterScriptedMenu(MENU_OPTIONS);
					return true;
				break;
		
				case IDC_MAIN_QUIT:
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
				break;
			}
		}
		else if (w.GetUserID() == IDC_MAIN_PLAY)
		{
			if (m_scene && m_scene.m_demoUnit)
			{
				//saves demounit for further use
				if (m_scene.m_demoUnit.GetInventory().FindAttachment(InventorySlots.BODY) && m_scene.CurrentCharacterID() == -1)		m_scene.SetCharacterInfo();
				m_scene.SaveCharName();
				if (!g_Game.IsNewCharacter()) 		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectLastSession");
				else 								GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectBestServer");
			}
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if ( w.GetName() == "CharacterRotationFrame" )
		{
			if (m_scene) m_scene.CharacterRotationStart(); //m_scene.CameraRotationStart();
			return true;
		}
		
		if ( w.GetUserID() == ID_FEEDBACK_LINK )
		{
			GetGame().OpenURL( "https://feedback.bistudio.com/tag/dayz" );
			
			return true;
		}

		if ( w.GetUserID() == ID_FORUMS_LINK )
		{
			GetGame().OpenURL( "https://forums.dayz.com" );
			
			return true;
		}
		
		if ( w.GetUserID() == ID_TWITTER_LINK )
		{
			GetGame().OpenURL( "https://twitter.com/dayzdevteam" );
			
			return true;
		}
		
		if ( w.GetUserID() == ID_YOUTUBE_LINK )
		{
			GetGame().OpenURL( "https://www.youtube.com/user/DayZDevTeam" );
			
			return true;
		}
		
		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		if ( m_scene ) 
		{
			m_scene.CharacterRotationStop(); //m_scene.CameraRotationStop();
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseEnter(	Widget w, int x, int y )
	{
		super.OnMouseEnter( w, x, y );
		
		if ( w.GetUserID() >= 150 && w.GetUserID() <= 153 )
		{
			w.SetColor( ARGB( 255, 255, 0, 0 ) );		//red
			
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseLeave(	Widget w, Widget enterW, int x, int y )
	{
		super.OnMouseLeave( w, enterW, x, y );
		
		if ( w.GetUserID() >= 150 && w.GetUserID() <= 153 )
		{
			w.SetColor( ARGB( 255, 255, 255, 255 ) );	//white
			
			return true;
		}
		
		return false;
	}	
	
	/*override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		super.OnEvent(eventType, target, parameter0, parameter1)	
	}*/

	//============================================
	// DATA
	//============================================
	void ConnectLastSession()
	{
		//TODO fix code-side
		if ( !g_Game.ConnectLastSession(this, m_scene.CurrentCharacterID()) )
		{
			g_Game.GetUIManager().EnterServerBrowser(this);
		}
	}
	
	void ConnectBestServer()
	{
		//TODO add functionality! For now works as follows:
		ConnectLastSession();
	}
	
	void UpdateStats()
	{
		m_GlobalStatsTextListBox.ClearItems();

		// NEW STATS API
		if ( m_scene && m_scene.m_demoUnit )
		{
			string lvalue = "";
			string lname = "";
			
			lname = "Play time";
			m_scene.m_demoUnit.StatGetAsTime("playtime", lvalue);
			int line_index = m_GlobalStatsTextListBox.AddItem(lname, NULL, 0);
			m_GlobalStatsTextListBox.SetItem(line_index, lvalue, NULL, 1);
			
			string debug_string = "";
			float fvalue = m_scene.m_demoUnit.StatGet("playtime");
			debug_string = "playtime: " + fvalue.ToString();
			//Print(debug_string);
			
			lname = "Distance traveled";
			m_scene.m_demoUnit.StatGetCounter("dist", lvalue);
			line_index = m_GlobalStatsTextListBox.AddItem(lname, NULL, 0);
			m_GlobalStatsTextListBox.SetItem(line_index, lvalue, NULL, 1);
			
			debug_string = "dist: " + lvalue;
			//Print(debug_string);
			
			m_statisticsLoadedSuccessfully = true;
		}
	}
	
	//TODO
	//HOTFIX - server browser cursor is missing for some reason
	override void OnHide()
	{
		//super.OnHide();
	}
	
	//news feed
	void GetNewsFeedData( out string news_data )
	{
		FileHandle file_handle = OpenFile( NEWS_FEED_DATA, FileMode.READ );
		string line_content;
		
		while( FGets( file_handle, line_content ) > 0 )
		{
			news_data += line_content;
		}
		
		CloseFile( file_handle );
	}
}
