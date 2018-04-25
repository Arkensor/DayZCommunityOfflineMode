class PluginMissionConfig extends PluginConfigHandler
{	
	protected const string FILE_NAME				= "scene_editor.cfg";
	protected const string SCENE_EDITOR_SCENE		= "scene_load";
	
	//========================================
	// GetInstance
	//========================================
	static PluginMissionConfig GetInstance()
	{
		return PluginMissionConfig.Cast( GetPlugin(PluginMissionConfig) );
	}
	
	//========================================
	// OnInit
	//========================================
	override void OnInit()
	{
		super.OnInit();
		
		CfgParamString cfg_scene_name = CfgParamString.Cast( GetParamByName( SCENE_EDITOR_SCENE, CFG_TYPE_STRING ) );
		
		if ( cfg_scene_name.GetValue() == STRING_EMPTY )
		{
			cfg_scene_name.SetValue( PluginSceneManager.SCENE_DEFAULT_NAME );
			SaveConfigToFile();
		}
	}
	
	//========================================
	// GetFileName
	//========================================
	override string GetFileName()
	{
		return g_Game.GetMissionFolderPath() +"\\"+ FILE_NAME;
	}

	//========================================
	// GetSceneEditorName
	//========================================
	string GetSceneEditorName()
	{
		CfgParamString cfg_scene_name = CfgParamString.Cast( GetParamByName( SCENE_EDITOR_SCENE, CFG_TYPE_STRING ) );
		
		if ( cfg_scene_name.GetValue() == STRING_EMPTY )
		{
			cfg_scene_name.SetValue( PluginSceneManager.SCENE_DEFAULT_NAME );
			SaveConfigToFile();
		}
		
		return cfg_scene_name.GetValue();
	}

	//========================================
	// SetSceneEditorName
	//========================================
	void SetSceneEditorName( string value )
	{
		CfgParamString cfg_scene_name = CfgParamString.Cast( GetParamByName( SCENE_EDITOR_SCENE, CFG_TYPE_STRING ) );
		cfg_scene_name.SetValue( value );
		SaveConfigToFile();
	}
}