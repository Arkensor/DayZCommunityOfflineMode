/**
 * \defgroup Mouse Events
 * \desc constants for mouse events in PluginKeyBinding
 * @{
 */

const int MB_EVENT_PRESS			= 0;
const int MB_EVENT_CLICK			= 1;
const int MB_EVENT_RELEASE			= 2;
const int MB_EVENT_DOUBLECLICK		= 3;
const int MB_EVENT_DRAG				= 4;
/** @}*/

class PluginKeyBinding extends PluginBase
{
	static PluginKeyBinding instance;
	
	// System Public Events
	void PluginKeyBinding()
	{
		if ( instance == NULL )
		{
			instance = this;
		}
	}

	//============================================
	// OnInit
	//============================================
	override void OnInit()
	{
		super.OnInit();
		
		m_KeyBindings	= new array<ref KeyBinding>;
		m_MouseBindings	= new array<ref MouseBinding>;
		m_MouseButtons	= new array<ref MouseButtonInfo>;
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.LEFT ) );
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.RIGHT ) );
		m_MouseButtons.Insert ( new MouseButtonInfo( MouseState.MIDDLE ) );
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.OnFrame);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Keyboard Binds	|UI_ID							|Key1					|Key2				|Callback Plugin		|Callback Function				|Info Shrtcut					|Info Description
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//					|constants.h					|constants.h			|constants.h		|only plugin name		|only function					|								|
		//					|MENU_***						|KeyCode.KC_***			|KeyCode.KC_***		|						|in plugin						|								|
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		RegisterKeyBind(	 MENU_ANY						,KeyCode.KC_LWIN		,KeyCode.KC_F1		,"PluginDeveloper"		,"ToggleHelpScreen" 			,"[LWin]+[F1]"					,"Show/Hide help screen");
		RegisterKeyBind(	 MENU_NONE|MENU_SCRIPTCONSOLE	,KeyCode.KC_LWIN		,KeyCode.KC_GRAVE	,"PluginDeveloper"		,"ToggleScriptConsole" 			,"[LWin]+[~]"					,"Show/Hide this script console");
		RegisterKeyBind(	 MENU_NONE|MENU_SCRIPTCONSOLE	,KeyCode.KC_RWIN		,KeyCode.KC_GRAVE	,"PluginDeveloper"		,"ToggleScriptConsole" 			,"[RWin]+[~]"					,"Show/Hide this script console");
		RegisterKeyBind(	 MENU_NONE|MENU_SCENE_EDITOR	,KeyCode.KC_LWIN		,KeyCode.KC_TAB		,"PluginSceneManager"	,"EditorToggle"					,"[LWin]+[TAB]"					,"Show/Hide Scripted Scene Editor");
		RegisterKeyBind(	 MENU_NONE|MENU_SCENE_EDITOR	,KeyCode.KC_RWIN		,KeyCode.KC_TAB		,"PluginSceneManager"	,"EditorToggle"					,"[RWin]+[TAB]"					,"Show/Hide Scripted Scene Editor");
		RegisterKeyBind(	 MENU_MAIN						,KeyCode.KC_LWIN 		,KeyCode.KC_E		,"PluginDeveloper"		,"ToggleMissionLoader"			,"[Win]+[E]"					,"Show/Hide mission loader");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_RSHIFT		,-1					,"PluginDeveloper"		,"SpawnFromClipboard"			,"[Right Shift]"				,"Ingame => Spawn item from clipboard");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_INSERT		,-1					,"PluginDeveloper"		,"TeleportAtCursor"				,"[Insert]"		 				,"Ingame => Teleport player to cursor position");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_NEXT		,-1					,"PluginDeveloper"		,"ToggleGodModeThisPlayer"		,"[Page Down]"					,"Ingame => Toggle god mode");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_HOME		,-1					,"PluginDeveloper"		,"ToggleFreeCamera"				,"[Home]"						,"Ingame => Toggle free camera, teleport player on free camera position");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_DIVIDE		,-1					,"PluginDeveloper"		,"ToggleFreeCameraBackPos"		,"[Divide numeric]"				,"Ingame => Toggle free camera, back on position");
		RegisterKeyBind(	 MENU_SCRIPTCONSOLE				,KeyCode.KC_PRIOR		,-1					,"PluginDeveloper"		,"ScriptHistoryBack"			,"[Page Up]"					,"Debug Console => Script history back");
		RegisterKeyBind(	 MENU_SCRIPTCONSOLE				,KeyCode.KC_NEXT		,-1					,"PluginDeveloper"		,"ScriptHistoryNext"			,"[Page Down]"					,"Debug Console => Script history next");	
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_DELETE		,-1					,"PluginSceneManager"	,"DeleteSelectedObject"			,"[Delete]"						,"Scene Editor => Delete current selected object");	
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_LCONTROL	,KeyCode.KC_S		,"PluginSceneManager"	,"SceneSave"					,"[LCtrl]+[S]"					,"Scene Editor => Save Current Scene");
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_LCONTROL	,KeyCode.KC_D		,"PluginSceneManager"	,"SelectedObjectDuplicate"		,"[LCtrl]+[D]"					,"Scene Editor => Duplicate current selected object");
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_LCONTROL	,KeyCode.KC_F		,"PluginSceneManager"	,"SelectedObjectFocus"			,"[LCtrl]+[F]"					,"Scene Editor => Focus camera on selected object");
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_LCONTROL	,KeyCode.KC_E		,"PluginSceneManager"	,"RulerToggle"					,"[LCtrl]+[E]"					,"Scene Editor => Enable/Disable addition of ruler points");
		RegisterKeyBind(	 MENU_SCENE_EDITOR				,KeyCode.KC_LCONTROL	,KeyCode.KC_PRIOR	,"PluginRecipesManager"	,"CallbackGenerateCache"		,"[LCtrl]+[BACKSPACE]"			,"PluginRecipesManager => Generate recipe cache and save it to file");
		RegisterKeyBind(	 MENU_ANY						,KeyCode.KC_LCONTROL	,KeyCode.KC_DECIMAL	,"PluginItemDiagnostic"	,"ToggleDebugWindowEvent"		,"[LCtrl]+[DECIMAL]"			,"Close the Item Debug window");
		RegisterKeyBind(	 MENU_ANY						,KeyCode.KC_LWIN		,KeyCode.KC_NUMPAD0	,"PluginDayzPlayerDebug","ToggleDebugWindowEvent"		,"[LWin]+[NUMPAD0]"				,"Open/Close Player debug window");
		RegisterKeyBind(	 MENU_ANY						,KeyCode.KC_LWIN		,KeyCode.KC_DECIMAL	,"PluginDayzPlayerDebug","ToggleDebugWindowEventP"		,"[LWin]+[DECIMAL]"				,"Open/Close Player debug window");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_LWIN		,KeyCode.KC_NUMPAD9	,"PluginDeveloperSync"	,"ToggleFocus"					,"[LWin]+[NUMPAD9]"				,"Toggle game focus");
		RegisterKeyBind(	 MENU_NONE						,KeyCode.KC_LWIN		,KeyCode.KC_SCROLL	,"PluginDoorRuler"		,"CheckInit"					,"[LWin]+[SCROLL LOCK]"			,"Enable Door Ruler");
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Mouses Binds		|UI_ID				|Mouse Button	|Mouse Event			|Callback Plugin		|Callback Function			|Info Shrtcut			|Info Description
		//----------------------------------------------------------------------------------------------------------------------------------------------------------------
		//					|constants.h		|MouseState.LEFT		|MB_EVENT_CLICK			|only plugin name		|only function				|						|
		//					|MENU_***			|MouseState.RIGHT		|MB_EVENT_DOUBLECLICK   |						|in plugin					|						|
		//										|MouseState.MIDDLE		|MB_EVENT_DRAG			|						|							|						|
		//										|						|MB_EVENT_RELEASE		|						|							|						|
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.LEFT		,MB_EVENT_CLICK			,"PluginSceneManager"	,"Event_OnClick"			,"[LMB] Click"			,"Mission Editor => Select object in scene");
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.LEFT		,MB_EVENT_DOUBLECLICK	,"PluginSceneManager"	,"Event_OnDoubleClick" 		,"[LMB] Double Click"	,"Mission Editor => Craete new object in scene");
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.LEFT		,MB_EVENT_DRAG			,"PluginSceneManager"	,"Event_OnDrag" 			,"[LMB] Hold"			,"Mission Editor => Moving with selected object");
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.MIDDLE		,MB_EVENT_CLICK			,"PluginSceneManager"	,"Event_OnClickMiddle"		,"[MMB] Click"			,"Mission Editor => Link clicked object with selected object");
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.RIGHT		,MB_EVENT_PRESS			,"PluginSceneManager"	,"FreeCameraControlEnable"	,"[RMB] Hold"			,"Mission Editor => Enable camera movement");
		RegisterMouseBind( 	MENU_SCENE_EDITOR	,MouseState.RIGHT		,MB_EVENT_RELEASE		,"PluginSceneManager"	,"FreeCameraControlDisable"	,"[RMB] Release"		,"Mission Editor => Disable camera movement");
		RegisterMouseBind( 	MENU_NONE			,MouseState.LEFT		,MB_EVENT_CLICK			,"PluginDayZCreatureAIDebug","Event_OnClick"		,"[LMB] Click"			,"DayZCreature graph debug => Capture entity during capture mode");
		// TO DO: (Boris V.) Remove the following commented-out mouse binds when weapon particles can be implemented properly. Also remove the events they call.
		//RegisterMouseBind( 	MENU_NONE			,MouseState.LEFT		,MB_EVENT_PRESS			,"PluginDeveloper"		,"MuzzleParticleTestStart"	,"[LMB] Hold"			,"Debug particle");
		//RegisterMouseBind( 	MENU_NONE			,MouseState.LEFT		,MB_EVENT_RELEASE		,"PluginDeveloper"		,"MuzzleParticleTestEnd"	,"[LMB] Release"		,"Debug particle2");
		
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	//==========================================
	// OnInit (System Event)
	//==========================================
	override void OnDestroy()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.OnFrame);
	}
	
	//============================================
	// OnKeyPress
	//============================================
	void OnKeyPress(int key)
	{
		//Print("OnKeyPress "+ key );

		for ( int i = 0; i < m_KeyBindings.Count(); ++i )
		{
			KeyBinding key_binding = m_KeyBindings.Get(i);
			
			if ( !IsCurrentUIPageValid( key_binding.GetUIMenuID() ) )
			{
				continue;
			}
			
			bool key1_match = false;
			bool key2_match = false;
			
			if ( key_binding.GetKey1() > -1 )
			{
				if( IsKeyPressed(key_binding.GetKey1()) )
				{
					// Key1 is down
					key1_match = true;
				}
			}
			else
			{
				// Key1 is not set
				key1_match = true;
			}
			
			if ( key_binding.GetKey2() > -1 )
			{
				if( IsKeyPressed(key_binding.GetKey2()) )
				{
					// Key2 is down
					key2_match = true;
				}
			}
			else
			{
				// Key2 is not set
				key2_match = true;
			}
			
			if ( key1_match && key2_match )
			{
				//Log( "Call Method -> key press: " + itoa(key) + " " + key_binding.GetInfoDescription() );
				
				PluginBase m = GetPlugin(key_binding.GetCallbackTarget().ToType());
				GetGame().GameScript.CallFunction(m, key_binding.GetCallbackFunction(), NULL, 0);
				
				break;
			}
		}
	}
	
	//============================================
	// OnKeyRelease
	//============================================
	void OnKeyRelease(int key)
	{
	}
	
	//============================================
	// OnMouseButtonPress
	//============================================
	void OnMouseButtonPress(int button)
	{
		MouseButtonInfo button_info = GetMouseButtonInfo( button );
		if (button_info == NULL) return;
		button_info.Press();
		
	//Log("****************************");
	//Log(" MB_EVENT_PRESS");
		
		OnMouse( MB_EVENT_PRESS, button );
	}
	
	//============================================
	// OnMouseButtonRelease
	//============================================
	void OnMouseButtonRelease(int button)
	{
		MouseButtonInfo button_info = GetMouseButtonInfo( button );
		if (button_info == NULL) return;
		int time_curr			= GetGame().GetTime();
		int time_last_press		= button_info.GetTimeLastPress();
		int time_last_release	= button_info.GetTimeLastRelease();	
		int time_delta_press	= time_curr - time_last_press;
		int time_delta_relase	= time_curr - time_last_release;
		
		if ( time_delta_relase < DOUBLE_CLICK_TIME )
		{
	//Log(" DOUBLE_CLICK_TIME ");
			
			OnMouse( MB_EVENT_DOUBLECLICK, button );
		}
		else if ( time_delta_press < CLICK_TIME )
		{
	//Log(" MB_EVENT_CLICK ");
			
			OnMouse( MB_EVENT_CLICK, button );
		}
		
	//Log(" MB_EVENT_RELEASE");
		
		OnMouse( MB_EVENT_RELEASE, button );
		
		button_info.Release();
	}
	
	//============================================
	// OnFrame
	//============================================
	void OnFrame()
	{
		for ( int i = 0; i < m_MouseButtons.Count(); ++i )
		{
			MouseButtonInfo info = m_MouseButtons.Get(i);
			
			if ( info.IsButtonDown() )
			{		
				int time_curr = GetGame().GetTime();
				int time_hold = info.GetTimeLastPress() + HOLD_CLICK_TIME_MIN;
				
				if ( time_hold < time_curr )
				{
	//Log("MB_EVENT_DRAG");
					
					OnMouse( MB_EVENT_DRAG, info.GetButtonID() );
				}
			}
		}
	}
	
	//--------------------------------------------
	// GetKeyBindings
	//--------------------------------------------
	array<ref KeyBinding> GetKeyBindings()
	{
		return m_KeyBindings;
	}

	//--------------------------------------------
	// GetMouseBindings
	//--------------------------------------------
	array<ref MouseBinding> GetMouseBindings()
	{
		return m_MouseBindings;
	}
	
	protected const int CLICK_TIME			= 200; //ms
	protected const int DOUBLE_CLICK_TIME	= 300; //ms
	protected const int HOLD_CLICK_TIME_MIN	= 300; //ms
	
	protected ref array<ref KeyBinding>			m_KeyBindings;
	protected ref array<ref MouseButtonInfo>	m_MouseButtons;
	protected ref array<ref MouseBinding>		m_MouseBindings;
	
	//--------------------------------------------
	// RegisterKeyBind
	//--------------------------------------------
	protected void RegisterKeyBind( int ui_id, int key_code1, int key_code2, string plugin_name, string fnc_name, string info_shortcut, string info_description )
	{
		m_KeyBindings.Insert( new KeyBinding(ui_id, key_code1, key_code2, plugin_name, fnc_name, info_shortcut, info_description) );
	}

	//--------------------------------------------
	// RegisterMouseBind
	//--------------------------------------------
	protected void RegisterMouseBind( int ui_id, int mouse_button, int mouse_event, string plugin_name, string fnc_name, string info_shortcut, string info_description )
	{
		m_MouseBindings.Insert( new MouseBinding(ui_id, mouse_button, mouse_event, plugin_name, fnc_name, info_shortcut, info_description) );
	}

	//--------------------------------------------
	// IsCurrentUIPageValid
	//--------------------------------------------
	protected bool IsCurrentUIPageValid( int ui_page_request )
	{
		int ui_page_current = MENU_NONE;
		
		if ( GetGame().GetUIManager().GetMenu() )
		{
			ui_page_current = GetGame().GetUIManager().GetMenu().GetID();
		}
		
		if ( ui_page_current < MENU_ANY || !CheckMask( ui_page_request, ui_page_current ) )
		{			
			if ( CheckMask( ui_page_request, MENU_NONE ) || !CheckMask( ui_page_request, MENU_ANY ) )
			{
				return false;
			}
		}
		
		return true;
	}

	bool CheckMask( int source_mask, int target_mask )
	{
		if ( ( source_mask & target_mask ) == target_mask )
		{
			return true;
		}
		
		return false;
	}

	//--------------------------------------------
	// OnMouse
	//--------------------------------------------
	protected void OnMouse( int event_id, int button )
	{	
		for ( int i = 0; i < m_MouseBindings.Count(); ++i )
		{
			MouseBinding mouse_binding = m_MouseBindings.Get(i);
			
			if ( mouse_binding.GetButtonID() == button && mouse_binding.GetMouseEvent() == event_id )
			{
				if ( IsCurrentUIPageValid( mouse_binding.GetUIMenuID() ) )
				{
					PluginBase m = GetPlugin(mouse_binding.GetCallbackTarget().ToType());
					GetGame().GameScript.CallFunction(m, mouse_binding.GetCallbackFunction(), NULL, 0);
				}
			}			
		}
	}

	//--------------------------------------------
	// OnMouse
	//--------------------------------------------
	protected MouseButtonInfo GetMouseButtonInfo( int button )
	{	
		for ( int i = 0; i < m_MouseButtons.Count(); ++i )
		{
			MouseButtonInfo info = m_MouseButtons.Get(i);
			
			if ( info.GetButtonID() == button )
			{
				return info;
			}
		}
		return NULL;
	}

	//--------------------------------------------
	// IsKeyPressed
	//--------------------------------------------
	protected bool IsKeyPressed(int key)
	{	
		return ( KeyState(key) == 1 );
	}
}
