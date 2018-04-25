class ButtonOptionsAccessUI extends OptionsAccessUI
{
	override void Update()
	{
		SwitchOptionsAccess switchOption = SwitchOptionsAccess.Cast( m_optionAccess );
		ButtonWidget btn = ButtonWidget.Cast( m_widget );

		if (switchOption && btn)
		{
			string text;
			switchOption.GetItemText(text);
			btn.SetText(text);
		}
	}

	override void SetValue()
	{
		SwitchOptionsAccess switchOption = SwitchOptionsAccess.Cast( m_optionAccess );

		if (switchOption)
		{
			switchOption.Switch();
			Update();
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		SetValue();
		return false;
	}
}
