class PlayerPreview: ContainerBase
{
	protected PlayerPreviewWidget m_CharacterPanelWidget;

	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;
	protected vector m_CharacterOrientation;
	
	void PlayerPreview( ContainerBase parent )
	{
		m_CharacterPanelWidget = PlayerPreviewWidget.Cast( m_Parent.GetMainPanel().FindAnyWidget( "CharacterPanelWidget" ) );

		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_Parent.GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "MouseButtonDown" );
		WidgetEventHandler.GetInstance().RegisterOnMouseWheel( m_Parent.GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "MouseWheel" );
		
		m_CharacterScaleDelta = 1;
		m_CharacterPanelWidget.SetPlayer( GetGame().GetPlayer() );
		m_CharacterPanelWidget.SetModelPosition( "0 -1 0.605" );
		m_CharacterPanelWidget.SetSize( 1.34, 1.34 );  // default scale
		UpdateScale();
	}
	
	void UpdateRotation( int mouse_x, int mouse_y, bool is_dragging )
	{
		vector orientation = m_CharacterOrientation;
		orientation[1] = orientation[1] - ( m_CharacterRotationX - mouse_x );
		m_CharacterPanelWidget.SetModelOrientation( orientation );

		if ( !is_dragging )
		{	
			m_CharacterOrientation = orientation;
		}
	}

	void UpdateScale()
	{
		float w, h;
		m_CharacterPanelWidget.GetSize( w, h );
		w = w + ( m_CharacterScaleDelta / 25 );
		h = h + ( m_CharacterScaleDelta / 25 );
		if ( w > 0.62 && w < 3 )
		{
			m_CharacterPanelWidget.SetSize( w, h );
		}
		else if ( w < 0.62 )
		{
			m_CharacterPanelWidget.SetSize( 0.62, 0.62 );
		}
		else if ( w > 3 )
		{
			m_CharacterPanelWidget.SetSize( 3, 3 );
		}
	}

	bool MouseButtonDown()
	{
		g_Game.GetMousePos( m_CharacterRotationX, m_CharacterRotationY );
		GetGame().GetDragQueue().Call( this, "UpdateRotation" );
		return true;
	}

	bool MouseWheel( int  x, int  y, int wheel )
	{
		m_CharacterScaleDelta = wheel;
		GetGame().GetDragQueue().Call( this, "UpdateScale" );
		return true;
	}	
}
