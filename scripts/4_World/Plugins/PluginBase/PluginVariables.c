class PluginVariables extends PluginBase
{
	void PluginVariables()
	{
		m_Id = 0;
		m_Variables = new map<int, string>;
		//				("variable name")
		RegisterVariable("varNote");
		RegisterVariable("varColor");
		
	}

	void ~PluginVariables()
	{
	}
	
	void RegisterVariable(string name)
	{
		m_Id++;
		m_Variables.Set(m_Id,name);//REWORK.V maybe have 2 maps, one with key
		
	}

	int m_Id;
	ref map<int, string> m_Variables;

	string GetName(int id)
	{
		return m_Variables.Get(id);
	}

	int GetID(string name)
	{
		return m_Variables.GetKeyByValue(name);
	}
}
