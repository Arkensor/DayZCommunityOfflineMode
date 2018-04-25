class DispatcherCaller extends Dispatcher
{
	private void SendLogAtClient(Param1<string> p)
	{
		PluginDeveloper m = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
		
		if ( m != NULL )
		{
			m.SendServerLogToClient(p.param1);
		}
	}
	
	private void ScriptConsoleAddPrint(Param1<string> p)
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ScriptConsole.Cast( ui_menu );
			scripted_console.Add(p.param1);
		}
	}
	
	private void ScriptConsoleHistoryBack()
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ScriptConsole.Cast( ui_menu );
			scripted_console.HistoryBack();
		}
	}
	
	private void ScriptConsoleHistoryForward()
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ScriptConsole.Cast( ui_menu );
			scripted_console.HistoryForward();
		}
	}
		
	private void SceneEditorCommand(Param params)
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu && ui_menu.GetID() == MENU_SCENE_EDITOR )
		{
			SceneEditorMenu editor_menu = SceneEditorMenu.Cast( ui_menu );
			editor_menu.SceneEditorCommand( params );
		}
	}

	private void MissionGameplayHideInventory()
	{
		MissionGameplay mission = MissionGameplay.Cast( GetGame().GetMission() );
		if ( mission )
		{
			mission.HideInventory();
		}
	}
	
	override Param CallMethod(CallID call_id, Param params)
	{				
		switch(call_id)
		{
		case CALL_ID_SEND_LOG:
			SendLogAtClient(Param1<string>.Cast( params ));
			break;
		case CALL_ID_SCR_CNSL_ADD_PRINT:
			ScriptConsoleAddPrint(Param1<string>.Cast( params ));
			break;
		case CALL_ID_SCR_CNSL_HISTORY_BACK:
			ScriptConsoleHistoryBack();
			break;		
		case CALL_ID_SCR_CNSL_HISTORY_NEXT:
			ScriptConsoleHistoryForward();
			break;
		case CALL_ID_SCENE_EDITOR_COMMAND:
			SceneEditorCommand( params );
			break;
		case CALL_ID_HIDE_INVENTORY:
			MissionGameplayHideInventory();
			break;
		}
		
		return null;
	}
}
