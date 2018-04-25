[WorkbenchPluginAttribute("Check localisation in UI", "Find non localised text in UI", "ctrl+l", "", {"ResourceManager"})]
class CheckLocalisationPlugin: WorkbenchPlugin
{
	ref array<string> m_results = new array<string>;
	WBModuleDef m_module;
	
	void FindTexts(WidgetSource src)
	{
		if (src)
		{
			int idx = src.VarIndex("text");
			
			if (idx != -1)
			{
				string text;
				if (src.Get(idx, text) && !text.Contains("#"))
				{
					m_results.Insert("Text = \"" + text + "\" in Widget: " + src.GetName());
				}
			}
			
			FindTexts(src.GetChildren());
			FindTexts(src.GetSibling());
		}
	}
	
	void CheckTextIDs(string file)
	{
		m_module.SetOpenedResource(file);	
		WidgetSource cont = m_module.GetContainer();
		int lastIndex = m_results.Count();
		FindTexts(cont);
		if (lastIndex != m_results.Count())
		{
			m_results.InsertAt(file + ":", lastIndex);
			m_results.Insert("");
		}
	}
	
	override void Run()
	{
		m_module = Workbench.GetModule("ResourceManager");
		Workbench.SearchResources(".layout", CheckTextIDs);
		
		if (m_results.Count())
		{
			string res;
			foreach(string file: m_results)
			{
				res += file;
				res += "\n";
			}
		}
		else
		{
			res = "All texts are localised.";
		}

		Workbench.Dialog("Results", res);
	}
};