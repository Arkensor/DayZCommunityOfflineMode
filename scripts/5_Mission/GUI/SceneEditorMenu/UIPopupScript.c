class UIPopupScript
{
	//================================================
	// UIPopupScript
	//================================================
	void UIPopupScript(Widget wgt)
	{
		m_WgtRoot = wgt;
	}

	void OnOpen(Param param)
	{
	}
	
	void OnClose()
	{
	}
	
	void Show(bool show)
	{
		m_WgtRoot.Show(show);
	}
	
	bool OnClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		return false;
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// PopupBack
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	protected UIPopupScript PopupBack()
	{
		SceneEditorMenu menu = SceneEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
		return menu.PopupBack();
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// PopupOpen
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	protected UIPopupScript PopupOpen(int popup_id, Param param)
	{
		SceneEditorMenu menu = SceneEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
		return menu.PopupOpen(popup_id, param);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// GetSceneEditor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	protected PluginSceneManager GetSceneEditor()
	{
		PluginSceneManager m = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
		return m;
	}
	
	protected Widget m_WgtRoot;
}
