class EditorMenu extends UIScriptedMenu 
{

	protected ButtonWidget m_ObjectButton;
	protected ButtonWidget m_PositionButton;
	protected ButtonWidget m_WeatherButton;
	protected ButtonWidget m_GameButton;
	protected ButtonWidget m_CameraButton;
	protected ButtonWidget m_ObjectEditorButton;
	
	protected Widget m_objectMenu;
	protected Widget m_weatherMenu;
	protected Widget m_positionMenu;
	protected Widget m_gameMenu;
	protected Widget m_objectInfoMenu;
	protected Widget m_cameraMenu;

	void EditorMenu()
	{
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
		m_ObjectEditorButton = layoutRoot.FindAnyWidget( "objectEditor_button" );


		// object menu
		m_objectMenu   = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\ObjectMenu.layout", layoutRoot );
		m_weatherMenu  = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\WeatherMenu.layout", layoutRoot );
		m_positionMenu = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Admintool\\gui\\layouts\\PositionMenu.layout", layoutRoot );
		m_gameMenu 	   = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\GameMenu.layout", layoutRoot );
		m_objectInfoMenu = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\ObjectEditorInfo.layout", layoutRoot );
		m_cameraMenu = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\gui\\layouts\\CameraSettings.layout", layoutRoot );

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

        ObjectEditor.Cast( GetModuleManager().GetModule( ObjectEditor )).EditorState( false );

        GetGame().GetInput().ResetGameFocus( INPUT_DEVICE_MOUSE );
        GetPlayer().GetInputController().OverrideMovementSpeed( false, 0 );

        CameraSettings.CAMERA_ROT.Show( false );
        CameraSettings.CAMERA_PHI.Show( false );
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
		PopupMenu popMenu;

		if ( w == m_ObjectButton ) 
		{
			m_objectMenu.GetScript( popMenu );
		}
		if ( w == m_PositionButton ) 
		{
			m_positionMenu.GetScript( popMenu );
		}

		if ( w == m_WeatherButton ) 
		{
			m_weatherMenu.GetScript( popMenu );
		}

		if ( w == m_GameButton ) 
		{
			m_gameMenu.GetScript( popMenu );
		}
		if ( w == m_ObjectEditorButton ) 
		{
			m_objectInfoMenu.GetScript( popMenu );

			ObjectEditor.Cast( GetModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}

		if ( w == m_CameraButton ) 
		{
			if ( CTRL() ) 
			{
				// GetGame().GetUIManager().ShowScriptedMenu( new CameraToolsMenu(), this );
				//GetGame().GetUIManager().ShowScriptedMenu( new CameraSettings(), this );
				m_cameraMenu.GetScript( popMenu );
				// CameraTool.CAMERA_ROT.Show( !CameraTool.CAMERA_ROT.IsVisible() );
			}
			else 
			{

				ref CameraTool cmt = GetModuleManager().GetModule( CameraTool );
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(cmt.ToggleCamera ); // Fix crash
			}
		}

		if ( popMenu ) 
		{

			if ( popMenu.GetLayoutRoot().IsVisible() ) 
			{
				popMenu.GetLayoutRoot().Show( false );
				popMenu.OnHide();
			}
			else 
			{
				popMenu.GetLayoutRoot().Show( true );
				popMenu.OnShow();
			}

			SetButtonFocus( w );
			HideMenus( popMenu.GetLayoutRoot() );
		}

		return false;
	}

	void SetButtonFocus( Widget focus ) 
	{

		Widget m_ObjectButtonBkg   = layoutRoot.FindAnyWidget("objects_button_bkg");
		Widget m_PositionButtonBkg = layoutRoot.FindAnyWidget("position_button_bkg");
		Widget m_WeatherButtonBkg  = layoutRoot.FindAnyWidget("weather_button_bkg");
		Widget m_GameButtonBkg     = layoutRoot.FindAnyWidget("game_button_bkg");

		m_ObjectButtonBkg.SetColor(ARGB(0, 255, 255, 255)); // reset colors
		m_PositionButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_WeatherButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_GameButtonBkg.SetColor(ARGB(0, 255, 255, 255));

		if ( m_ObjectButton == focus && m_objectMenu.IsVisible() ) 
		{
			m_ObjectButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_WeatherButton == focus && m_weatherMenu.IsVisible() )
		{
			m_WeatherButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_PositionButton == focus && m_positionMenu.IsVisible() )
		{
			m_PositionButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_GameButton == focus && m_gameMenu.IsVisible() )
		{
			m_GameButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void HideMenus( Widget focus ) 
	{
		if ( m_objectMenu != focus && m_objectMenu.IsVisible() ) 
		{
			m_objectMenu.Show(false);
		}
		if ( m_weatherMenu != focus && m_weatherMenu.IsVisible() ) 
		{
			m_weatherMenu.Show(false);
		}
		if ( m_positionMenu != focus && m_positionMenu.IsVisible() ) 
		{
			m_positionMenu.Show(false);
		}
		if ( m_gameMenu != focus && m_gameMenu.IsVisible() ) 
		{
			m_gameMenu.Show(false);
		}
		if ( m_objectInfoMenu != focus && m_objectInfoMenu.IsVisible() ) 
		{
			m_objectInfoMenu.Show(false);
		}
	}

	override void Update( float timeslice ) 
	{
		//GetPlayer().MessageStatus( (GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK).ToString() );

		if ( GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( false );
				GetGame().GetInput().ResetGameFocus( INPUT_DEVICE_MOUSE );
			}
		}
		else
		{
			if ( !GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( true );
				GetGame().GetInput().ChangeGameFocus( 1, INPUT_DEVICE_MOUSE );
			}
		}
	}
}

