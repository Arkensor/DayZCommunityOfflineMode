
// Changed m_SceneObjects to Objects to match vanilla objectSpawnerArr json format.
// Saving and loading object scale.
// Added ScaleObject function. (Objects cannot be moved or orientated after scaling, objects must be recreated as they are moved, if scaled)
// Checking ObjectInfoMenu.editingPos before updating ObjectInfoMenu positions on SelectObject.
// - Brandon10x15

class ObjectEditor extends Module
{
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	Object m_SelectedObject;
	Object building;

	// protected ref Scene active_Scene;
	autoptr array<Object> m_Objects = new array<Object>;

	string BASE_SCENE_DIR = BASE_COM_DIR + "Scenes";

	ref SceneManager sceneManager;
	SceneData currentSceneData;

	void ObjectEditor()
	{
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
        toCopy += "    auto obj = GetGame().CreateObject_WIP( type, position, ECE_CREATEPHYSICS );\n";
        toCopy += "    obj.SetFlags( EntityFlags.STATIC, false );\n";
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
			toCopy = toCopy + "SpawnObject( \"" + m_object.GetType() + "\", \"" + COM_VectorToString( m_object.GetPosition() ) + "\", \"" + COM_VectorToString( m_object.GetOrientation() ) + "\" );\n";
		}

		GetGame().CopyToClipboard( toCopy );

