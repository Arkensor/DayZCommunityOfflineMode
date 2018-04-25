// #include "Scripts/DayZGame.c"

// ---------------------
CGame CreateGame()
{
	Print("CreateGame()");
	g_Game = new DayZGame;
	return g_Game;
}
