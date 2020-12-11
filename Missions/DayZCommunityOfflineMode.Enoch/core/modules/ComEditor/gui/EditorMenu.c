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
	    SetID( 133742 );
	}
	
	void ~EditorMenu()
	{
	}
	
	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\EditorMenu.layout" );

		m_ObjectButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("objects_button") );
		m_PositionButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("position_button") );
		m_WeatherButton  = ButtonWidget.Cast( layoutRoot.FindAnyWidget("weather_button") );
		m_GameButton     = ButtonWidget.Cast( layoutRoot.FindAnyWidget("game_button") );
		m_CameraButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_button") );
		m_ObjectEditorButton = layoutRoot.FindAnyWidget( "objectEditor_button" );


		// object menu
		m_objectMenu   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectMenu.layout", layoutRoot );
		m_weatherMenu  = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\WeatherMenu.layout", layoutRoot );
		m_positionMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\Admintool\\gui\\layouts\\PositionMenu.layout", layoutRoot );
		m_gameMenu 	   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\GameMenu.layout", layoutRoot );
		m_objectInfoMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectEditorInfo.layout", layoutRoot );
		m_cameraMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\CameraTool\\gui\\layouts\\CameraSettings.layout", layoutRoot );

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

        GetGame().GetInput().ChangeGameFocus( 1 );
        GetGame().GetUIManager().ShowUICursor( true );
    }

    override void OnHide()
    {
        super.OnHide();

        ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).EditorState( false );

        GetGame().GetInput().ResetGameFocus();

        if ( !CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).IsUsingCamera() ) 
        {
			COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
        }
        

        CameraSettings.CAMERA_ROT.Show( false );
        CameraSettings.CAMERA_PHI.Show( false );
    }

    override bool OnDoubleClick( Widget w, int x, int y, int button ) 
    {
    	if ( w == layoutRoot ) 
    	{
	    	ObjectMenu objectMenu;
	    	m_objectMenu.GetScript( objectMenu );
	    	string strSelection = objectMenu.GetCurrentSelection();

	    	if ( strSelection != "" ) 
	    	{
	    		bool ai = false;

	        	if ( GetGame().IsKindOf( strSelection, "DZ_LightAI" ) ) 
	        	{
	        		ai = true;
	        	}

	    		Object obj = GetGame().CreateObject( strSelection, COM_GetPointerPos(), true, ai );
	    		obj.PlaceOnSurface(); // reeeeeeeeeeeee
	    		COM_ForceTargetCollisionUpdate( obj );
	    		ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).SelectObject( obj );
	    		ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).addObject( obj );

	    		ObjectInfoMenu.listBox.AddItem(obj.GetType(), obj, 0);
	    	}
    	}

    	return false;
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

			ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}

		if ( w == m_CameraButton ) 
		{
			m_cameraMenu.GetScript( popMenu );
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
		// todo change this so that its based on visible layout and not click event

		Widget m_ObjectButtonBkg   = layoutRoot.FindAnyWidget("objects_button_bkg");
		Widget m_PositionButtonBkg = layoutRoot.FindAnyWidget("position_button_bkg");
		Widget m_WeatherButtonBkg  = layoutRoot.FindAnyWidget("weather_button_bkg");
		Widget m_GameButtonBkg     = layoutRoot.FindAnyWidget("game_button_bkg");
		Widget m_ObjectEditorBkg   = layoutRoot.FindAnyWidget("objectEditor_button_bkg");
		Widget m_CameraButtonBkg   = layoutRoot.FindAnyWidget("camera_button_bkg");

		m_ObjectButtonBkg.SetColor(ARGB(0, 255, 255, 255)); // reset colors
		m_PositionButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_WeatherButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_GameButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_ObjectEditorBkg.SetColor(ARGB(0, 255, 255, 255));
		m_CameraButtonBkg.SetColor(ARGB(0, 255, 255, 255));

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
		if ( m_ObjectEditorButton == focus && m_objectInfoMenu.IsVisible() )
		{
			m_ObjectEditorBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_CameraButton == focus && m_cameraMenu.IsVisible() )
		{
			m_CameraButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void HideMenus( Widget focus ) 
	{
		if ( m_objectInfoMenu != focus && m_objectMenu != focus && m_objectMenu.IsVisible() ) 
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
		if ( m_cameraMenu != focus && m_cameraMenu.IsVisible() ) 
		{
			m_cameraMenu.Show(false);
		}
		if ( m_objectMenu != focus && m_objectInfoMenu != focus && m_objectInfoMenu.IsVisible() ) 
		{
			m_objectInfoMenu.Show(false);
			ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).ToggleEditor();
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
				GetGame().GetInput().ResetGameFocus( );
			}
		}
		else
		{
			if ( !GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( true );
				GetGame().GetInput().ChangeGameFocus( 1 );
			}
		}
	}
}

