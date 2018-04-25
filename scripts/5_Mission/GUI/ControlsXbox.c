class ControlsXbox extends UIScriptedMenu
{
	protected ImageWidget 	m_ControlsLayoutImage;
	
	//============================================
	// ControlsXbox
	//============================================
	void ControlsXbox()
	{
	}
	
	void ~ControlsXbox()
	{
		PPEffects.SetBlurMenu( 0 );
	}

	//============================================
	// Menu Controls
	//============================================	
	void OpenMenu()
	{
		GetGame().GetUIManager().EnterScriptedMenu( MENU_XBOX_CONTROLS, NULL );
	}
	
	void CloseMenu()
	{
		GetGame().GetUIManager().Back();
	}
		
	//============================================
	// Init
	//============================================
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/day_z_xbox_controls.layout" );
		m_ControlsLayoutImage = ImageWidget.Cast( layoutRoot.FindAnyWidget( "XboxControlsImage" ) );
		
#ifdef PLATFORM_XBOX
		m_ControlsLayoutImage.LoadImageFile( 0, "{F12419054D147408}Gui/textures/day_z_xbox_controls.edds" );
#endif
		
#ifdef PLATFORM_PS4
		m_ControlsLayoutImage.LoadImageFile( 0, "{8CC5531F7A593B80}Gui/textures/dayz_ps4_controls.edds" );
#endif
		PPEffects.SetBlurMenu( 0.6 );
		return layoutRoot;
	}
		
	//============================================
	// Events
	//============================================	
	override bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);
		
		if ( control == ControlID.CID_BACK )
		{
			CloseMenu();
			
			return true;
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		return false;
	}		
}
