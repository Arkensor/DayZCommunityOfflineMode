class EditorMenu extends UIScriptedMenu 
{

	protected ButtonWidget m_ObjectButton;
	protected ButtonWidget m_PositionButton;
	protected ButtonWidget m_WeatherButton;
	protected ButtonWidget m_GameButton;
	protected ButtonWidget m_CameraButton;
	
	// Object Menu
	ref ObjectMenu  m_objectMenu;
	ref WeatherMenu m_weatherMenu;
	ref PositionMenu m_positionMenu;

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

		m_ObjectButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("objects_button") );
		m_PositionButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("position_button") );
		m_WeatherButton  = ButtonWidget.Cast( layoutRoot.FindAnyWidget("weather_button") );
		m_GameButton     = ButtonWidget.Cast( layoutRoot.FindAnyWidget("game_button") );
		m_CameraButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_button") );

		// object menu
		m_objectMenu   = new ObjectMenu ( layoutRoot );
		m_weatherMenu  = new WeatherMenu( layoutRoot );
		m_positionMenu = new PositionMenu ( layoutRoot );

        return layoutRoot;

	}

	override bool UseMouse() 
	{
		return false;
	}

	override bool UseKeyboard() 
	{
		return false;
	}

    override void OnShow()
    {
        super.OnShow();

        GetGame().GetInput().ChangeGameFocus( 1, INPUT_DEVICE_MOUSE);
        GetGame().GetUIManager().ShowUICursor( true );
    }

    override void OnHide()
    {
        super.OnHide();

        GetGame().GetInput().ResetGameFocus( INPUT_DEVICE_MOUSE );
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_ObjectButton ) 
		{
			m_objectMenu.Toggle();
			SetButtonFocus( w );
			HideMenus( m_objectMenu.layoutRoot );
		}
		if ( w == m_PositionButton ) 
		{
			m_positionMenu.Toggle();
			SetButtonFocus( w );
			HideMenus( m_positionMenu.layoutRoot );
		}

		if ( w == m_WeatherButton ) 
		{
			m_weatherMenu.Toggle();
			SetButtonFocus( w );
			HideMenus( m_weatherMenu.layoutRoot );
		}

		if ( w == m_GameButton ) 
		{
			// Todo
		}

		if ( w == m_CameraButton ) 
		{
			CameraTool.Cast(GetModuleManager().GetModule( CameraTool )).ToggleCamera();
		}

		m_objectMenu  .OnClick( w, x, y, button );
		m_weatherMenu .OnClick( w, x, y, button );
		m_positionMenu.OnClick( w, x, y, button );

		return false;
	}

	void SetButtonFocus( Widget focus ) 
	{

		Widget m_ObjectButtonBkg   = layoutRoot.FindAnyWidget("objects_button_bkg");
		Widget m_PositionButtonBkg = layoutRoot.FindAnyWidget("position_button_bkg");
		Widget m_WeatherButtonBkg  = layoutRoot.FindAnyWidget("weather_button_bkg");

		m_ObjectButtonBkg.SetColor(ARGB(0, 255, 255, 255)); // reset colors
		m_PositionButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_WeatherButtonBkg.SetColor(ARGB(0, 255, 255, 255));

		if ( m_ObjectButton == focus && m_objectMenu.layoutRoot.IsVisible() ) 
		{
			m_ObjectButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_WeatherButton == focus && m_weatherMenu.layoutRoot.IsVisible() )
		{
			m_WeatherButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_PositionButton == focus && m_positionMenu.layoutRoot.IsVisible() )
		{
			m_PositionButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void HideMenus( Widget focus ) 
	{
		if ( m_objectMenu.layoutRoot != focus && m_objectMenu.layoutRoot.IsVisible() ) 
		{
			m_objectMenu.layoutRoot.Show(false);
		}
		if ( m_weatherMenu.layoutRoot != focus && m_weatherMenu.layoutRoot.IsVisible() ) 
		{
			m_weatherMenu.layoutRoot.Show(false);
		}
		if ( m_positionMenu.layoutRoot != focus && m_positionMenu.layoutRoot.IsVisible() ) 
		{
			m_positionMenu.layoutRoot.Show(false);
		}
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		super.OnChange( w, x, y, finished );

		m_objectMenu .OnChange( w, x, y, finished );
		m_weatherMenu.OnChange( w, x, y, finished );

        return false;
    }

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{

		return m_positionMenu.OnKeyPress( w, x, y, key );
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		return m_positionMenu.OnMouseLeave( w, enterW, x, y );
	}
	
	override bool OnMouseEnter(Widget w, int x, int y) 
	{
		return m_positionMenu.OnMouseEnter( w, x, y );
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn ) 
	{
		return m_positionMenu.OnItemSelected( w, x, y, row, column, oldRow, oldColumn );
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		
		return false;
	}


}