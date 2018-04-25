class PluginConfigScene extends PluginConfigHandler
{
	protected string FILE_ROOT					= "$saves:";
	protected const string FILE_ROOT_SCENES		= "scenes";
	
	protected const string PARAM_MISSION		= "Mission";
	protected const string PARAM_TIME			= "InitTime";
	protected const string PARAM_YEAR			= "InitYear";
	protected const string PARAM_MONTH			= "InitMonth";
	protected const string PARAM_DAY			= "InitDay";
	protected const string PARAM_HOUR			= "InitHour";
	protected const string PARAM_MINUTE			= "InitMinute";
	protected const string PARAM_OVERCAST		= "WeatherInitOvercast";
	protected const string PARAM_RAIN			= "WeatherInitRain";
	protected const string PARAM_FOG			= "WeatherInitFog";
	protected const string PARAM_WIND_F			= "WeatherInitWindForce";
	protected const string PARAM_PLAYER			= "Player";
	protected const string PARAM_OBJ_COUNT		= "SceneObjectsCount";
	protected const string PARAM_OBJ_NAME		= "SceneObject";
	protected const string PARAM_OBJ_ATT		= "Att";
	protected const string PARAM_OBJ_LNK		= "Lnk";
	protected const string PARAM_OBJ_TYPE		= "type";
	protected const string PARAM_OBJ_POS		= "position";
	protected const string PARAM_OBJ_ROT		= "rotation";
	protected const string PARAM_OBJ_DMG		= "damage";
	protected const string PARAM_OBJ_HLT		= "health";
	protected const string PARAM_OBJ_ISCR		= "init_script";
	
	TStringArray m_CfgTemplate;
	
	protected string m_FileSceneName;

	//========================================
	// OnInit
	//========================================
	override void OnInit()
	{
		//super.OnInit();
			
		FILE_ROOT = g_Game.GetMissionFolderPath();
	}
	
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
		return GetPathScenes()+"\\"+m_FileSceneName;
	}

	//========================================
	// SceneRename
	//========================================
	void SceneRename(string scene_name, string scene_name_new)
	{
		string file_path = GetPathScenes()+"\\"+scene_name+"."+PluginSceneManager.SCENE_SUFIX;
		string file_path_new = GetPathScenes()+"\\"+scene_name_new+"."+PluginSceneManager.SCENE_SUFIX;
		PluginFileHandler.FileRename(file_path, file_path_new);
	}

	//========================================
	// SceneDelete
	//========================================
	void SceneDelete(string scene_name)
	{
		string file_path = GetPathScenes()+"\\"+scene_name+"."+PluginSceneManager.SCENE_SUFIX;
		PluginFileHandler.FileDelete(file_path);
	}

	//========================================
	// SceneDuplicate
	//========================================
	void SceneDuplicate(string scene_name)
	{
		string file_path = GetPathScenes()+"\\"+scene_name+"."+PluginSceneManager.SCENE_SUFIX;
		string file_path_new = file_path;
		
		int i = 0;
		while ( FileExist(file_path_new) )
		{
			file_path_new = GetPathScenes()+"\\"+scene_name+"_copy"+i.ToString()+"."+PluginSceneManager.SCENE_SUFIX;
			i++;
		}
		
		PluginFileHandler.FileDuplicate(file_path, file_path_new);
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
		
		string path_find_pattern = GetPathScenes()+"/*."+PluginSceneManager.SCENE_SUFIX; //*/
		FindFileHandle file_handler = FindFile(path_find_pattern, file_name, file_attr, flags);
		
		bool found = true;
		while ( found )
		{
			int pos = file_name.IndexOf(".");
			
			if ( pos > -1 )
			{
				list.Insert( file_name.Substring(0, pos) );
			}
			
			found = FindNextFile(file_handler, file_name, file_attr);
		}
		
		return list;
	}

	//========================================
	// EncodeInitScript
	//========================================	
	string EncodeInitScript( string script )
	{
		script.Replace( "\"", "^" );
		script.Replace( "\n", "$" );
		return script;
	}

	//========================================
	// DecodeInitScript
	//========================================	
	string DecodeInitScript( string script )
	{
		script.Replace( "^", "\"" );
		script.Replace( "$", "\n" );
		return script;
	}

