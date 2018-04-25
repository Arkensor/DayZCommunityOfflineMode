class PluginBase
{
	void PluginBase()
	{
	}
	
	void ~PluginBase()
	{
	}
	
	void OnInit()
	{
	}
	
	void OnUpdate(float delta_time)
	{
	}
	
	void OnDestroy()
	{
	}
	
	void Log(string msg, string label)
	{
		Debug.Log(msg, GetModuleName(), "n/a", label, "n/a");
	}
	
	string GetModuleName()
	{
		return ClassName();
	}

	typename GetModuleType()
	{
		return GetModuleName().ToType();
	}
}











