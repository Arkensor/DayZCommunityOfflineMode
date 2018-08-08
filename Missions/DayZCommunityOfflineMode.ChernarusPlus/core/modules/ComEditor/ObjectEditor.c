class ObjectEditor extends Module
{
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	protected Object m_SelectedObject;

	void ObjectEditor()
	{
	}

	void ~ObjectEditor()
	{
	}
	
	override void Init() 
	{
		super.Init();
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
		KeyMouseBinding objectSelect  = new KeyMouseBinding( GetModuleType(), "ClickObject"  , "(LMB)+(Click)" , "Selects object on cursor." );
		KeyMouseBinding objectDrag    = new KeyMouseBinding( GetModuleType(), "DragObject"   , "(LMB)+(Drag)"  , "Drag objects on cursor."   );
		KeyMouseBinding objectScroll  = new KeyMouseBinding( GetModuleType(), "ScrollObject" , "[Shift][Ctrl][Alt]+(Wheel)" , "Raise or lower objects with mouse wheel as well as rotate." );
		KeyMouseBinding objectDelete  = new KeyMouseBinding( GetModuleType(), "DeleteObject" , "[Delete]"	   , "Deletes selected object."  );
		KeyMouseBinding objectGround  = new KeyMouseBinding( GetModuleType(), "GroundObject" , "(Middle Mouse)", "Snaps objects to ground."  );

		toggleEditor.AddKeyBind( KeyCode.KC_LSHIFT, KeyMouseBinding.KB_EVENT_HOLD    ); 
		toggleEditor.AddKeyBind( KeyCode.KC_END   , KeyMouseBinding.KB_EVENT_RELEASE ); // Press END. Using Release prevents key HOLD spam from onKeyPress (could use ClearKey in onKeyPress however)
		objectDelete.AddKeyBind( KeyCode.KC_DELETE, KeyMouseBinding.KB_EVENT_RELEASE ); // Pretty much making KB_EVENT_PRESS useless since you can just use KB_EVENT_HOLD instead.
		
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
		return ( m_ObjectEditorActive && ( GetGame().GetUIManager().GetMenu() == NULL ) );
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
			}
		}
	}

	void ScrollObject( int state ) 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}
		
		if ( m_SelectedObject )
		{
			vector pos = m_SelectedObject.GetPosition();
			vector yaw = m_SelectedObject.GetOrientation();
			vector pitch = m_SelectedObject.GetOrientation();
			vector roll = m_SelectedObject.GetOrientation();
			
			bool up = state < 0;
			int value = 1;
			if ( up ) value = -1;
			
			if ( SHIFT() )
			{
				pitch [ 1 ] = pitch [ 1 ] + value;
				m_SelectedObject.SetOrientation(pitch);
			}
			else if ( CTRL() )
			{
				yaw [ 0 ] = yaw [ 0 ] + value;

				m_SelectedObject.SetOrientation( yaw );
			}
			else if ( ALT() )
			{
				roll[ 2 ] = roll[ 2 ] + value;

				m_SelectedObject.SetOrientation( roll );
			}
			else 
			{
				pos [ 1 ] = pos [ 1 ] + value*0.05;

				m_SelectedObject.SetPosition( pos );
			}
		}
	}

	void ClickObject() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}
	
		vector dir = GetGame().GetPointerDirection();
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + ( dir * 10000 );

		set< Object > objects = GetObjectsAtCursor(from, to, GetGame().GetPlayer() );

		bool selected = false;
		
		for ( int nObject = 0; ( ( nObject < objects.Count() ) && !selected ); ++nObject )
		{
			Object obj = objects.Get( nObject );
			SelectObject( obj );
			selected = true;
			
			GetPlayer().MessageStatus("Selected object.");
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
			vector pos = m_SelectedObject.GetPosition();
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			
			vector clippingInfo;
			vector objectBBOX;
			
			m_SelectedObject.GetCollisionBox(objectBBOX);
			
			pos[1] = pos[1] - objectBBOX[1] + clippingInfo[1];
			
			m_SelectedObject.SetPosition(pos);
		}
	}
}