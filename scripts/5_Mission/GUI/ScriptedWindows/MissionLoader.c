class MissionLoader extends UIScriptedWindow
{
	string				m_PathToMissions;
	TextListboxWidget	m_WgtLstMsnList;
	ButtonWidget		m_WgtBtnMsnPlay;
	ButtonWidget		m_WgtBtnMsnClose;
	
	ref TStringArray		m_ListMissionsNames;
	
	void MissionLoader( int id )
	{
		m_Id = id;
	}
		
	override Widget Init()
	{
		m_ListMissionsNames = GetMissionList();
		
		m_WgtRoot			= GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_mission_loader.layout", GetGame().GetUIManager().GetMenu().GetLayoutRoot() );	
		
		m_WgtLstMsnList = TextListboxWidget.Cast( m_WgtRoot.FindAnyWidget("wgt_lst_missions") );
		m_WgtBtnMsnPlay = ButtonWidget.Cast( m_WgtRoot.FindAnyWidget("wgt_btn_mission_play") );
		m_WgtBtnMsnClose = ButtonWidget.Cast( m_WgtRoot.FindAnyWidget("wgt_btn_mission_close") );
		
		for ( int i = 0; i < m_ListMissionsNames.Count(); ++i )
		{
			string mission_name = m_ListMissionsNames.Get(i);
			m_WgtLstMsnList.AddItem(mission_name, NULL, 0);
		}

		return m_WgtRoot;
	}

	private TStringArray GetMissionList()
	{
		string	file_name;
		int 	file_attr;
		int		flags;
		TStringArray list = new TStringArray;
		
		m_PathToMissions = "$saves:\\missions";
		
		string path_find_pattern = m_PathToMissions+"\\*"; //*/
		FindFileHandle file_handler = FindFile(path_find_pattern, file_name, file_attr, flags);
		
		bool found = true;
		while ( found )
		{				
			if ( file_attr & FileAttr.DIRECTORY )
			{
				list.Insert(file_name);
			}
			
			found = FindNextFile(file_handler, file_name, file_attr);;
		}
		
		return list;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w == m_WgtBtnMsnPlay )
		{
			int row_index = m_WgtLstMsnList.GetSelectedRow();
			
			string mission_name = m_ListMissionsNames.Get(row_index);
			
			string mission_path = m_PathToMissions+"\\"+mission_name;
			
			Log(mission_path);
			
			GetGame().PlayMission(mission_path);
			
			return true;
		}
		
		if ( w == m_WgtBtnMsnClose )
		{
			CloseWindow();
			
			return true;
		}
		
		return false;
	}	
}