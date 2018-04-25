typedef Param3<string, string, string> LogTemplate;
typedef int LogTemplateID;

class LogTemplates
{
	static private ref map<LogTemplateID, ref LogTemplate> m_LogTemplates;
	
	static private void	RegisterLogTamplate(LogTemplateID template_id, string author, string plugin, string label)
	{
		if ( m_LogTemplates == NULL )
		{
			m_LogTemplates	= new map<LogTemplateID, ref LogTemplate>;
		}
		
		if ( m_LogTemplates.Contains(template_id) )
		{
			Debug.Log("Template ID: "+string.ToString(template_id)+" is alredy exist!", "LogTemplate.h -> OnInit()", "System", "Template Registration", "None");
		}
		else
		{
			LogTemplate params = new LogTemplate(author, plugin, label);
			m_LogTemplates.Set(template_id, params);
		}
	}
	
	// Steps to register of new log template:
	// 1.) Crete new LogTemplateID in below of this comment.
	// 2.) Set Template Name in format TEMPLATE_[CUSTOM NAME] => TEMPLATE_MY_LOG
	// 3.) Set Template ID which is your id + your custom id =>  + CustomID(0) = 50190
	//////////////////////////////////////////////////////////////
	//			 			Template Name				Template ID
	static LogTemplateID	TEMPLATE_UNKNOWN			= 0;
	static LogTemplateID	TEMPLATE_JANOSIK			= 1;
	static LogTemplateID	TEMPLATE_PLAYER_WEIGHT		= 2;
	static LogTemplateID	TEMPLATE_BROADCAST			= 3;
	
	static void Init()
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////Register Log templates//////////////////////////////////////////////////////////////////////////////////////
		//						| Template Name			| author			| plugin				| label					////
		RegisterLogTamplate(	TEMPLATE_UNKNOWN		,"Unknown"			,"Unknown"				,"Unknown"				);//
		RegisterLogTamplate(	TEMPLATE_JANOSIK		,"Janosik"			,"GUI"					,"None"					);//
		RegisterLogTamplate(	TEMPLATE_PLAYER_WEIGHT	,"Unknown"			,"PlayerBase"			,"Weight"				);//
		RegisterLogTamplate(	TEMPLATE_BROADCAST		,"Unknown"			,"PluginMessageManager"	,"Broadcast"			);//
		
	}
	
	static LogTemplate GetTemplate(LogTemplateID template_id)
	{
		if ( m_LogTemplates && m_LogTemplates.Contains(template_id) )
		{
			return m_LogTemplates.Get(template_id);
		}
		
		Debug.Log("Template ID: "+string.ToString(template_id)+" does not exist!", "LogTemplate.h -> GetTemplate()", "System", "Get Log Template", "None");
		return NULL;
	}
}

/**
\brief Creates debug log (optional) from LogTemplate which are registred
	\param template_id \p LogTemplateID ID of LogTemplate which was registred in proto/Logtemplate.h -> "class LogTemplates"
	\param message \p string Debug message for log
	\return \p void
	@code
		Log("This is some debug log message", TEMPLATE_JINDRICH);
		
		>> output to scriptExt.log (for GamutLogViewer)
	@endcode
*/
void Log(string message, LogTemplateID template_id = 0)
{
	LogTemplate log_template = LogTemplates.GetTemplate(template_id);
	
	Debug.Log(message, log_template.param2, log_template.param1, log_template.param3);
}

/**
\brief Creates info log (optional) from LogTemplate which are registred
	\param template_id \p LogTemplateID ID of LogTemplate which was registred in proto/Logtemplate.h -> "class LogTemplates"
	\param message \p string Info message for log
	\return \p void
	@code
		LogInfo("This is some info log message", TEMPLATE_JINDRICH);
		
		>> output to scriptExt.log (for GamutLogViewer)
	@endcode
*/
void LogInfo(string message, LogTemplateID template_id = 0)
{
	LogTemplate log_template = LogTemplates.GetTemplate(template_id);
	
	Debug.LogInfo(message, log_template.param2, log_template.param1, log_template.param3);
}

/**
\brief Creates warning log  (optional) from LogTemplate which are registred
	\param template_id \p LogTemplateID ID of LogTemplate which was registred in proto/Logtemplate.h -> "class LogTemplates"
	\param message \p string Warning message for log
	\return \p void
	@code
		LogT(TEMPLATE_JINDRICH, "This is some warning log message");
		
		>> output to scriptExt.log (for GamutLogViewer)
	@endcode
*/
void LogWarning(string message, LogTemplateID template_id = 0)
{
	LogTemplate log_template = LogTemplates.GetTemplate(template_id);
	
	Debug.LogWarning(message, log_template.param2, log_template.param1, log_template.param3);
}

/**
\brief Creates error log (optional) from LogTemplate which are registred
	\param template_id \p LogTemplateID ID of LogTemplate which was registred in proto/Logtemplate.h -> "class LogTemplates"
	\param message \p string Error message for log
	\return \p void
	@code
		LogT(TEMPLATE_JINDRICH, "This is some error log message");
		
		>> output to scriptExt.log (for GamutLogViewer)
	@endcode
*/
void LogError(string message, LogTemplateID template_id = 0)
{
	LogTemplate log_template = LogTemplates.GetTemplate(template_id);
	
	Debug.LogError(message, log_template.param2, log_template.param1, log_template.param3);
}

void SQFPrint(string sqf_msg)
{
	Print(sqf_msg);
}

void SQFLog(string sqf_msg)
{
	Log(sqf_msg);
}