
class ObjectEditor extends Module
{
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	Object m_SelectedObject;

	protected ref ObjectMenu m_ObjectMenu;

	// protected ref Scene active_Scene;
	protected ref array< ref Object> m_Objects = new array< ref Object>;
	string BASE_COM_DIR = "$saves:CommunityOfflineMode";
	string BASE_SCENE_DIR = BASE_COM_DIR + "\\Scenes";

	void ObjectEditor()
	{
		MakeDirectory(BASE_SCENE_DIR);

	}

	void ~ObjectEditor()
	{
	}
	
	void addObject( Object trackedObject ) 
	{
		m_Objects.Insert( trackedObject );
	}

	override void Init() 
	{
		super.Init();

		LoadScene();
	}

	/**
		
		Works very similar to pluginkeybinding system with extended features such as multiple keybinds (mouse + keyboard) per function
		Please refrain from assigning same mouse keys to different functions even with more keybinds.
		ie: shift+click function vs click function will both call when pressing the former.
		
		WORK AROUND COULD BE TO MAKE PRIORITY TO FUNCTIONS THAT REQUIRE MORE KEYS/MOUSE BINDS
		IE: SHIFT+END TOGGLEOBJECTEDITOR VS CTRL+SHIFT+END SOMEOTHERFUNCTION
		THE FUNCTION WITH MORE BINDS SHOULD BE REGISTERED FIRST.
		
		TODO: MAKE SYSTEM LIKE THAT ^
		- dannydog
	
	**/
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleEditor  = new KeyMouseBinding( GetModuleType(), "ToggleEditor" , "[Shift]+[End]" , "Toggle object editor."     );
		KeyMouseBinding objectSelect  = new KeyMouseBinding( GetModuleType(), "ClickObject"  , "(LMB)+(Click)" , "Selects object on cursor.", true );
		KeyMouseBinding objectDrag    = new KeyMouseBinding( GetModuleType(), "DragObject"   , "(LMB)+(Drag)"  , "Drag objects on cursor.", true   );
		KeyMouseBinding objectScroll  = new KeyMouseBinding( GetModuleType(), "ScrollObject" , "[Shift][Ctrl][Alt]+(Wheel)" , "Raise or lower objects with mouse wheel as well as rotate.", true );
		KeyMouseBinding objectDelete  = new KeyMouseBinding( GetModuleType(), "DeleteObject" , "[Delete]"	   , "Deletes selected object.", true  );
		KeyMouseBinding objectGround  = new KeyMouseBinding( GetModuleType(), "GroundObject" , "(Middle Mouse)", "Snaps objects to ground.", true  );
		KeyMouseBinding sceneSave     = new KeyMouseBinding( GetModuleType(), "SaveScene"    , "CTRL+S"	       , "Saves current scene of objects", true);

		toggleEditor.AddKeyBind( KeyCode.KC_LSHIFT, KeyMouseBinding.KB_EVENT_HOLD    ); 
		toggleEditor.AddKeyBind( KeyCode.KC_END   , KeyMouseBinding.KB_EVENT_RELEASE ); // Press END. Using Release prevents key HOLD spam from onKeyPress (could use ClearKey in onKeyPress however)
		objectDelete.AddKeyBind( KeyCode.KC_DELETE, KeyMouseBinding.KB_EVENT_RELEASE ); // Pretty much making KB_EVENT_PRESS useless since you can just use KB_EVENT_HOLD instead.
		sceneSave.AddKeyBind( KeyCode.KC_LCONTROL,  KeyMouseBinding.KB_EVENT_HOLD );
		sceneSave.AddKeyBind( KeyCode.KC_S, 	 KeyMouseBinding.KB_EVENT_RELEASE );


