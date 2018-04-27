class LoadingScreen
{
	ref Widget m_WidgetRoot;
	Widget m_FrameWidgetSmall;
	Widget m_FrameWidgetLarge;
	ProgressBarWidget m_ProgressBarSmall;
	ProgressBarWidget m_ProgressBarLarge;
	ProgressBarWidget m_ProgressBar;
	TextWidget m_TextWidgetTitle;
	TextWidget m_TextWidgetError;
	ImageWidget m_ImageWidgetBackground;

	int m_Counter;
	
	void LoadingScreen(DayZGame game)
	{
		m_WidgetRoot = game.GetWorkspace().CreateWidgets("gui/layouts/loading.layout");
		m_FrameWidgetSmall = m_WidgetRoot.FindAnyWidget("ProgressBarFrameSmall");
		m_FrameWidgetLarge = m_WidgetRoot.FindAnyWidget("ProgressBarFrameLarge");
		Class.CastTo(m_ProgressBarSmall, m_WidgetRoot.FindAnyWidget("ProgressBarWidgetSmall"));
		Class.CastTo(m_ProgressBarLarge, m_WidgetRoot.FindAnyWidget("ProgressBarWidgetLarge"));
		Class.CastTo(m_TextWidgetTitle, m_WidgetRoot.FindAnyWidget("TextWidget"));
		Class.CastTo(m_TextWidgetError, m_WidgetRoot.FindAnyWidget("ErrorText"));
		Class.CastTo(m_ImageWidgetBackground, m_WidgetRoot.FindAnyWidget("ImageBackground"));
	
		m_ProgressBar = m_ProgressBarLarge;
		m_FrameWidgetSmall.Show( false );
		m_ImageWidgetBackground.LoadImageFile( 0, GetRandomLoadingBackground() );
		m_ImageWidgetBackground.Show( false );
		m_ProgressBar.SetCurrent(0);
		m_Counter = 0;
	}
	
	void Inc()
	{
		m_Counter++;
	
		if ( m_Counter == 1 )
		{
			Show();
		}	
	}
	
	void Dec()
	{
		m_Counter = m_Counter - 1;
	
		if ( m_Counter <= 0 )
		{
			m_WidgetRoot.Show( false );
			m_Counter = 0;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( g_Game.CheckDialogs );
		}
	}
	
	bool IsLoading()
	{
		if ( m_WidgetRoot.IsVisible() )
		{
			return true;
		}
	
		return false;
	}

	void SetTitle( string title )
	{
		m_TextWidgetTitle.SetText( title );
	}
	
	void HideTitle()
	{
		m_TextWidgetTitle.Show(false);
	}
	
	void ShowTitle()
	{
		m_TextWidgetTitle.Show(true);
	}
	
	void SetError( string error )
	{
		Print("Received DB Error: " + error);
		m_TextWidgetError.SetText( error );
	}
	
	void SetProgress( float val )
	{
		if ( m_Counter > 0 )
		{
			m_ProgressBar.SetCurrent( val * 100 );
		}
	}
	
	void OnUpdate( float timeslice )
	{
		
	}
	
	void Show()
	{
		if ( !m_WidgetRoot.IsVisible() )
		{
			if ( g_Game.GetUIManager().IsDialogVisible() )
			{
				g_Game.GetUIManager().HideDialog();
			}
			
			if ( g_Game.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU )
			{
				m_ProgressBar = m_ProgressBarSmall;
				m_FrameWidgetSmall.Show( true );
				m_FrameWidgetLarge.Show( false );
				m_ImageWidgetBackground.Show( true );		
				m_TextWidgetError.Show(true);
			}
			else
			{
				m_ProgressBar = m_ProgressBarLarge;
				m_FrameWidgetSmall.Show( false );
				m_FrameWidgetLarge.Show( true );
				m_ImageWidgetBackground.Show( false );
				m_TextWidgetError.Show(false);
			}
			
			m_WidgetRoot.Show( true );
			m_ProgressBar.SetCurrent( 0 );
			m_TextWidgetTitle.SetText( "" );
			m_TextWidgetError.SetText( "" );
		}
	}
	
	void Hide()
	{
		if ( m_Counter <= 0 && m_WidgetRoot.IsVisible() )
		{
			m_WidgetRoot.Show( false );
		}
	}
};

