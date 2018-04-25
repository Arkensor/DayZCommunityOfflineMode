enum DemoPlayerClasses
{
	HUNTER 		= 0,
	SOLDIER 	= 1,
	POLICEMAN 	= 2,
	VETERAN 	= 3
}

enum DemoWeaterType
{
	NO_RAIN_NO_OVERCAST 	= 0,
	NO_RAIN_MED_OVERCAST 	= 1,
	NO_RAIN_MAX_OVERCAST 	= 2,
	MED_RAIN_NO_OVERCAST 	= 3,
	MED_RAIN_MED_OVERCAST 	= 4,
	MED_RAIN_MAX_OVERCAST 	= 5,
	MAX_RAIN_NO_OVERCAST 	= 6,
	MAX_RAIN_MED_OVERCAST 	= 7,
	MAX_RAIN_MAX_OVERCAST 	= 8
}

enum DemoTimeOfDay
{
	MORNING 	= 0,
	NOON 		= 1,
	AFTERNOON 	= 2,
	EVENING 	= 3,
	NIGHT 		= 4
}

class jsonXboxConnectParams
{
	string m_IP;
	int m_Port;
	string m_Password;
}

class MainMenuXbox extends UIScriptedMenu
{
	protected ref WidgetFadeTimer 	m_fade_timer;
	protected DayZIntroScene 		m_scene;
	bool 							m_disableClick;

	//character
	/*
	protected bool 					m_female;
	protected string 				m_format;
	protected int					m_skinIndex;
	protected int					m_topIndex;
	protected int					m_pantsIndex;
	protected int					m_shoesIndex;
	ref TStringAdvanceArray 		m_genderList;
	ref TStringAdvanceArray 		m_personalityMaleList;
	ref TStringAdvanceArray		 	m_personalityFemaleList;
	ref TStringAdvanceArray 		m_allCharacters;
	ref TStringAdvanceArray 		m_shirtList;
	ref TStringAdvanceArray 		m_pantsList;
	ref TStringAdvanceArray		 	m_shoesList;
	*/
	//

	//game parameters
	protected ref array<string> 	m_ListDemoClasses;
	protected ref array<string> 	m_ListDemoWeather;
	protected ref array<string>		m_ListDemoTimeOfDay;
	//
	protected XComboBoxWidget m_WidgetDemoClasses;
	protected XComboBoxWidget m_WidgetDemoWeather;
	protected XComboBoxWidget m_WidgetDemoTimeOfDay;
	
	//UI ids
	const int COMBO_DEMO_PLAYER_CLASS 	= 1;
	const int COMBO_DEMO_WEATHER 		= 2;
	const int COMBO_DEMO_TIME_OF_DAY 	= 3;
	const int BUTTON_PLAY_ONLINE		= 200;	
	void MainMenuXbox()
	{
		m_fade_timer = new WidgetFadeTimer;
		
		//character
		/*
		m_genderList = new TStringAdvanceArray;
		m_personalityMaleList = new TStringAdvanceArray;
		m_personalityFemaleList = new TStringAdvanceArray;
		m_allCharacters = new TStringAdvanceArray;
		m_shirtList = new TStringAdvanceArray;
		m_pantsList = new TStringAdvanceArray;
		m_shoesList = new TStringAdvanceArray;
		*/
		//
		
		//game parameters
		m_ListDemoClasses = new array<string>;
		m_ListDemoClasses.Insert( "Hunter" );
		m_ListDemoClasses.Insert( "Soldier" );
		m_ListDemoClasses.Insert( "Policeman" );
		m_ListDemoClasses.Insert( "Veteran" );
		
		m_ListDemoWeather = new array<string>;
		m_ListDemoWeather.Insert( "No rain, no clouds" );
		m_ListDemoWeather.Insert( "No rain, med clouds" );
		m_ListDemoWeather.Insert( "No rain, max clouds" );
		m_ListDemoWeather.Insert( "Med rain, no clouds" );
		m_ListDemoWeather.Insert( "Med rain, med clouds" );
		m_ListDemoWeather.Insert( "Med rain, max clouds" );
		m_ListDemoWeather.Insert( "Max rain, no clouds" );
		m_ListDemoWeather.Insert( "Med rain, med clouds" );
		m_ListDemoWeather.Insert( "Max rain, max clouds" );
		
		m_ListDemoTimeOfDay = new array<string>;
		m_ListDemoTimeOfDay.Insert( "Morning" );
		m_ListDemoTimeOfDay.Insert( "Noon" );
		m_ListDemoTimeOfDay.Insert( "Afternoon" );
		m_ListDemoTimeOfDay.Insert( "Evening" );
		m_ListDemoTimeOfDay.Insert( "Night" );
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_mainmenu_xbox.layout");
		
		MissionMainMenu mission = MissionMainMenu.Cast( g_Game.GetMission() );
		m_scene = mission.GetIntroScene();

		//set version text
		string version;
		GetGame().GetVersion( version );
		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget( "VersionText" ) );
		version_widget.SetText( version );

