class UIPopupScriptInitScript extends UIPopupScript
{
	private MultilineEditBoxWidget	m_MedxInitScript;
	
	private ButtonWidget			m_BtnRun;
	private ButtonWidget			m_BtnSave;
	private ButtonWidget			m_BtnCancel;
	
	private int						m_SceneObjectIndex;
	private SceneObject				m_SceneObject;
	
	private PluginSceneManager		m_ModuleSceneManager;
	
	//================================================
	// UIPopupScriptInitScript
	//================================================	
	void UIPopupScriptInitScript(Widget wgt)
	{
		m_ModuleSceneManager = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
		
		m_MedxInitScript	= MultilineEditBoxWidget.Cast( wgt.FindAnyWidget("pnl_ppp_is_init_script_value") );
		
		m_BtnRun			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_is_run") );
		m_BtnSave			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_is_save") );
		m_BtnCancel			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_is_cancel") );
	}

	//================================================
	// OnOpen
	//================================================	
	override void OnOpen(Param param)
	{
		m_MedxInitScript.SetText( "" );
		
		if ( param != NULL )
		{
			Param2<int, SceneObject> param_scene_object = Param2<int, SceneObject>.Cast( param );
			int index = param_scene_object.param1;
			SceneObject scene_object = param_scene_object.param2;
			
			if ( index > -1 && scene_object != NULL )
			{
				m_SceneObjectIndex = index;
				m_SceneObject = scene_object;
				m_MedxInitScript.SetText( m_SceneObject.GetInitScript() );
			}
		}
	}

	//================================================
	// OnClick
	//================================================	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_BtnRun )
		{
			if ( m_SceneObject != NULL )
			{
				string text_run;
				m_MedxInitScript.GetText( text_run );
				m_ModuleSceneManager.ExecuteEnforceScript( m_SceneObjectIndex, m_SceneObject, text_run );
			}
		}
		else if ( w == m_BtnSave )
		{
			if ( m_SceneObject != NULL )
			{
				string text_script;
				m_MedxInitScript.GetText( text_script );
				m_SceneObject.SetInitScript( text_script );
			}
			
			PopupBack();
			
			return true;
		}
		else if ( w == m_BtnCancel )
		{
			PopupBack();
			
			return true;
		}
		
		return false;
	}
}
