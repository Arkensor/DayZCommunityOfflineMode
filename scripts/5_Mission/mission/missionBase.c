class NotificationMessage
{
	const float FADE_TIMEOUT = 5;
	const float FADE_OUT_DURATION = 1;
	const float FADE_IN_DURATION = 0.5;

	Widget m_notification_widget;

	TextWidget m_text_widget;
	private TextWidget m_notification_label;
	private TextWidget m_notification_text;

	private ref WidgetFadeTimer m_fade_timer;
	private ref Timer m_timeout_timer;

	void NotificationMessage( Widget root_widget )
	{
		m_notification_widget = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/notification_info.layout", (Widget)root_widget.FindAnyWidget( "NotificationFrameWidget" ) );
		m_notification_label = TextWidget.Cast( root_widget.FindAnyWidget( "NoticiationLabellTextWidget" ) );
		m_notification_text = TextWidget.Cast( root_widget.FindAnyWidget( "NotificationMultilineTextWidget" ) );
		m_notification_widget.Show(false);

		m_fade_timer = new WidgetFadeTimer;
		m_timeout_timer = new Timer(CALL_CATEGORY_GUI);
	}

	void ~NotificationMessage()
	{
		/*if (m_notification_widget) 
		{
			m_notification_widget.Destroy();
			m_notification_widget = NULL;
		}*/
	}

	void Show( string label, string text, int priority )
	{
		// disabled for now
		// waiting for new message scheduler at application side
		//   (ask [rennerale] for details if really needed)

		bool do_show = false;
		/*
		switch( priority )
		{
		case EVENT_PRIORITY_ERROR:
			m_notification_label.SetColor( COLOR_RED );
			m_notification_text.SetColor( COLOR_RED );
			break;
		case EVENT_PRIORITY_WARNING:
			m_notification_label.SetColor( COLOR_YELLOW );
			m_notification_text.SetColor( COLOR_YELLOW );
			break;
		case EVENT_PRIORITY_INFO:
			m_notification_label.SetColor( COLOR_GREEN );
			m_notification_text.SetColor( COLOR_GREEN );
			break;
		 default:
			do_show = false;
			// log about bad data here ...
			break;
		}
		*/
		if ( do_show )
		{
			if ( label != "" )
			{
				m_notification_label.SetText( label );
			}
			if ( text != "" )	
			{
				m_notification_text.SetText(text);
			}
			m_fade_timer.FadeIn( m_notification_widget, FADE_IN_DURATION);
			m_timeout_timer.Run(FADE_TIMEOUT, m_fade_timer, "FadeOut", new Param2<Widget, float>(m_notification_widget, FADE_OUT_DURATION));
		}
	}
	
	void Hide()
	{
		m_notification_widget.Show(false);
		m_timeout_timer.Stop();
		m_fade_timer.Stop();
	}
}

class MissionBase extends Mission
{
	PluginDeveloper 		m_ModuleDeveloper;
	PluginKeyBinding		m_ModuleKeyBinding
	PluginAdditionalInfo	m_ModuleServerInfo;
	
	ref NotificationMessage m_notification_widget;
	
	ref array<PlayerBase> m_DummyPlayers = new array<PlayerBase>;
	
	void MissionBase()
	{
		SetDispatcher(new DispatcherCaller);
		
		PluginManagerInit();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CheckNotification, 15000, true);

		//Debug.DestroyAllShapes();

