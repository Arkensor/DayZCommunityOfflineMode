class ScriptConsoleNewPresetDialog extends UIScriptedMenu
{
	void ScriptConsoleNewPresetDialog()
	{

	}

	void ~ScriptConsoleNewPresetDialog()
	{
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/script_console_dialog_preset.layout");
		m_Editbox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("PrimaryEditBox") );
		m_Label = TextWidget.Cast( layoutRoot.FindAnyWidget("WindowLabel") );
		m_Label.SetText("NEW PRESET");

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w.GetUserID() == IDC_OK)
		{
			UIScriptedMenu ui_menu = GetGame().GetUIManager().FindMenu(MENU_SCRIPTCONSOLE);
			if ( ui_menu != NULL )
			{
				ScriptConsole scripted_console = ScriptConsole.Cast( ui_menu );
				scripted_console.NewPreset( m_Editbox.GetText() );;
			}

			Close();
			return true;
		}
		else if (w.GetUserID() == IDC_CANCEL)
		{
			Close();
			return true;
		}

		return false;
	}
	
	EditBoxWidget m_Editbox;
	TextWidget m_Label;
	TextWidget m_Message;
}
