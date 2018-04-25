class UIPopupScriptSceneNew extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	
	//================================================
	// UIPopupScriptSceneNew
	//================================================	
	void UIPopupScriptSceneNew(Widget wgt)
	{
		m_BtnOk		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sn_ok") );
		m_BtnCancel	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sn_cancel") );
	}

	//================================================
	// OnClick
	//================================================	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_BtnOk )
		{
			EditBoxWidget wgt_text = EditBoxWidget.Cast( m_WgtRoot.FindAnyWidget("fld_ppp_sn_new_name") );
			
			PluginSceneManager editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );

			Log("OnClick -> SceneLoad");
			
			editor.SceneLoad(wgt_text.GetText());
			editor.SceneSave();
			
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
