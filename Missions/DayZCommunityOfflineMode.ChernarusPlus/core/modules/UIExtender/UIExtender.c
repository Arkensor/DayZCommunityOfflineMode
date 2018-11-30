/*
	Manages all keybinds for COM
*/

class OverrideValid
{
	bool m_Multiplayer = true;
	bool m_Singleplayer = true;

	void OverrideValid(bool multiplayer = true, bool singleplayer = true)
	{
		m_Multiplayer = multiplayer;
		m_Singleplayer = singleplayer;
	}

	bool CanShow()
	{
		bool IsMultiplayer = GetGame().IsMultiplayer();

		if (m_Multiplayer && IsMultiplayer == true) {
			return true;
		}
		
		if (m_Singleplayer && IsMultiplayer == false) {
			return true;
		}
		
		return false;
	}
}

class UIExtender extends Module
{
	protected ref array<ref CustomPauseButton> m_PauseButtons = new array<ref CustomPauseButton>;

	protected ref UIScriptedMenu 		m_CIGM;

	void UIExtender()
	{
		AddPauseButton( new CustomPauseButton( "#main_menu_continue", IDC_MAIN_CONTINUE, GetModuleType(), "ContinueMission" ));
		AddPauseButton( new CustomPauseButton( "#main_menu_exit", IDC_MAIN_QUIT, GetModuleType(), "QuitMission" ));
		AddPauseButton( new CustomPauseButton( "#main_menu_restart", IDC_INT_RETRY, GetModuleType(), "RetryMission" ), new OverrideValid(false, true));
		AddPauseButton( new CustomPauseButton( "#main_menu_respawn", IDC_INT_RETRY, GetModuleType(), "RespawnPlayer" ), new OverrideValid(true, false));
		AddPauseButton( new CustomPauseButton( "#layout_xbox_ingame_menu_options", IDC_MAIN_OPTIONS, GetModuleType(), "OptionsMenu" ));
	}

	void ~UIExtender()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}

    UIScriptedMenu CreateScriptedMenu( int id, UIScriptedMenu menu )
    {
        switch (id)
		{
			case MENU_INGAME:
				m_CIGM = new CustomInGameMenu( this );
				menu = m_CIGM;
				break;
		}

        return menu;
    }

	UIScriptedMenu GetPauseMenu()
	{
		return m_CIGM;
	}

	void AddPauseButton( ref CustomPauseButton button )
	{
		m_PauseButtons.Insert( button );
	}

	void AddPauseButton( ref CustomPauseButton button, OverrideValid valids )
	{
		if ( !valids.CanShow() ) return;

		AddPauseButton( button );
	}

	void AddPauseButton( ref CustomPauseButton button, int index )
	{		
		m_PauseButtons.InsertAt( button, index );
	}

	void AddPauseButton( ref CustomPauseButton button, OverrideValid valids, int index )
	{
		if ( !valids.CanShow() ) return;

		AddPauseButton( button, index );
	}

	ref array<ref CustomPauseButton> GetPauseButtons()
	{
		return m_PauseButtons;
	}

	void ContinueMission()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetMission().Continue);
	}

	void QuitMission()
	{
		GetGame().GetUIManager().ShowDialog("EXIT", "Are you sure you want to exit?", IDC_INT_EXIT, DBT_YESNO, DBB_YES, DMT_QUESTION, m_CIGM);
	}

	void OptionsMenu()
	{
		m_CIGM.EnterScriptedMenu(MENU_OPTIONS);
	}

	void RetryMission()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RestartMission);
	}

	void RespawnPlayer()
	{
		GetGame().GetUIManager().ShowDialog("#main_menu_respawn", "#main_menu_respawn_question", IDC_INT_RETRY, DBT_YESNO, DBB_YES, DMT_QUESTION, m_CIGM);
	}

}