[WorkbenchPluginAttribute("SVN Blame", "Just for testing", "ctrl+b", "", {"ScriptEditor"})]
class SVNBlamePlugin: WorkbenchPlugin
{
	[Attribute("TortoiseProc /command:blame /path:$path /startrev:1 /endrev:\"-1\" /ignoreeol /ignoreallspaces /line:$line", "editbox")]
	string CommandLine;
	
	override void Run()
	{
		ScriptEditor mod = Workbench.GetModule("ScriptEditor");
		if (mod)
		{
			string file;
			string absPath;
			if (mod.GetCurrentFile(file) && Workbench.GetAbsolutePath(file, absPath))
			{
				int line = mod.GetCurrentLine();
				string command = CommandLine;
				command.Replace("$path", absPath);
				command.Replace("$line", (line + 1).ToString());
				Workbench.RunCmd(command);
			}
		}
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Configure SVN Blame", "Usage: \n$path - will be replaced with file name\n$line - will be replaced with current line number", this);
	}
	
	[ButtonAttribute("OK")]
	void OkButton() {}
}

