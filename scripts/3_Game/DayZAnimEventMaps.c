class SoundLookupTable
{
	void SoundLookupTable()
	{
		m_soundBuilders = new map<int, ref array<SoundObjectBuilder>>();
	}
	
	void InitTable(string tableCategoryName, string parameterName)
	{
		m_tableCategoryName = tableCategoryName;
		m_parameterName = parameterName;
	}
	
	void LoadTable(string soundLookupTableName)
	{
		string path = "CfgSoundTables " + m_tableCategoryName + " " + soundLookupTableName;
		
		//load all classes names
		int soundCount = GetGame().ConfigGetChildrenCount(path);

		for(int i = 0; i < soundCount; i++)
		{
			string soundClassName;
			GetGame().ConfigGetChildName(path, i, soundClassName);
			string soundClassPath = path + " " + soundClassName + " ";

			string parameter;
			GetGame().ConfigGetText(soundClassPath + m_parameterName, parameter);

			array<string> soundSetNames = new array<string>;
			GetGame().ConfigGetTextArray(soundClassPath + "soundSets", soundSetNames);

			//TODO create SoundObject for every entry, save in Game?
			array<SoundObjectBuilder> soundObjectBuilders = new array<SoundObjectBuilder>;
			for(int j = 0; j < soundSetNames.Count(); j++)
			{
				AnimSoundObjectBuilderBank bank = AnimSoundObjectBuilderBank.GetInstance();
				SoundObjectBuilder soundObjectBuilder = bank.GetBuilder(soundSetNames.Get(j));

				if(soundObjectBuilder != NULL)
					soundObjectBuilders.Insert(soundObjectBuilder);
			}

			if(soundObjectBuilders.Count() > 0)
			{
				//Print("SoundLookupTable::LoadTable: path: " + path + " param:" + parameter + " param#:" + parameter.Hash() + " objBuildersCount: " + soundObjectBuilders.Count());
				m_soundBuilders.Insert(parameter.Hash(), soundObjectBuilders);
			}
		}
	}
	
	SoundObjectBuilder GetSoundBuilder(int parameterHash)
	{
		array<SoundObjectBuilder> soundObjects = m_soundBuilders.Get(parameterHash);

		if(soundObjects == NULL || soundObjects.Count() == 0)
		{
			return NULL;
		}
		else if (soundObjects.Count() == 1)
		{
			return soundObjects.Get(0);
		}
		else
		{
			int index = Math.RandomInt(0, soundObjects.Count());
			return soundObjects.Get(index);
		}
	}
	
	
	private string m_tableCategoryName;
	private string m_parameterName;
	private ref map<int, ref array<SoundObjectBuilder>> m_soundBuilders;
}


class StepSoundLookupTable extends SoundLookupTable<StepSoundTraits>
{
	void StepSoundLookupTable()
	{
		InitTable("CfgStepSoundTables", "surface");
	}
}

class AttachmentSoundLookupTable extends SoundLookupTable
{
	void AttachmentSoundLookupTable()
	{
		InitTable("CfgAttachmentSoundTables", "category");
	}
}


class AnimSoundObjectBuilderBank
{
	void AnimSoundObjectBuilderBank()
	{
		m_pBuilders = new map<int, ref SoundObjectBuilder>();
	}


	static AnimSoundObjectBuilderBank GetInstance()
	{
		if(m_instance == NULL)
			m_instance = new AnimSoundObjectBuilderBank();

		return m_instance;
	}


	SoundObjectBuilder GetBuilder(string soundSetName)
	{
		int soundSetNameHash = soundSetName.Hash();

		SoundObjectBuilder builder = m_pBuilders.Get(soundSetNameHash);
		if(builder == NULL)
		{
			SoundParams params = new SoundParams(soundSetName);
			if(params.IsValid())
			{
				builder = new SoundObjectBuilder(params);
				m_pBuilders.Insert(soundSetNameHash, builder);
			}
			else
			{
				Print("AnimSoundObjectBuilderBank: Invalid sound set \"" + soundSetName + "\".");
				return NULL;
			}
		}
		return builder;
	}

	private static ref AnimSoundObjectBuilderBank m_instance;
	private autoptr map<int, ref SoundObjectBuilder> m_pBuilders;
}


class AnimSoundLookupTableBank
{
	void AnimSoundLookupTableBank()
	{
		m_pTables = new map<int, ref SoundLookupTable>();
	}


	static AnimSoundLookupTableBank GetInstance()
	{
		if(m_instance == NULL)
			m_instance = new AnimSoundLookupTableBank();

		return m_instance;
	}


	SoundLookupTable GetStepTable(string tableName)
	{
		int tableNameHash = tableName.Hash();

		SoundLookupTable table = m_pTables.Get(tableNameHash);
		if(table == NULL)
		{
			table = new StepSoundLookupTable();
			table.LoadTable(tableName);
			m_pTables.Insert(tableNameHash, table);
		}
		return table;
	}
	
	SoundLookupTable GetAttachmentTable(string tableName)
	{
		int tableNameHash = tableName.Hash();

		SoundLookupTable table = m_pTables.Get(tableNameHash);
		if(table == NULL)
		{
			table = new AttachmentSoundLookupTable();
			table.LoadTable(tableName);
			m_pTables.Insert(tableNameHash, table);
		}
		return table;
	}

	private static ref AnimSoundLookupTableBank m_instance;
	private autoptr map<int, ref SoundLookupTable> m_pTables;
}