		//set player name
		TextWidget player_name_widget = TextWidget.Cast( layoutRoot.FindAnyWidget( "PlayerNameText" ) );
		string player_name;
		GetGame().GetPlayerName( player_name );
		player_name_widget.SetText( player_name );

		//set possible game parameters
		m_WidgetDemoClasses 	= XComboBoxWidget.Cast( layoutRoot.FindAnyWidget( "CharacterXCombo" ) );
		FillXComboWidget( m_WidgetDemoClasses, m_ListDemoClasses );
		m_WidgetDemoClasses.SetCurrentItem( g_DemoPlayerClass );
		
		m_WidgetDemoWeather 	= XComboBoxWidget.Cast( layoutRoot.FindAnyWidget( "WeatherXCombo" ) );
		FillXComboWidget( m_WidgetDemoWeather, m_ListDemoWeather );
		m_WidgetDemoWeather.SetCurrentItem( g_DemoWeather );
		
		m_WidgetDemoTimeOfDay 	= XComboBoxWidget.Cast( layoutRoot.FindAnyWidget( "TimeOfDayXCombo" ) );
		FillXComboWidget( m_WidgetDemoTimeOfDay, m_ListDemoTimeOfDay );
		m_WidgetDemoTimeOfDay.SetCurrentItem( g_DemoTimeOfDay );
		
#ifdef PREVIEW_BUILD		
		m_WidgetDemoClasses.Show( false );
		m_WidgetDemoWeather.Show( false );
		m_WidgetDemoTimeOfDay.Show( false );
		
		ButtonWidget change_server = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ChangeServer") );
		change_server.Show( false );
#endif

		return layoutRoot;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( w.GetUserID() == COMBO_DEMO_PLAYER_CLASS )
		{
			g_DemoPlayerClass = m_WidgetDemoClasses.GetCurrentItem();
			
			return true;
		}
		else if ( w.GetUserID() == COMBO_DEMO_WEATHER )
		{
			g_DemoWeather = m_WidgetDemoWeather.GetCurrentItem();
			
			return true;
		}		
		else if ( w.GetUserID() == COMBO_DEMO_TIME_OF_DAY )
		{
			g_DemoTimeOfDay = m_WidgetDemoTimeOfDay.GetCurrentItem();
			
			return true;
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick( w, x, y, button);
		
		if (m_disableClick)
		{
			return false;
		}
		
		switch (w.GetUserID())
		{
			case IDC_MAIN_OPTIONS:
				EnterScriptedMenu(MENU_XBOX_CONTROLS);
				return true;
			break;
			case IDC_MAIN_MULTIPLAYER:
				GetGame().GetUIManager().EnterServerBrowser( this );
				return true;
			break;			
			case IDC_MAIN_PLAY:				//Play solo
				#ifdef PLATFORM_PS4
					string mission_path = "missions\\default.Staroye_pr";
				#else
					//string mission_path = "G:\\missions\\xbox_gdc.Staroye_pr";
string mission_path = "missions\\xbox_gdc.Staroye_pr";
				#endif
				GetGame().PlayMission( mission_path );
				return true;
			case BUTTON_PLAY_ONLINE:		//Connect to server
				string ip;
				int port;
				string password;
				GetXboxConnectParams( ip, port, password );
				GetGame().Connect( this, ip, port, password );
				return true;				
			break;				
		}
		
		return false;
	}
	
