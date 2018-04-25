class HelpScreen extends UIScriptedMenu
{
	TextListboxWidget m_KeyBindingsTextListboxWidget;
	TextListboxWidget m_MouseBindingsTextListboxWidget;

	ButtonWidget m_CloseConsoleButton;

	void HelpScreen()
	{
		
	}

	void ~HelpScreen()
	{	
	}

	override Widget Init()
	{
		PluginKeyBinding module_keybinding = PluginKeyBinding.Cast( GetPlugin(PluginKeyBinding) );
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/help_screen.layout");
		m_KeyBindingsTextListboxWidget = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("KeyBindingsTextListboxWidget") );
		m_MouseBindingsTextListboxWidget = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("MouseBindingsTextListboxWidget") );
		
		array<ref KeyBinding> keybindings = module_keybinding.GetKeyBindings();
		for ( int i = 0; i < keybindings.Count(); i++ )
		{
			m_KeyBindingsTextListboxWidget.AddItem( keybindings.Get(i).GetInfoBind(), NULL, 0 );
			m_KeyBindingsTextListboxWidget.SetItem( i, keybindings.Get(i).GetInfoDescription(), NULL, 1 );
		}
		
		array<ref MouseBinding> mousebindings = module_keybinding.GetMouseBindings();
		for ( i = 0; i < mousebindings.Count(); i++ )
		{
			m_MouseBindingsTextListboxWidget.AddItem( mousebindings.Get(i).GetInfoBind(), NULL, 0 );
			m_MouseBindingsTextListboxWidget.SetItem( i, mousebindings.Get(i).GetInfoDescription(), NULL, 1 );
		}
		
		m_CloseConsoleButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("CloseButtonWidget") );
		
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_CloseConsoleButton )
		{
			Close();
			return true;
		}
		
		return false;
	}
}
