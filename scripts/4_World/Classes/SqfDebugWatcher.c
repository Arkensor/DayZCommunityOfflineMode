class SqfDebugWatcher
{
	private int m_Id;
	private string m_SqfCommand;
	
	private bool m_IsRunning;
		

	void SqfDebugWatcher( int id, string sqf_command )
	{
		m_Id = id;
		m_SqfCommand = sqf_command;
		m_IsRunning = false;
	}

	int GetId()
	{
		return m_Id;
	}

	void SetId( int id )
	{
		m_Id = id;
	}

	string GetSqfCommand()
	{
		return m_SqfCommand;
	}

	void SetSqfCommand( string sqf_command )
	{
		m_SqfCommand = sqf_command;
	}

	bool IsRunning()
	{
		return m_IsRunning;
	}

	void Run()
	{
		m_IsRunning = true;
	}

	void Stop()
	{
		m_IsRunning = false;
	}

	void Execute()
	{
		if ( m_IsRunning && m_SqfCommand != "" )
		{
//			string sqf_command = "_result = " + m_SqfCommand + "; _resultStr = format ['%1', _result]; null = callFunction ['OnSqfDebugWatcherResult', " + m_Id.ToString() + ", _resultStr];";
//			GetGame().ExecuteSQF( sqf_command );
		}
	}
}