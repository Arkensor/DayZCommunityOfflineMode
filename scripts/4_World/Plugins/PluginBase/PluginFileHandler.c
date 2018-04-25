class PluginFileHandler extends PluginBase
{
	static bool FileDuplicate(string source_name, string dest_name)
	{
		return CopyFile(source_name, dest_name);
	}

	static bool FileDelete(string file)
	{
		return DeleteFile(file);
	}

	static void FileRename(string source_name, string dest_name)
	{
		FileDuplicate(source_name, dest_name);
		FileDelete(source_name);
	}	
	
	bool	m_ReadOnly;
	int		m_LineLimit;
	
	ref TStringArray	m_FileContent;
	
	void PluginFileHandler()
	{
		m_FileContent = new TStringArray;
		m_LineLimit = -1;
		
		LoadFile();
	}

	void ~PluginFileHandler()
	{
		ClearFileNoSave();
	}
	
	override void OnInit()
	{
		super.OnInit();
	}
	
	string GetFileName()
	{
		return string.Empty;
	}
	
	void ClearFileNoSave()
	{
		m_FileContent.Clear();	
	}

	bool LoadFile()
	{
		ClearFileNoSave();

		FileHandle file_index = OpenFile(GetFileName(), FileMode.READ);
		
		if ( file_index == 0 )
		{
			return false;
		}
		
		string line_content = "";
		int char_count = FGets( file_index,  line_content );
		while ( char_count != -1 )
		{
			m_FileContent.Insert(line_content);
			
			char_count = FGets( file_index,  line_content );
		}
		
		CloseFile(file_index);
		
		return true;
	}

	bool SaveFile()
	{
		if ( m_ReadOnly )
		{
			return false;
		}
		
		//Log("SaveFile -> Opening File: "+GetFileName());
		
		FileHandle file_index = OpenFile(GetFileName(), FileMode.WRITE);
		
		if ( file_index == 0 )
		{
			return false;
		}
		
		for ( int i = 0; i < m_FileContent.Count(); ++i)
		{
			//Log("SaveFile -> Writing: "+m_FileContent.Get(i));
			FPrintln(file_index, m_FileContent.Get(i));
		}
		
		CloseFile(file_index);
		
		return true;
	}

	bool IsReadOnly()
	{
		return m_ReadOnly;
	}

	void AddText(string text)
	{
		AddNewLineNoSave(text);
		SaveFile();
	}

	void AddNewLineNoSave(string text)
	{
		if ( m_LineLimit > -1 )
		{
			if ( m_LineLimit == 0 )
			{
				return;
			}
			
			int lines_count = m_FileContent.Count();
			
			if ( lines_count > 0 && lines_count >= m_LineLimit )
			{
				int remove_indexes = lines_count - m_LineLimit;
				
				for ( int i = 0; i <= remove_indexes; ++i )
				{
					m_FileContent.RemoveOrdered(0);
				}
			}
		}
		
		m_FileContent.Insert(text);
	}

	TStringArray GetFileContent()
	{
		return m_FileContent;
	}
}
