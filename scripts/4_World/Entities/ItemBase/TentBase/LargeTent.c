class LargeTent extends TentBase
{
	void LargeTent()
	{
		m_ToggleSelections.Insert( new ToggleSelections("door_open", "door_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_1_open", "window_1_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_2_open", "window_2_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_3_open", "window_3_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_4_open", "window_4_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_5_open", "window_5_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_6_open", "window_6_closed"), 0 );
		m_ToggleSelections.Insert( new ToggleSelections("window_7_open", "window_7_closed"), 0 );
		
		m_ShowSelectionsWhenPitched.Insert( "door_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_1_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_2_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_3_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_4_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_5_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_6_open" );
		m_ShowSelectionsWhenPitched.Insert( "window_7_open" );
		
		m_HideSelectionsWhenPacked.Insert( "door_open" );
		m_HideSelectionsWhenPacked.Insert( "window_1_open" );
		m_HideSelectionsWhenPacked.Insert( "window_2_open" );
		m_HideSelectionsWhenPacked.Insert( "window_3_open" );
		m_HideSelectionsWhenPacked.Insert( "window_4_open" );
		m_HideSelectionsWhenPacked.Insert( "window_5_open" );
		m_HideSelectionsWhenPacked.Insert( "window_6_open" );
		m_HideSelectionsWhenPacked.Insert( "window_7_open" );
		m_HideSelectionsWhenPacked.Insert( "door_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_1_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_2_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_3_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_4_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_5_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_6_closed" );
		m_HideSelectionsWhenPacked.Insert( "window_7_closed" );
		
		Init();
	}
};
