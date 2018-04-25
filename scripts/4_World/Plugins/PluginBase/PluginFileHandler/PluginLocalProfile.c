class PluginLocalProfile extends PluginFileHandler
{
	ref map<string, string>						m_ConfigParams;
	ref map<string, ref TStringArray>				m_ConfigParamsArray;
	ref map<string, ref map<string, string>>		m_ConfigParamsInArray;
	ref map<string, ref array<ref map<string, string>>>	m_ConfigParamsArrayInArray;
	
	void PluginLocalProfile()
	{
		m_ConfigParams				= new map<string, string>;
		m_ConfigParamsArray			= new map<string, ref TStringArray>;
		m_ConfigParamsInArray		= new map<string, ref map<string, string>>;
		m_ConfigParamsArrayInArray	= new map<string, ref array<ref map<string, string>>>;
	}

	override string GetFileName()
	{
		return CFG_FILE_USER_PROFILE;
	}

	override void OnInit()
	{
		super.OnInit();
		
		LoadConfigFile();
	}
	
	bool LoadConfigFile()
	{
		TStringArray file_content = GetFileContent();
		
		string line_content = "";
		string param_name = "";
		string param_value = "";
		TStringArray param_content;
		int pos_sep = -1;
		
		for ( int i = 0; i < file_content.Count(); ++i )
		{
			line_content = file_content.Get(i);
			
			param_content = ParseTextToParameter(line_content);
		
			if ( param_content.Count() == 0 )
			{
				PrintString(GetFileName()+"("+i.ToString()+"): Error in config -> Maybe missing '=' !");
				return false;			
			}
			
			param_name	= param_content.Get(0);
			param_value	= param_content.Get(1);
			
			//{{name = apple, damage = 0.5, quantity = 2},{name = banana, damage = 0.5, quantity = 2}}
			
			TStringArray array_items = new TStringArray;
			ParseTextToArray(param_value, array_items);
			//{name = apple, damage = 0.5, quantity = 2}
			//{name = banana, damage = 0.5, quantity = 2}
			
			TStringArray array_content_raw = new TStringArray;
			
			if ( array_items.Count() > 0 )
			{
				m_ConfigParamsArray.Set(param_name, array_items);
				
				for ( int j = 0; j < array_items.Count(); ++j )
				{
					string array_content = array_items.Get(j);
					//{name = apple, damage = 0.5, quantity = 2}
					
					array_content_raw.Clear();
					
					ParseTextToArray(array_content, array_content_raw);
					//name = apple
					//damage = 0.5
					//quantity = 2
					
					if ( array_content_raw.Count() > 0 )
					{
						if ( !m_ConfigParamsArrayInArray.Contains(param_name) )
						{
							m_ConfigParamsArrayInArray.Set(param_name, new array<ref map<string, string>>);
						}
						
						array<ref map<string, string>> params_in_array = m_ConfigParamsArrayInArray.Get(param_name);					
						map<string, string> parameters = new map<string, string>;
						
						for ( int k = 0; k < array_content_raw.Count(); ++k )
						{
							string params = array_content_raw.Get(k);
							TStringArray p = ParseTextToParameter(params);
							
							if ( p.Count() == 2 )
							{
								parameters.Set(p.Get(0), p.Get(1));
							}
						}
						
						params_in_array.Insert(parameters);					
					}
				}
			}
			else
			{
				m_ConfigParams.Set(param_name, param_value);
			}
		}
		
		return true;
	}

	void SaveConfigToFile()
	{
		ClearFileNoSave();
		
		int i;
		int j;
		int k;
		
		string variable_name;
		string variable_value;
		string text;
		TStringArray variable_array;
		
		for ( i = 0; i < m_ConfigParams.Count(); ++i)
		{
			variable_name = m_ConfigParams.GetKey(i);
			variable_value = m_ConfigParams.GetElement(i);
			
			text = variable_name + " = " + variable_value;
			
			AddNewLineNoSave(text);
		}
		
		for ( i = 0; i < m_ConfigParamsArray.Count(); ++i)
		{
			variable_name = m_ConfigParamsArray.GetKey(i);
			variable_array = m_ConfigParamsArray.GetElement(i);
			
			if ( m_ConfigParamsArrayInArray.Contains(variable_name) )
			{
				continue;
			}
			
			text = variable_name + " = {";
			
			for ( j = 0; j < variable_array.Count(); ++j )
			{
				if ( j > 0 )
				{
					text += ",";
				}
				
				text += variable_array.Get(j);			
			}
			
			text += "}";
			
			AddNewLineNoSave(text);
		}
		
		for ( i = 0; i < m_ConfigParamsArrayInArray.Count(); ++i)
		{
			variable_name								= m_ConfigParamsArrayInArray.GetKey(i);
			array<ref map<string, string>>	variable_param	= m_ConfigParamsArrayInArray.GetElement(i);
			
			text = variable_name + " = {";
			
			if ( variable_param != NULL )
			{
				for ( j = 0; j < variable_param.Count(); ++j )
				{
					if ( j > 0 )
					{
						text += ",";
					}			
					text += "{";
					
					map<string, string> params = variable_param.Get(j);
					for ( k = 0; k < params.Count(); ++k )
					{
						if ( k > 0 )
						{
							text += ",";
						}
						
						string param_name = params.GetKey(k);
						string param_value = params.GetElement(k);
						
						text += param_name+"="+param_value;
					}
					
					text += "}";
				}
			}
			
			text += "}";
			
			AddNewLineNoSave(text);
		}
		
		SaveFile();
	}
	
	bool IsTextArray(string text)
	{
		if ( text.Length() > 0 && text.Substring(0, 1) == "{" )
		{
			if ( text.Substring(text.Length() - 1, 1) == "}" )
			{
				return true;
			}
		}
		
		return false;
	}
	
	TStringArray ParseTextToParameter(string text)
	{
		TStringArray ret = new TStringArray;
		
		int pos = text.IndexOf("=");
			
		if ( pos > 0 )
		{
			string param_name = text.Substring(0, pos).Trim();
			int pos_end = text.Length() - pos;
			string param_value = text.Substring(pos + 1, pos_end - 1).Trim();
			
			int lenght = param_value.Length();
			if (lenght > 1 && param_value.Substring(0, 1) == "\"" && param_value.Substring(lenght - 1, 1) == "\"" )
			{
				param_value = param_value.Substring(1, lenght - 2);
			}
			
			ret.Insert(param_name);
			ret.Insert(param_value);
		}
		
		return ret;
	}

	TStringArray ParseTextToArray(string text, out TStringArray ret)
	{
		if ( !IsTextArray(text) )
		{
			return NULL;
		}
		
		text = text.Substring(1, text.Length() - 2);
		
		int pos_array_start = text.IndexOf("{");
		int pos_array_end = text.IndexOf("}");
		
		if ( pos_array_start > -1 && pos_array_end > -1 )
		{		
			while ( true )
			{
				string array_value = text.Substring(pos_array_start , pos_array_end - pos_array_start + 1  );
				ret.Insert(array_value);
				
				pos_array_start = text.IndexOfFrom(pos_array_end, "{");
				pos_array_end = text.IndexOfFrom(pos_array_start, "}");
				
				if ( pos_array_start == -1 )
				{
					break;
				}
			}
		}
		else
		{
			text.Split(",", ret);
		}
		
		return ret;
	}

	array<ref map<string, string>> ParseTextArrayToParameters(string text)
	{
		TStringArray array_items = new TStringArray;
		ParseTextToArray(text, array_items);
		
		array<ref map<string, string>> ret = new array<ref map<string, string>>;
		
		map<string, string> ret_param = new map<string, string>;
		
		for ( int i = 0; i < array_items.Count(); ++i )
		{
			string param = array_items.Get(i);
			TStringArray t_param = ParseTextToParameter(param);
			
			string param_name = t_param.Get(0);
			string param_value = t_param.Get(1);
			
			ret_param.Set(param_name, param_value);
		}
		
		ret.Insert(ret_param);	
		
		return ret;
	}
	
	string LoadParameter(string param_name)
	{
		string ret_value = STRING_EMPTY;
		
		if ( m_ConfigParams.Contains(param_name) )
		{
			ret_value = m_ConfigParams.Get(param_name);
		}

		return ret_value;
	}

	TStringArray LoadParameterArray(string param_name)
	{
		if ( m_ConfigParamsArray.Contains(param_name) )
		{
			return m_ConfigParamsArray.Get(param_name);
		}

		return NULL;
	}

	string LoadSubParameter(string param_name, int sub_array_index, string sub_param_name)
	{
		string 						ret_value = STRING_EMPTY;
		map<string, string>			param;
		array<ref map<string, string>>	sub_arrays;
		
		if ( m_ConfigParamsArrayInArray.Contains(param_name) )
		{
			sub_arrays = m_ConfigParamsArrayInArray.Get(param_name);
			
			if ( sub_arrays != NULL )
			{
				if ( sub_array_index < sub_arrays.Count() )
				{
					param = sub_arrays.Get(sub_array_index);
					
					if ( param.Contains(sub_param_name) )
					{
						ret_value = param.Get(sub_param_name);
					}
				}		
			}
		}
		
		return ret_value;
	}
	
	void SaveParameter(string param_name, string value, bool save_in_file = true)
	{
		if ( param_name == "" || param_name == "scene_editor_scene" )
		{
			Log("SaveParameter: "+param_name+" = "+value, "");
		}
		
		m_ConfigParams.Set(param_name, value);
		
		if ( save_in_file )
		{
			SaveConfigToFile();	
		}
	}

	void SaveParameterArray(string param_name, TStringArray value, bool save_in_file = true)
	{
		m_ConfigParamsArray.Set(param_name, value);
		
		if ( save_in_file )
		{
			SaveConfigToFile();	
		}
	}

	void SaveSubParameterInArray(string param_name, int sub_array_index, string sub_param_name, string sub_param_value, bool save_in_file = true)
	{
		array<ref map<string, string>> sub_arrays = m_ConfigParamsArrayInArray.Get(param_name);
		map<string, string> sub_parameter = sub_arrays.Get(sub_array_index);
		sub_parameter.Set(sub_param_name, sub_param_value);	
		
		if ( save_in_file )
		{
			SaveConfigToFile();	
		}
	}
	
	void RenameParameter(string old_name, string new_name, bool save_in_file = true)
	{
		if ( m_ConfigParams.Contains(old_name) )
		{
			m_ConfigParams.ReplaceKey(old_name, new_name);
			
			if ( save_in_file )
			{
				SaveConfigToFile();	
			}
		}
	}

	void RenameParameterArray(string old_name, string new_name, bool save_in_file = true)
	{
		if ( m_ConfigParamsArray.Contains(old_name) )
		{
			m_ConfigParamsArray.ReplaceKey(old_name, new_name);
			
			if ( save_in_file )
			{
				SaveConfigToFile();	
			}
		}
	}

	void RemoveParameter(string name, bool save_in_file = true)
	{
		m_ConfigParams.Remove(name);
		
		if ( save_in_file )
		{
			SaveConfigToFile();	
		}
	}

	void RemoveParameterArray(string name, bool save_in_file = true)
	{
		m_ConfigParamsArray.Remove(name);
		
		if ( save_in_file )
		{
			SaveConfigToFile();	
		}
	}
	
	void SetParameterString(string name, string value, bool save_in_file = true)
	{
		string s = "\""+value+"\"";
		SaveParameter(name, s, save_in_file);
	}

	void SetParameterInt(string name, int value, bool save_in_file = true)
	{
		SaveParameter(name, value.ToString(), save_in_file);
	}

	void SetParameterFloat(string name, float value, bool save_in_file = true)
	{
		SaveParameter(name, value.ToString(), save_in_file);
	}

	void SetParameterBool(string name, bool value, bool save_in_file = true)
	{
		SaveParameter(name, value.ToString(), save_in_file);
	}

	void SetParameterArray(string name, TStringArray value, bool save_in_file = true)
	{
		SaveParameterArray(name, value, save_in_file);
	}

	void SetSubParameterInArray(string param_name, int sub_array_index, string sub_param_name, string sub_param_value, bool save_in_file = true)
	{
		SaveSubParameterInArray(param_name, sub_array_index, sub_param_name, sub_param_value, save_in_file);
	}
	
	string GetParameterString(string name)
	{
		string loaded_param = LoadParameter(name);
		string ret_value = STRING_EMPTY;
		
		if ( loaded_param != "" )
		{
			TStringArray arr = new TStringArray;
			loaded_param.Split("\"", arr);
			if ( arr.Count() > 0 )
			{
				ret_value = arr.Get(0);	
			}
		}
		else
		{
			SetParameterString(name, ret_value);
		}
		
		return ret_value;
	}

	int GetParameterInt(string name)
	{
		string loaded_param = LoadParameter(name);
		int ret_value = 0;
		
		if ( loaded_param != "" )
		{
			ret_value = loaded_param.ToInt();
		}
		else
		{
			SetParameterInt(name, ret_value);
		}
		
		return ret_value;	
	}

	float GetParameterFloat(string name)
	{
		string loaded_param = LoadParameter(name);
		float ret_value = 0;
		
		if ( loaded_param != "" )
		{
			ret_value = loaded_param.ToFloat();
		}
		else
		{
			SetParameterFloat(name, ret_value);
		}
		
		return ret_value;
	}

	bool GetParameterBool(string name)
	{
		string loaded_param = LoadParameter(name);
		bool ret_value = false;
		
		if ( loaded_param != "" )
		{
			int v = loaded_param.ToInt();
			if ( v == 1 )
			{
				ret_value = true;
			}
		}
		else
		{
			SetParameterBool(name, ret_value);
		}
		
		return ret_value;
	}

	TStringArray GetParameterArray(string name)
	{
		TStringArray ret_value;
		
		if ( m_ConfigParamsArray.Contains(name) )
		{
			ret_value = m_ConfigParamsArray.Get(name);
		}
		else
		{
			ret_value = new TStringArray;
			SetParameterArray(name, ret_value);
		}
		
		return ret_value;
	}

	string GetSubParameterInArrayString(string param_name, int sub_array_index, string sub_param_name)
	{
		string loaded_param = LoadSubParameter(param_name, sub_array_index, sub_param_name);
		string ret_value = STRING_EMPTY;
		
		TStringArray arr = new TStringArray;
		
		if ( loaded_param != "" )
		{
			arr.Clear();
			loaded_param.Split("\"", arr);
			ret_value = arr.Get(1);			
		}
		
		return ret_value;
	}

	TStringArray GetAllSubParametersInArrayString(string param_name, string sub_param_name)
	{
		TStringArray ret_value = NULL;
		
		if ( m_ConfigParamsArrayInArray.Contains(param_name) )
		{
			ret_value = new TStringArray;
			
			array<ref map<string, string>> arrays_in_parameter = m_ConfigParamsArrayInArray.Get(param_name);
			
			for ( int i = 0; i < arrays_in_parameter.Count(); ++i )
			{			
				map<string, string> sub_parameter = arrays_in_parameter.Get(i);
				
				string sub_param = STRING_EMPTY;
				
				if ( sub_parameter.Contains(sub_param_name) )
				{
					sub_param = sub_parameter.Get(sub_param_name);
				}
				
				ret_value.Insert(sub_param);
			}
		}
		
		return ret_value;
	}
}
