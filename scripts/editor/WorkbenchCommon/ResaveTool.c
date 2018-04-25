[WorkbenchPluginAttribute("Re-Save Tool", "Saves all files with given extension", "", "", {"ResourceManager"})]
class ResavePlugin: WorkbenchPlugin
{
	[Attribute(".layout", "editbox", "File extension" )]
	string Extension;
	WBModuleDef m_module;
	
	void Resave(string file)
	{
		Print("Resaving: " + file);
		m_module.SetOpenedResource(file);	
		m_module.Save();
	}
	
	override void Run()
	{
		if (Workbench.ScriptDialog("Resave", "Which files you want to resave?", this))
		{
			m_module = Workbench.GetModule("ResourceManager");
			Workbench.SearchResources(Extension, Resave);
		}
	}
	
	override void RunCommandline() 
	{
		m_module = Workbench.GetModule("ResourceManager");
		
		if (m_module.GetCmdLine("-extension", Extension))
		{
			Extension.Replace("\"", "");
		}
		
		Workbench.SearchResources(Extension, Resave);
		Workbench.GetModule("ResourceManager").Close();
	}
	
	[ButtonAttribute("Re-Save")]
	bool OK()
	{
		return true;
	}
	
	[ButtonAttribute("Cancel")]
	bool Cancel()
	{
		return false;
	}
};