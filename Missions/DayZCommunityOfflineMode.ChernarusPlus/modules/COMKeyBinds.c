/**

	Manages all keybinds for COM

**/
class COMKeyBinds extends Module 
{
	bool IsHudVisible = true;
	
	void COMKeyBinds( CommunityOfflineMode mission )
	{
	}

	void ~COMKeyBinds()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( int timeslice )
	{
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCursor = new KeyMouseBinding( MiscFunctions   , "ToggleCursor"  , "[U]"    , "Toggles the cursor."   );
		KeyMouseBinding toggleUI     = new KeyMouseBinding( MiscFunctions   , "ToggleUI"      , "[HOME]" , "Toggles the UI."       );
		KeyMouseBinding openEditor   = new KeyMouseBinding( GetModuleType() , "OpenEditor"    , "[Y]"    , "Opens the editor."     ); 
		
		toggleCursor.AddKeyBind( KeyCode.KC_U,    KB_EVENT_RELEASE );
		toggleUI    .AddKeyBind( KeyCode.KC_HOME, KB_EVENT_RELEASE );
		openEditor  .AddKeyBind( KeyCode.KC_Y,    KB_EVENT_RELEASE );
		
		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleUI );
		RegisterKeyMouseBinding( openEditor );
	}
	
	void OpenEditor() 
	{
		
	}
}