		COM_Message( "Copied to clipboard" );
	}

	void SaveScene()
	{
		MakeDirectory("$mission:custom\\");
		ref ObjectSave scene = new ObjectSave();
		ref array<ref ObjectSave> scenes = new ref array<ref ObjectSave>;
		int fileNum = 0;
		TStringArray objSpawnerArr = new TStringArray;
		TStringArray objSpawnerArrRemove = new TStringArray;
		if(m_Objects.Count() > 0)
		{
			foreach(auto obj : m_Objects) {
				scene.Objects.Insert(new ObjectData(obj.GetType(), obj.GetPosition(), obj.GetOrientation(), obj.GetScale()));
				if (scene.Objects.Count() == 150)
				{
					scenes.Insert(scene);
					scene = new ObjectSave();
				}
			}
			scenes.Insert(scene);
			foreach(auto sc : scenes)  {
				string sceneFilename = "$mission:custom\\" + objectsFilename + fileNum.ToString() + ".json";
				JsonFileLoader<ObjectSave>.JsonSaveFile(sceneFilename, sc);
				string objSpawnerName = "custom/" + objectsFilename + fileNum.ToString() + ".json";
				objSpawnerArr.Insert( objSpawnerName );
				fileNum += 1;
			}
		}
		for (int i = fileNum; i < 100; i++) {
			string oldFilename = "$mission:custom\\" + objectsFilename + i.ToString() + ".json";
			string oldObjSpawnerName = "custom/" + objectsFilename + i.ToString() + ".json";
			objSpawnerArrRemove.Insert( oldObjSpawnerName );
			if(FileExist(oldFilename)) { DeleteFile(oldFilename); }
		}
		COM_Message("Saved objects to " + "$mission:custom\\" + objectsFilename);
		UpdateObjectSpawnerArr(objSpawnerArr, objSpawnerArrRemove);
	}
	void UpdateObjectSpawnerArr(TStringArray newObjectSpawnerArr, TStringArray objSpawnerArrRemove)
	{
		CfgGameplayHandler.m_Data = new CfgGameplayJson;
	    JsonFileLoader<CfgGameplayJson>.JsonLoadFile( "$mission:cfgGameplay.json", CfgGameplayHandler.m_Data );

	    TStringArray objectSpawnerArr = CfgGameplayHandler.m_Data.WorldsData.objectSpawnersArr;

	    if(newObjectSpawnerArr.Count() > 0) {
			foreach(auto objFilename: newObjectSpawnerArr)
			{
				if(objectSpawnerArr.Find( objFilename ) == -1)
				{
					CfgGameplayHandler.m_Data.WorldsData.objectSpawnersArr.Insert( objFilename );
				}
			}
		}

	    if(objSpawnerArrRemove.Count() > 0) {
			foreach(auto objFilenameRemove : objSpawnerArrRemove)
			{
				if(objectSpawnerArr.Find( objFilenameRemove ) != -1)
				{
					CfgGameplayHandler.m_Data.WorldsData.objectSpawnersArr.RemoveItem( objFilenameRemove );
				}
			}
		}
		JsonFileLoader<CfgGameplayJson>.JsonSaveFile( "$mission:cfgGameplay.json", CfgGameplayHandler.m_Data );

	}

	void LoadScene()
	{
		// Delete all existing objects (if any)
		foreach(auto obj : m_Objects)
		{
			obj.SetPosition(vector.Zero);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, obj);
		}
     	m_Objects.Clear();

     	// Reload cfgGameplay.json in-case of any manual changes.
		CfgGameplayHandler.m_Data = new CfgGameplayJson;
	    JsonFileLoader<CfgGameplayJson>.JsonLoadFile( "$mission:cfgGameplay.json", CfgGameplayHandler.m_Data );
	    TStringArray objectSpawnerArr = CfgGameplayHandler.m_Data.WorldsData.objectSpawnersArr;

	    if(objectSpawnerArr.Count() > 0)
	    {
	        foreach(auto objectFilename: objectSpawnerArr)
    		{
    			string objectPath = "$mission:" + objectFilename;
    			if ( FileExist( objectPath ) )
    			{
    				ObjectSave fileData;
    				JsonFileLoader<ObjectSave>.JsonLoadFile( objectPath, fileData );

    				foreach (auto data : fileData.Objects)
    				{
						Object object = GetGame().CreateObject( data.name, data.pos, false, false );
						object.SetOrientation( data.ypr );
						COM_ForceTargetCollisionUpdate( object );
						object.SetScale( data.scale );
						m_Objects.Insert(object);
    				}
					Print("All objects loaded from save file: " + objectPath);
    			}
    		}
    	}
	}

	void ScaleObject(Object obj, float scale) { // NEEDS WORK
		if(scale <= 0)
		{
			return;
		}
		bool select = false;
		if(m_SelectedObject == obj)
		{
			select = true;
		}
		//COM_Message("Setting scale to: " + scale.ToString());
		Object object = GetGame().CreateObjectEx(obj.GetType(), obj.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
		if ( object )
		{
			object.SetOrientation( obj.GetOrientation() );
			object.SetScale(scale);
			m_Objects.Insert( object );
			m_Objects.RemoveItem( obj );
			ObjectInfoMenu.UpdateObjectList();
			if(select)
			{
				SelectObject(object);
			}
			obj.SetPosition( vector.Zero );
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( GetGame().ObjectDelete, 100, false, obj );
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
			COM_GetPB().MessageStatus("Object Editor Enabled");
		}
		else
		{
			COM_GetPB().MessageStatus("Object Editor Disabled");
		}
	}

	void ToggleEditor()
	{
		m_ObjectEditorActive = !m_ObjectEditorActive;

		if ( m_ObjectEditorActive )
		{
			COM_GetPB().MessageStatus("Object Editor Enabled");
		}
		else
		{
			COM_GetPB().MessageStatus("Object Editor Disabled");
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

		if( !ObjectInfoMenu.editingPos ) {
			if ( COM_CTRL() )
			{
				vector modelPos = m_SelectedObject.WorldToModel( object.GetPosition() );

				ObjectInfoMenu.infoPosX.SetText( modelPos[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( modelPos[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( modelPos[2].ToString() );
				ObjectInfoMenu.infoPosYaw.SetText( "0.0" );
				ObjectInfoMenu.infoPosPitch.SetText( "0.0" );
				ObjectInfoMenu.infoPosRoll.SetText( "0.0" );
				ObjectInfoMenu.infoPosScale.SetText( "0.0" );

			}
			else
			{
				ObjectInfoMenu.infoPosX.SetText( object.GetPosition()[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( object.GetPosition()[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( object.GetPosition()[2].ToString() );

				ObjectInfoMenu.infoPosYaw.SetText( object.GetOrientation()[0].ToString() );
				ObjectInfoMenu.infoPosPitch.SetText( object.GetOrientation()[1].ToString() );
				ObjectInfoMenu.infoPosRoll.SetText( object.GetOrientation()[2].ToString() );

				ObjectInfoMenu.infoPosScale.SetText( object.GetScale().ToString() );
			}
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

			if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, NULL, m_SelectedObject, COM_GetPB(), false, false ) )
			{
				//vector oOrgPos = m_SelectedObject.GetPosition();
				//float fSurfaceHight = GetGame().SurfaceY( oOrgPos [ 0 ], oOrgPos [ 2 ] );

				//float nHightOffsetToGround = oOrgPos [ 1 ] - fSurfaceHight; // todo: store offset of existing objects for later use. for snap to ground perhaps?

				//contact_pos [ 1 ] = contact_pos [ 1 ] + nHightOffsetToGround;

				float oldScale = m_SelectedObject.GetScale();

				m_SelectedObject.SetPosition( contact_pos );
				m_SelectedObject.PlaceOnSurface();

				COM_ForceTargetCollisionUpdate( m_SelectedObject );

				ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );

				if(oldScale.ToString() != "1")
				{
					ScaleObject(m_SelectedObject, oldScale);
				}
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
//			if ( COM_SHIFT() )
//			{
//				if ( ori[0] > 0 ) // seemless pitch change
//				{
//					value = -value;
//				}
//				ori[1] = ori[1] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( COM_CTRL() )
//			{
//				ori [ 0 ] = ori [ 0 ] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( COM_ALT() )
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

		set< Object > objects = COM_GetObjectsAt(from, to, COM_GetPB(), 0.5 );
		bool selected = false;
		//Print ("Building Type = " + building.GetType());

		if ( objects ) 
		{
			for ( int newObject = 0; ( ( newObject < objects.Count() ) && !selected ); ++newObject )
			{
				Object obj = objects.Get( newObject );

				if ( COM_CTRL() ) 
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

					ObjectInfoMenu.infoPosScale.SetText( m_SelectedObject.GetScale().ToString() );
				}
			}
		}

		if ( !selected && m_SelectedObject )
		{
			COM_GetPB().MessageStatus("Current object deselected.");
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
			//COM_SnapToGroundNew( m_SelectedObject );

			float oldScale = m_SelectedObject.GetScale();
			m_SelectedObject.PlaceOnSurface();

			if(oldScale.ToString() != "1")
			{
				ScaleObject(m_SelectedObject, oldScale);
			}
		}
	}
}
