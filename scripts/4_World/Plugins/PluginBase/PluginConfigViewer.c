class PluginConfigViewer extends PluginBase
{
	void PluginConfigViewer()
	{
	}
	
	string MakeTabs( int count, bool inheritance = false )
	{
		if ( count == 0 )
		{
			return "";
		}

		string tabs = "|--";
		if ( inheritance )
		{
			tabs = "|<<";
		}
		
		for ( int i = 0; i < count - 1; i++ )
		{
			tabs = "|  " + tabs;
		}

		return tabs;
	}

	string GetOnlyChildPath( string config_class_path, string class_path )
	{
		int config_class_path_len = config_class_path.Length();
		int class_path_len = class_path.Length();
		
		if ( class_path_len > config_class_path_len )
		{
			int start = config_class_path_len;
			int count = class_path_len - start;
			
			return class_path.Substring(start, count).Trim();
		}
		else
		{
			return "";
		}
	}

	string GetBaseClassPath( string config_class_path, string class_path, string config_class )
	{
		if ( class_path == "" )
		{
			return "";
		}
		
		int start = config_class_path.Length();
		int count = class_path.Length() - start;
		string class_path_without_config_class = GetOnlyChildPath( config_class_path, class_path );
		
		local ref TStringArray full_path = new TStringArray;
		GetGame().ConfigGetFullPath( config_class_path, full_path );
		
		if ( full_path.Count() > 1 && class_path_without_config_class != "" )
		{
			return GetBaseClassPathRecursive( config_class + " " + full_path.Get(1), class_path_without_config_class, config_class );
		}
		else
		{
			return "";
		}
	}

	string GetBaseClassPathCombined( string child_path, string base_class )
	{
		int current = -1;
		int last = 0;
		
		while ( true )
		{
			current = child_path.IndexOfFrom(last, " " );
			if ( current > -1 )
			{
				last = current+1;
			}
			else
			{
				break;
			}
		}
		
		string result = child_path.Substring(0, last ) + base_class;
		return result;
	}

	string GetBaseClassPathRecursive( local string config_class_path, local string class_path, local string config_class )
	{
		if ( ContainsFirstClass(config_class_path, class_path) )
		{
			return config_class_path + " " + class_path;
		}
		else 
		{
			local ref TStringArray full_path = new TStringArray;
			GetGame().ConfigGetFullPath( config_class_path, full_path );
		
			if ( full_path.Count() > 1 )
			{
				return GetBaseClassPathRecursive( config_class + " " + full_path.Get(1), class_path, config_class );	
			}
			else
			{
				return "";
			}
		}
	}

	bool ContainsFirstClass( string config_class_path, string class_path )
	{
		int start = class_path.IndexOf(" ");
		string name = class_path;
		if ( start > -1 )
		{
			name = class_path.Substring(0, start );
		}
		
		int cfg_class_count = GetGame().ConfigGetChildrenCount( config_class_path );
		
		for ( int i = 0; i < cfg_class_count; i++ )
		{
			string cfg_class_name = "";
			GetGame().ConfigGetChildName( config_class_path, i, cfg_class_name );
			
			if ( cfg_class_name == name )
			{
				string full_name = config_class_path + " " + name;
				int type = GetGame().ConfigGetType( full_name );
				
				if ( type == CT_CLASS )
				{
					return true;
				}
			}
		}
		
		if ( start == -1 )
		{
			return false;
		}
		else
		{
			string new_name =  class_path.Substring(start + 1, class_path.Length() - (start + 1) );
			return ContainsFirstClass( config_class_path + " " + name, new_name );
		}
	}

	TStringArray GetConfigRecursive( local string path, local string name, local string config_class_path, local string config_class, local array<string> overridden, local int depth = 0, local bool check_base_class_of_class = false )
	{
		local string tabs = MakeTabs( depth + 1 );
		local string child_path = path;
		local int count = GetGame().ConfigGetChildrenCount( child_path );
		local int i = 0;
	  
		local bool name_printed = false;
	  
		local TStringArray result = new TStringArray;

		for ( i = 0; i < count; i++ )
		{
			local string child_name = "";
			GetGame().ConfigGetChildName( child_path, i, child_name );
			local string c_child_path = child_path + " " + child_name;

			local string child_name_lower = child_name;
			child_name_lower.ToLower();

			if ( overridden.Find(child_name_lower) == -1 )
			{
				if ( !name_printed )
				{
					result.Insert( ":" + MakeTabs(depth, check_base_class_of_class) + "!" + name );
					name_printed = true;
				}
				overridden.Insert( child_name_lower );
					
				local int type = GetGame().ConfigGetType( c_child_path );
				
				if ( type != CT_CLASS )
				{
					if ( type != CT_ARRAY )
					{
						result.Insert( "-" + tabs + "!" + child_name + " = " + GetGame().ConfigGetTextOut(c_child_path) );
					}
					else
					{
						result.Insert( "-" + tabs + "!" + child_name + "[] = {" );
						
						local TStringArray strs = new TStringArray;
						GetGame().ConfigGetTextArray( c_child_path, strs );
						string tabs_array = MakeTabs( depth + 2 );
						int j;
						for ( j = 0; j < strs.Count(); j++ )
						{
							result.Insert( "-" + tabs + "!" + child_name + "[]" + j.ToStringLen(6) + "~" + strs.Get(j) + "," );
						}
						
						result.Insert( "-" + tabs + "!" + child_name + "[]" + j.ToStringLen(6) + "~}" );
					}
				}
				else
				{
					array<string> overridden_sub = new array<string>;
					
					if ( GetGame().ConfigGetChildrenCount(c_child_path) > 0 )
					{
						TStringArray config1 = GetConfigRecursive( c_child_path, child_name, config_class_path, config_class, overridden_sub, depth + 1 );
						result.InsertAll( config1 );
					}
					else 
					{
						result.Insert( ":" + tabs + "!" + child_name );
					}
					
					string cc_child_path = c_child_path;
					string c_config_class_path = config_class_path;
					string c_child_name = child_name;
					
					int cc = 0;
					
					while ( true ) 
					{
						local string base_class = "";
					
						GetGame().ConfigGetBaseName( cc_child_path, base_class );
					
						int start = c_config_class_path.Length() + 1;
						int len = cc_child_path.Length();
						len -= c_config_class_path.Length() + c_child_name.Length();
						len -= 2;
						
						if ( base_class != "" )
						{
							local string only_child_path = "";
							if ( len < 0 )
							{
								only_child_path = base_class;
							}
							else
							{
								only_child_path = cc_child_path.Substring( start, len ) + " " + base_class;
							}
							
							string c_base_child_path = GetBaseClassPathCombined( cc_child_path, base_class );
							string base_class_path = GetBaseClassPath( c_config_class_path, c_base_child_path, config_class );
							
							if ( base_class_path != "" )
							{
								TStringArray config2 = GetConfigRecursive( base_class_path, base_class, base_class_path, config_class, overridden_sub, depth + 1, true );
								result.InsertAll( config2 );
							}
							else
							{
								TStringArray config3 = GetConfigRecursive( base_class, base_class, base_class, config_class, overridden_sub, depth + 1, true );
								result.InsertAll( config3 );
								
								break;
							}
							
							cc_child_path = base_class_path;
							c_child_name = base_class;
							c_config_class_path = cc_child_path.Substring( 0, cc_child_path.Length() - only_child_path.Length() - 1);
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		if ( name_printed )
		{
			result.Insert( ":" + MakeTabs(depth, check_base_class_of_class) + "#" + name );
		}

		if ( !check_base_class_of_class )
		{
			TStringArray full_path = new TStringArray;
			GetGame().ConfigGetFullPath( child_path, full_path );

			if ( full_path.Count() > 1 )
			{
				TStringArray config4 = GetConfigRecursive( config_class + " " + full_path.Get(1), full_path.Get(1), config_class + " " + full_path.Get(1), config_class, overridden, depth );
				result.InsertAll( config4 );
			}
		}
		else
		{
			string class_base_class = "";
			GetGame().ConfigGetBaseName( child_path, class_base_class );
			
			if ( class_base_class != "" )
			{
				string base_child_path = GetBaseClassPathCombined( child_path, class_base_class );
				string cc_base_child_path = GetBaseClassPath( config_class_path, base_child_path, config_class );

				if ( cc_base_child_path != "" )
				{
					TStringArray config5 = GetConfigRecursive( cc_base_child_path, class_base_class, cc_base_child_path, config_class, overridden, depth, true );
					result.InsertAll( config5 );
				}
				else
				{
					TStringArray config6 = GetConfigRecursive( class_base_class, class_base_class, class_base_class, config_class, overridden, depth, true );
					result.InsertAll( config6 );
				}
			}
		}

		return result;
	}

	TStringArray GetConfig( string class_path, string filter_char )
	{
		int i;
		TStringArray filtered = new TStringArray;
		
		if ( class_path != "" )
		{
			PluginDeveloper module_dev = PluginDeveloper.Cast( GetPlugin( PluginDeveloper ) );
			
			TStringArray path_nodes = new TStringArray;
			class_path.Split( " ", path_nodes );
			
			if ( path_nodes.Count() >= 3 )
			{
				string c_class_path = path_nodes.Get(0) + " " + path_nodes.Get(1) + " " + path_nodes.Get(2);
				
				array<string> overridden = new array<string>;
				string config_class = path_nodes.Get( 1 );
				string class_name = path_nodes.Get( 2 );
				
				TStringArray result = GetConfigRecursive( c_class_path, class_name, c_class_path, config_class, overridden );
				
				TStringArray nested_start = new TStringArray;
				TStringArray nested_end = new TStringArray;
				TStringArray nested_inherited_start = new TStringArray;
				TStringArray nested_inherited_end = new TStringArray;
				string tabs = MakeTabs( 1 );
				string tabs_inherited = MakeTabs( 1, true );
				string check_tabs = MakeTabs( path_nodes.Count() - 1, true );
				
				nested_start.Insert( ":!" );
				nested_end.Insert( ":#" );
				nested_inherited_start.Insert( ":!" );
				nested_inherited_end.Insert( ":#" );
				
				string last_node = filter_char + MakeTabs( nested_start.Count() ) + "!";
				
				if ( path_nodes.Count() == 3 )
				{
					last_node = filter_char + MakeTabs( 1 ) + "!";
				}
				else 
				{
					for ( i = 1; i < path_nodes.Count() - 2; i++ )
					{
						tabs = MakeTabs( i );
						tabs_inherited = MakeTabs( i, true );
						
						string path_node = path_nodes.Get(i+2);
						path_node.ToLower(  );
						
						nested_start.Insert( ":" + tabs + "!" + path_node );
						nested_end.Insert( ":" + tabs + "#" + path_node );
						nested_inherited_start.Insert( ":" + tabs_inherited + "!" + path_node );
						nested_inherited_end.Insert( ":" + tabs_inherited + "#" + path_node );
					}
					
					last_node = filter_char + MakeTabs( i ) + "!";
				}
				
				int current_nest = 0;
				
				for ( i = 0; i < result.Count(); i++ )
				{
					string current = result.Get( i );
					string current_lower = current;
					current_lower.ToLower( );
					
					int len_current = current.Length();
		
					if ( current_nest < nested_start.Count() )
					{
						int len_start = nested_start.Get(current_nest).Length();
							
						if ( len_current >= len_start )
						{
							string prefix_start = current_lower;
							
							if ( current_nest == 0 )
							{
								prefix_start = current_lower.Substring( 0, len_start );
							}
								
							if ( prefix_start == nested_start.Get(current_nest) || prefix_start == nested_inherited_start.Get(current_nest) )
							{
								current_nest++;
								continue;
							}
						}
					}
							
					if ( current_nest >= 1 )
					{
						int len_end = nested_end.Get(current_nest - 1).Length();
							
						if ( len_current >= len_end )
						{
							string prefix_end = current_lower;
									
							if ( current_nest == 0 )
							{
								prefix_start = current_lower.Substring( 0, len_start );
							}		
									
							if ( prefix_end == nested_end.Get(current_nest - 1) || prefix_end == nested_inherited_end.Get(current_nest - 1) )
							{
								current_nest--;
							}
						}
					}

					if ( current_nest == nested_start.Count() )
					{
						string first_char = current.Substring( 0, 1 );
							
						if ( first_char == filter_char )
						{
							int bang_pos = current.IndexOf( "!" );
								
							if ( bang_pos > -1 )
							{
								int len_last_node = last_node.Length();
								
								if ( len_current >= len_last_node )
								{
									string prefix_current = current_lower.Substring( 0, len_last_node );

									if ( last_node == prefix_current )
									{
										filtered.Insert( current.Substring(bang_pos + 1, current.Length() - (bang_pos + 1)) );
									}
								}
							}
						}
					}		
				}
				
				TStringArray without_duplications = new TStringArray;
				
				map<string, string> lowered = new map<string, string>;
				
				for ( i = 0; i < filtered.Count(); i++ )
				{
					string c = filtered.Get( i );
					string lower_c = c;
					lower_c.ToLower();
					
					if ( without_duplications.Find(lower_c) < 0 )
					{
						without_duplications.Insert( lower_c );
						
						if ( lower_c != c )
						{
							lowered.Set( lower_c, c );
						}
					}
				}
				
				module_dev.SortStringArray( without_duplications );
					
				for ( i = 0; i < without_duplications.Count(); i++ )
				{
					string cc = without_duplications.Get( i );
					
					if ( lowered.Contains(cc) )
					{
						cc = lowered.Get( cc );
					}
					
					int tilda_index = cc.IndexOf( "~" );
					if ( tilda_index > -1 )
					{
						string spaces = "    ";
						if ( cc.IndexOf( "~}" ) > -1 )
						{
							spaces = "";
						}
						cc = spaces + cc.Substring(tilda_index + 1, cc.Length() - (tilda_index + 1) );
					}
					
					without_duplications.Set( i, cc );
				}
					
				return without_duplications;
			}
			else if ( filter_char == ":" )
			{
				int cnt_config = GetGame().ConfigGetChildrenCount( class_path );
				
				for ( i = 0; i < cnt_config; i++ )
				{
					string config_name = "";
					GetGame().ConfigGetChildName( class_path, i, config_name );
					filtered.Insert( config_name );
				}
				
				module_dev.SortStringArray( filtered );	
			}
		}
		else if ( filter_char == ":" )
		{
			filtered.Insert( "configfile" );
			filtered.Insert( "missionconfigfile" );
		}
			
		return filtered;
	}

	TStringArray GetConfigVariables( string class_path )
	{
		return GetConfig( class_path, "-" );
	}

	TStringArray GetConfigHierarchy( string class_path )
	{
		return GetConfig( class_path, ":" );
	}
}
