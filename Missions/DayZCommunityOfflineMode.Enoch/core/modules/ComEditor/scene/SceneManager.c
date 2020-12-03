string SCENE_DIR = "$profile:SceneManager";
string SCENE_ACTIVE = SCENE_DIR + "\\ActiveScenes";
string SCENE_DATA = SCENE_DIR + "\\Scenes";

class SceneManager
{
	ref map< SceneData, ref array< Scene >> sm_Scenes = new ref map< SceneData, ref array< Scene>>;

	void SceneManager() 
	{
		MakeDirectory( SCENE_DATA );

		Init();
	}

	void ~SceneManager() 
	{

	}

	void Init() 
	{
		// load scenes
	}

	void LoadScenes() 
	{
		FileAttr fileAttr;
		string fileName;

		FindFileHandle fileHandle = FindFile( SCENE_DIR + "\\" + "*.json", fileName, fileAttr, FindFileFlags.ALL );
		if ( fileName != "" ) 
		{
			while (FindNextFile( fileHandle, fileName, fileAttr )) 
			{
				sm_Scenes.Insert( LoadSceneData( fileName ), new array< Scene > );
			}
		}
	}

	ref SceneData LoadSceneData( string fileName ) 
	{
		SceneData data;
		JsonFileLoader<SceneData>.JsonLoadFile( SCENE_DIR + "\\" + fileName + ".json", data );
		return data;
	}
}