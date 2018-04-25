/*
//Mission = "ChernarusPlus"
typedef CfgParamString DefCfgParamString

//InitTime = 220
typedef CfgParamInt DefCfgParamInt

//InitWeatherRain = 0.5
typedef CfgParamFloat DefCfgParamFloat

//InitWeatherFog = 0.3
typedef CfgParamFloat DefCfgParamFloat

//SceneObjectsCount = 1
typedef CfgParamInt DefCfgParamInt
*/

class PluginLocalProfileScene extends PluginLocalProfile
{
	private const string FILE_ROOT			= "$saves:";
	private const string FILE_ROOT_SCENES	= "Scenes";
	private const string PARAM_MISSION		= "Mission";
	private const string PARAM_TIME			= "InitTime";
	private const string PARAM_RAIN			= "InitWeatherRain";
	private const string PARAM_FOG			= "InitWeatherFog";
	private const string PARAM_OBJ_COUNT	= "SceneObjectsCount";
	private const string PARAM_OBJ_NAME		= "SceneObject";
	
	private string m_FileSceneName;
	
	//========================================
	// GetPathScenes
	//========================================
	string GetPathScenes()
	{
		return FILE_ROOT+"\\"+FILE_ROOT_SCENES;
	}
	
	//========================================
	// GetFileName
	//========================================
	override string GetFileName()
	{
		string file_name = GetPathScenes()+"\\"+m_FileSceneName;
		return file_name;
	}
	
	//========================================
	// OnInit
	//========================================
	override void OnInit()
	{
		super.OnInit();
	}
	
	//========================================
	// GetSceneList
	//========================================	
	TStringArray GetSceneList()
	{
		if ( !FileExist( GetPathScenes() ) )
		{
			MakeDirectory( GetPathScenes() );
		}
		
		string	file_name;
		int 	file_attr;
		int		flags;
		TStringArray list = new TStringArray;
		
		string path_find_pattern = GetPathScenes()+"/*.scene"; 
		FindFileHandle file_handler = FindFile(path_find_pattern, file_name, file_attr, flags);
		
		bool found = true;
		while ( found )
		{				
			list.Insert(file_name);
			
			found = FindNextFile(file_handler, file_name, file_attr);
		}
		
		return list;
	}

	//========================================
	// SceneSave
	//========================================	
	void SceneSave(SceneData scene)
	{
		m_FileSceneName = scene.GetNameScene()+".scene";
		
		array<ref SceneObject> objects = scene.GetSceneObjects();
		
		// Save Mission Name
		SetParameterString(PARAM_MISSION, scene.GetNameMission(), false);
		//Save Init Time
		SetParameterFloat(PARAM_TIME, scene.GetInitTime(), false);
		//Save Init Weather Rain
		SetParameterFloat(PARAM_RAIN, scene.GetInitRain(), false);
		//Save Init Weather Rain
		SetParameterFloat(PARAM_FOG, scene.GetInitFog(), false);
		// Save Count Of missions
		SetParameterInt(PARAM_OBJ_COUNT, objects.Count(), false);
		
		
		for ( int i = 0; i < objects.Count(); ++i )
		{
			SceneObject obj = objects.Get(i);
			
			string param_name = PARAM_OBJ_NAME+"_"+i.ToString();
			Print(param_name);
			SetSubParameterInArray (param_name, 0, "ClassName", obj.GetTypeName(), false);
		}
		
		
		SaveConfigToFile();
	}
}