class ConnectionLost
{
	private ref Widget m_WidgetRoot;
	private TextWidget m_TextWidgetTitle;
	private float m_duration;
		
	void ConnectionLost(DayZGame game)
	{		
		m_WidgetRoot = game.GetWorkspace().CreateWidgets("gui/layouts/day_z_connection_lost.layout");
		m_WidgetRoot.Show(false);
		
		Class.CastTo(m_TextWidgetTitle, m_WidgetRoot.FindAnyWidget("TextWidget"));
		m_duration = 0.0;
	}
	
	void Show()
	{
		if ( !m_WidgetRoot.IsVisible() )
		{
			if ( g_Game.GetUIManager().IsDialogVisible() )
			{
				g_Game.GetUIManager().HideDialog();
			}
			
			m_WidgetRoot.Show( true );
			m_TextWidgetTitle.SetText( "" );
		}
	}
	
	void Hide()
	{
		if ( m_WidgetRoot.IsVisible() )
		{
			m_WidgetRoot.Show( false );
		}
	}
	
	void SetText(string text)
	{
		m_TextWidgetTitle.SetText(text);
	}
	
	float GetDuration()
	{
		return m_duration;
	}
	
	void SetDuration(float duration)
	{
		m_duration = duration;
	}
};

typedef Param2<string, bool> DayZProfilesOption;
class DayZProfilesOptions
{
	private ref map<EDayZProfilesOptions, ref DayZProfilesOption> m_DayZProfilesOptions;
	private DayZGame m_Game

	void RegisterProfileOption(EDayZProfilesOptions option, string profileOptionName, bool def = true)
	{
		if ( m_DayZProfilesOptions == NULL )
		{
			m_DayZProfilesOptions = new map<EDayZProfilesOptions, ref DayZProfilesOption>;
		}
		
		if ( !m_DayZProfilesOptions.Contains(option) )
		{
			//! init of DayZProfilesOption - profileOptionName, value from Profiles files, or use default value
			bool profileVal = GetGame().GetProfileValueBool(profileOptionName, def);

			m_DayZProfilesOptions.Set(option, new DayZProfilesOption(profileOptionName, profileVal));
			SetProfileOption(option, profileVal);
		}
	}
	
	void SetProfileOption(EDayZProfilesOptions option, bool value)
	{
		//Print("SetProfileOption");
		if ( m_DayZProfilesOptions && m_DayZProfilesOptions.Contains(option) )
		{
			DayZProfilesOption po = m_DayZProfilesOptions.Get(option);
			
			//! saves value to mem and to config
			po.param2 = value;
			GetGame().SetProfileValueBool(po.param1, value);
			GetGame().SaveProfile();
		}	
	}

	bool GetProfileOption(EDayZProfilesOptions option)
	{
		if ( m_DayZProfilesOptions && m_DayZProfilesOptions.Contains(option) )
		{
			DayZProfilesOption po = m_DayZProfilesOptions.Get(option);
			
			//! returns saved profile option
			return po.param2;
		}
		
		return true;
	}
	
	map<EDayZProfilesOptions, ref DayZProfilesOption> GetProfileOptionMap()
	{
		if ( m_DayZProfilesOptions )
		{			
			//! returns whole options map
			return m_DayZProfilesOptions;
		}
		return NULL;
	}
}

class DayZGame extends CGame
{
	const int MISSION_STATE_MAINMENU = 0;
	const int MISSION_STATE_GAME = 1;
	const int MISSION_STATE_FINNISH = 2;
	
	private int m_MissionState;

	ref LoadingScreen m_loading;
	private ref ConnectionLost m_connectionLost;
	private ref TimerQueue m_timerQueue[CALL_CATEGORY_COUNT];
	private ref ScriptCallQueue m_callQueue[CALL_CATEGORY_COUNT];
	private ref ScriptInvoker m_updateQueue[CALL_CATEGORY_COUNT];
	private ref DragQueue m_dragQueue;
	private ref DayZProfilesOptions m_DayZProfileOptions;
	private bool m_early_access_dialog_accepted;
	private UIScriptedMenu m_keyboard_handler;
	private ScriptModule m_mission_module;
	private string 	m_MissionPath;
	private string 	m_MissionFolderPath;
	private bool	m_IsCtrlHolding;
	private bool	m_IsWinHolding;
	private bool	m_IsLeftAltHolding;
	private bool	m_IsRightAltHolding;
	
