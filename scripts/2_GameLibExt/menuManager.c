#ifdef GAME_TEMPLATE

enum DialogPriority
{
	INFORMATIVE,
	WARNING,
	CRITICAL	
};

enum DialogResult
{
	PENDING,
	OK,
	YES,
	NO,
	CANCEL,
};

enum ScriptMenuPresetEnum
{
	
};

class MenuManager
{
	proto native MenuBase OpenMenu(ScriptMenuPresetEnum preset, int userId = 0, bool unique = false);
	proto native MenuBase OpenDialog(ScriptMenuPresetEnum preset, int priority = DialogPriority.INFORMATIVE, int iUserId = 0, bool unique = false);
	proto native MenuBase FindMenuByPreset(ScriptMenuPresetEnum preset);
	proto native MenuBase FindMenuByUserId(int userId);
	proto native MenuBase GetTopMenu();
	
	proto native bool IsAnyMenuOpen();	
	proto native bool IsAnyDialogOpen();

	proto native bool CloseMenuByPreset(ScriptMenuPresetEnum preset);
	proto native bool CloseMenuByUserId(int userId);
	proto native bool CloseMenu(MenuBase menu);
	
	protected void MenuManager();
	protected void ~MenuManager();
};

class MenuBindAttribute
{
	string m_MenuItemName;
	
	void MenuBindAttribute(string menuItemName = "")
	{
		m_MenuItemName = menuItemName;
	}
}

class MenuBase: ScriptedWidgetEventHandler
{
	proto native int GetUserId();
	proto native Widget GetRootWidget();
	proto external MenuBase BindItem(string menuItemName, func callback);
	proto native MenuBase SetLabel(string menuItemName, string text);
	proto native Widget GetItemWidget(string menuItemName);
	proto native MenuManager GetManager();
	proto native void Close();
	
	void	OnMenuFocusGained() {}
	void	OnMenuFocusLost() {}
	void	OnMenuShow() {}
	void	OnMenuHide() {}
	void	OnMenuOpen() {}
	void	OnMenuClose() {}
	void	OnMenuInit() {}
	void	OnMenuUpdate(float tDelta) {}
	void	OnMenuItem(string menuItemName, bool changed, bool finished) {}
	
	protected void MenuBase();
	protected void ~MenuBase();
};

class MessageBox: MenuBase
{
	[MenuBindAttribute()]
	void Ok()
	{
		Close();
	}
}

#ifdef PLATFORM_WINDOWS
class WorldEditorIngame: MenuBase
{
	proto native bool LoadWorld(string worldFilePath);
	proto native bool SaveWorld();
};
#endif

#endif

