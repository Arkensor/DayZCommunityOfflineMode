class ObjectEditor
{
	protected CommunityOfflineMode m_OfflineMission;
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	protected Object m_SelectedObject;


	void ObjectEditor( CommunityOfflineMode mission )
	{
		m_OfflineMission = mission;
	}


	void ~ObjectEditor()
	{
	}


	void ToggleEditor( bool enable )
	{
		m_ObjectEditorActive = enable;
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


	void onMouseDrag() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			vector dir = GetGame().GetPointerDirection();

			vector from = GetCameraPosition();

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


	void onMouseScrollUp()
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{			
			if ( m_OfflineMission.SHIFT() )
			{
				vector pitch = m_SelectedObject.GetOrientation();

				if ( pitch [ 2 ] < 0 )
				{
					pitch [ 1 ] = pitch [ 1 ] + 1;
				}
				else
				{
					pitch [ 1 ] = pitch [ 1 ] - 1;
				}
			
				m_SelectedObject.SetOrientation(pitch);

			} 
			else if ( m_OfflineMission.CTRL() )
			{
				vector yaw = m_SelectedObject.GetOrientation();

				yaw [ 0 ] = yaw [ 0 ] + 1;

				m_SelectedObject.SetOrientation( yaw );
			}
			else if ( m_OfflineMission.ALT() )
			{
				vector roll = m_SelectedObject.GetOrientation();

				roll[ 2 ] = roll[ 2 ] + 1;

				m_SelectedObject.SetOrientation( roll );
			}
			else 
			{
				vector pos = m_SelectedObject.GetPosition();

				pos [ 1 ] = pos [ 1 ] + 0.05;

				m_SelectedObject.SetPosition( pos );
			}
		}
	}


	void onMouseScrollDown() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			if ( m_OfflineMission.SHIFT() )
			{
				vector pitch = m_SelectedObject.GetOrientation();

				if ( pitch [ 2 ] < 0 )
				{
					pitch [ 1 ] = pitch [ 1 ] - 1;
				}
				else
				{
					pitch [ 1 ] = pitch [ 1 ] + 1;
				}

				m_SelectedObject.SetOrientation(pitch);
			} 
			else if ( m_OfflineMission.CTRL() )
			{
				vector orientation = m_SelectedObject.GetOrientation();

				orientation[ 0 ] = orientation[ 0 ] - 1;

				m_SelectedObject.SetOrientation( orientation );
			}
			else if ( m_OfflineMission.ALT() )
			{
				vector roll = m_SelectedObject.GetOrientation();

				roll [ 2 ] = roll [ 2 ] - 1;

				m_SelectedObject.SetOrientation( roll );
			}
			else 
			{
				vector pos = m_SelectedObject.GetPosition();

				pos [ 1 ] = pos [ 1 ] - 0.05;

				m_SelectedObject.SetPosition( pos );
			}
		}
	}


	void onMouseClick() 
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		vector dir = GetGame().GetPointerDirection();
		vector from = GetCameraPosition();
		vector to = from + ( dir * 10000 );

		set< Object > objects = GetObjectsAtCursor(from, to);

		bool selected = false;
		
		for ( int nObject = 0; ( ( nObject < objects.Count() ) && !selected ); ++nObject )
		{
			Object obj = objects.Get( nObject );
			SelectObject( obj );
			selected = true;	
		}
		
		if ( !selected )
		{
			DeselectObject();
		}
	}


	void onMouseDoubleClick() 
	{
        //Todo
	}


	private set< Object > GetObjectsAtCursor( vector from, vector to )
	{
		vector contact_pos;
		vector contact_dir;
		int contact_component;

		set< Object > objects = new set< Object >;
		
		if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, objects, NULL, GetGame().GetPlayer(), false, false, ObjIntersectView, 0.5 ) )
		{
			return objects;
		}

		return NULL;
	}


	private vector GetCameraPosition() 
	{
		return GetGame().GetCurrentCameraPosition();
	}
}