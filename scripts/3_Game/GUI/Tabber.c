// -----------------------------------------------------------
class Tabber : ScriptedWidgetEventHandler
{
	Widget ButtonsPanel;
	Widget ContentPanel;
	reference int SelectedTab;
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		SelectTab(SelectedTab);
	}
	
	// -----------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w && w.GetParent() == ButtonsPanel)
		{
			SelectedTab = 0;
			Widget iw = ButtonsPanel.GetChildren();
			while(iw)
			{
				if (iw == w) break;
				SelectedTab++;
				iw = iw.GetSibling();
			}
	
			SelectTab(SelectedTab);
		}
		
		return false;
	}
	
	// -----------------------------------------------------------
	protected void SelectTab(int index)
	{
		Print(index);
		int n = 0;
		Widget w = ContentPanel.GetChildren();
		while(w)
		{
			if (n == index)
			{
				w.Show(true);
			}
			else
			{
				w.Show(false);
			}
			n++;
			w = w.GetSibling();
		}
	}
};