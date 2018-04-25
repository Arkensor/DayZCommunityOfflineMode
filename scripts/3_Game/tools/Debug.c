// TODO:
// 1. Alredy exist some key in array / map
// 2. Object is null (check object & log error)
// 3. Debug version only
// 4. Destructor of static classes
// 5. Debug Console Interface:
//		- Clear Log
//		- Filter
//		- Log per frame
// 6. Per frame log ?
// 7. Zapis do fajlu

class Debug
{	
	static private const string	LOG_DEBUG	= "Debug";
	static private const string	LOG_INFO	= "Info";
	static private const string	LOG_WARNING	= "Warning";
	static private const string	LOG_ERROR	= "Error";
	static private const string	LOG_DEFAULT	= "n/a";
	
	static private ref array<Shape>	m_DebugShapes;

	static void	Init()
	{
		m_DebugShapes	= new array<Shape>;
	}
	
	static void DestroyAllShapes()
	{
		for ( int i = 0; i < m_DebugShapes.Count(); ++i )
		{
			if ( m_DebugShapes.Get(i) )
			{
				m_DebugShapes.Get(i).Destroy();
			}
		}
		
		m_DebugShapes.Clear();
	}
	
	static void RemoveShape(out Shape shape)
	{
		if(!shape) return;
		for ( int i = 0; i < m_DebugShapes.Count(); i++ )
		{
			Shape found_shape = m_DebugShapes.Get(i);
			
			if ( found_shape  &&  found_shape == shape )
			{
				found_shape.Destroy();
				m_DebugShapes.Remove(i); // Mandatory! Otherwise the Destroy() function causes crash!
				shape = null;
				return;
			}
		}
	}

	static void SetEnabledLogs(bool enable)
	{
		m_EnabledLogs = enable;
	}
	