	override bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);

		if (control == ControlID.CID_BACK && value != 0)
		{
			ResetGamepads();
			return true;
		}
		
		return false;
	}
	//============================================
	// Game parameters
	//============================================	
	protected void FillXComboWidget( XComboBoxWidget widget, array<string> data )
	{
		widget.ClearAll();
		
		//add items
		for ( int i = 0; i < data.Count(); ++i )
		{
			widget.AddItem( data.Get( i ) );
		}
	}
	
	protected void GetXboxConnectParams( out string ip, out int port, out string password )
	{
		//read json string from file
		string file_path =	"Xbox\\xbox_connect.json";
		FileHandle file = OpenFile( file_path, FileMode.READ );
		
		string line_content;
		string content;
		while ( FGets( file, line_content ) > 0 )
		{
			content += line_content;
		}
		
		CloseFile( file );		
		
		//get data from json string
		JsonSerializer js = new JsonSerializer();
		jsonXboxConnectParams xbox_con_params = new jsonXboxConnectParams();
		string js_error;
		
		js.ReadFromString( xbox_con_params, content, js_error );
		
		ip = xbox_con_params.m_IP;
		port = xbox_con_params.m_Port;
		password = xbox_con_params.m_Password;
	}
	
	//============================================
	// Character
	//============================================	
	/*
	void DefaultCharacterScreen()
	{
		int i = 0;
		int j = 0;
		string root_path = "cfgCharacterCreation";
		TStringArray defInv = new TStringArray;
		
		g_Game.ConfigGetTextArray(root_path + " personalityMale", m_personalityMaleList);
		g_Game.ConfigGetTextArray(root_path + " personalityFemale", m_personalityFemaleList);
		g_Game.ConfigGetTextArray(root_path + " gender", m_genderList);
		g_Game.ConfigGetTextArray(root_path + " top", m_shirtList);
		g_Game.ConfigGetTextArray(root_path + " bottom", m_pantsList);
		g_Game.ConfigGetTextArray(root_path + " shoe", m_shoesList);

		m_allCharacters.InsertArray(m_personalityMaleList);
		m_allCharacters.InsertArray(m_personalityFemaleList);
		
		g_Game.ConfigGetText(root_path + " format", m_format);

		string params[2];		
		
		string tmp;
		vector preloaded_pos;
		Man preloaded_object = NULL;

		// preload all possible characters
		if (m_scene.m_preloaded.Count() == 0)
		{
			//preloaded_pos = m_scene.SnapToGround(Vector(1000,0,1000)).Multiply4(m_scene.m_cameraTrans);
			preloaded_pos = Vector(m_scene.m_demoPos[0], m_scene.m_demoPos[1] - 2, m_scene.m_demoPos[2]);
			for (i = 0; i < m_personalityMaleList.Count(); i++)
			{
				params[0] = m_genderList.Get(1).Substring(0, 1);
				params[1] = m_personalityMaleList.Get(i);
	
				g_Game.FormatString(m_format, params, tmp);
				
				g_Game.PreloadObject(tmp, 1.0);
				Class.CastTo(preloaded_object, g_Game.CreateObject(tmp, preloaded_pos));
	
				if (preloaded_object)
				{
					preloaded_object.SetPosition(preloaded_pos);
					preloaded_object.DisableSimulation(true);
	
					m_scene.m_preloaded.Insert(preloaded_object);	
				}
			}
	
			for (i = 0; i < m_personalityFemaleList.Count(); i++)
			{
				params[0] = m_genderList.Get(0).Substring(0, 1);
				params[1] = m_personalityFemaleList.Get(i);
	
				g_Game.FormatString(m_format, params, tmp);
	
				g_Game.PreloadObject(tmp, 1.0);
				Class.CastTo(preloaded_object, g_Game.CreateObject(tmp, preloaded_pos));
	
				if (preloaded_object)
				{
					preloaded_object.SetPosition(preloaded_pos);
					preloaded_object.DisableSimulation(true);
	
					m_scene.m_preloaded.Insert(preloaded_object);
				}
			}
		}
		
		RandomCharacter();
	}
	
	void SetCharacter()
	{
		string character;
		string params[2];

		if (m_female)
		{
			if (m_skinIndex < 0)									m_skinIndex = m_personalityFemaleList.Count() - 1;
			if (m_skinIndex > m_personalityFemaleList.Count() - 1)	m_skinIndex = 0;
			
			params[0] = m_genderList.Get(0).Substring(0, 1);
			params[1] = m_personalityFemaleList.Get(m_skinIndex);
		}
		else
		{
			if (m_skinIndex < 0)									m_skinIndex = m_personalityMaleList.Count() - 1;
			if (m_skinIndex > m_personalityMaleList.Count() - 1)	m_skinIndex = 0;
			
			params[0] = m_genderList.Get(1).Substring(0, 1);
			params[1] = m_personalityMaleList.Get(m_skinIndex);
		}
		
		g_Game.FormatString(m_format, params, character);
		m_scene.CreateCharacter(character);
		if (m_scene.m_demoUnit)
		{
			SetAttachment(m_shirtList.Get(m_topIndex), InventorySlots.BODY);
			SetAttachment(m_pantsList.Get(m_pantsIndex), InventorySlots.LEGS);
			SetAttachment(m_shoesList.Get(m_shoesIndex), InventorySlots.FEET);
			/////////allows for attachment of items to other slots, if needed
			SetAttachment("", InventorySlots.SHOULDER);
			SetAttachment("", InventorySlots.BOW);
			SetAttachment("", InventorySlots.MELEE);
			SetAttachment("", InventorySlots.VEST);
			SetAttachment("", InventorySlots.HIPS);
			SetAttachment("", InventorySlots.BACK);
			SetAttachment("", InventorySlots.HEADGEAR);
			SetAttachment("", InventorySlots.MASK);
			SetAttachment("", InventorySlots.EYEWEAR);
			SetAttachment("", InventorySlots.GLOVES);
			SetAttachment("", InventorySlots.ARMBAND);
			SetAttachment("", InventorySlots.HANDS); //5 = hands slot

			/////////
			m_scene.InitCharacterAnimations(m_scene.m_demoUnit.GetHumanInventory().GetEntityInHands());
			m_disableClick = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DemoCharSetPos, 250);
		}
	}

	void DemoCharSetPos()
	{
		m_scene.m_demoUnit.SetPosition(m_scene.m_demoPos);
		m_scene.m_demoUnit.SetOrientation(m_scene.m_demoRot);
		m_disableClick = false;
	}

	void RandomCharacter()
	{
		// make random selection
		RandomSelectGender();

		if (m_female)
		{
			m_skinIndex =  RandomSelectItem(m_personalityFemaleList);
		}
		else
		{
			m_skinIndex =  RandomSelectItem(m_personalityMaleList);
		}

		m_topIndex = RandomSelectItem(m_shirtList);
		m_pantsIndex = RandomSelectItem(m_pantsList);
		m_shoesIndex = RandomSelectItem(m_shoesList);

		// update character
		SetCharacter();
	}

	int RandomSelectItem(TStringAdvanceArray list)
	{
		if (list)
		{
			int rnd = Math.RandomInt(0, list.Count());
			return rnd;
		}
		return -1;
	}
	
	void RandomSelectGender()
	{
		int rnd = Math.RandomInt(0, 2);
		
		if (rnd == 0)	m_female = true;
		else			m_female = false;
	}
	
	void SetAttachment(string type, int slot)
	{
		if (!m_scene.m_demoUnit) return;
		g_Game.ObjectDelete(m_scene.m_demoUnit.GetInventory().FindAttachment(slot));
		EntityAI entity;
		Class.CastTo(entity, g_Game.CreateObject(type, "0 0 0", true));
		if (entity)
		{
			m_scene.m_demoUnit.LocalTakeEntityAsAttachmentEx(entity, slot);
		}
	}

	int ListLoad(string path, out TStringArray list);
	*/
	//============================================
	// 
	//============================================	
}
