
class ObjectEditor extends Module
{
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	Object m_SelectedObject;
	Object building;

	protected ref ObjectMenu m_ObjectMenu;

	// protected ref Scene active_Scene;
	ref array< ref Object> m_Objects = new array< ref Object>;

	string BASE_COM_DIR = "$saves:CommunityOfflineMode";
	string BASE_SCENE_DIR = BASE_COM_DIR + "\\Scenes";

	ref SceneManager sceneManager;
	SceneData currentSceneData;

	void ObjectEditor()
	{
//		MakeDirectory( BASE_COM_DIR );
//		MakeDirectory( BASE_SCENE_DIR );

		sceneManager = new SceneManager();
	}

	void ExportObjectLoad() // requested by robotstar78
	{
		// Copy paste your exported objects in here to make it look like the example below:
		/*
		SpawnObject( "Land_CementWorks_Hall2_Brick", "12941.188477 60.166824 5238.811523", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_Hall2_Brick", "12961.633789 54.630013 5247.288086", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_MillA", "12965.892578 70.050812 5223.445313", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_RotFurnace", "12957.807617 69.867195 5234.208984", "0.000000 0.000000 0.000000" );
		*/
	}

	void SpawnObject( string type, vector position, vector orientation )
	{
	    auto obj = GetGame().CreateObject( type, position );
	    obj.SetPosition( position );
	    obj.SetOrientation( orientation );
	    obj.SetOrientation( obj.GetOrientation() );
	    obj.Update();
	    obj.SetAffectPathgraph( true, false );
        GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );

