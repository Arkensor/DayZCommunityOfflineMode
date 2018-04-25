class PluginConfigHandler extends PluginFileHandler
{	
	ref array<ref CfgParam> m_CfgParams;
	
	void PluginConfigHandler()
	{
		m_CfgParams = new array<ref CfgParam>;
		
		LoadConfigFile();
	}

	override void OnInit()
	{
		super.OnInit();
	}
	
	override string GetFileName()
	{
		return CFG_FILE_USER_PROFILE;
	}
	
	void LoadConfigFile()
	{
		LoadFile();
		
		m_CfgParams.Clear();
		
		for ( int j = 0; j < m_FileContent.Count(); j++ )
		{
			m_CfgParams.Insert( ParseText(m_FileContent.Get(j)) );
		}	
	}
	
	void SaveConfigToFile()
	{
		ClearFileNoSave();
		
		for ( int i = 0; i < m_CfgParams.Count(); ++i )
		{
			string s = ParseParam(m_CfgParams.Get(i));

			m_FileContent.Insert(s);
		}
		
		SaveFile();
	}
	
	CfgParam GetParamByName(local string name, local int cfg_type)
	{
		CfgParam param;
		
		for ( int i = 0; i < m_CfgParams.Count(); ++i )
		{
			param = m_CfgParams.Get(i);
			
			if ( param.GetName() == name )
			{
				return param;
			}
		}
		
		// cfg_type == -1 returns NULL
		param = CfgParam.CreateParam(name, cfg_type);
		m_CfgParams.Insert(param);
		return param;
	}
	
	array<ref CfgParam> GetAllParams()
	{
		return m_CfgParams;
	}
	
	void ClearParams()
	{
		m_CfgParams.Clear();
	}
	
	void RemoveParamByName( string name )
	{
		for ( int i = 0; i < m_CfgParams.Count(); ++i )
		{
			CfgParam param = m_CfgParams.Get(i);
			
			if ( param.GetName() == name )
			{
				m_CfgParams.Remove( i );
				return;
			}
		}
	}

	void RenameParam( string name, string new_name )
	{
		for ( int i = 0; i < m_CfgParams.Count(); ++i )
		{
			CfgParam param = m_CfgParams.Get(i);
			
			if ( param.GetName() == name )
			{
				param.SetName( new_name );
				return;
			}
		}
	}
	
	bool ParamExist(local string name)
	{
		local CfgParam param;
		
		for ( int i = 0; i < m_CfgParams.Count(); ++i )
		{
			param = m_CfgParams.Get(i);
			
			if ( param.GetName() == name )
			{
				return true;
			}
		}
		
		return false;
	}
	
	string ParseParam(local CfgParam param)
	{
		local string text = STRING_EMPTY;
		
		if ( param.GetName() != STRING_EMPTY )
		{
			text += param.GetName()+"=";
		}
		
		switch ( param.GetType() )
		{
			case CFG_TYPE_ARRAY:
			{
				CfgParamArray param_array = CfgParamArray.Cast( param );
				array<ref CfgParam> params = param_array.GetValues();
				
				text += "{";
				
				for ( local int i = 0; i < params.Count(); ++i )
				{
					if ( i > 0 )
					{
						text += ",";
					}
					
					text += ParseParam(params.Get(i));
				}
				
				text += "}";
				
				break;
			}
			case CFG_TYPE_STRING:
			{
				local CfgParamString param_string = CfgParamString.Cast( param );
				text += "\""+param_string.GetValue()+"\"";
				
				break;
			}
			case CFG_TYPE_INT:
			{
				local CfgParamInt param_int = CfgParamInt.Cast( param );
				text += param_int.GetValue().ToString();
				
				break;
			}
			case CFG_TYPE_FLOAT:
			{
				local CfgParamFloat param_float = CfgParamFloat.Cast( param );
				local string s = param_float.GetValue().ToString();
				
				if ( s.IndexOf(".") == -1 )
				{
					s += ".0";
				}
				
				text += s;
				
				break;
			}
			case CFG_TYPE_BOOL:
			{
				local CfgParamBool param_bool = CfgParamBool.Cast( param );
				
				if ( param_bool.GetValue() )
				{
					text += "true";
				}
				else
				{
					text += "false";
				}
				
				break;
			}
		}
		
		return text;
	}

	CfgParam ParseText(local string text)
	{
		string param_name = STRING_EMPTY;
		string param_value = text.Trim();;
		int text_type = GetTextType(param_value);
		
		//Log("ParseText -> text_type: "+itoa(text_type));
		
		if ( text_type == CFG_TYPE_PARAM )
		{
			local int pos = param_value.IndexOf("=");
			
			if ( pos > 0 )
			{
				param_name = param_value.Substring(0, pos).Trim();
				local int pos_end = param_value.Length() - pos;
				param_value = param_value.Substring(pos + 1, pos_end - 1).Trim();
				
				//Log("ParseText -> param -> "+param_name+" = "+param_value);
				
				CfgParam param = ParseText(param_value);
				param.SetName(param_name);
				
				return param;
			}
		}
		else if ( text_type == CFG_TYPE_ARRAY )
		{				
			//Log("ParseText -> array -> "+param_name+" = "+param_value);

			CfgParamArray param_array = new CfgParamArray(param_name);
			array<ref CfgParam> array_params = ParseToValueArray(param_value);
			param_array.SetParams(array_params);
			
			return param_array;
		}
		else if ( text_type == CFG_TYPE_STRING )
		{					
			//Log("ParseText -> string -> "+param_value);
			
			CfgParamString param_string = new CfgParamString(param_name);
			param_string.SetValue( ParseToValueString(param_value) );
			
			return param_string;
		}
		else if ( text_type == CFG_TYPE_INT )
		{					
			//Log("ParseText -> int -> "+param_name+" = "+param_value);
			
			CfgParamInt param_int = new CfgParamInt(param_name);
			param_int.SetValue( ParseToValueInt(param_value) );
			
			return param_int;
		}
		else if ( text_type == CFG_TYPE_FLOAT )
		{
			//Log("ParseText -> float -> "+param_name+" = "+param_value);
			
			CfgParamFloat param_float = new CfgParamFloat(param_name);
			param_float.SetValue( ParseToValueFloat(param_value) );
			
			return param_float;
		}
		else if ( text_type == CFG_TYPE_BOOL )
		{
			//Log("ParseText -> bool -> "+param_name+" = "+param_value);
			
			local CfgParamBool param_bool = new CfgParamBool(param_name);
			param_bool.SetValue( ParseToValueBool(param_value) );
			
			return param_bool;
		}
		
		//Log("ParseText -> none -> "+param_name+" = "+param_value);
		
		return NULL;
	}

	protected int GetTextType(local string text)
	{	
		int lenght = text.Length();
			
		// Check Array
		int pos1 = text.IndexOf("{");
		if ( pos1 == 0 && text.Substring(lenght - 1, 1) == "}" )
		{
			return CFG_TYPE_ARRAY;
		}	
			
		// Check String
		pos1 = text.IndexOf("\"");
		
		if ( pos1 == 0 && text.Substring(lenght - 1, 1) == "\"" )
		{
			string string_text = text.Substring(1, lenght - 2);
			if ( string_text.IndexOf("\"") == -1 )
			{
				return CFG_TYPE_STRING;
			}
		}
		
		// Check Param
		pos1 = text.IndexOf("=");
		if ( pos1 > -1 )
		{
			return CFG_TYPE_PARAM;
		}
		
		// Check Param True
		pos1 = text.IndexOf("true");
		if ( lenght == 4 && pos1 > -1 )
		{
			return CFG_TYPE_BOOL;
		}
		
		// Check Param False
		pos1 = text.IndexOf("false");
		if ( lenght == 5 && pos1 > -1 )
		{
			return CFG_TYPE_BOOL;
		}
		
		// Check Param Float
		pos1 = text.IndexOf(".");
		if ( pos1 > -1 )
		{
			return CFG_TYPE_FLOAT;
		}
		
		// Is Int
		return CFG_TYPE_INT;
	}
	
	protected bool IsString(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_STRING )
		{
			return true;
		}
		
		return false;
	}

	protected bool IsParam(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_PARAM )
		{
			return true;
		}
		
		return false;
	}

	protected bool IsArray(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_ARRAY )
		{
			return true;
		}
		
		return false;
	}

	protected bool IsFloat(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_FLOAT )
		{
			return true;
		}
		
		return false;
	}

	protected bool IsInt(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_INT )
		{
			return true;
		}
		
		return false;
	}

	protected bool IsBool(string text)
	{
		if ( GetTextType(text) == CFG_TYPE_BOOL )
		{
			return true;
		}
		
		return false;
	}
	
	protected string ParseToValueString(local string param_value)
	{	
		return param_value.Substring(1, param_value.Length() - 2);
	}

	protected float ParseToValueFloat(local string param_value)
	{
		return param_value.ToFloat();
	}

	protected int ParseToValueInt(local string param_value)
	{
		return param_value.ToInt();
	}

	protected bool ParseToValueBool(local string param_value)
	{
		if ( param_value.Contains("true") )
		{
			return true;
		}
		
		return false;
	}

	protected array<ref CfgParam> ParseToValueArray(local string param_value)
	{
		array<ref CfgParam> param_parent = new array<ref CfgParam>;
		
		TStringArray strings = new TStringArray;
		
		array<int> a = new array<int>;
		array<int> b = new array<int>;

		int pos_a = 0;
		int pos_b = 0;
		int element = 0;
		int value_start = 0;
		int value_end = 0;
		
		while ( pos_a > -1 ||  pos_b > -1 )
		{
			pos_a = param_value.IndexOfFrom(pos_a, "{" );
			if ( pos_a > -1 )
			{
				a.Insert(pos_a);
				pos_a++;
			}
			
			pos_b = param_value.IndexOfFrom(pos_b, "}" );
			if ( pos_b > -1 )
			{
				b.Insert(pos_b);
				pos_b++;
			}
		}	
		
		pos_a = 0;
		pos_b = 0;
		
		for ( pos_a = 0; pos_a < a.Count(); ++pos_a )
		{
			if ( a.Get(pos_a) > b.Get(pos_b) )
			{
				element--;
				
				if ( element <= 1 )
				{
					value_end = b.Get(pos_b) - value_start + 1;
					
					//Log("1 -> "+substr(param_value, value_start, value_end))
					
					strings.Insert( param_value.Substring(value_start, value_end) );
					
					value_start = b.Get(pos_b) + 1;
				}
				
				pos_a--;
				pos_b++;
			}
			else if ( a.Get(pos_a) < b.Get(pos_b) )
			{
				if ( pos_a == a.Count() - 1 && pos_b < b.Count() - 1 )
				{
					element--;
				
					if ( element == 0 )
					{
						value_end = b.Get(pos_b) - value_start;
						
						//Log("2 -> "+substr(param_value, value_start + 1, value_end))
						
						strings.Insert( param_value.Substring(value_start + 1, value_end) );
						
						value_start = b.Get(pos_b) + 1;
					}
					
					pos_a--;
					pos_b++;
				}
				else
				{
					element++;
				
					if ( element == 2 )
					{
						value_start = a.Get(pos_a);
					}
				}			
			}
		}
		
		//Log(param_value+" => "+itoa(a.Count())+" / "+itoa(b.Count())+"    count: "+itoa(strings.Count()));
		
		if ( strings.Count() == 0 )
		{
			//Log(param_value+" -> ZERO {}");
			
			string param_value_content = param_value.Substring(1, param_value.Length() - 2);
			
			//Log(param_value+" -> Trimmed -> "+param_value_content);
			
			param_value_content.Split(",", strings);
		}
		
		for ( local int i = 0; i < strings.Count(); ++i )
		{
			string str = strings.Get(i);
			
			//Log("To Parse => "+str);
			
			local CfgParam param = ParseText(str);
			
			param_parent.Insert(param);		
		}
		
		return param_parent;
	}
}
