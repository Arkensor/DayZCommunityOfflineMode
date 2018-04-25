#ifdef GAME_TEMPLATE

class Settings
{
	static void		OnChange(string variableName) {}
	static void 	OnAnyChange() {}	
	static void 	OnLoad() {}
	static void 	OnSave() {}
	static void 	OnReset() {}
	static void 	OnRevert() {}
	static void 	OnApply() {}
	
	private void Settings() {}
	private void ~Settings() {}
};

class GameSettings: Settings
{
	[Attribute("false", "checkbox", "Is debug mode enabled")]
	static bool Debug;
	
	override static void OnAnyChange() 
	{
		GetGame().SetDebug(Debug);
	}
}

class SettingsMenu: MenuBase
{
	proto native external bool AddSettings(typename settingsClass);	
	proto native void Save();	
	proto native void Reset();
	proto native void Revert();
	proto native void Apply();
	proto native void Back();
};
#endif