//=====================================================================================
//=====================================================================================
// SAVE SCENE
//=====================================================================================
//=====================================================================================	
	
	//========================================
	// SceneSave
	//========================================	
	void SceneDataSave(SceneData scene)
	{
		PluginSceneManager module_scene_editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
		
		EntityAI player = PluginSceneManager.PLAYER;
		
		// Log("Saving started....")
		m_FileSceneName = scene.GetNameScene()+"."+PluginSceneManager.SCENE_SUFIX;
		
		// Save Settings
		SaveSceneSettings(scene);
		
		// Save Weater
		SaveSceneWeather(scene);
		
		// Save Players
		SaveScenePlayer(scene);
		
		// Save Objects
		SaveSceneObjects(scene);		
		
		// Save To File
		SaveConfigToFile();
	}
	
	
	//=============================================
	// SAVE: SaveSceneSettings (Mission Name, Time, Year, Month, Day, Hour, Minute)
	//=============================================
	void SaveSceneSettings(SceneData scene)
	{
		// Save Mission Name
		CfgParamString cfg_mission = CfgParamString.Cast( GetParamByName(PARAM_MISSION, CFG_TYPE_STRING) );
		cfg_mission.SetValue(scene.GetNameMission());
				
		// Save Time
		CfgParamFloat cfg_time = CfgParamFloat.Cast( GetParamByName(PARAM_TIME, CFG_TYPE_FLOAT) );
		cfg_time.SetValue(scene.GetInitTime());
		
		// Save Year
		CfgParamInt cfg_year = CfgParamInt.Cast( GetParamByName(PARAM_YEAR, CFG_TYPE_INT) );
		cfg_year.SetValue(scene.GetInitYear());
		
		// Save Month
		CfgParamInt cfg_month = CfgParamInt.Cast( GetParamByName(PARAM_MONTH, CFG_TYPE_INT) );
		cfg_month.SetValue(scene.GetInitMonth());
		
		// Save Day
		CfgParamInt cfg_day = CfgParamInt.Cast( GetParamByName(PARAM_DAY, CFG_TYPE_INT) );
		cfg_day.SetValue(scene.GetInitDay());
		
		// Save Hour
		CfgParamInt cfg_hour = CfgParamInt.Cast( GetParamByName(PARAM_HOUR, CFG_TYPE_INT) );
		cfg_hour.SetValue(scene.GetInitHour());
		
		// Save Minute
		CfgParamInt cfg_minute = CfgParamInt.Cast( GetParamByName(PARAM_MINUTE, CFG_TYPE_INT) );
		cfg_minute.SetValue(scene.GetInitMinute());
	}
	
	//=============================================
	// SAVE: SaveSceneWeather (Overcast, Rain, Fog, Wind Force)
	//=============================================
	void SaveSceneWeather(SceneData scene)
	{
		// Save Weather Overcast
		CfgParamFloat cfg_overcast = CfgParamFloat.Cast( GetParamByName(PARAM_OVERCAST, CFG_TYPE_FLOAT) );
		cfg_overcast.SetValue(scene.GetInitOvercast());
		
		// Save Weather Rain
		CfgParamFloat cfg_rain = CfgParamFloat.Cast( GetParamByName(PARAM_RAIN, CFG_TYPE_FLOAT) );
		cfg_rain.SetValue(scene.GetInitRain());
				
		// Save Weather Fog
		CfgParamFloat cfg_fog = CfgParamFloat.Cast( GetParamByName(PARAM_FOG, CFG_TYPE_FLOAT) );
		cfg_fog.SetValue(scene.GetInitFog());
		
		// Save Weather Wind Force
		CfgParamFloat cfg_wind_force = CfgParamFloat.Cast( GetParamByName(PARAM_WIND_F, CFG_TYPE_FLOAT) );
		cfg_wind_force.SetValue(scene.GetInitWindForce());
	}
	
	//=============================================
	// SAVE: SaveScenePlayer (position, rotation, health, script)
	//=============================================
	void SaveScenePlayer(SceneData scene)
	{
		array<ref ScenePlayer> players = scene.GetScenePlayers();
		
		if ( players.Count() > 0 )
		{
			ScenePlayer plyr = players.Get(0);
						
			// Save Player
			CfgParamArray cfg_param_player = CfgParamArray.Cast( GetParamByName(PARAM_PLAYER, CFG_TYPE_ARRAY) );
			
			// Obj Position
			CfgParamString cfg_param_player_pos = CfgParamString.Cast( cfg_param_player.GetValueByName(PARAM_OBJ_POS, CFG_TYPE_STRING) );
			cfg_param_player_pos.SetValue(plyr.GetPositionAsString());
			
			// Obj Rotation
			CfgParamFloat cfg_param_player_rot = CfgParamFloat.Cast( cfg_param_player.GetValueByName(PARAM_OBJ_ROT, CFG_TYPE_FLOAT) );
			cfg_param_player_rot.SetValue(plyr.GetRotation());
		
			// Obj Health
			CfgParamFloat cfg_param_player_hlt = CfgParamFloat.Cast( cfg_param_player.GetValueByName(PARAM_OBJ_HLT, CFG_TYPE_FLOAT) );
			cfg_param_player_hlt.SetValue(plyr.GetHealth());
			
			// Obj Init Script
			CfgParamString cfg_param_player_iscr = CfgParamString.Cast( cfg_param_player.GetValueByName(PARAM_OBJ_ISCR, CFG_TYPE_STRING) );
			cfg_param_player_iscr.SetValue( EncodeInitScript(plyr.GetInitScript()) );
		}
	}
	
	//=============================================
	// SAVE: SaveSceneObjects (position, rotation, health, script, scripted links)
	//=============================================
	void SaveSceneObjects(SceneData scene)
	{
		array<ref SceneObject> objects = scene.GetSceneObjects();
		
		// Save Objects
		for ( int i = 0; i < objects.Count(); ++i )
		{
			SceneObject obj = objects.Get(i);
			
			string param_name;
			
			param_name = PARAM_OBJ_NAME+"_"+i.ToString();
							
			// Save Scene Object
			CfgParamArray cfg_param_obj = CfgParamArray.Cast( GetParamByName(param_name, CFG_TYPE_ARRAY) );
			
			// Obj Type Name
			CfgParamString cfg_param_obj_type = CfgParamString.Cast( cfg_param_obj.GetValueByName(PARAM_OBJ_TYPE, CFG_TYPE_STRING)  );
			cfg_param_obj_type.SetValue(obj.GetTypeName());
			
			// Obj Position
			CfgParamString cfg_param_obj_pos = CfgParamString.Cast( cfg_param_obj.GetValueByName(PARAM_OBJ_POS, CFG_TYPE_STRING)  );
			cfg_param_obj_pos.SetValue(obj.GetPositionAsString());
			
			// Obj Rotation
			CfgParamFloat cfg_param_obj_rot = CfgParamFloat.Cast( cfg_param_obj.GetValueByName(PARAM_OBJ_ROT, CFG_TYPE_FLOAT)  );
			cfg_param_obj_rot.SetValue(obj.GetRotation());
			
			// Obj Health
			CfgParamFloat cfg_param_obj_hlt = CfgParamFloat.Cast( cfg_param_obj.GetValueByName(PARAM_OBJ_HLT, CFG_TYPE_FLOAT)  );
			cfg_param_obj_hlt.SetValue(obj.GetHealth());
			
			// Obj Init Script
			CfgParamString cfg_param_obj_iscr = CfgParamString.Cast( cfg_param_obj.GetValueByName(PARAM_OBJ_ISCR, CFG_TYPE_STRING) );
			cfg_param_obj_iscr.SetValue( EncodeInitScript(obj.GetInitScript()) );
			
			local EntityAI e_obj = obj.GetObject();

			local int attcount = e_obj.GetInventory().AttachmentCount();
				
			if ( attcount > 0 )
			{
				local string param_name_att = param_name+"_"+PARAM_OBJ_ATT;
				CfgParamArray cfg_param_att = CfgParamArray.Cast( GetParamByName(param_name_att, CFG_TYPE_ARRAY) );
				cfg_param_att.Clear();
				
				for (int att = 0; att < attcount; att++)
				{	
					EntityAI attachment = e_obj.GetInventory().GetAttachmentFromIndex(att);
					
					CfgParamString cfg_param_att_str = new CfgParamString("");
					cfg_param_att_str.SetValue( attachment.GetType() );
					cfg_param_att.InsertValue( cfg_param_att_str );
				}
			}
			
			array<SceneObject> linked_scene_objects = obj.GetLinkedSceneObjects();
			if ( linked_scene_objects.Count() > 0 )
			{
				string param_name_lnk = param_name + "_" + PARAM_OBJ_LNK;
				CfgParamArray cfg_param_lnk = CfgParamArray.Cast( GetParamByName( param_name_lnk, CFG_TYPE_ARRAY ) );
				cfg_param_lnk.Clear();
				
				for ( int lnk = 0; lnk < linked_scene_objects.Count(); lnk++ )
				{	
					CfgParamInt cfg_param_lnk_int = new CfgParamInt("");
					
					int index = objects.Find( linked_scene_objects.Get(lnk) );
					
					if ( index >= 0 )
					{
						cfg_param_lnk_int.SetValue( index );
						cfg_param_lnk.InsertValue( cfg_param_lnk_int );
					}
				}
			}
		}
		
		// Save Count Of objects
		CfgParamInt cfg_obj_count = CfgParamInt.Cast( GetParamByName(PARAM_OBJ_COUNT, CFG_TYPE_INT) );
		cfg_obj_count.SetValue(objects.Count());
	}
	