	private int 	m_queueTime;
	bool			m_loadingScreenOn;
	int 			m_OriginalCharactersCount;
	private string 	m_PlayerName;
	private bool 	m_IsNewCharacter;
	private float	m_UserFOV;
	
	ref TIntArray demounit = new TIntArray;

	// CGame override functions
	void DayZGame()
	{
		m_MissionState = MISSION_STATE_GAME;
		
		m_keyboard_handler = NULL;
		m_early_access_dialog_accepted = IsDebug();
		
		for (int i = 0; i < CALL_CATEGORY_COUNT; i++)
		{
			m_callQueue[i] = new ScriptCallQueue;
			m_updateQueue[i] = new ScriptInvoker;
			m_timerQueue[i] = new TimerQueue;
		}
		
		m_dragQueue = new DragQueue;
		
		m_ParamOldInventory	= false;
		
		m_queueTime 		= 0;	
		
		string tmp;
		if ( IsDebug() )
		{
			if ( CommandlineGetParam("enOldInventory", tmp) )
			{
				m_ParamOldInventory = true;
			}
			
			if ( CommandlineGetParam("enDoNoLogs", tmp) )
			{
				m_ParamDoNoLogs = true;
			}
			
			
		}
		
		/*m_ParamNewMenu= false;
		if ( CommandlineGetParam("enNewMenu", tmp) )
		{
			m_ParamNewMenu = true;
		}*/	
		
	#ifndef NO_GUI	
		m_loading = new LoadingScreen(this);
		m_loading.Show();
	#endif
		
		Debug.Init();
		Component.Init();
		LogTemplates.Init();
		CashedObjectsParams.Init();
		CashedObjectsArrays.Init();

		m_DayZProfileOptions = new DayZProfilesOptions;
		GetCallQueue(CALL_CATEGORY_GUI).Call(DeferredInit);
		//m_isTileSet = true;
	}
	
	// ------------------------------------------------------------
	void ~DayZGame()
	{
		g_Game = NULL;
		SetDispatcher(NULL);
		Print("~DayZGame()");
	}
	
	void DeferredInit()
	{
		RegisterProfilesOptions();
		m_UserFOV = GetUserFOVFromConfig();
	}
	
