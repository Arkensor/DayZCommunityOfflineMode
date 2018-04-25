class PresetsMenu extends UIScriptedMenu
{
	void PresetsMenu()
	{
	}

	void ~PresetsMenu()
	{
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_options_controls_preset.layout");
		
		m_schemes_list = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("PresetListboxWidget") );
		
		int c = GetGame().GetInput().GetProfilesCount();
		for (int i = 0; i < c; i++)
		{
			string tmp;
			GetGame().GetInput().GetProfileName(i, tmp);
			m_schemes_list.AddItem(tmp, NULL, 0);
		}

		i = GetGame().GetInput().GetCurrentProfile();
		if (i >= 0 && i < c)
		{
			m_schemes_list.SelectRow(i);
		}
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_CANCEL:
			Close();
			return true;

			case IDC_OK:
			int index = m_schemes_list.GetSelectedRow();
			if (index != -1)
			{
				GetGame().GetInput().SetProfile(index);

				ref TIntArray keys = new TIntArray;

				for (int i = 0; i < UAN; i++)
				{
					GetGame().GetInput().GetCurrentProfileActionKeys(i, keys);
					GetGame().GetInput().SetActionKeys(i, keys);
				}

				Close();
			}
			return true;
		}

		return false;
	}

	private TextListboxWidget m_schemes_list;
}
