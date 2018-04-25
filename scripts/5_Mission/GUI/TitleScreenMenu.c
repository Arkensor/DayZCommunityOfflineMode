/*! Xbox menu */
class TitleScreenMenu extends UIScriptedMenu
{
	protected DayZIntroScene m_scene;
	
	void TitleScreenMenu()
	{
	}
	
	void ~TitleScreenMenu()
	{
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_title_screen.layout");
		
		MissionMainMenu mission = MissionMainMenu.Cast( g_Game.GetMission() );
		m_scene = mission.GetIntroScene();
		
		RichTextWidget text_widget = RichTextWidget.Cast( layoutRoot.FindAnyWidget("InputPromptText") );
		if (text_widget)
		{
			string gamertag;
			GetGame().GetPlayerName(gamertag);
#ifdef PLATFORM_XBOX
		text_widget.SetText("Press <image set=\"xbox_buttons\" name=\"A\" /> to start the game");
#endif
		
#ifdef PLATFORM_PS4
		text_widget.SetText("Press <image set=\"playstation_buttons\" name=\"cross\" /> to start the game");
#endif		
		}
		
		return layoutRoot;
	}
		
	override bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);
		
		return false;
	}
	
	override void Update(float timeslice)
	{
		// temporary for testing ui on PC
#ifdef PLATFORM_WINDOWS
#ifdef PLATFORM_XBOX
		if ( GetGame().GetInput().GetActionDown(UAUISelect, false) )
		{
			EnterScriptedMenu(MENU_MAIN);
		}
#endif
#endif
	}
	
	override bool OnXboxEvent(int xboxEvent)
	{
	#ifdef PREVIEW_BUILD
		if (xboxEvent == ACTIVE_GAMEPAD_SET)
		{
			EnterScriptedMenu(MENU_MAIN);
			return true;
		}
	#else
		if (xboxEvent == ACTIVE_GAMEPAD_SET)
		{
			bool isUser = IsGamepadUser();
			if (!isUser)
			{
				ShowAccountPicker();
			}
			return true;
		}
		else if (xboxEvent == ACTIVE_USER_CHANGED)
		{
			m_scene.ChangeCharacter(-1);
			EnterScriptedMenu(MENU_MAIN);
			return true;
		}
		else if (xboxEvent == CLOSED_USER_PICKER)
		{
			ResetGamepads();
			return true;
		}
	#endif
		return false;
	}
}