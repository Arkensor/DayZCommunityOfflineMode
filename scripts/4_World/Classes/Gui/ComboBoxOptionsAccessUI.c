class ComboBoxOptionsAccessUI extends OptionsAccessUI
{
	override void Init()
	{
		ListOptionsAccess list = ListOptionsAccess.Cast( m_optionAccess );
		XComboBoxWidget combo = XComboBoxWidget.Cast( m_widget );

		if (list && combo)
		{
			combo.ClearAll();

			string text;
			int c = list.GetItemsCount();
			for (int i = 0; i < c; i++)
			{
				list.GetItemText(i, text);
				combo.AddItem(text);
			}
		}
	}

	override void Update()
	{
		ListOptionsAccess list;
		XComboBoxWidget combo;

		if (Class.CastTo(list, m_optionAccess) && Class.CastTo(combo, m_widget))
		{
			combo.SetCurrentItem(list.GetIndex());
		}
	}

	override void SetValue()
	{
		ListOptionsAccess list;
		XComboBoxWidget combo;

		if (Class.CastTo(list, m_optionAccess) && Class.CastTo(combo, m_widget))
		{
			list.SetIndex(combo.GetCurrentItem());
			Update();
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		SetValue();
		return false;
	}
}
