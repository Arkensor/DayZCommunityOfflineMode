class UIPopupScriptPresetRename extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	private EditBoxWidget m_Editbox;
	
	//================================================
	// UIPopupScriptSceneNew
	//================================================	
	void UIPopupScriptPresetRename( Widget wgt )
	{
		m_BtnOk		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pr_ok") );
		m_BtnCancel	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pr_cancel") );
		m_Editbox = EditBoxWidget.Cast( wgt.FindAnyWidget("fld_ppp_pr_new_name") );
	}
	
	override void OnOpen( Param param )
	{
		SceneEditorMenu menu = SceneEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
		m_Editbox.SetText( menu.GetCurrentPresetName() );	
	}

	//================================================
	// OnClick
	//================================================	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if ( w == m_BtnOk )
		{
			EditBoxWidget wgt_text = EditBoxWidget.Cast( m_WgtRoot.FindAnyWidget("fld_ppp_pr_new_name") );		
			SceneEditorMenu menu = SceneEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
			menu.RenamePreset( wgt_text.GetText() );
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
