class ClosableContainer: Container
{
	protected bool m_Closed;
	protected EntityAI m_Entity;

	void ClosableContainer( ContainerBase parent )
	{
		m_Body = new array<ref ContainerBase>;
		m_Body.Insert( new ClosableHeader( this, "CloseButtonOnMouseButtonDown" ) );
		
		m_MainPanel.GetScript( m_Spacer );
	}

	void Open()
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), true );
		m_Closed = false;
		OnShow();
		m_Parent.m_Parent.Refresh();
		( Container.Cast( m_Parent.m_Parent ) ).UpdateBodySpacers();
	}
	
	void SetOpenState( bool state )
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), state );
		m_Closed = !state;
		if( !m_Closed )
		{
			OnShow();
		}
		else
		{
			OnHide();
		}
		//m_Parent.m_Parent.Refresh();
		(Container.Cast( m_Parent.m_Parent ) ).UpdateBodySpacers();
	}

	void Close()
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), false );
		m_Closed = true;
		this.OnHide();
//m_Parent.m_Parent.Refresh();
		( Container.Cast( m_Parent.m_Parent ) ).UpdateBodySpacers();
	}

	bool IsOpened()
	{
		return !m_Closed;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	override void OnShow()
	{
		if( !m_Closed )
		{
			super.OnShow();
		}
	}

	override void Insert( ContainerBase container )
	{
		m_Body.Insert( container );
	}

	override ContainerBase Get( int x )
	{
		return m_Body.Get( x );
	}

	override void Refresh()
	{
		if( !m_Closed )
		{
			super.Refresh();
		}
	}

	void CloseButtonOnMouseButtonDown()
	{
		m_Closed = true;
		this.OnHide();
		m_Parent.m_Parent.Refresh();
	}
}