	    m_Objects.Insert( obj );
	}

	void addObject( Object trackedObject )
	{
		m_Objects.Insert( trackedObject );
	}

	override void Init()
	{
		super.Init();

		LoadScene();
		ExportObjectLoad();
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
//		KeyMouseBinding toggleEditor  = new KeyMouseBinding( GetModuleType(), "ToggleEditor" , "[Shift]+[End]" , "Toggle object editor."            );
		KeyMouseBinding objectSelect  = new KeyMouseBinding( GetModuleType(), "ClickObject"  , "Selects object on cursor.", true  );
		KeyMouseBinding objectDrag    = new KeyMouseBinding( GetModuleType(), "DragObject"   , "Drag objects on cursor.",   true  );
		KeyMouseBinding objectDelete  = new KeyMouseBinding( GetModuleType(), "DeleteObject" , "Deletes selected object.",  true  );
		KeyMouseBinding objectGround  = new KeyMouseBinding( GetModuleType(), "GroundObject" , "Snaps objects to ground.",  true  );
		KeyMouseBinding sceneSave     = new KeyMouseBinding( GetModuleType(), "ExportScene"  , "Saves current scene of objects", true);
		KeyMouseBinding tabFix        = new KeyMouseBinding( GetModuleType(), "TabFix"       , "Fixes issue with tabbing out of the game", true );
//		KeyMouseBinding objectScroll  = new KeyMouseBinding( GetModuleType(), "ScrollObject" , "[Shift][Ctrl][Alt]+(Wheel)" , "Raise or lower objects with mouse wheel as well as rotate.", true );

//		toggleEditor.AddBinding( KeyCode.KC_LSHIFT, KeyMouseBinding.KB_EVENT_HOLD    );
//		toggleEditor.AddBinding( KeyCode.KC_END   , KeyMouseBinding.KB_EVENT_RELEASE ); // Press END. Using Release prevents key HOLD spam from onKeyPress (could use ClearKey in onKeyPress however)
		objectDelete.AddBinding( "kDelete" ); // Pretty much making KB_EVENT_PRESS useless since you can just use KB_EVENT_HOLD instead.
		sceneSave.AddBinding( "kLControl" );
		sceneSave.AddBinding( "kS" );
		tabFix.AddBinding( "kLMenu" );

		objectSelect.AddBinding( "mBLeft" ); // Left Click
		objectDrag.AddBinding( "mBLeft" );
		objectDrag.SetActionType( KeyMouseActionType.HOLD );
		objectGround.AddBinding( "mBMiddle" );

		RegisterKeyMouseBinding( objectSelect );
		RegisterKeyMouseBinding( objectDrag   );
		RegisterKeyMouseBinding( objectDelete );
		RegisterKeyMouseBinding( objectGround );
		RegisterKeyMouseBinding( sceneSave );
		RegisterKeyMouseBinding( tabFix );
	}

    void TabFix()
    {
        m_SelectedObject = NULL;
    }

	void ExportScene()
	{
		string toCopy;
        toCopy += "//Spawn helper function\n";
        toCopy += "void SpawnObject( string type, vector position, vector orientation )\n";
        toCopy += "{\n";
        toCopy += "    auto obj = GetGame().CreateObject( type, position );\n";
        toCopy += "    obj.SetPosition( position );\n";
        toCopy += "    obj.SetOrientation( orientation );\n";
        toCopy += "    obj.SetOrientation( obj.GetOrientation() ); //Collision fix\n";
        toCopy += "    obj.Update();\n";
        toCopy += "    obj.SetAffectPathgraph( true, false );\n";
        toCopy += "    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );\n";
        toCopy += "}\n";
        toCopy += "\n";
        toCopy += "//Your custom spawned objects\n";

		foreach( Object m_object : m_Objects )
		{
			toCopy = toCopy + "SpawnObject( \"" + m_object.GetType() + "\", \"" + VectorToString( m_object.GetPosition() ) + "\", \"" + VectorToString( m_object.GetOrientation() ) + "\" );\n";
		}

		GetGame().CopyToClipboard( toCopy );

		Message( "Copied to clipboard" );
	}

	void SaveScene()
	{

		// loot spots
		// pumpkins = civilian
		// apple = medical
		// orange = industral
		// plum = military

/*
		SceneInfo sceneData = new SceneInfo("Test");
		// vector position = GetPlayer().WorldToModel(); // origin point

		foreach( Object m_object : m_Objects )
		{
			vector pos = GetPlayer().WorldToModel( m_object.GetPosition() );

			if ( m_object.GetType() != "Pumpkin" )
			{
				sceneData.AddObject( m_object, pos ); // add objects
			}
			// then add loot spots
		}

		foreach( Object m_objectt : m_Objects )
		{
			vector poss = GetPlayer().WorldToModel( m_object.GetPosition() );

			if ( m_object.GetType() == "Pumpkin" )
			{
				sceneData.AddObject( m_objectt, poss ); // add objects
			}
			// then add loot spots
		}

		JsonFileLoader< SceneInfo >.JsonSaveFile( SCENE_DATA + "\\" + sceneData.GetName() + ".json", sceneData );

		*/

		/*
		string toCopy = "";

		foreach( Object m_object : m_Objects )
		{
			if ( m_object.GetType() == "Pot" ) //save loot positions
			{
				vector modelPos = m_SelectedObject.WorldToModel( m_object.GetPosition() );

				toCopy = toCopy + "{" + modelPos[0] + "," + modelPos[1] + "," + modelPos[2] + "},";
			}
		}
		toCopy = m_SelectedObject.GetType() + "[] = {" + toCopy + "};";

		GetGame().CopyToClipboard( toCopy );
		m_Objects.Clear();
		*/


		ref SceneSaveST scene = new SceneSaveST();
		scene.name = "latest";

		foreach( Object m_object : m_Objects )
		{
			ref Param objectParam = new Param3<string, vector, vector>( m_object.GetType(), m_object.GetPosition(), m_object.GetOrientation() );
			scene.m_SceneObjects.Insert( objectParam );
		}

		Message( "Saved objects to latest.json" );
//		JsonFileLoader< SceneSaveST >.JsonSaveFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );
		JsonFileLoader< SceneSaveST >.JsonSaveFile( "$saves:COMObjectEditorSave.json", scene );

	}

	void LoadScene()
	{
		ref SceneSaveST scene = new SceneSaveST();

//		JsonFileLoader<SceneSaveST>.JsonLoadFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );
		JsonFileLoader<SceneSaveST>.JsonLoadFile( "$saves:COMObjectEditorSave.json", scene );

		foreach( auto param : scene.m_SceneObjects )
		{
			Object object = GetGame().CreateObject( param.param1, param.param2, false, false );
			object.SetOrientation( param.param3 );
			ForceTargetCollisionUpdate( object );

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

		if ( CTRL() ) 
		{
			vector modelPos = m_SelectedObject.WorldToModel( object.GetPosition() );

			ObjectInfoMenu.infoPosX.SetText( modelPos[0].ToString() );
			ObjectInfoMenu.infoPosY.SetText( modelPos[1].ToString() );
			ObjectInfoMenu.infoPosZ.SetText( modelPos[2].ToString() );

		}
		else 
		{
			ObjectInfoMenu.infoPosX.SetText( object.GetPosition()[0].ToString() );
			ObjectInfoMenu.infoPosY.SetText( object.GetPosition()[1].ToString() );
			ObjectInfoMenu.infoPosZ.SetText( object.GetPosition()[2].ToString() );

			ObjectInfoMenu.infoPosYaw.SetText( object.GetOrientation()[0].ToString() );
			ObjectInfoMenu.infoPosPitch.SetText( object.GetOrientation()[1].ToString() );
			ObjectInfoMenu.infoPosRoll.SetText( object.GetOrientation()[2].ToString() );
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

			if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, NULL, m_SelectedObject, GetPlayer(), false, false ) )
			{
				//vector oOrgPos = m_SelectedObject.GetPosition();
				//float fSurfaceHight = GetGame().SurfaceY( oOrgPos [ 0 ], oOrgPos [ 2 ] );

				//float nHightOffsetToGround = oOrgPos [ 1 ] - fSurfaceHight; // todo: store offset of existing objects for later use. for snap to ground perhaps?

				//contact_pos [ 1 ] = contact_pos [ 1 ] + nHightOffsetToGround;

				m_SelectedObject.SetPosition( contact_pos );
				m_SelectedObject.PlaceOnSurface();

				ForceTargetCollisionUpdate( m_SelectedObject );

				ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );
			}
		}
	}

