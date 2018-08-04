#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\CameraSettings.c"
class MiscFunctions extends Module 
{
	bool IsHudVisible = true;
	
	void MiscFunctions( CommunityOfflineMode mission )
	{
	}

	void ~MiscFunctions()
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
	}
	
	void ToggleCursor() 
	{
		if ( GetGame().GetInput().HasGameFocus(INPUT_DEVICE_MOUSE) ) 
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetUIManager().ShowUICursor(true);	
		} 
		else 
		{
			GetGame().GetUIManager().ShowUICursor(false);
			GetGame().GetInput().ResetGameFocus(INPUT_DEVICE_MOUSE);
		}
	}
	
	void ToggleUI() 
	{
		//IsHudVisible = !IsHudVisible;
		//m_Mission.GetHud().Show(IsHudVisible);
		
		// Camera ui test
		GetGame().GetUIManager().ShowScriptedMenu( new CameraSettings(), NULL );
	}
}