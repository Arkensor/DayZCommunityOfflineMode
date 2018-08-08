class EditorMenu extends UIScriptedMenu 
{

	protected RichTextWidget m_changelogbox;
	protected ButtonWidget m_ToolBoxButton;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_ObjectButton;
	protected ButtonWidget m_WeatherButton;
	
	void EditorMenu()
	{
		Init();
	}	
	
	void ~EditorMenu()
	{
	}
	
	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\EditorMenu.layout" );

		/*
		m_changelogbox = RichTextWidget.Cast( layoutRoot.FindAnyWidget( "changelog" ) );
		m_ToolBoxButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ttm_tm_tools") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ttm_tm_position") );
		m_ObjectButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ttm_tm_object") );
		m_WeatherButton  = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ttm_tm_weather") );
*/
		
        return layoutRoot;

	}

    override void OnShow()
    {
        super.OnShow();
    }

    override void OnHide()
    {
        super.OnHide();
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
		/*
		PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus(w.ToString());
		if ( w == m_TeleportButton ) 
		{
			Close();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetUIManager().ShowScriptedMenu, new PositionMenu(), NULL);	
		} else if ( w == m_ObjectButton ) 
		{
			Close();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetUIManager().ShowScriptedMenu, new ObjectMenu(), NULL);	
		} else if ( w == m_WeatherButton ) 
		{
			Close();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetUIManager().ShowScriptedMenu, new WeatherMenu(), NULL);	
		} else if ( w == m_ToolBoxButton ) 
		{
			Close();
			// GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetUIManager().ShowScriptedMenu, new ToolsMenu(), NULL);	
		} else if ( w.GetName() == "close_button" ) 
		{
			Close();
		}
*/
		return false;
	}


	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		return false;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if ( w == m_changelogbox ) 
		{
			float offset = m_changelogbox.GetContentOffset() - wheel*10;
			m_changelogbox.SetContentOffset(offset, false);
		}
		
		return false;
	}
	
}