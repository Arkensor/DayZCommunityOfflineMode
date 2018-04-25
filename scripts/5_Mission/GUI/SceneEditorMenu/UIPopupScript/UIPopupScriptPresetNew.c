class UIPopupScriptPresetNew extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	
	//================================================
	// UIPopupScriptSceneNew
	//================================================	
	void UIPopupScriptPresetNew( Widget wgt )
	{
		m_BtnOk		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pn_ok") );
		m_BtnCancel	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pn_cancel") );
	}

	//================================================
	// OnClick
	//================================================	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if ( w == m_BtnOk )
		{
			EditBoxWidget wgt_text = EditBoxWidget.Cast( m_WgtRoot.FindAnyWidget("fld_ppp_pn_new_name") );
			PluginConfigDebugProfile m_ConfigDebugProfile = PluginConfigDebugProfile.Cast( GetPlugin(PluginConfigDebugProfile) );
			m_ConfigDebugProfile.PresetAdd( wgt_text.GetText() );
			
			SceneEditorMenu menu = SceneEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
			menu.RefreshLists();
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
