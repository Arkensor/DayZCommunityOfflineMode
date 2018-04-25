class JsonObject
{	
	ref map<string, string> 	m_Strings;
	ref map<string, int>		m_Ints;
	ref map<string, float>		m_Floats;
	ref map<string, bool>		m_Bools;
	ref map<string, ref Vector2>	m_Vectors2;
		
	void JsonObject()
	{
		m_Strings	= new map<string, string>;
		m_Ints		= new map<string, int>;
		m_Floats	= new map<string, float>;
		m_Bools		= new map<string, bool>;
		m_Vectors2	= new map<string, ref Vector2>;
	}
	
	void ~JsonObject()
	{
		Clear();
	}
		
	void Clear()
	{
		m_Strings.Clear();
		m_Ints.Clear();
		m_Floats.Clear();
		m_Bools.Clear();
		m_Vectors2.Clear();
	}
	
	void AddString(string name, string value)
	{
		array<string> strgs = new array<string>;
		value.Split("\"", strgs);
		
		if ( strgs.Count() > 1 )
		{
			value = "";
			int str_count = strgs.Count();
			
			for ( int i = 0; i < str_count; ++i )
			{
				string s = strgs[i];
				
				int length = s.Length();
	
				if ( s[length - 1] != "\\" )
				{
					value += s;
					
					if (i < str_count - 1 )
					{
						value += "\\";
						value += "\"";
					}
				}
			}
		}
				
		m_Strings.Insert(name, value);
	}
	
	void AddInt(string name, int value)
	{
		m_Ints.Insert(name, value);
	}
	
	void AddFloat(string name, float value)
	{
		m_Floats.Insert(name, value);
	}
	
	void AddBool(string name, bool value)
	{
		m_Bools.Insert(name, value);
	}
	
	void AddVector2(string name, float x, float y)
	{
		Vector2 v = new Vector2(x, y);
		m_Vectors2.Insert(name, v);
	}
	
	string GetJson()
	{
		string name;
		int i;
		
		string jsn = "";
		jsn += "{";
		
		// Parse Strings
		for ( i = 0; i < m_Strings.Count(); ++i )
		{
			if ( jsn.Length() > 1 )
			{
				jsn += ",";
			}
				
			name = m_Strings.GetKey(i);
			string value_str = m_Strings.GetElement(i);
			
			jsn += "\""+name+"\":\""+value_str+"\"";
		}
			
		// Parse Ints
		for ( i = 0; i < m_Ints.Count(); ++i )
		{
			if ( jsn.Length() > 1 )
			{
				jsn += ",";
			}
				
			name = m_Ints.GetKey(i);
			int value_int = m_Ints.GetElement(i);
			
			jsn += "\""+name+"\":"+value_int;
		}
				
		// Parse Floats
		for ( i = 0; i < m_Floats.Count(); ++i )
		{
			if ( jsn.Length() > 1 )
			{
				jsn += ",";
			}
				
			name = m_Floats.GetKey(i);
			float value_flt = m_Floats.GetElement(i);
			
			jsn += "\""+name+"\":"+value_flt;
		}
			
		// Parse Bools
		for ( i = 0; i < m_Bools.Count(); ++i )
		{
			if ( jsn.Length() > 1 )
			{
				jsn += ",";
			}
				
			name = m_Bools.GetKey(i);
							
			if ( m_Bools.GetElement(i) )
			{
				jsn += "\""+name+"\":true";
			}
			else
			{
				jsn += "\""+name+"\":false";
			}
		}
		
		// Parse Vectors2
		for ( i = 0; i < m_Vectors2.Count(); ++i )
		{
			if ( jsn.Length() > 1 )
			{
				jsn += ",";
			}
				
			name = m_Vectors2.GetKey(i);
			Vector2 value_vct = m_Vectors2.GetElement(i);
			
			jsn += "\""+name+"\":{\"x\":"+value_vct.x+",\"y\":"+value_vct.y+"}";
		}
				
		jsn += "}";
		
		return jsn;
	}
};