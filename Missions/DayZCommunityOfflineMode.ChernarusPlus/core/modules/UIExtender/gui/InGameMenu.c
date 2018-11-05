class CustomInGameMenu extends UIScriptedMenu
{
	protected ref UIExtender m_OM;

	protected ref map< int, ButtonWidget > m_Buttons = new map< int, ButtonWidget >;

	Widget m_RightPanel;

	void CustomInGameMenu( ref UIExtender om )
	{
		m_OM = om;
	}
	
	void ~CustomInGameMenu()
	{
		Mission mission = GetMission();
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
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\UIExtender\\gui\\layouts\\CustomInGameMenu.layout");
				
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

		Mission mission = GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.ToggleHud( false, true );
				hud.ToggleQuickBar( false, true );
			}
		}
		
		CustomPauseButton data;
		ButtonWidget button;
		
		Widget topButtonContainer = layoutRoot.FindAnyWidget( "top_buttons" );
		Widget bottomButtonContainer = layoutRoot.FindAnyWidget( "bottom_buttons" );

		if ( bottomButtonContainer )
		{
			data = m_OM.GetPauseButtons().Get( 0 );

			button = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\UIExtender\\gui\\layouts\\BottomButtonTemplate.layout", bottomButtonContainer ) );
			
			button.SetUserID( data.m_UserID );
			button.SetText( data.m_Text );
			button.SetUserData( data );
			
			m_Buttons.Set( data.m_UserID, button );
		}

		if ( topButtonContainer )
		{
			for ( int i = 1; i < m_OM.GetPauseButtons().Count(); i++ )
			{
				data = m_OM.GetPauseButtons().Get( i );

				button = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\UIExtender\\gui\\layouts\\TopButtonTemplate.layout", topButtonContainer ) );

				button.SetUserID( data.m_UserID );
				button.SetText( data.m_Text );
				button.SetUserData( data );

				m_Buttons.Set( data.m_UserID, button );
			}
		}

		m_RightPanel = layoutRoot.FindAnyWidget( "extra_options_left" );

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Mission mission = GetMission();

		ButtonWidget bwButton = m_Buttons.Get( w.GetUserID() );

		Print( "ID: " + w.GetUserID() + " Button: " + bwButton.ToString() );

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
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call( GetMission().CreateLogoutMenu, this );
			}
			else
			{
				// skip logout screen in singleplayer
				GetMission().AbortMission();
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
			
			MissionGameplay missionGP = MissionGameplay.Cast(GetMission());
			missionGP.DestroyAllMenus();
			//---------------------------------------------------
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetMission().Continue);
			return true;
		}
	
		return false;
	}
}