	void RegisterProfilesOptions()
	{
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.CROSSHAIR, SHOW_CROSSHAIR);
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.HUD , SHOW_HUD);
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.QUICKBAR, SHOW_QUICKBAR);
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.SERVER_MESSAGES, SYSTEM_CHAT_MSG);
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.USERS_CHAT, DIRECT_CHAT_MSG);
		m_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.RADIO_CHAT, RADIO_CHAT_MSG);
	}

	void SetMissionPath(string path)
	{
		m_MissionPath = path;
		
		int pos_end = 0;
		int pos_cur = 0;
				
		while ( pos_cur != -1 )
		{
			pos_end = pos_cur;
			pos_cur = path.IndexOfFrom(pos_cur + 1 , "\\");
		}
		
		m_MissionFolderPath = path.Substring(0, pos_end);	
	}
	
	string GetMissionPath()
	{
		return m_MissionPath;
	}
	
	string GetMissionFolderPath()
	{
		return m_MissionFolderPath;
	}
	
	override ScriptCallQueue GetCallQueue(int call_category)
	{
		return m_callQueue[call_category];
	}

	override ScriptInvoker GetUpdateQueue(int call_category)
	{
		return m_updateQueue[call_category];
	}
	
	override TimerQueue GetTimerQueue(int call_category)
	{
		return m_timerQueue[call_category];
	}

	override DragQueue GetDragQueue()
	{
		return m_dragQueue;
	}

	// ------------------------------------------------------------
	int GetMissionState()
	{
		return m_MissionState;
	}
	
	// ------------------------------------------------------------
	void SetMissionState( int state )
	{
		m_MissionState = state;
	}
	// ------------------------------------------------------------
	bool GetProfileOption(EDayZProfilesOptions option)
	{
		return m_DayZProfileOptions.GetProfileOption(option);
	}
	
	void SetProfileOption(EDayZProfilesOptions option, bool value)
	{
		m_DayZProfileOptions.SetProfileOption(option, value);
	}
	
	map<EDayZProfilesOptions, ref DayZProfilesOption> GetProfileOptionMap()
	{
		return m_DayZProfileOptions.GetProfileOptionMap();
	}
	
	// ------------------------------------------------------------
	override bool IsInventoryOpen()
	{
		if (GetUIManager().FindMenu(MENU_INVENTORY) != NULL)
		{
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	void EarlyAccessDialog(UIScriptedMenu parent)
	{
		if (!m_early_access_dialog_accepted)
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_EARLYACCESS, parent);
			m_early_access_dialog_accepted = true;
		}
	}
	
	// ------------------------------------------------------------
	//! create custom main menu part (submenu)
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
	#ifndef NO_GUI
		Mission mission = GetMission();
		if (mission)
		{
			return mission.CreateScriptedMenu(id);
		}
	#endif
		return NULL;
	}
		
	// ------------------------------------------------------------
	void ReloadMission()
	{
	#ifdef DEVELOPER
		Print("Reloading mission module!");
		CreateMission(m_MissionPath);
	#endif
	}
	
	// ------------------------------------------------------------
	void CancelQueueTime()
	{
		GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ClientSpawning);
	}
	
	// ------------------------------------------------------------
	override void OnEvent(EventType eventTypeId, Param params)
	{
		switch(eventTypeId)
		{
		case StartupEventTypeID:
			break;
		
		//-----------------------------------------------------------------------------
		case MPSessionStartEventTypeID:
			LoadingShow();
			break;
		
		case MPSessionEndEventTypeID:
			CancelQueueTime();
			break;
		
		case MPSessionFailEventTypeID:
			LoadingHide();
			break;
			
		case MPSessionPlayerReadyEventTypeID:
			LoadingHide();
			// turn the lights on
			GetGame().SetEVUser(0);	
		
			// fade out from the black screen
			#ifndef NO_GUI	
			if ( GetUIManager().ScreenFadeVisible() )
			{
				GetUIManager().ScreenFadeOut(0.5);
			}
			#endif
			break;

		//-----------------------------------------------------------------------------
		case MPConnectionLostEventTypeID:
			MPConnectionLostEventParams conLost_params;
			if (Class.CastTo(conLost_params, params))
			{
				int duration = conLost_params.param1;
				OnMPConnectionLostEvent(duration);
			}
			break;
			
		case WorldCleaupEventTypeID:
			LoadingShow();
			break;
		
		case DialogQueuedEventTypeID:
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.CheckDialogs);
			break;
	
		case ChatMessageEventTypeID:
			ChatMessageEventParams chat_params;
			if (Class.CastTo(chat_params, params))
			{
			#ifndef NO_GUI
				if (GetUIManager().ScreenFadeVisible() && (chat_params.param1 == CCSystem || chat_params.param1 == CCStatus))
				{
					GetUIManager().ScreenFadeIn(0, chat_params.param3, FadeColors.BLACK, FadeColors.WHITE);
				}
			#endif
			}
			break;
	
		case ProgressEventTypeID:
			ProgressEventParams prog_params;
			if (Class.CastTo(prog_params, params))
				LoadProgressUpdate(prog_params.param1, prog_params.param2, prog_params.param3);
			break;
	
		case EntityNetworkUpdateEventTypeID:
			//EntityNetworkUpdateEventParams ent_params = params;
			// update entity info
			break;
	
		case ClientSpawningEventTypeID:
			ClientSpawningEventParams spawningParams;
			if (Class.CastTo(spawningParams, params))
			{
				m_queueTime = spawningParams.param1;
				OnClientSpawningEvent(m_queueTime, spawningParams.param2);
			}	
			break;
		case LogoutEventTypeID:
			LogoutEventParams logoutParams;
			if (Class.CastTo(logoutParams, params))
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().StartLogoutMenu, logoutParams.param1);						
			}	
			break;
		case DatabaseErrorEventTypeID:
			DatabaseErrorEventParams databaseErrorParams;
			Class.CastTo(databaseErrorParams, params);
		
			string error = databaseErrorParams.param1;
			if (m_loading)
			{
				m_loading.HideTitle();
				m_loading.SetError(error);
			}
			break;
		}
	
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnEvent(eventTypeId, params);
		}
	}
	
	// ------------------------------------------------------------
	void OnClientSpawningEvent(int queueTime, bool newChar)
	{
		/* NOTE: StoreLoginData must be called for game to continue !!! */
		
		// turn the lights off
		GetGame().SetEVUser(-5);
		// timer for spawning screen
		if (queueTime > 0)
		{
			GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ClientSpawning, 1000, true, newChar);
		}
		else
		{
			// no spawning screen needed
			ClientSpawningFinished(newChar);
		}
	}
	
	// ------------------------------------------------------------
	void ClientSpawning(bool newChar)
	{
		// countdown on the spawning screen
		if (m_queueTime >= 0) // count all the way to zero
		{
		#ifndef NO_GUI	
			string text = "Spawning in " + m_queueTime.ToString() + " seconds...";
			GetUIManager().ScreenFadeIn(0, text, FadeColors.BLACK, FadeColors.WHITE);
		#endif
			m_queueTime--;
		}
		else
		{
			// hide spawning text
			CancelQueueTime();
			ClientSpawningFinished(newChar);
		}
	}
	
	// ------------------------------------------------------------
	void ClientSpawningFinished(bool newChar)
	{		
		// tell game to continue
		StoreLoginData();
		
		//Toggles logo (loading screen)
		Mission mission = GetMission();
		
		if ( mission && m_loadingScreenOn )
		{
			m_loading = new LoadingScreen(this);
			m_loading.Show();
			m_loading.SetTitle("Loading...");
			m_loadingScreenOn = false;
		}

	}
	
	// ------------------------------------------------------------
	// send additional information to server (must be called)
	void StoreLoginData()
	{
		int top = -1;
		int bottom = -1;
		int shoes = -1;
		int skin = -1;
		
		if (demounit.Count() > 0)
		{
			top = demounit.Get(0);
			bottom = demounit.Get(1);
			shoes = demounit.Get(2);
			skin = demounit.Get(3);
		}
		
		ref array<ref Param> params = new array<ref Param>;
			
		ref Param topParam = new Param1<int>(top);
		params.Insert(topParam);
			
		ref Param bottomParam = new Param1<int>(bottom);
		params.Insert(bottomParam);

		ref Param shoesParam = new Param1<int>(shoes);
		params.Insert(shoesParam);
		
		ref Param skinParam = new Param1<int>(skin);
		params.Insert(skinParam);
					
		GetGame().StoreLoginData(params);
	}
	
	// ------------------------------------------------------------
	void OnMPConnectionLostEvent(int duration)
	{
#ifndef NO_GUI
		if( duration < 0 )
		{
			if( m_connectionLost )
				m_connectionLost.Hide();
			
			return;
		}
		
		// initialize new message
		if (!m_connectionLost )
		{
			m_connectionLost = new ConnectionLost(this);
		}
		
		if (duration >= 5)
		{
			// show message
			m_connectionLost.Show();
			
			// update message
			if (m_connectionLost.GetDuration() != duration)
			{
				// use STR_MP_NO_MESSAGE 
				m_connectionLost.SetText("No message received for %1 " + duration.ToString() + " seconds");	
				m_connectionLost.SetDuration(duration);
			}
		}
		else
		{
			// hide unnecessary message
			m_connectionLost.Hide();	
		}
#endif
	}
	
	// ------------------------------------------------------------
	void LoadProgressUpdate(int progressState, float progress, string title)
	{
	#ifndef NO_GUI
		switch(progressState)
		{
		case PROGRESS_START:
			{
			#ifndef NO_GUI	
				// get out of the black screen immediately
				GetUIManager().ScreenFadeOut(0);
			#endif
				m_loading.Inc();
				m_loading.SetTitle(title);
			}
			break;
	
		case PROGRESS_FINISH:
			{
				m_loading.Dec();
			}
			break;
	
		case PROGRESS_PROGRESS:
			{
				m_loading.SetProgress(progress);
			}
			break;
	
		case PROGRESS_UPDATE:
			{
				m_loading.SetProgress(0);
			}
			break;
		}
	#endif
	}
	
	// ------------------------------------------------------------
	override void OnAfterCreate()
	{
		Math.Randomize(-1);
	}
	
	bool IsLeftCtrlDown()
	{
		return m_IsCtrlHolding;
	}
	
	// ------------------------------------------------------------
	override void OnKeyPress(int key)
	{
	
		if ( key == KeyCode.KC_LCONTROL ) 
		{
			m_IsCtrlHolding = true;
		}

		if (key == KeyCode.KC_LMENU)
		{
			m_IsLeftAltHolding = true;
		}
			
		if (key == KeyCode.KC_RMENU)
		{
			m_IsRightAltHolding = true;
		}
	
		if (m_keyboard_handler)
		{
			m_keyboard_handler.OnKeyDown(NULL, 0, 0, key);
		}
		
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnKeyPress(key);
		}
	
