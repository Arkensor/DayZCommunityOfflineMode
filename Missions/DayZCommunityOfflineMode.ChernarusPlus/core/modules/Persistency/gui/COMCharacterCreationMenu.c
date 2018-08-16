class COMCharacterCreationMenu extends UIScriptedMenu
{
	protected ref COMCharacterCreationScene	m_Scene;
	
	protected Widget						m_CharacterRotationFrame;
	protected Widget						m_Apply;
	protected Widget						m_RandomizeCharacter;
	protected Widget						m_BackButton;
	
	protected EditBoxWidget					m_PlayerName;
	protected TextWidget					m_Version;
	
	protected ref OptionSelectorMultistate	m_GenderSelector;
	protected ref OptionSelectorMultistate	m_SkinSelector;
	protected ref OptionSelectorMultistate	m_TopSelector;
	protected ref OptionSelectorMultistate	m_BottomSelector;
	protected ref OptionSelectorMultistate	m_ShoesSelector;
	
	protected ref PersistencyModule 				m_oPersistencyModule;
	protected ref COMCharacterSpawnMenu 	m_oCharacterMenu;

	void COMCharacterCreationMenu( ref PersistencyModule oPersistencyModule, ref COMCharacterSpawnMenu oCharacterMenu )
	{
		m_oPersistencyModule = oPersistencyModule;
		m_oCharacterMenu = oCharacterMenu;

		m_Scene = new COMCharacterCreationScene;

		MissionMainMenu mission = MissionMainMenu.Cast( GetGame().GetMission() );
		
		// m_Scene.m_Camera.LookAt( Vector( m_Scene.m_DemoPos[0], m_Scene.m_DemoPos[1] + 1, m_Scene.m_DemoPos[2] ) );
	}
	
	override Widget Init()
	{
		// Temp;
		GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
		GetGame().GetUIManager().ShowUICursor(true);	

		GetGame().SelectPlayer( NULL, NULL );

		layoutRoot							= GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\layouts\\COMCharacterCreation.layout" );
		
		m_CharacterRotationFrame			= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		m_Apply								= layoutRoot.FindAnyWidget( "apply" );
		m_RandomizeCharacter				= layoutRoot.FindAnyWidget( "randomize_character" );
		m_BackButton						= layoutRoot.FindAnyWidget( "back" );
		
		m_PlayerName						= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "general_name_setting_text" ) );
		m_Version							= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		Refresh();
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );
		
		m_GenderSelector					= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_Scene.m_CharGenderList );
		if ( m_Scene.IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector	= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.m_CharPersonalityFemaleList );
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector	= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.m_CharPersonalityMaleList );
		}
		
		m_TopSelector						= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_top_setting_option" ), 0, null, false, m_Scene.m_CharShirtList );
		m_BottomSelector					= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_bottom_setting_option" ), 0, null, false, m_Scene.m_CharPantsList );
		m_ShoesSelector						= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_shoes_setting_option" ), 0, null, false, m_Scene.m_CharShoesList );
		
		if( m_Scene && m_Scene.GetIntroSceneCharacter() )
		{
			PlayerBase scene_char = m_Scene.GetIntroSceneCharacter();
			
			Object obj = scene_char.GetInventory().FindAttachment(InventorySlots.BODY);
			if( obj )
				m_TopSelector.SetValue( obj.GetType() );
			
			obj = scene_char.GetInventory().FindAttachment(InventorySlots.LEGS);
			if( obj )
				m_BottomSelector.SetValue( obj.GetType() );
			
			obj = scene_char.GetInventory().FindAttachment(InventorySlots.FEET);
			if( obj )
				m_ShoesSelector.SetValue( obj.GetType() );
			
			m_SkinSelector.SetValue( scene_char.GetType() );
		}
		
		m_GenderSelector.m_OptionChanged.Insert( GenderChanged );
		m_SkinSelector.m_OptionChanged.Insert( SkinChanged );
		m_TopSelector.m_OptionChanged.Insert( TopChanged );
		m_BottomSelector.m_OptionChanged.Insert( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Insert( ShoesChanged );
		
		return layoutRoot;
	}
	
	void ~COMCharacterCreationMenu()
	{
		m_GenderSelector.m_OptionChanged.Remove( GenderChanged );
		m_SkinSelector.m_OptionChanged.Remove( SkinChanged );
		m_TopSelector.m_OptionChanged.Remove( TopChanged );
		m_BottomSelector.m_OptionChanged.Remove( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Remove( ShoesChanged );
	}
	
	//Button Events
	void Apply()
	{
		g_Game.SetPlayerGameName( m_PlayerName.GetText() );
		
		Close();

		m_oPersistencyModule.CreateNew(m_Scene.GetCharacterName(), m_Scene.GetPlayerUnit());
	}
	
	void SetCharacter()
	{
		if (m_Scene.GetPlayerUnit())
		{
			m_PlayerName.SetText( g_Game.GetPlayerGameName() );
			
			m_Scene.SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
			m_Scene.SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
			m_Scene.SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
			
			if (g_Game.IsNewCharacter())
			{
				m_Scene.SetAttachment("", InventorySlots.SHOULDER);
				m_Scene.SetAttachment("", InventorySlots.BOW);
				m_Scene.SetAttachment("", InventorySlots.MELEE);
				m_Scene.SetAttachment("", InventorySlots.VEST);
				m_Scene.SetAttachment("", InventorySlots.HIPS);
				m_Scene.SetAttachment("", InventorySlots.BACK);
				m_Scene.SetAttachment("", InventorySlots.HEADGEAR);
				m_Scene.SetAttachment("", InventorySlots.MASK);
				m_Scene.SetAttachment("", InventorySlots.EYEWEAR);
				m_Scene.SetAttachment("", InventorySlots.GLOVES);
				m_Scene.SetAttachment("", InventorySlots.ARMBAND);
				m_Scene.SetAttachment("", InventorySlots.HANDS);
			}
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_Scene.SceneCharacterSetPos, 250 );
		}
	}
	
	void RandomizeCharacter()
	{
		g_Game.SetNewCharacter(true);
		
		// m_Scene.SetCurrentCharacterID( -1 );
		
		// make random selection
		m_Scene.RandomSelectGender();
		
		if ( m_Scene.IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityFemaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityMaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
		
		m_TopSelector.SetRandomValue();
		m_BottomSelector.SetRandomValue();
		m_ShoesSelector.SetRandomValue();
		
		SetCharacter();
		
		CheckNewOptions();
	}
	
	//Selector Events
	void GenderChanged()
	{
		m_Scene.SetCharacterFemale( ( m_GenderSelector.GetStringValue() == "Female" ) );
		
		if ( m_Scene.IsCharacterFemale() )
		{
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityFemaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
		else
		{
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityMaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
	}
	
	void SkinChanged()
	{
		m_Scene.CreateNewCharacter( m_SkinSelector.GetStringValue() );
		
		layoutRoot.FindAnyWidget( "character_root" ).Show( g_Game.IsNewCharacter() );
		
		TopChanged();
		BottomChanged();
		ShoesChanged();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_Scene.SceneCharacterSetPos, 100 );
	}
	
	void TopChanged()
	{
		m_Scene.SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
	}
	
	void BottomChanged()
	{
		m_Scene.SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
	}
	
	void ShoesChanged()
	{
		m_Scene.SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
	}
	
	override bool UseMouse()
	{
		return true;
	}

	override bool UseKeyboard()
	{
		return true;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		if (key == KeyCode.KC_ESCAPE)
		{
			m_oPersistencyModule.LoadLast();

			Close();

			return true;
		}

		return super.OnKeyPress( w, x, y, key );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( w == m_Apply )
		{
			Apply();
			return true;
		}
		else if ( w == m_RandomizeCharacter )
		{
			RandomizeCharacter();
			return true;
		}
		else if ( w == m_BackButton )
		{
			Close();
			m_oPersistencyModule.ShowCharacterMenu();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if ( w == m_CharacterRotationFrame )
		{
			if (m_Scene)
				m_Scene.CharacterRotationStart();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if (m_Scene)
			m_Scene.CharacterRotationStop();
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Apply || w == m_RandomizeCharacter || w == m_BackButton );
	}
	
	void CheckNewOptions()
	{
		layoutRoot.FindAnyWidget( "character_gender_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_head_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_top_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_bottom_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_shoes_button" ).Show( g_Game.IsNewCharacter() );
	}
	
	override void OnShow()
	{
		SetFocus( m_Apply );
		CheckNewOptions();
		
		if( m_Scene && m_Scene.GetCamera() )
		{
			m_Scene.GetCamera().LookAt( m_Scene.GetIntroSceneCharacter().GetPosition() + Vector( 0, 1, 0 ) );
		}
	}
	
	override void Refresh()
	{
	}
	
	override void OnHide()
	{
		super.OnHide();
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}
}