		//TODO clea up after Gamescom
		m_ModuleServerInfo = PluginAdditionalInfo.Cast( GetPlugin(PluginAdditionalInfo) );
		//
	}

	void ~MissionBase()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.CheckNotification);
		PluginManagerDelete();
	}
	
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = NULL;

		switch (id)
		{
		case MENU_MAIN:
#ifdef PLATFORM_XBOX
			menu = new MainMenuXbox;
#else
#ifdef PLATFORM_PS4
			menu = new MainMenuXbox;
#else
			menu = new MainMenu;
#endif
#endif	
			break;
		case MENU_INGAME:
#ifdef PLATFORM_XBOX
			menu = new InGameMenuXbox;
#else
#ifdef PLATFORM_PS4
			menu = new InGameMenuXbox;
#else
			menu = new InGameMenu;
#endif
#endif
			break;
		case MENU_CHARACTER:
			menu = new CharacterMenu;
			break;
		case MENU_OPTIONS:
			menu = new OptionsMenu;
			break;
		case MENU_ASSIGNMENT:
			menu = new AssignmentMenu;
			break;
		case MENU_STARTUP:
			menu = new StartupMenu;
			break;
		case MENU_LOADING:
			menu = new LoadingMenu;
			break;
		case MENU_INVENTORY:
		if(!GetGame().IsOldInventory())
		{
			menu = new InventoryMenuNew;
		}
		else
		{
			menu = new InventoryMenu;
		}
			break;
		case MENU_INSPECT:
			menu = new InspectMenuNew;
			break;
		case MENU_EARLYACCESS:
			menu = new EarlyAccessMenu;
			break;
		case MENU_SCRIPTCONSOLE:
			menu = new ScriptConsole;
			break;
		case MENU_SCRIPTCONSOLE_DIALOG_PRESET_NAME:
			menu = new ScriptConsoleNewPresetDialog;
			break;
		case MENU_SCRIPTCONSOLE_DIALOG_PRESET_RENAME:
			menu = new ScriptConsoleRenamePresetDialog;
			break;
		case MENU_CHAT_INPUT:
			menu = new ChatInputMenu;
			break;
		case MENU_CONTROLS_PRESET:
			menu = new PresetsMenu;
			break;
		case MENU_NOTE:
			menu = new NoteMenu;
			break;
		case MENU_MAP:
			menu = new MapMenu;
			break;
		case MENU_BOOK:
			menu = new BookMenu;
			break;
		case MENU_SCENE_EDITOR:
			menu = new SceneEditorMenu;
			break;
		case MENU_HELP_SCREEN:
			menu = new HelpScreen;
			break;
		case MENU_GESTURES:
			menu = new GesturesMenu;
			break;	
		case MENU_LOGOUT:
			menu = new LogoutMenu;
			break;
		case MENU_TITLE_SCREEN:
			menu = new TitleScreenMenu;
			break;
		case MENU_XBOX_CONTROLS:
			menu = new ControlsXbox;
			break;
		case MENU_RADIAL_QUICKBAR:
			menu = new RadialQuickbarMenu;
			break;			
		}

		if (menu)
		{
			menu.SetID(id);
		}

		return menu;
	}

	override UIScriptedWindow CreateScriptedWindow(int id)
	{
		UIScriptedWindow window = NULL;

		switch (id)
		{
		case GUI_WINDOW_MISSION_LOADER:
			window = new MissionLoader( GUI_WINDOW_MISSION_LOADER );
			break;
		}

		return window;
	}

	void SpawnItems()
	{
		/*vector player_pos = "2558 16 2854";
		if (g_Game.GetPlayer())
		{
			g_Game.GetPlayer().GetPosition();
		}

		Print(player_pos);
		
		Print("Spawning items.");
		ref TStringArray items = new TStringArray;
		items.Insert("Hoodie_Blue");
		items.Insert("WoolCoat_Red"); 
		items.Insert("Raincoat_Orange"); 
		items.Insert("PressVest_Blue"); 
		items.Insert("Gorka_pants_summer"); 
		items.Insert("MilitaryBoots_Black");
		items.Insert("WoodAxe");
		items.Insert("Container_FirstAidKit");
		items.Insert("Consumable_DuctTape");
		items.Insert("Fruit_AppleFresh");
		items.Insert("ItemBookHunger");
		
		for (int i = 0; i < items.Count(); i++)
		{
			string item = items.Get(i);
			vector item_pos = player_pos;
			int x = i % 4;
			int z = i / 4;
			item_pos[0] = item_pos[0] + (x * 1);
			item_pos[2] = item_pos[2] + (z * 1);
			g_Game.CreateObject(item, item_pos, false);
		}*/
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnKeyPress(key);
			}
		}
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnKeyRelease(key);
			}
		}
	}
	
	override void OnMouseButtonPress(int button)
	{
		super.OnMouseButtonPress(button);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnMouseButtonPress(button);
			}
		}
	}
	
	override void OnMouseButtonRelease(int button)
	{
		super.OnMouseButtonRelease(button);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnMouseButtonRelease(button);
			}
		}
	}

	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		/*
		switch(eventTypeId)
		{
		case ScriptLogEventTypeID:
			if ( GetGame().IsDebug() )
			{
				Param1<string> log_params = params;
				OnLog(log_params.param1);
			}
			
			break;
		}
		*/
	}

	void OnLog(string msg_log)
	{
		if ( PluginDeveloper.GetInstance() )
		{
			if ( GetGame().IsServer() && !GetGame().IsMultiplayer() )
			{
				PluginDeveloper.GetInstance().PrintLogClient(msg_log);
			}
			else if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
			{
				PluginDeveloper.GetInstance().PrintLogClient(msg_log);
			}
		}
	}
	
	void CheckNotification()
	{
		// disabled for now
		// waiting for new message scheduler at application side
		//   (ask [rennerale] for details if really needed)
		/*
		if ( GetGame().CheckHiveEvents() )
		{
			string notification_message;
			int notification_value;
			int notification_priority
			GetGame().GetLastHiveEvent( notification_message, notification_value, notification_priority );
			notification_message = String( notification_message + " " + notification_value.ToString() + " " + notification_priority.ToString() );
			ShowNotification( "NOTIFICATION", notification_message, notification_priority );  //uncomment to test notification widget
		}
		*/
	}

	void ShowNotification( string label, string text, int priority)
	{
		if ( m_notification_widget )
		{
			m_notification_widget.Show( label, text, priority );
		}
	}
	
		
	void UpdateDummyScheduler()
	{
		for(int i = 0; i < m_DummyPlayers.Count(); i++)
		{
			if (m_DummyPlayers.Get(i))
			{
				m_DummyPlayers.Get(i).OnTick();
			}
		}
	}
	
	override void AddDummyPlayerToScheduler(Man player)
	{
		m_DummyPlayers.Insert(PlayerBase.Cast( player ));
	}
	
}

class MissionDummy extends MissionBase
{
	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		if (key == KeyCode.KC_Q)
		{
			// PlayerBase player = GetGame().GetPlayer();
		}
	}
}
