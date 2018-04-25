Mission CreateMission(string path)
{
	Print("Creating Mission: "+ path);
	
	g_Game.SetMissionPath(path);

	if (g_Game.IsMultiplayer() && g_Game.IsServer())
	{
		return new MissionServer;
	}

#ifdef NO_GUI
	return new MissionDummy;
#endif
	MissionMainMenu m;
	if (path.Contains("NoCutscene"))
	{
		m = new MissionMainMenu();
		m.m_NoCutscene = true;
		return m;
	}
	
	if (path.Contains("intro"))
	{
		m = new MissionMainMenu();
		m.m_NoCutscene = false;
		return m;
	}
	else
	{
#ifndef NO_GUI_INGAME
		return new MissionGameplay;
#else
		return new MissionDummy;
#endif
	}
}
