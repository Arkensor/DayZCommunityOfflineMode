class StartupMenu extends UIScriptedMenu
{
	void StartupMenu()
	{
	}

	void ~StartupMenu()
	{
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/startup.layout");
		
		m_label = TextWidget.Cast( layoutRoot.FindAnyWidget("TextWidget") );

		return layoutRoot;
	}

	TextWidget m_label;
}