#ifdef DEVELOPER
		if ((m_IsLeftAltHolding || m_IsLeftAltHolding) && key == KeyCode.KC_F4)
		{
			RequestExit(0);
		}
#endif
		
	}
	
	// ------------------------------------------------------------
	override void OnKeyRelease(int key)
	{
		if ( key == KeyCode.KC_LCONTROL ) 
		{
			m_IsCtrlHolding = false;
		}
		
		if ( key == KeyCode.KC_LWIN ) 
		{
			m_IsWinHolding = false;
		}

		if (key == KeyCode.KC_LMENU || key == KeyCode.KC_RMENU)
		{
			m_IsLeftAltHolding = false;
		}

		if (key == KeyCode.KC_RMENU)
		{
			m_IsRightAltHolding  = false;
		}

		if (m_keyboard_handler)
		{
			m_keyboard_handler.OnKeyUp(NULL, 0, 0, key);
		}
	
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnKeyRelease(key);
		}
	}
	
	// ------------------------------------------------------------	
	override void OnMouseButtonPress(int button)
	{
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnMouseButtonPress(button);
		}
	}
	
	// ------------------------------------------------------------
	override void OnMouseButtonRelease(int button)
	{
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnMouseButtonRelease(button);
		}
	}
	
	// ------------------------------------------------------------
	override void OnUpdate(bool doSim, float timeslice)
	{
		Mission mission = GetMission();
		bool gameIsRunning = false;
	
		if (doSim && mission && !mission.IsPaused())
		{
			gameIsRunning = true;
		}
	
		if (doSim && mission)
		{
			mission.OnUpdate(timeslice);
		}
	
		// queues and timers update
		GetCallQueue(CALL_CATEGORY_SYSTEM).Tick(timeslice);
		GetUpdateQueue(CALL_CATEGORY_SYSTEM).Invoke(timeslice);
		GetTimerQueue(CALL_CATEGORY_SYSTEM).Tick(timeslice);
	
	#ifndef NO_GUI	
		if (m_loading && m_loading.IsLoading())
		{
			m_loading.OnUpdate(timeslice);
		}
		else
		{
			GetCallQueue(CALL_CATEGORY_GUI).Tick(timeslice);
			GetUpdateQueue(CALL_CATEGORY_GUI).Invoke(timeslice);
			GetTimerQueue(CALL_CATEGORY_GUI).Tick(timeslice);
			GetDragQueue().Tick();
		}
	#endif
	
		if (gameIsRunning)
		{
			GetCallQueue(CALL_CATEGORY_GAMEPLAY).Tick(timeslice);
			GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Invoke(timeslice);
			GetTimerQueue(CALL_CATEGORY_GAMEPLAY).Tick(timeslice);
		}
	}
	
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		//Print("DayZGame::OnRPC");
		if (target)
		{
			// call rpc on target
			target.OnRPC(sender, rpc_type, ctx);
		}
		else
		{
			// global rpc's handling
		}
	}
	
	// ------------------------------------------------------------
	void CheckDialogs()
	{
		#ifndef NO_GUI	
		Mission mission = GetMission();
		if (mission && !m_loading.IsLoading() && GetUIManager().IsDialogQueued())
		{
			mission.Pause();
			GetUIManager().ShowQueuedDialog();
		}
		#endif
	}
	
	// ------------------------------------------------------------
	bool IsLoading()
	{
		if (m_loading && m_loading.IsLoading())
		{
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	void SetKeyboardHandle(UIScriptedMenu handler)
	{
		m_keyboard_handler = handler;
	}
	
	// ------------------------------------------------------------
	void LoadingShow()
	{
		#ifndef NO_GUI	
			m_loading.Show();
		#endif
	}
	
	// ------------------------------------------------------------
	void LoadingHide()
	{
		#ifndef NO_GUI	
			m_loading.Hide();
		#endif
	}

	// ------------------------------------------------------------
	override string CreateDefaultPlayer()
	{				
		string path = "cfgVehicles";
		string child_name = ""; 
		int count = GetGame().ConfigGetChildrenCount ( path );
		//array<string> char_class_names = new array<string>;
		
		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( path, p, child_name );
			
			if (GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf(child_name,"SurvivorBase"))
			{
				//char_class_names.Insert(child_name);
				return child_name;
			}
		}
		return "";
		//return char_class_names.Get(0);
	}
	
	// ------------------------------------------------------------
	override string CreateRandomPlayer()
	{
		string path = "cfgVehicles";
		string child_name = "";
		int count = GetGame().ConfigGetChildrenCount ( path );
		array<string> char_class_names = new array<string>;
		
		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( path, p, child_name );
			
			if (GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf(child_name,"SurvivorBase"))
			{
				char_class_names.Insert(child_name);
			}
		}
		return char_class_names.GetRandomElement();
	}
	
	// ------------------------------------------------------------
	override TStringArray ListAvailableCharacters()
	{
		string path = "cfgVehicles";
		string child_name = "";
		int count = GetGame().ConfigGetChildrenCount ( path );
		array<string> char_class_names = new array<string>;
		
		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( path, p, child_name );
			
			if (GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf(child_name,"SurvivorBase"))
			{
				char_class_names.Insert(child_name);
			}
		}
		return char_class_names;
	}
	
	// ------------------------------------------------------------
	override void SetCharacterInfo(int top,int bottom,int shoes,int characterName)
	{
		if (demounit)	demounit.Clear();
		demounit.Insert(top);
		demounit.Insert(bottom);
		demounit.Insert(shoes);
		demounit.Insert(characterName);
	}
	
	// ------------------------------------------------------------
	void ExplosionEffects(Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		/*
		Print("ExplosionEffects");
		Print("##################################");
		Print(directHit);
		Print(componentIndex);
		Print(surface);
		Print(pos);
		Print(surfNormal);
		Print(energyFactor);
		Print(explosionFactor);
		Print(isWater);
		Print(ammoType);
		*/
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			if ( directHit  &&  !directHit.IsInherited(Man)  &&  !directHit.IsInherited(DayZCreature) )
			{
				Particle p1 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAngl1 = surfNormal.VectorToAngles();
				p1.SetOrientation(surfNormalAngl1);
			}
			else if ( !directHit )
			{
				Particle p2 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAng2 = surfNormal.VectorToAngles();
				p2.SetOrientation(surfNormalAng2);
			}
			else
			{
				Particle p3 = Particle.Play(ParticleList.BLOOD_SPLAT, pos);
				vector surfNormalAng3 = surfNormal.VectorToAngles();
				p3.SetOrientation(surfNormalAng3);
			}
		}
	}
	
	// ------------------------------------------------------------
	void FirearmEffects(Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
		/*
		float inSpeedf = inSpeed.Length();
		float outSpeedf = outSpeed.Length();
	
		Print("FirearmEffects");
		Print("##################################");
		Print(directHit);
		Print(componentIndex);
		Print(surface);
		Print(pos);
		Print(surfNormal);
		Print(exitPos);	
		Print(inSpeedf);
		Print(outSpeedf);
		Print(isWater);
		Print(deflected);
		Print(ammoType);	
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			ImpactMaterials.EvaluateImpactEffect(surface, pos, ImpactTypes.UNKNOWN, surfNormal, exitPos, inSpeed, outSpeed, deflected, ammoType);
		}
		
		*/
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			if ( directHit  &&  !directHit.IsInherited(Man)  &&  !directHit.IsInherited(DayZCreature) )
			{
				Particle p1 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAngl1 = surfNormal.VectorToAngles();
				surfNormalAngl1 = surfNormalAngl1 + "0 270 0";
				p1.SetOrientation(surfNormalAngl1);
			}
			else if ( !directHit )
			{
				Particle p2 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAng2 = surfNormal.VectorToAngles();
				surfNormalAng2 = surfNormalAng2 + "0 270 0";
				p2.SetOrientation(surfNormalAng2);
			}
			else
			{
				Particle p3 = Particle.Play(ParticleList.BLOOD_SPLAT, pos);
				vector surfNormalAng3 = surfNormal.VectorToAngles();
				p3.SetOrientation(surfNormalAng3);
			}
		}
	}
	
	// ------------------------------------------------------------
	void CloseCombatEffects(Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 bool isWater, string ammoType) 
	{
		/*Print("CloseCombatEffects");
		Print("##################################");
		Print(directHit);
		Print(componentIndex);
		Print(surface);
		Print(pos);
		Print(surfNormal);
		Print(isWater);
		Print(ammoType);*/
		
		//ImpactMaterials.EvaluateImpactEffect(surface, pos, ImpactTypes.MELEE, surfNormal, "0 0 0", "0 0 0", "0 0 0", false, ammoType);
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			if ( directHit  &&  !directHit.IsInherited(Man)  &&  !directHit.IsInherited(DayZCreature) )
			{
				Particle p1 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAngl1 = surfNormal.VectorToAngles();
				p1.SetOrientation(surfNormalAngl1);
			}
			else if ( !directHit )
			{
				Particle p2 = Particle.Play(ParticleList.IMPACT_TEST, pos);
				vector surfNormalAng2 = surfNormal.VectorToAngles();
				p2.SetOrientation(surfNormalAng2);
			}
			else
			{
				Particle p3 = Particle.Play(ParticleList.BLOOD_SPLAT, pos);
				vector surfNormalAng3 = surfNormal.VectorToAngles();
				p3.SetOrientation(surfNormalAng3);
			}
		}
	}
	
	void InitCharacterMenuDataInfo(int menudata_count)
	{
		m_OriginalCharactersCount = menudata_count;
	}
	
	void SetPlayerGameName(string name)
	{
		m_PlayerName = name;
	}
	
	string GetPlayerGameName()
	{
		return m_PlayerName;
	}
	
	void SetNewCharacter(bool state)
	{
		m_IsNewCharacter = state;
	}
	
	bool IsNewCharacter()
	{
		return m_IsNewCharacter;
	}
	
	void SetUserFOV(float pFov)
	{
		if ( pFov < OPTIONS_FIELD_OF_VIEW_MIN ) pFov = OPTIONS_FIELD_OF_VIEW_MIN;
		if ( pFov > OPTIONS_FIELD_OF_VIEW_MAX ) pFov = OPTIONS_FIELD_OF_VIEW_MAX;
		
		m_UserFOV = pFov;
	}
	
	float GetUserFOV()
	{
		return m_UserFOV;
	}

	//! TODO: find some better way of accessing options
	static float GetUserFOVFromConfig()
	{
		if (g_Game.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU || g_Game.GetMissionState() == DayZGame.MISSION_STATE_GAME)
		{
			OptionsAccess optionAccess;
			GameOptions gameOptions = new GameOptions;
		
			int c = gameOptions.GetOptionsCount();
		
			for (int i = 0; i < c; i++)
			{
				if ( gameOptions.GetOption(i).GetAccessType() == AT_OPTIONS_FIELD_OF_VIEW )
				{
					optionAccess = gameOptions.GetOption(i);
				}
			}

			NumericOptionsAccess noa = NumericOptionsAccess.Cast(optionAccess);
		
			return noa.ReadValue();
		}
		
		return -1.0;
	}
};


DayZGame g_Game;

DayZGame GetDayZGame()
{
	return g_Game;
}