class InGameMenu extends UIScriptedMenu
{
	void InGameMenu()
	{
		//nic
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_ingamemenu.layout");
				
		string version;
		GetGame().GetVersion(version);
		
		TextWidget version_widget;
		Class.CastTo(version_widget, layoutRoot.FindAnyWidget("VersionTextWidget"));
		version_widget.SetText("DayZ offline mode by Arkensor for version #main_menu_version " + version);

		#ifdef PREVIEW_BUILD
			version_widget.SetText("THIS IS PREVIEW");
		#endif
		ButtonWidget restart_btn;
		Class.CastTo(restart_btn, layoutRoot.FindAnyWidgetById(IDC_INT_RETRY));
		
		Man player = GetGame().GetPlayer();
		bool player_is_alive = false;
		if (player)
		{
			int life_state = player.GetPlayerState();

			if (life_state == EPlayerStates.ALIVE)
			{
				player_is_alive = true;
			}
		}

		if (GetGame().IsMultiplayer())
		{
			restart_btn.SetText("#main_menu_respawn");
		}
		else
		{
			restart_btn.SetText("#main_menu_restart");
		}

		if (GetGame().IsMultiplayer() && GetGame().CanRespawnPlayer() == false)
		{
			restart_btn.Enable(false);
		}
		
	#ifdef BULDOZER
		delete restart_btn;
	#endif

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Mission mission = GetGame().GetMission();

		switch (w.GetUserID())
		{
		case IDC_MAIN_CONTINUE:
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().Continue);
			return true;
			
		case IDC_MAIN_OPTIONS:
			EnterScriptedMenu(MENU_OPTIONS);
			return true;

		case IDC_MAIN_QUIT:
			GetGame().GetUIManager().ShowDialog("EXIT", "Are you sure you want to exit?", IDC_INT_EXIT, DBT_YESNO, DBB_YES, DMT_QUESTION, NULL);
			return true;

		case IDC_INT_RETRY:
			if (!GetGame().IsMultiplayer())
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RestartMission);
			}
			else
			{
				GetGame().GetUIManager().ShowDialog("#main_menu_respawn", "#main_menu_respawn_question", 0, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
				/*WIP, maybe add timer?
				if (g_Game.m_isTileSet)
				{
					g_Game.m_isTileSet = false;
					g_Game.m_tilePublic.Show(false);
				}*/
			}
			return true;
		}

		return false;
	}
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		if ( code == IDC_INT_EXIT && result == DBB_YES )
		{
			if (GetGame().IsMultiplayer())
			{
				GetGame().LogoutRequestTime();
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().CreateLogoutMenu, this);
			}
			else
			{
				// skip logout screen in singleplayer
				GetGame().GetMission().AbortMission();
			}	
			g_Game.CancelQueueTime();	
			return true;	
		
		}
		else if (result == DBB_YES && GetGame().IsMultiplayer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RespawnPlayer);
			//turns off dead screen, hides HUD for countdown
			//---------------------------------------------------
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if(player) 	
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(player.ShowDeadScreen, DayZPlayerImplement.DEAD_SCREEN_DELAY, false, false);
			}
			
			MissionGameplay missionGP = MissionGameplay.Cast(GetGame().GetMission());
			missionGP.DestroyAllMenus();
			//---------------------------------------------------
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().Continue);
			return true;
		}
	
		return false;
	}
}
