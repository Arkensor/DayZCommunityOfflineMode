class UIPopupScriptSceneRename extends UIPopupScript
{
	private ButtonWidget	m_BtnOk;
	private ButtonWidget	m_BtnCancel;
	private string			m_RenameName;
	
	//================================================
	// UIPopupScriptSceneRename
	//================================================	
	void UIPopupScriptSceneRename(Widget wgt)
	{
		m_BtnOk			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sn_rnm_ok") );
		m_BtnCancel		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sn_rnm_cancel") );
	}

	//================================================
	// OnClick
	//================================================	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_BtnOk )
		{		
			EditBoxWidget wgt_text = EditBoxWidget.Cast( m_WgtRoot.FindAnyWidget("fld_ppp_sn_rnm_new_name") );
			
			PluginSceneManager editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
			
			editor.SceneRename(m_RenameName, wgt_text.GetText());
			m_RenameName = STRING_EMPTY;
			
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

	//================================================
	// SetRenameName
	//================================================	
	void SetRenameName(string rename_name)
	{
		m_RenameName = rename_name;
	}
}
