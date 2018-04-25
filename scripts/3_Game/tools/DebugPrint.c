class DebugPrint
{	
	static private const int 	MSG_LOG		= 0;
	static private const int 	MSG_WARNING	= 1;
	static private const int 	MSG_ERROR	= 2;
	static private const int 	MSG_COUNT	= 3;
	
	static private string		s_MsgPrefix[MSG_COUNT];
	static private string		s_MsgStackMarkStart;
	static private string		s_MsgStackMarkEnd;
	static private bool			s_MsgStackMarked;
	static private bool			s_TraceAllLogs;
	
	static void OnInit()
	{
		s_MsgPrefix[MSG_LOG]		= "Log";
		s_MsgPrefix[MSG_WARNING]	= "Warning";
		s_MsgPrefix[MSG_ERROR]		= "Error";
		
		s_MsgStackMarkStart			= "-- Stack trace --";
		s_MsgStackMarked			= false;
		s_MsgStackMarkEnd			= "-----------------";
		
		s_TraceAllLogs				= false;
	}
	
	/**
	\brief Prints debug message with normal prio
		\param msg \p string Debug message for print
		\return \p void None
		@code
			DebugPrint.Log("Hello World");
			
			>> [Log]: Hello World;
		@endcode
	*/
	static void	Log(string msg)
	{
		LogMessage(msg, MSG_LOG, s_TraceAllLogs);
	}
	
	/**
	\brief Prints debug message as normal message and prints stack trace of calls
		\param msg \p string Debug message for print
		\return \p void None
		@code
			DebugPrint.LogAndTrace("Hello World, this is normal log");
			
			>> [Log]: Hello World, this is normal log
			>>		-- Stack trace --
			>>   	OnKeyPress() Scripts/mission/missionGameplay.c : 230
			>>   	OnKeyPress() Scripts/DayZGame.c : 346
			>>   	-----------------
		@endcode
	*/
	static void	LogAndTrace(string msg)
	{
		LogMessage(msg, MSG_LOG, true);
	}
	
	/**
	\brief Prints debug message as warning message
		\param msg \p string Debug message for warning print
		\return \p void None
		@code
			DebugPrint.LogWarning("Hello World, this is warning log");
			
			>> [Warning]: Hello World, this is warning log
		@endcode
	*/
	static void	LogWarning(string msg)
	{
		LogMessage(msg, MSG_WARNING, s_TraceAllLogs);
	}
	
	/**
	\brief Prints debug message as warning message and prints stack trace of calls
		\param msg \p string Debug message for warning print
		\return \p void None
		@code
			DebugPrint.LogWarningAndTrace("Hello World, this is warning log");
			
			>> [Warning]: Hello World, this is warning log
			>>		-- Stack trace --
			>>   	OnKeyPress() Scripts/mission/missionGameplay.c : 230
			>>   	OnKeyPress() Scripts/DayZGame.c : 346
			>>   	-----------------
		@endcode
	*/
	static void	LogWarningAndTrace(string msg)
	{
		LogMessage(msg, MSG_WARNING, true);
	}
	
	/**
	\brief Prints debug message as error message
		\param msg \p string Debug message for error print
		\return \p void None
		@code
			DebugPrint.LogError("Hello World, this is error log");
			
			>> [Error]: Hello World, this is error log
		@endcode
	*/
	static void	LogError(string msg)
	{
		LogMessage(msg, MSG_ERROR, s_TraceAllLogs);
	}
	
	/**
	\brief Prints debug message as error message and prints stack trace of calls
		\param msg \p string Debug message for error print
		\return \p void None
		@code
			DebugPrint.LogErrorAndTrace("Hello World, this is error log");
			
			>> [Error]: Hello World, this is error log
			>>		-- Stack trace --
			>>   	OnKeyPress() Scripts/mission/missionGameplay.c : 230
			>>   	OnKeyPress() Scripts/DayZGame.c : 346
			>>   	-----------------
		@endcode
	*/
	static void LogErrorAndTrace(string msg)
	{
		LogMessage(msg, MSG_ERROR, true);
	}
	
	/**
	\brief Function adjust received message for debug console (Do not use)
		\param msg \p string Message for adjust
		\return \p string Adjusted Message
		@code
		
			string msg = DebugPrint.AdjustDebugLog("s = 'Hello World, this is error log'");
			DebugPrint.Log(msg);
			
			>> Hello World, this is error log
		@endcode
	*/
	static string AdjustDebugLog(string msg)
	{
		if ( IsStackTrace(msg) )
		{
			return TrimStackTrace(msg);
		}
		
		if ( IsDebugLog(msg) ) 
		{
			return TrimDebugLog(msg);
		}
		
		return msg;
	}
	
	static void EnableTracingLogs(bool enable)
	{
		s_TraceAllLogs = enable;
	}
	
	static private bool		IsDebugLog(string msg)
	{
		for ( int i = 0; i < MSG_COUNT; ++i )
		{
			if ( msg.IndexOf(s_MsgPrefix[i]) != -1 )
			{
				return true;
			}
		}
		
		return false;
	}
	static private string	TrimDebugLog(string msg)
	{
		int msg_lenght = msg.Length();
		int log_start = msg.IndexOf("'") + 1;
		
		if ( log_start == -1 )
		{
			return msg;
		}
		
		int log_lenght = msg_lenght - log_start - 2;
		
		return msg.Substring(log_start, log_lenght);
	}
	static private bool		IsStackTrace(string msg)
	{
		if ( s_MsgStackMarked && msg.IndexOf(s_MsgStackMarkEnd) != -1 )
		{
			s_MsgStackMarked = false;
			return false;
		}
		
		if ( s_MsgStackMarked )
		{
			return true;
		}
		
		if ( msg.IndexOf(s_MsgStackMarkStart) != -1 )
		{
			s_MsgStackMarked = true;
			return true;
		}
		
		return false;
	}
	static private string	TrimStackTrace(string msg)
	{
		if ( msg.IndexOf("DebugPrint.c") != -1 )
		{
			return string.Empty;
		}
		
		return msg;
	}

	static private void		LogMessage(string msg, int msg_type, bool trace=false)
	{	
		msg = "["+s_MsgPrefix[msg_type]+"]: "+msg;
		
		Print(msg);
		
		if ( trace )
		{
			DumpStack();
		}
	}
};