	static bool IsLogsEnabled()
	{
		return m_EnabledLogs;
	}

	
	/**
	\brief Prints debug message with normal prio
		\param msg \p string Debug message for print
		\return \p void None
		@code
			Debug.Log("Hello World");
			
			>> [Log]: Hello World;
		@endcode
	*/
	static void	Log(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
	{
		LogMessage(LOG_DEBUG, plugin, entity, author, label, message);
	}
	
	/**
	\brief Prints debug message with normal prio
		\param msg \p string Debug message for print
		\return \p void None
		@code
			Debug.Log("Hello World");
			
			>> [Log]: Hello World;
		@endcode
	*/
	static void	LogInfo(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
	{
		LogMessage(LOG_INFO, plugin, entity, author, label, message);
	}
	
	/**
	\brief Prints debug message as warning message
		\param msg \p string Debug message for warning print
		\return \p void None
		@code
			Debug.LogWarning("Hello World, this is warning log");
			
			>> [Warning]: Hello World, this is warning log
		@endcode
	*/
	static void	LogWarning(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
	{
		LogMessage(LOG_WARNING, plugin, entity, author, label, message);
	}
	
	/**
	\brief Prints debug message as error message
		\param msg \p string Debug message for error print
		\return \p void None
		@code
			Debug.LogError("Hello World, this is error log");
			
			>> [Error]: Hello World, this is error log
		@endcode
	*/
	static void	LogError(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT){
		LogMessage(LOG_ERROR, plugin, entity, author, label, message);
	}
	
	static void	LogArrayInt(array<int> arr = NULL, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
	{
		if (arr== NULL) return;
		for(int i = 0; i < arr.Count(); i++)
		{
			LogMessage(LOG_DEBUG, plugin, entity, author, label, arr.Get(i).ToString() );
		
		}
	}
	
	static void	LogArrayString(array<string> arr = NULL, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
	{
		if (arr== NULL) return;	
		for(int i = 0; i < arr.Count(); i++)
		{
			LogMessage(LOG_DEBUG, plugin, entity, author, label, arr.Get(i) );
		
		}
	}
	
	static void	ReceivedLogMessageFromServer(string message)
	{
		SaveLog(message);
	}
	
	static void ClearScriptLogs(){
		ClearLogs();
	}

	static Shape DrawBox(vector pos1, vector pos2, int color = 0x1fff7f7f)
	{
		Shape shape = Shape.Create(ShapeType.BBOX, color, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, pos1, pos2);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static Shape DrawCube(vector pos, float size = 1, int color = 0x1fff7f7f)
	{
		vector min = pos;
		vector max = pos;
		
		float size_h = size * 0.5;
	
		min[0] = min[0] - size_h;
		min[1] = min[1] - size_h;
		min[2] = min[2] - size_h;
		
		max[0] = max[0] + size_h;
		max[1] = max[1] + size_h;
		max[2] = max[2] + size_h;
		
		Shape shape = Shape.Create(ShapeType.DIAMOND, color, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, min, max);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static Shape DrawSphere(vector pos, float size = 1, int color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE)
	{
		Shape shape = Shape.CreateSphere(color, flags, pos, size);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static Shape DrawCylinder(vector pos, float radius, float height = 1, int color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE )
	{
		Shape shape = Shape.CreateCylinder(color, flags, pos, radius, height);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	/**
	DrawLine
	\nFlags:
	\n ShapeFlags.NOZBUFFER
	\n ShapeFlags.NOZUPDATE
	\n ShapeFlags.DOUBLESIDE
	\n ShapeFlags.WIREFRAME
	\n ShapeFlags.TRANSP
	\n ShapeFlags.ONCE
	\n ShapeFlags.NOOUTLINE
	\n ShapeFlags.NOCULL
	*/
	static Shape DrawLine(vector from, vector to, int color = 0xFFFFFFFF, int flags = 0)
	{
		vector pts[2]
		pts[0] = from;
		pts[1] = to;
		
		Shape shape = Shape.CreateLines(color, flags, pts, 2);
		m_DebugShapes.Insert(shape);
		//m_DebugShapes.Debug();
		return shape;
	}

	static Shape DrawLines(vector[] positions, int count, int color = 0xFFFFFFFF, int flags = 0)
	{
		
		Shape shape = Shape.CreateLines(color, flags, positions, count);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static Shape DrawArrow(vector from, vector to, float size = 0.5, int color = 0xFFFFFFFF, float flags = 0)
	{
		Shape shape = Shape.CreateArrow(from, to, size, color, flags);
		m_DebugShapes.Insert(shape);
		return shape;
	}

	
	
	/**
	\brief Returns some of base config classes strings like CfgVehicles, CfgWeapons, etc. for searching purposes
		\param base_classes \p out TStringArray Array containing some of base config classes
	*/
	static void GetBaseConfigClasses( out TStringArray base_classes )
	{
		base_classes.Clear();
		base_classes.Insert(CFG_VEHICLESPATH);
		base_classes.Insert(CFG_WEAPONSPATH);
		base_classes.Insert(CFG_MAGAZINESPATH);
		base_classes.Insert(CFG_RECIPESPATH);
	}
	
	/**
	\brief Returns config classes containing search_string in name
		\param search_string \p string String to search in class names
		\param filtered_classes \p out TStringArray Array containing filtered classes based on search_string
		\param only_public \p bool Set to true to return only public classes, set to false to return all filtered classes
	*/
	static void GetFiltredConfigClasses( string search_string, out TStringArray filtered_classes, bool only_public = true )
	{	
		TStringArray searching_in = new TStringArray;
		GetBaseConfigClasses( searching_in );
		
		filtered_classes.Clear();
		
		search_string.ToLower();
		
		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string config_path = searching_in.Get(s);
			
			int objects_count = GetGame().ConfigGetChildrenCount(config_path);
			for (int i = 0; i < objects_count; i++)
			{
				string childName;
				GetGame().ConfigGetChildName(config_path, i, childName);
	
				if ( only_public )
				{
					int scope = GetGame().ConfigGetInt( config_path + " " + childName + " scope" );
					if ( scope == 0 )
					{
						continue;
					}
				}
				
				string nchName = childName;
				nchName.ToLower();
	
				if ( nchName.Contains(search_string) != -1)
				{
					filtered_classes.Insert(childName);
				}
			}
		}
	}
	
	//---------------------------------------------------------------
	//-------private
	
	static private bool				m_EnabledLogs;
		
	static private void LogMessage(string level, string plugin, string entity, string author, string label, string message)
	{
		if( CGame.IsDoNoLogs() ) return;
		bool is_server_log = false;
		if (GetGame() == NULL) return;		
		
		// Is server
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			is_server_log = true;
		}
		
		// Is debug mod
		if ( GetGame().IsDebug() == false )
		{
			return;
		}
		
		// Script Console checkbox
		if ( !is_server_log && IsLogsEnabled() == false )
		{
			return;
		}
		
		// Formation output to external file
		// %date{MM-dd HH:mm:ss} | %Enviroment | %Level | %Module | %Entity | %Author | %Label | %Message | %CallStack	
		string date			= GetDate();
		string callStack	= LOG_DEFAULT;
		string env			= "Client";
		string msg			= string.Empty;
		
		if ( is_server_log )
		{
			env = "Server";
		}
		
		msg += date			+ " | ";
		msg += env			+ " | ";
		msg += level		+ " | ";
		msg += plugin		+ " | ";
		msg += entity		+ " | ";
		msg += author		+ " | ";
		msg += label		+ " | ";
		msg += message		+ " | ";
		msg += callStack;
	
		if ( is_server_log )
		{
			SaveLog(msg);
			Param1<string> msg_p = new Param1<string>(msg);
			CallMethod(CALL_ID_SEND_LOG, msg_p); 
		}
		else
		{
			SaveLog(msg);
		}
	}
	
	static private void	SaveLog(string log_message)
	{
		FileHandle file_index = OpenFile(GetFileName(), FileMode.APPEND);
		
		if ( file_index == 0 )
		{
			return;
		}
			
		FPrintln(file_index, log_message);
		
		CloseFile(file_index);
	}
	
	static private void	ClearLogs()
	{
		if ( FileExist( GetFileName() ) )
		{
			FileHandle file_index = OpenFile( GetFileName(), FileMode.WRITE );
			
			if ( file_index == 0 )
			{
				return;
			}
				
			FPrintln( file_index, "" );
			
			CloseFile( file_index );	
		}
	}
	
	static private string GetFileName()
	{
		return CFG_FILE_SCRIPT_LOG_EXT;
	}
	
	static private string GetDate()
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);
		
		string date = month.ToStringLen(2) + "-" + day.ToStringLen(2) + " " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
		
		return date;
	}
};
