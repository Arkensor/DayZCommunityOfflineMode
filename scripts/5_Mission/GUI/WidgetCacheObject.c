class WidgetCacheObject
{
	ref Widget m_widget;
	AutoHeightSpacer spacer;
	string m_name;
	bool m_visible, m_enabled;
	int m_flags, m_sort, m_color;
	float m_x, m_y, m_width, m_height, m_screen_x, m_screen_y, m_screen_width, m_screen_height;
	float m_alpha;

	ref map<string, ref WidgetCacheObject> m_widgets_cache;
	ref array<ref WidgetCacheObject> m_registred_children;

	void WidgetCacheObject( Widget w )
	{
		m_widgets_cache = new map<string, ref WidgetCacheObject>;
		m_registred_children = new array<ref WidgetCacheObject>;
		m_widget = w;
	}

	void ~WidgetCacheObject()
	{
		//Print("was here");
	}

	WidgetCacheObject FindAnyWidget( string widget_name )
	{
		WidgetCacheObject wco = m_widgets_cache.Get( widget_name );

		if( wco == NULL )
		{
			Widget temp = m_widget.FindAnyWidget( widget_name );
			wco = new WidgetCacheObject( temp );
			wco.SaveOriginalValeus();
			m_widgets_cache.Insert( widget_name, wco );
		}

		return wco;
	}

	void RegisterOnMouseButtonDown( ContainerBase w, string function_name )
	{
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_widget, w, function_name );
	}

	Widget GetWidget()
	{
		return m_widget;
	}
	
	void SetParent( Widget parent )
	{
		parent.AddChild( m_widget );
	}

	void AddChild( WidgetCacheObject wco )
	{
		m_registred_children.Insert( wco );
		m_widget.AddChild( wco.GetWidget() );
	}

	void Update()
	{
		m_widget.Update();
	}

	Widget GetParent()
	{
		return m_widget.GetParent();
	}

	string GetName()
	{
		return m_widget.GetName();
	}

	void SetName( string name )
	{
		m_widget.SetName( name );
	}

	void Show( bool show )
	{
		m_widget.Show( show );
	}

	void Enable( bool enable )
	{
		m_widget.Enable( enable );
	}

	int GetFlags()
	{
		return m_widget.GetFlags();
	}

	int SetFlags( int flags, bool immedUpdate = true )
	{
		return m_widget.SetFlags( flags, immedUpdate );
	}

	int GetSort()
	{
		return m_widget.GetSort();
	}

	void SetSort( int sort, bool immedUpdate = true )
	{
		m_widget.SetSort( sort, immedUpdate );
	}

	int ClearFlags( int flags, bool immedUpdate = true )
	{
		return m_widget.ClearFlags( flags, immedUpdate );
	}

	bool IsVisible()
	{
		return m_widget.IsVisible();
	}

	void SetPos( float x, float y, bool immedUpdate = true )
	{
		m_widget.SetPos( x, y, immedUpdate );
	}

	void SetSize( float x, float y, bool immedUpdate = true )
	{
		m_widget.SetSize( x, y, immedUpdate );
	}

	void SetColor( int color )
	{
		m_widget.SetColor( color );
	}

	int GetColor()
	{
		return m_widget.GetColor();
	}

	void SetRotation( float roll, float pitch, float yaw, bool immedUpdate = true )
	{
		m_widget.SetRotation( roll, pitch, yaw, immedUpdate );
	}

	void SetAlpha( float alpha )
	{
		m_widget.SetAlpha( alpha );
	}

	float GetAlpha()
	{
		return m_widget.GetAlpha();
	}

	void GetPos( out float x, out float y )
	{
		m_widget.GetPos( x, y);
	}

	void GetSize( out float width, out float height )
	{
		m_widget.GetSize( width, height);
	}

	void GetScreenPos( out float x, out float y )
	{
		m_widget.GetScreenPos( x, y );
	}

	void GetScreenSize( out float width, out float height )
	{
		m_widget.GetScreenSize( width, height );
	}

	AutoHeightSpacer GetScript()
	{
		m_widget.GetScript(spacer);

		return spacer;
	}

	void SaveOriginalValeus()
	{
		m_name = m_widget.GetName();
		m_visible = m_widget.IsVisible();
		m_flags = m_widget.GetFlags();
		m_sort = m_widget.GetSort();
		m_color = m_widget.GetColor();
		m_widget.GetPos( m_x, m_y );
		m_widget.GetSize( m_width, m_height );
		m_alpha = m_widget.GetAlpha();
	}

	void RestoreOriginalValeus()
	{
		m_widget.SetName( m_name );
		m_widget.Show( m_visible );
		m_widget.SetFlags( m_flags );
		m_widget.SetSort( m_sort );
		m_widget.SetColor( m_color );
		m_widget.SetPos( m_x, m_y );
		m_widget.SetSize( m_width, m_height );
		m_widget.SetAlpha( m_alpha );
		m_widget.SetRotation( 0, 0, 0 );

		if( m_widgets_cache.Count() > 0 )
		{
			foreach ( WidgetCacheObject wco: m_widgets_cache )
			{
				wco.RestoreOriginalValeus();
			}
		}

		for( int i = 0; i < m_registred_children.Count(); i++ )
		{
			WidgetCacheObject w = m_registred_children.Get( i );
			m_widget.RemoveChild( w.GetWidget() );
		}
	}
}