		objectSelect.AddMouseBind( MouseState.LEFT		, KeyMouseBinding.MB_EVENT_CLICK ); // Left Click
		objectDrag.  AddMouseBind( MouseState.LEFT 		, KeyMouseBinding.MB_EVENT_DRAG  );
		objectScroll.AddMouseBind( MouseState.WHEEL		, 0 							 ); // Doesn't matter what event for wheel
		objectGround.AddMouseBind( MouseState.MIDDLE	, KeyMouseBinding.MB_EVENT_CLICK );
		
		RegisterKeyMouseBinding( toggleEditor );
		RegisterKeyMouseBinding( objectSelect );
		RegisterKeyMouseBinding( objectDrag   );
		RegisterKeyMouseBinding( objectScroll );
		RegisterKeyMouseBinding( objectDelete );
		RegisterKeyMouseBinding( objectGround );
		RegisterKeyMouseBinding( sceneSave );
		
	}

	void SaveScene() 
	{	
		ref Scene scene = new Scene();
		scene.name = "latest";

		string tocopy = "";

		foreach( Object m_object : m_Objects ) 
		{
			ref Param objectParam = new Param3<string, vector, vector>( m_object.GetType(), m_object.GetPosition(), m_object.GetOrientation() );
			scene.m_SceneObjects.Insert( objectParam );

			tocopy = tocopy + "{\"" + m_object.GetType() + "\"" + "," + m_object.GetPosition()[0] + " " + m_object.GetPosition()[1] + " "  + m_object.GetPosition()[2] + "," + m_object.GetOrientation()[0] + " " + m_object.GetOrientation()[1] + " " + m_object.GetOrientation()[2] + "},";
		}
		tocopy = "auto data = { \n" + tocopy;
		tocopy = tocopy + "\n}; \nforeach( auto x : data ) { auto obj = GetGame().CreateObject( x[0], x[1] ); obj.SetPosition( x[1] ); obj.SetOrientation( x[2] ); }";

		GetGame().CopyToClipboard(tocopy);
		JsonFileLoader< Scene >.JsonSaveFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );
	}

	void LoadScene() 
	{
		ref Scene scene = new Scene();

		JsonFileLoader<Scene>.JsonLoadFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );

		foreach( auto param : scene.m_SceneObjects ) 
		{

			Object object = GetGame().CreateObject( param.param1, param.param2, false, false );
			object.SetOrientation( param.param3 );

			m_Objects.Insert( object );
		}
	}

	void EditorState( bool state ) 
	{
		if ( m_ObjectEditorActive == state ) 
		{
			return;
		}

		m_ObjectEditorActive = state;

		if ( m_ObjectEditorActive ) 
		{	
			GetPlayer().MessageStatus("Object Editor Enabled");
		} 
		else 
		{
			GetPlayer().MessageStatus("Object Editor Disabled");
		}
	}
	
	void ToggleEditor()
	{
		m_ObjectEditorActive = !m_ObjectEditorActive;
		
		if ( m_ObjectEditorActive ) 
		{	
			GetPlayer().MessageStatus("Object Editor Enabled");
		} 
		else 
		{
			GetPlayer().MessageStatus("Object Editor Disabled");
		}
	}

	bool IsEditing() 
	{
		return m_ObjectEditorActive;
	}

	void SelectObject( Object object )
	{
		if ( ( ( m_SelectedObject != NULL ) && ( m_SelectedObject == object ) ) || object.IsInherited( PlayerBase ) )
		{
			return;
		}
		
		m_SelectedObject = object;
	}

	void DeselectObject() 
	{
		m_SelectedObject = NULL;
	}
	
	void DragObject() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			vector dir = GetGame().GetPointerDirection();

			vector from = GetGame().GetCurrentCameraPosition();

			vector to = from + ( dir * 10000 );

			vector contact_pos;
			vector contact_dir;
			vector bbox;

			int contact_component;

			m_SelectedObject.GetCollisionBox( bbox );

			if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, NULL, NULL, NULL, false, true ) )
			{
				vector oOrgPos = m_SelectedObject.GetPosition();
				float fSurfaceHight = GetGame().SurfaceY( oOrgPos [ 0 ], oOrgPos [ 2 ] );

				float nHightOffsetToGround = oOrgPos [ 1 ] - fSurfaceHight;

				contact_pos [ 1 ] = contact_pos [ 1 ] + nHightOffsetToGround;

				m_SelectedObject.SetPosition( contact_pos );

				ForceTargetCollisionUpdate( m_SelectedObject );

				ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );
			}
		}
	}

	void ScrollObject( int state ) 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}
		/*
		
		if ( m_SelectedObject )
		{
			vector pos = m_SelectedObject.GetPosition();
			vector ori = m_SelectedObject.GetOrientation();

			bool up = state < 0;
			int value = 1;
			if ( up ) value = -1;
			
			if ( SHIFT() )
			{
				if ( ori[0] > 0 ) // seemless pitch change
				{
					value = -value;
				}
				ori[1] = ori[1] + value;

				m_SelectedObject.SetOrientation( ori );
			}
			else if ( CTRL() )
			{
				ori [ 0 ] = ori [ 0 ] + value;

				m_SelectedObject.SetOrientation( ori );
			}
			else if ( ALT() )
			{
				ori[ 2 ] = ori[ 2 ] + value;

				m_SelectedObject.SetOrientation( ori );
			}
			else 
			{
				pos [ 1 ] = pos [ 1 ] + value*0.05;

				m_SelectedObject.SetPosition( pos );
			}
		}
		*/
	}

	void ClickObject() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}
		Widget widgetCursor = GetGame().GetUIManager().GetWidgetUnderCursor();

		if ( widgetCursor && widgetCursor.GetName() != "EditorMenu" ) 
		{
			return;
		}

		vector dir = GetGame().GetPointerDirection();
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + ( dir * 10000 );

		set< Object > objects = GetObjectsAt(from, to, GetGame().GetPlayer() );

		bool selected = false;
		
		for ( int nObject = 0; ( ( nObject < objects.Count() ) && !selected ); ++nObject )
		{
			Object obj = objects.Get( nObject );
			SelectObject( obj );
			selected = true;
			
			GetPlayer().MessageStatus("Selected object.");

			ObjectInfoMenu.infoPosYaw.SetText( m_SelectedObject.GetOrientation()[0].ToString() );
			ObjectInfoMenu.infoPosPitch.SetText( m_SelectedObject.GetOrientation()[1].ToString() );
			ObjectInfoMenu.infoPosRoll.SetText( m_SelectedObject.GetOrientation()[2].ToString() );
		}
	
		if ( !selected && m_SelectedObject )
		{
			GetPlayer().MessageStatus("Current object deselected.");
			DeselectObject();
		}
	}
	
	void DeleteObject() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			m_SelectedObject.SetPosition(vector.Zero); // If object does not physically delete, teleport it to 0 0 0
			GetGame().ObjectDelete( m_SelectedObject ); 
			m_SelectedObject = NULL;	
		}
	}
	
	void GroundObject() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			/*
			TStringArray sArray = new TStringArray;
			m_SelectedObject.GetSelectionList( sArray );

			foreach( string s : sArray ) 
			{
				Print( s );
				//GetPlayer().MessageStatus( s);
			}
			*/
			/*

			vector pos = m_SelectedObject.GetPosition();
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			
			vector clippingInfo[2];
			vector objectBBOX[2];
			
			m_SelectedObject.GetCollisionBox( objectBBOX );
			m_SelectedObject.ClippingInfo( clippingInfo );
		
			//float clipY = objectBBOX[1][1] / 2.0//- clippingInfo[0][1];
			//pos[1] = pos[1] + objectBBOX[1][1] - clipY;
			pos[1] = pos[1] + clippingInfo[1][1] / 2.0;//objectBBOX[0][1] - clipY

			m_SelectedObject.SetPosition(pos);
			*/
			SnapToGroundNew( m_SelectedObject );
		}
	}
}