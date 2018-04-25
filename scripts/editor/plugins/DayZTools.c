class DayZTool: WorkbenchPlugin
{
	void RunDayZBat(string filepath, bool wait = false)
	{
		if (filepath.Length() < 2) return;
		
		filepath.Replace("\\", "/");
		
		if (filepath[1] != ":")
		{
			string cwd;
			Workbench.GetCwd(cwd);
			filepath = cwd + "/" + filepath;
		}
		
		int index = filepath.IndexOf("/");
		int last_index = index;
		
		while(index != -1)
		{
			last_index = index;
			index = filepath.IndexOfFrom(last_index + 1, "/");
		}
		
		if (last_index == -1) return;
		
		string path = filepath.Substring(0, last_index);
		string bat = filepath.Substring(last_index + 1, filepath.Length() - last_index - 1);
		/*Print(filepath);
		Print(path);
		Print(bat);*/
		Workbench.RunCmd("cmd /c \"cd " + path + " & call " + bat + "\"", wait);
	}
	
	override void Configure() 
	{
		Workbench.ScriptDialog("Mission directory","", this);
	}
	
	[ButtonAttribute("OK")]
	void DialogOk()
	{
	}
};

[WorkbenchPluginAttribute("DayZ Restart", "Just for testing", "ctrl+1", "", {"ScriptEditor"})]
class RestartDayzTool: DayZTool
{
	[Attribute("day_z_data_missions/killDayZ.bat", "fileeditbox", "Path to missions dir", "")]
	string KillBatPath;
	
	[Attribute("day_z_data_missions/_default_single/default_SampleMap3_Empty.bat", "fileeditbox", "Path to missions dir", "")]
	string MissionBatPath;
	
	override void Run()
	{
		RunDayZBat(KillBatPath, true);
		RunDayZBat(MissionBatPath);
	}
}

[WorkbenchPluginAttribute("DayZ Run", "Just for testing", "ctrl+2", "", {"ScriptEditor"})]
class RunDayzTool: DayZTool
{
	[Attribute("day_z_data_missions/_default_single/default_SampleMap3_Empty.bat", "fileeditbox", "Path to missions dir", "")]
	string MissionBatPath;
	
	override void Run()
	{
		RunDayZBat(MissionBatPath);
	}
}

[WorkbenchPluginAttribute("DayZ Kill", "Just for testing", "ctrl+3", "", {"ScriptEditor"})]
class KillDayzTool: DayZTool
{
	[Attribute("day_z_data_missions/killDayZ.bat", "fileeditbox", "Path to missions dir", "")]
	string KillBatPath;
	
	override void Run()
	{
		RunDayZBat(KillBatPath);
	}
}





