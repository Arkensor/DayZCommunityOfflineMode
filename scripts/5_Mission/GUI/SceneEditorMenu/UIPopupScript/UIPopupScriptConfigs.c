class UIPopupScriptConfigs extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	private ButtonWidget m_BtnCopyToClipboard;
	
	private TextListboxWidget m_ConfigHierarchyTextListbox;
	private TextListboxWidget m_ConfigVariablesTextListbox;
	
	private PluginConfigViewer m_ModuleConfigViewer;
	
	private EditBoxWidget m_ObjectConfigFilter;
	
	void UIPopupScriptConfigs( Widget wgt )
	{
		m_BtnOk		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pc_ok") );
		m_ConfigHierarchyTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget("ConfigHierarchy") );
		m_ConfigVariablesTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget("ConfigVariables") );
		m_BtnCopyToClipboard = ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pc_copy") );
		m_ObjectConfigFilter = EditBoxWidget.Cast( wgt.FindAnyWidget("ObjectConfigFilter") );
	}
	
	void ChangeConfigFilter()
	{
		if ( m_ObjectConfigFilter.GetText() == "" )
		{
			ClearHierarchy();
		}
		else
		{
			FindInHierarchy( m_ObjectConfigFilter.GetText() );
		}
	}
	
	override void OnOpen(Param param)
	{
		m_ModuleConfigViewer	= PluginConfigViewer.Cast( GetPlugin( PluginConfigViewer ) );
		ClearHierarchy();
	}
	
	void AddItemToClipboard( TextListboxWidget text_listbox_widget )
	{
		int selected_row_index = text_listbox_widget.GetSelectedRow();
		if ( selected_row_index != -1 )
		{
			string item_name;
			text_listbox_widget.GetItemText( selected_row_index, 0, item_name );
			GetGame().CopyToClipboard( item_name );
		}
	}
	
	void RenderVariables( int row )
	{
		ConfigParams config_params;
		if( row > -1 && row < m_ConfigHierarchyTextListbox.GetNumItems() )
		{
			m_ConfigHierarchyTextListbox.GetItemData( row, 0, config_params );
			m_ConfigVariablesTextListbox.ClearItems();
			ref TStringArray variables;
			string path = config_params.param4;
			variables = m_ModuleConfigViewer.GetConfigVariables( path );
			for ( int i = 0; i < variables.Count(); i++ )
			{
				m_ConfigVariablesTextListbox.AddItem( variables.Get(i), NULL, 0);
			}
		}
	}
	
	void ExpandHierarchy( int row )
	{
		if( row <= -1 && row >= m_ConfigHierarchyTextListbox.GetNumItems() )
		{
			return;
		}
		
		ref TStringArray variables;

		// get current row data
		ConfigParams config_params;
		m_ConfigHierarchyTextListbox.GetItemData( row, 0, config_params );

		string config_path = config_params.param4;
		int deep = config_params.param5;

		string offset = "";
		for ( int i = 0; i < deep; i++)
		{
			offset = offset + "  ";
		}

		// change selected node
		variables = m_ModuleConfigViewer.GetConfigHierarchy( config_path );
		int childrens_count = variables.Count();
		m_ConfigHierarchyTextListbox.SetItem( row, offset + "- " + config_params.param2, new ConfigParams( true, config_params.param2, childrens_count, config_path, deep ), 0 );

		offset = offset + "  ";

		// render childrens
		deep = deep + 1;
		childrens_count = 0;
		for ( i = variables.Count() - 1; i >= 0; i-- )
		{
			string new_config_path = ( config_path + " " + variables.Get(i) ).Trim();
			m_ConfigHierarchyTextListbox.AddItem( offset + "+ " + variables.Get(i), new ConfigParams( false, variables.Get(i), childrens_count, new_config_path, deep  ), 0, (row + 1) );
		}
	}

	// niekde je bug, ked su len 2 polozky a expand/collapse na prvu tak zmaze aj druhu. ak collapse a expand na druhu tak crash lebo sa snazi zmazat riadok co neexistuje
	void CollapseHierarchy( int row )
	{
		if( row <= -1 && row >= m_ConfigHierarchyTextListbox.GetNumItems() )
		{
			return;
		}
		
		ConfigParams config_params;
		ConfigParams config_params_next;
		m_ConfigHierarchyTextListbox.GetItemData( row, 0, config_params );
		m_ConfigHierarchyTextListbox.GetItemData( row + 1, 0, config_params_next );

		int deep = config_params.param5;
		int deep_next = config_params_next.param5;
		int max_count = m_ConfigHierarchyTextListbox.GetNumItems();
		int remove_lines_count = 0;
		// Print(max_count);
		for ( int i = row + 1; i < max_count; i++)
		{
			if ( deep < deep_next && i <= max_count )
			{
				remove_lines_count = remove_lines_count + 1;
				m_ConfigHierarchyTextListbox.GetItemData( i, 0, config_params_next );
				deep_next = config_params_next.param5;
			}
		}

		// remove remove_lines_count lines from row
		// remove_lines_count = remove_lines_count - 1;
		for ( i = 1; i < remove_lines_count; i++ )
		{
			int x = row + 1;
			if ( x < max_count )
			{
				m_ConfigHierarchyTextListbox.RemoveRow( x );
			}
		}

		string offset = "";
		for ( i = 0; i < deep; i++)
		{
			offset = offset + "  ";
		}
		m_ConfigHierarchyTextListbox.SetItem( row, offset + "+ " + config_params.param2, new ConfigParams( false, config_params.param2, 0, config_params.param4, deep ), 0 );
	}
	
	void ClearHierarchy()
	{
		// config hierarchy
		// string config_path = "configfile CfgVehicles APC";
		// string config_path = "";

		m_ConfigHierarchyTextListbox.ClearItems();
		m_ConfigVariablesTextListbox.ClearItems();

		ref TStringArray base_classes = new TStringArray;
		Debug.GetBaseConfigClasses( base_classes );

		string config_path = "configfile";
		ref TStringArray variables = m_ModuleConfigViewer.GetConfigHierarchy( config_path );
		for ( int i = 0; i < variables.Count(); i++ )
		{
			string variable = variables.Get(i);

			for ( int j = 0; j < base_classes.Count(); j++ )
			{
				if ( variable == base_classes.Get(j) )
				{
					string new_config_path = ( config_path + " " + variable ).Trim();
					m_ConfigHierarchyTextListbox.AddItem( "+ " + variable, new ConfigParams( false, variable, 0, new_config_path, 0 ), 0);
				}
			}
		}
	}
	
	void FindInHierarchy( string class_name )
	{
		m_ConfigHierarchyTextListbox.ClearItems();
		m_ConfigVariablesTextListbox.ClearItems();

		class_name.ToLower(  );
		string config_base_path = "configfile";

		ref TStringArray base_classes = new TStringArray;
		Debug.GetBaseConfigClasses( base_classes );

		string filter_lower = class_name;
		filter_lower.ToLower(  );

		ref TStringArray filters = new TStringArray;
		filter_lower.Split( " ", filters );

		for ( int i = 0; i < base_classes.Count(); i++ )
		{
			string config_path = config_base_path + " " + base_classes.Get(i);
			ref TStringArray variables = m_ModuleConfigViewer.GetConfigHierarchy( config_path );

			for ( int j = 0; j < variables.Count(); j++ )
			{
				string variable = variables.Get(j);
				string variable_lower = variable;
				variable_lower.ToLower(  );

				for ( int k = 0; k < filters.Count(); k++ )
				{
					if ( variable_lower.Contains(filters.Get(k)))
					{
						string new_config_path = ( config_path + " " + variable ).Trim();
						m_ConfigHierarchyTextListbox.AddItem( "+ " + variable, new ConfigParams( false, variable, 0, new_config_path, 0 ), 0);
						break;
					}
				}
			}
		}
	}
	
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		super.OnChange( w, x, y, finished );

		if ( w == m_ObjectConfigFilter )
		{
			ChangeConfigFilter();
		}

		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if ( w == m_BtnOk )
		{		
			PopupBack();
			return true;
		}
		else if ( w == m_BtnCancel )
		{
			PopupBack();
			return true;
		}
		else if ( w == m_BtnCopyToClipboard )
		{
			AddItemToClipboard( m_ConfigVariablesTextListbox );
			return true;
		}
		else if ( w == m_ConfigHierarchyTextListbox )
		{
			int objects_row_index = m_ConfigHierarchyTextListbox.GetSelectedRow();
			
			if ( objects_row_index > -1 && objects_row_index < m_ConfigHierarchyTextListbox.GetNumItems() )
			{
				ConfigParams config_params;
				m_ConfigHierarchyTextListbox.GetItemData( objects_row_index, 0, config_params );
				
				if ( config_params.param1 == false )
				{
					ExpandHierarchy( objects_row_index );
				}
				else
				{
					CollapseHierarchy( objects_row_index );
				}
				RenderVariables( objects_row_index );
			}
			
			return true;
		}
		
		return false;
	}
}