//=====================================================================================
//=====================================================================================
// LOAD SCENE
//=====================================================================================
//=====================================================================================

	//=============================================
	// LOAD: SceneDataLoad (Settings, Weather, Player, Objects)
	//=============================================
	SceneData SceneDataLoad(string scene_name)
	{
		m_FileSceneName = scene_name+"."+PluginSceneManager.SCENE_SUFIX;
		
//PrintString("Loading scene: "+ scene_name);	
		
		LoadConfigFile();
		
		// Load Scene name
		SceneData scene = new SceneData;
		scene.SetNameScene(scene_name);
		
		// Load Scene settings
		LoadSceneSettings(scene);
		
		// Load Scene Weather
		LoadSceneWeather(scene);
		
		// Load Scene Player
		LoadScenePlayer(scene);
		
		// Load Scene Objects
		LoadSceneObjects(scene);
		
		if ( !FileExist(GetFileName()) )
		{
			SceneDataSave(scene);
		}
		
		return scene;
	}
	
	//=============================================
	// LOAD: LoadSceneSettings (Mission Name, Time, Year, Month, Day, Hour, Minute)
	//=============================================
	void LoadSceneSettings(SceneData scene)
	{
		// Load Mission Name
		CfgParamString cfg_mission = CfgParamString.Cast( GetParamByName(PARAM_MISSION, CFG_TYPE_STRING) );
		scene.SetNameMission(cfg_mission.GetValue());
				
		// Load Time
		CfgParamFloat cfg_time = CfgParamFloat.Cast( GetParamByName(PARAM_TIME, CFG_TYPE_FLOAT) );
		scene.SetInitTime(cfg_time.GetValue());
		
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		
		// Load Year
		if ( ParamExist(PARAM_YEAR) )
		{
			CfgParamInt cfg_year = CfgParamInt.Cast( GetParamByName( PARAM_YEAR, CFG_TYPE_INT ) );
			scene.SetInitYear( cfg_year.GetValue() );
		}
		else
		{
			scene.SetInitYear( year );
		}
		
		// Load Month
		if ( ParamExist(PARAM_MONTH) )
		{
			CfgParamInt cfg_month = CfgParamInt.Cast( GetParamByName( PARAM_MONTH, CFG_TYPE_INT ) );
			scene.SetInitMonth( cfg_month.GetValue() );
		}
		else
		{
			scene.SetInitMonth( month );
		}
		
		// Load Day
		if ( ParamExist(PARAM_DAY) )
		{
			CfgParamInt cfg_day = CfgParamInt.Cast(  GetParamByName( PARAM_DAY, CFG_TYPE_INT ) );
			scene.SetInitDay( cfg_day.GetValue() );
		}
		else
		{
			scene.SetInitDay( day );
		}
		
		// Load Hour
		if ( ParamExist(PARAM_HOUR) )
		{
			CfgParamInt cfg_hour = CfgParamInt.Cast( GetParamByName( PARAM_HOUR, CFG_TYPE_INT ) );
			scene.SetInitHour( cfg_hour.GetValue() );
		}
		else
		{
			scene.SetInitHour( hour );
		}
		
		// Load Minute
		if ( ParamExist(PARAM_MINUTE) )
		{
			CfgParamInt cfg_minute = CfgParamInt.Cast( GetParamByName( PARAM_MINUTE, CFG_TYPE_INT ) );
			scene.SetInitMinute( cfg_minute.GetValue() );
		}
		else
		{
			scene.SetInitMinute( minute );
		}
	}
	
	//=============================================
	// LOAD: LoadSceneWeather (Overcast, Rain, Fog, Wind Force)
	//=============================================
	void LoadSceneWeather(SceneData scene)
	{
		//Load Weather Overcast
		if ( ParamExist(PARAM_OVERCAST) )
		{   
			CfgParamFloat cfg_overcast = CfgParamFloat.Cast( GetParamByName(PARAM_OVERCAST, CFG_TYPE_FLOAT) );
			scene.SetInitOvercast(cfg_overcast.GetValue());
		}

		//Load Weather Rain
		if ( ParamExist(PARAM_RAIN) )
		{
			CfgParamFloat cfg_rain = CfgParamFloat.Cast( GetParamByName(PARAM_RAIN, CFG_TYPE_FLOAT) );
			scene.SetInitRain(cfg_rain.GetValue());
		}

		//Load Weather Fog
		if ( ParamExist(PARAM_FOG) )
		{
			CfgParamFloat cfg_fog = CfgParamFloat.Cast( GetParamByName(PARAM_FOG, CFG_TYPE_FLOAT) );
			scene.SetInitFog(cfg_fog.GetValue());
		}

		//Load Weather Wind Force
		if ( ParamExist(PARAM_WIND_F) )
		{
			CfgParamFloat cfg_wind_force = CfgParamFloat.Cast( GetParamByName(PARAM_WIND_F, CFG_TYPE_FLOAT) );
			scene.SetInitWindForce(cfg_wind_force.GetValue());
		}
	}
	
	//=============================================
	// LOAD: LoadScenePlayer (position, rotation, health, script)
	//=============================================
	void LoadScenePlayer(SceneData scene)
	{
		// Only in local game (without server, (GetGame.GetPlayer is null on server! -> by design)
		if ( GetGame().GetPlayer() != NULL )
		{
			CfgParamArray 	cfg_player 		= CfgParamArray.Cast( GetParamByName(PARAM_PLAYER, CFG_TYPE_ARRAY) );			
			CfgParamString	cfg_ply_pos		= CfgParamString.Cast( cfg_player.GetValueByName(PARAM_OBJ_POS, CFG_TYPE_STRING) );
			CfgParamFloat	cfg_ply_rot		= CfgParamFloat.Cast( cfg_player.GetValueByName(PARAM_OBJ_ROT, CFG_TYPE_FLOAT) );			
			CfgParamFloat	cfg_ply_hlt		= CfgParamFloat.Cast( cfg_player.GetValueByName(PARAM_OBJ_HLT, CFG_TYPE_FLOAT) );
			CfgParamString	cfg_ply_iscr	= CfgParamString.Cast( cfg_player.GetValueByName(PARAM_OBJ_ISCR, CFG_TYPE_STRING) );
			
			ScenePlayer scn_player = scene.CreateScenePlayer();
			scn_player.LinkEntityAI(GetGame().GetPlayer());
			
			if ( PluginSceneManager.GetInstance().GetLoadPlayerPos() )
			{
				// start scene position
				vector start_pos = cfg_ply_pos.GetValue().ToVector();
				
				if ( start_pos != vector.Zero )
				{
					// Set Position
					scn_player.SetPosition(cfg_ply_pos.GetValue().ToVector());
					
					// Set Rotation
					scn_player.SetRotation(cfg_ply_rot.GetValue());
				}
			}
			
			// Set Health
			// Health check. Do not spawn player character as dead. He can still be spawned as injured.
			float health2 = cfg_ply_hlt.GetValue();
			if (health2 <= 0)
				health2 = scn_player.GetMaxHealth();
			
			scn_player.SetHealth(health2);
			
			// Exe Script
			scn_player.SetInitScript( DecodeInitScript(cfg_ply_iscr.GetValue()) );
		}
//PrintString("LoadScenePlayer.. Finished");
	}
	
	//=============================================
	// LOAD: LoadSceneObjects (position, rotation, health, script, scripted links)
	//=============================================
	void LoadSceneObjects(SceneData scene)
	{
		CfgParamInt cfg_obj_count = CfgParamInt.Cast( GetParamByName(PARAM_OBJ_COUNT, CFG_TYPE_INT) );
		int obj_count = cfg_obj_count.GetValue();
		
		for ( int i = 0; i < obj_count; ++i )
		{
			local string param_name = PARAM_OBJ_NAME+"_"+i.ToString();
			
			// Load scene object name
			CfgParamArray cfg_obj = CfgParamArray.Cast( GetParamByName(param_name, CFG_TYPE_ARRAY) );
			
			CfgParamString	cfg_obj_type	= CfgParamString.Cast( cfg_obj.GetValueByName(PARAM_OBJ_TYPE, CFG_TYPE_STRING) );
			CfgParamString	cfg_obj_pos		= CfgParamString.Cast( cfg_obj.GetValueByName(PARAM_OBJ_POS, CFG_TYPE_STRING) );
			CfgParamFloat	cfg_obj_rot		= CfgParamFloat.Cast( cfg_obj.GetValueByName(PARAM_OBJ_ROT, CFG_TYPE_FLOAT) );
			CfgParamFloat	cfg_obj_hlt		= CfgParamFloat.Cast( cfg_obj.GetValueByName(PARAM_OBJ_HLT, CFG_TYPE_FLOAT) );
			CfgParamString	cfg_obj_iscr	= CfgParamString.Cast( cfg_obj.GetValueByName(PARAM_OBJ_ISCR, CFG_TYPE_STRING) );
			
//PrintString(itoa(i)+ " Loading param_name: "+ param_name+ " => object type: "+ cfg_obj_type.GetValue());
			
			// Create Scene Object + position
			SceneObject scn_obj = scene.CreateSceneObject(cfg_obj_type.GetValue(), cfg_obj_pos.GetValue().ToVector());
			
			// Setup rotation
			scn_obj.SetRotation(cfg_obj_rot.GetValue());
			// Setup health
			scn_obj.SetHealth(cfg_obj_hlt.GetValue());
			// Setup init script
			scn_obj.SetInitScript( DecodeInitScript(cfg_obj_iscr.GetValue()) );
			
			// Setup indices for linked objects
			CfgParamArray cfg_obj_param_links = CfgParamArray.Cast( GetParamByName(param_name + "_" + PARAM_OBJ_LNK, CFG_TYPE_ARRAY) );
			array<ref CfgParam> cfg_obj_links = cfg_obj_param_links.GetValues();
			
			for ( int lnko = 0; lnko < cfg_obj_links.Count(); lnko++ )
			{
				CfgParamInt cfg_obj_param_lnk_int = CfgParamInt.Cast( cfg_obj_links.Get(lnko) );
				scn_obj.m_LinkedSceneObjectsIndices.Insert( cfg_obj_param_lnk_int.GetValue() );
			}
			
			local string param_name_att = param_name +"_"+ PARAM_OBJ_ATT;
			
			if ( ParamExist(param_name_att) )
			{				
				CfgParamArray cfg_param_att = CfgParamArray.Cast( GetParamByName(param_name_att, CFG_TYPE_ARRAY) );
				
				array<ref CfgParam> cfg_param_attachments = cfg_param_att.GetValues();
				
				for ( int j = 0; j < cfg_param_attachments.Count(); ++j )
				{
					CfgParamString cfg_param_att_str = CfgParamString.Cast( cfg_param_attachments.Get(j) );
					scn_obj.GetObject().GetInventory().CreateAttachment( cfg_param_att_str.GetValue() );
				}
			}
		}
	}
}
