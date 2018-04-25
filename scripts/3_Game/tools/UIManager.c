class UIManager
{
	//! Create & open menu with specific id (see \ref MenuID) and set its parent
	proto native UIScriptedMenu EnterScriptedMenu(int id, UIMenuPanel parent);
	proto native void EnterServerBrowser(UIMenuPanel parentMenu);

	proto native UIScriptedMenu ShowScriptedMenu(UIScriptedMenu menu, UIMenuPanel parent);
	proto native void HideScriptedMenu(UIScriptedMenu menu);

	proto native Widget GetWidgetUnderCursor();
	proto native bool IsDialogVisible();
	proto native bool IsModalVisible();
	proto native void CloseDialog();
	proto native void HideDialog();
	/**
  \brief Shows message dialog
	@param caption 
	@param text
	@param id custom user id
	@param butts \ref DialogBoxType
	@param def \ref DialogBoxButton
	@param type \ref DialogMessageType
	@param handler
	\n usage :
	@code
	const int QUIT_DIALOG_ID = 76;
	GetGame().GetUIManager().ShowDialog("Quit", "Do You really want to quit?", QUIT_DIALOG_ID, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	...
	// after user pass dialog, callback on menu/event handler is called
	ScriptedWidgetEventHandler::OnModalResult( Widget  w,	int  x,	int  y,	int  code,	int  result	)
	{
		if (code == QUIT_DIALOG_ID && result == DBB_YES) // yes this is callback for dialog we show earlier and user press YES button
		{
			Quit();
		}
	}
	@endcode
	*/
	proto native void ShowDialog(string caption, string text, int id, int butts /*DBT_*/, int def/*DBB_*/, int type /*DMT_*/, UIScriptedMenu handler);
	proto native bool ShowCursor(bool visible);
	proto native bool IsCursorVisible();
	proto native bool IsDialogQueued();
	proto native bool ShowQueuedDialog();
	proto native bool GetLoginQueueDialog();
	proto native void SetLoginQueueDialog(bool state);
	proto native void EnterLoginQueueScript(UIMenuPanel parentMenu);
	proto native int  GetLoginQueuePosition();
	proto native bool ScreenFadeVisible();
	proto native void ScreenFadeIn(float duration, string text, int backgroundColor, int textColor);
	proto native void ScreenFadeOut(float duration);
	proto native bool IsScaledMode();
	proto native void SetScaledMode(bool enabled);
	
	//! Returns most-top open menu
	proto native UIScriptedMenu GetMenu();
	
	//! Close top window on windows stack, returns true when any window is closed
	bool Back()
	{
		if (IsDialogVisible() == false)
		{
			UIMenuPanel menu = GetMenu();
			if (menu)
			{
				menu.Close();
				return true;
			}		
		}

		return false;
	}
	
	//! Close all opened menus
	bool CloseAll()
	{
		UIMenuPanel menu = GetMenu();
		while (menu)
		{
			if (menu.GetParentMenu())
			{
				menu = menu.GetParentMenu();
			}
			else
			{
				menu.Close();
				return true;
			}
		}

		return false;
	}
	
	//! Close menu with specific ID (see \ref MenuID)
	bool CloseMenu(int id)
	{
		UIMenuPanel menu = GetMenu();
		
		while (menu)
		{if (menu.GetID() == id)
			{
				menu.Close();
				return true;
			}

			menu = menu.GetParentMenu();
		}

		return false;
	}

	bool HideMenu(int id)
	{
		UIScriptedMenu menu = GetMenu();

		while (menu)
		{
			if (menu.GetID() == id)
			{
				HideScriptedMenu( menu );
				return true;
			}

			menu = UIScriptedMenu.Cast( menu.GetParentMenu() );
		}

		return false;
	}

	//! Returns true if menu with specific ID is opened (see \ref MenuID)
	bool IsMenuOpen(int id)
	{
		if (FindMenu(id) != NULL)
		{
			return true;
		}

		return false;
	}

	//! Returns menu with specific ID if it is open (see \ref MenuID)
	UIScriptedMenu FindMenu(int id)
	{
		UIScriptedMenu menu = GetMenu();

		while (menu)
		{
			if (menu.GetID() == id)
			{
				return menu;
			}

			menu = UIScriptedMenu.Cast( menu.GetParentMenu() );
		}

		return NULL;
	}
	
	//Window management
	void OpenWindow( int id )
	{
		UIScriptedWindow window = UIScriptedWindow.GetWindow( id );
		
		//if window is already opened, close it
		if ( window )
		{
			CloseWindow( id );
			
			return;
		}
		
		//create new window
		switch( id )
		{
			case GUI_WINDOW_MISSION_LOADER: 
				window = GetGame().GetMission().CreateScriptedWindow( id );
				break;
				
			default: {};
		}
		
		if ( window )
		{
			window.Init();
			
			//add to active windows
			UIScriptedWindow.AddToActiveWindows( id, window );
		}
	}

	void CloseWindow( int id )
	{
		UIScriptedWindow window = UIScriptedWindow.GetWindow( id );
		
		if ( window )
		{
			UIScriptedWindow.RemoveFromActiveWindows( id );
			window.HideWindow();
			
			//delete window;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.DeleteWindow, window );
			/*
			wtf? leak			
			Timer delete_timer = new Timer ( CALL_CATEGORY_SYSTEM );
			Param1<UIScriptedWindow> params = new Param1<UIScriptedWindow>( window );
			delete_timer.Run( 0.5, this, "DeleteWindow", params, false );*/
			
		}
	}

	void DeleteWindow( UIScriptedWindow window )
	{
		delete window;
	}
	
	bool IsWindowOpened( int id )
	{
		if ( UIScriptedWindow.GetWindow( id ) )
		{
			return true;
		}
		
		return false;
	}
	
	//UI cursor
	void ShowUICursor( bool visible )
	{
#ifdef PLATFORM_XBOX
		ShowCursor( false );
#else
#ifdef PLATFORM_PS4
		ShowCursor( false );
#else
		ShowCursor( visible );
#endif
#endif
	}
};

//! Returns random loading background texture path
string GetRandomLoadingBackground()
{
	const string images[] = {"{655A1BF79F5B291}Gui/textures/loading_screens/loading_screen_1_co.edds", "{84BE5F7442BD4B}Gui/textures/loading_screens/loading_screen_2_co.edds"};
	Math.Randomize(-1);
	int index = Math.RandomInt(0, 100) % 2;
	return images[index];
}