//	void ScrollObject( int state )
//	{
//		if ( !m_ObjectEditorActive )
//		{
//			return;
//		}
//		/*
//
//		if ( m_SelectedObject )
//		{
//			vector pos = m_SelectedObject.GetPosition();
//			vector ori = m_SelectedObject.GetOrientation();
//
//			bool up = state < 0;
//			int value = 1;
//			if ( up ) value = -1;
//
//			if ( SHIFT() )
//			{
//				if ( ori[0] > 0 ) // seemless pitch change
//				{
//					value = -value;
//				}
//				ori[1] = ori[1] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( CTRL() )
//			{
//				ori [ 0 ] = ori [ 0 ] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( ALT() )
//			{
//				ori[ 2 ] = ori[ 2 ] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else
//			{
//				pos [ 1 ] = pos [ 1 ] + value*0.05;
//
//				m_SelectedObject.SetPosition( pos );
//			}
//		}
//		*/
//	}

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
		vector to = from + ( dir * 100 );

		set< Object > objects = GetObjectsAt(from, to, GetGame().GetPlayer(), 0.5 );
		bool selected = false;
		//Print ("Building Type = " + building.GetType());

		if ( objects ) 
		{
			for ( int newObject = 0; ( ( newObject < objects.Count() ) && !selected ); ++newObject )
			{
				Object obj = objects.Get( newObject );

				if ( CTRL() ) 
				{

					vector modelPos = obj.WorldToModel( m_SelectedObject.GetPosition() );

					ObjectInfoMenu.infoPosX.SetText( modelPos[0].ToString() );
					ObjectInfoMenu.infoPosY.SetText( modelPos[1].ToString() );
					ObjectInfoMenu.infoPosZ.SetText( modelPos[2].ToString() );
				}
				else 
				{
					SelectObject( obj );
					selected = true;

					ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
					ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
					ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );

					ObjectInfoMenu.infoPosYaw.SetText( m_SelectedObject.GetOrientation()[0].ToString() );
					ObjectInfoMenu.infoPosPitch.SetText( m_SelectedObject.GetOrientation()[1].ToString() );
					ObjectInfoMenu.infoPosRoll.SetText( m_SelectedObject.GetOrientation()[2].ToString() );
				}
			}
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
			m_Objects.RemoveItem(m_SelectedObject);
			m_SelectedObject.SetPosition(vector.Zero); // If object does not physically delete, teleport it to 0 0 0
			GetGame().ObjectDelete( m_SelectedObject );

			ObjectInfoMenu.UpdateObjectList();

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
			//SnapToGroundNew( m_SelectedObject );
			m_SelectedObject.PlaceOnSurface();
		}
	}
}
