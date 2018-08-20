class CustomInGameMenu extends UIScriptedMenu
{
	protected ref OverrideMenus m_OM;

	protected ref map< int, ButtonWidget > m_Buttons = new map< int, ButtonWidget >;

	void CustomInGameMenu( ref OverrideMenus om )
	{
		m_OM = om;
	}
	
	void ~CustomInGameMenu()
	{
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.ToggleHud( hud.GetHudState(), true );
				hud.ToggleQuickBar( hud.GetQuickBarState(), true );
			}
		}
		
		UnlockControls();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\gui\\layouts\\CustomInGameMenu.layout");
				
		string version;
		GetGame().GetVersion(version);
		
		TextWidget version_widget;
		Class.CastTo(version_widget, layoutRoot.FindAnyWidget("version"));
		version_widget.SetText("#main_menu_version " + version);

		#ifdef PREVIEW_BUILD
			version_widget.SetText("THIS IS PREVIEW");
		#endif
		
	#ifdef BULDOZER
		delete restart_btn;
	#endif

		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.ToggleHud( false, true );
				hud.ToggleQuickBar( false, true );
			}
		}

		
		Widget buttonContainer = layoutRoot.FindAnyWidget( "CampaignMenuWindow" );
		Widget logo = layoutRoot.FindAnyWidget( "PanelLogo" );

		float baseBCPosY = 0;
		float baseBCPosX = 0;

		float oldBCWidth = 0;
		float oldBCHeight = 0;

		float newBCWidth = 0;
		float newBCHeight = 0;

		float bWidth = 0;
		float bHeight = 0;

		//float newBPosX = 0;
		float newBPosY = 0;

		float logoOffset = 120;

		if ( logo )
		{
			float ignore;
			logo.GetScreenSize( ignore, logoOffset );

			// logoOffset = (logoOffset / 2);
		}

		if ( buttonContainer )
		{
			buttonContainer.GetPos(baseBCPosX, baseBCPosY);

			Print("Found container!");
			for ( int i = 0; i < m_OM.GetPauseButtons().Count(); i++ )
			{
				Print("Found button!");
				CustomPauseButton data = m_OM.GetPauseButtons().Get( i );

				ButtonWidget button = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\OverrideMenus\\gui\\layouts\\PauseButtonTemplate.layout", buttonContainer ) );

				button.SetUserID( data.m_UserID );
				button.SetText( data.m_Text );

				buttonContainer.GetSize(oldBCWidth, oldBCHeight);
				button.GetSize(bWidth, bHeight);

				newBCWidth = oldBCWidth;
				newBCHeight = oldBCHeight + bHeight;

				newBPosY = newBPosY + bHeight;
				
				button.SetPos(0, newBPosY);
				
				buttonContainer.SetPos(baseBCPosX, baseBCPosY + newBCHeight - logoOffset);
				buttonContainer.SetSize(newBCWidth, newBCHeight);

				if ( logo )
				{
					logo.SetPos(0, newBCHeight + logoOffset);
				}

				button.SetUserData( data );

				m_Buttons.Set( data.m_UserID, button );
				
				Print("Button: " + data.m_Text + " Details: OBCW: " + oldBCWidth + ", OBCH: " + oldBCHeight + ", NBCW: " + newBCWidth + ", NBCW: " + newBCHeight + ", BW: " + bWidth + ", BH: " + bHeight + ", PY: " + newBPosY);
			}
		}

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Mission mission = GetGame().GetMission();

		ButtonWidget bwButton = m_Buttons.Get( w.GetUserID() );

		if ( bwButton )
		{
			CustomPauseButton cpButton;

			bwButton.GetUserData( cpButton );

			if ( cpButton )
			{
				cpButton.RunFunction();
				return true;
			}
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
		else if ( code == IDC_INT_EXIT && result == DBB_NO )
		{
			g_Game.CancelQueueTime();
		}
		else if ( code == IDC_INT_RETRY && result == DBB_YES )
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
