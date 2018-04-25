class PluginLocalHistoryBase extends PluginFileHandler
{	
	void PluginLocalHistoryBase()
	{
		m_ReadOnly = false;
	}
		
	void ~PluginLocalHistoryBase()
	{
	}
	
	override void OnInit()
	{
		super.OnInit();
	}
	
	override string GetFileName()
	{
		Error( "Cannot call GetFileName on Base class PluginLocalHistoryBase" );
		return STRING_EMPTY;
	}
	
	void AddNewLine(string text)
	{
		// replace newline to \\n
		text.Replace("\n", "\\n" );
		m_FileContent.Insert(text);
		SaveFile();
	}

	TStringArray GetAllLines()
	{
		// replace \\n to new line
		TStringArray console_history = new TStringArray;
		for ( int i = 0; i < m_FileContent.Count(); i++)
		{
			string history_record = m_FileContent.Get(i);
			history_record.Replace("\\n", "\n");
			console_history.Insert( history_record );
		}
		return console_history;
	}

	TStringArray GetLastLine()
	{
		int count = m_FileContent.Count();
		string ret = "";
		
		if ( count > 0 )
		{
			ret = m_FileContent.Get(count - 1);
		}
		
		return NULL;
	}
}
