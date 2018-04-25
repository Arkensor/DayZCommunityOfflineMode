class ActionMenu
{
	private TextWidget m_defaultActionWidget;
	
	void ActionMenu()
	{
		
	}

	void Init( TextWidget default_action_widget)
	{
		m_defaultActionWidget = default_action_widget;
		m_defaultActionWidget.Show(false);
	}

	void NextAction()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		player.GetActionManager().SelectNextAction();
	}

	void PrevAction()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		player.GetActionManager().SelectPrevAction();
	}
}