class PlayerNameHandler extends ScriptedWidgetEventHandler
{
	static const string s_InvalidChars = " ~!@#$%^&*()+-=<>/?.:[]{}|\\0123456789abcdefghijlkmnoprqstuvwxyzABCDEFGHIJLKMNOPRQSTUVWXYZ";
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		super.OnFocusLost(w, x, y);
		
		EditBoxWidget name_edit_widget = EditBoxWidget.Cast( w );
		TextWidget name_text_widget = TextWidget.Cast(w.GetParent().FindWidget("CharacterNameText"));
		string player_name = name_edit_widget.GetText().Trim();
		
		if (name_text_widget) 	//is main menu screen
		{			
			name_edit_widget.SetText(player_name);
			name_text_widget.SetText(player_name);
			name_edit_widget.Show(false);
			name_text_widget.Show(true);
		}
		
		g_Game.SetPlayerGameName(player_name);
		//if (m_scene.CurrentCharacterID() > -1)	 m_scene.m_data.SetCharacterName(m_scene.CurrentCharacterID(), m_scene.m_player_name);
		//g_Game.SetPlayerName(player_name);

		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (!finished) return false;
		
		EditBoxWidget name_edit_widget = EditBoxWidget.Cast( w );
		TextWidget name_text_widget = TextWidget.Cast(w.GetParent().FindWidget("CharacterNameText"));

		string player_name = name_edit_widget.GetText().Trim();

		//g_Game.SetPlayerName(player_name);
		//g_Game.GetPlayerName(player_name);

		name_edit_widget.SetText(player_name);
		if (name_text_widget)
		{
			name_text_widget.SetText(player_name);
		}

		return false;
	}

	override bool OnKeyPress(Widget  w, int  x, int  y, int  key)
	{
		super.OnKeyPress(w, x, y, key);
		
		EditBoxWidget name_edit_widget = EditBoxWidget.Cast( w );
		
		if (name_edit_widget.GetText().Length() > 14) return true; // player name is too long, block another key
		if (key > 128) return true; // no extended characters(diacritics) allowed
		if (s_InvalidChars.IndexOf(key.AsciiToString()) == -1) return true; // not allowed character -> blocked
		
		return false;
	}
}