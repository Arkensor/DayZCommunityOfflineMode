class CollapsibleContainer: Container
{
	protected ref CollapsibleHeader	m_CollapsibleHeader;
	protected AutoHeightSpacer		m_RootSpacer;
	protected bool m_Hidden;

	void CollapsibleContainer( ContainerBase parent )
	{
		m_MainPanel.Show( true );

		m_Body = new array<ref ContainerBase>;
		m_CollapsibleHeader = new CollapsibleHeader( this, "CollapseButtonOnMouseButtonDown" );
		m_Body.Insert( m_CollapsibleHeader );
		m_OpenedContainers.Insert( m_CollapsibleHeader );
		m_MainPanel = m_MainPanel.FindAnyWidget( "body" );

		m_MainPanel.GetScript( m_Spacer );
		m_MainPanel.GetParent().GetScript( m_RootSpacer );
		UpdateSpacer();
	}
	
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.CollapsibleContainer;
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver ) { }

	override void Refresh()
	{
		super.Refresh();
		m_RootSpacer.Update();
	}

	override void OnShow()
	{
		super.OnShow();
		m_Hidden = false;
		for ( int i = 1; i < m_Body.Count(); i++ )
		{
			if( !m_Hidden )
			{
				m_Body.Get( i ).OnShow();
			}
		}
	}
	
	override void Insert( ContainerBase container )
	{
		super.Insert( container );
		RecomputeOpenedContainers();
		if( m_Body.Count() < 3 )
		{
			m_CollapsibleHeader.GetMainPanel().FindAnyWidget("collapse_button").Show(false);
		}
		else
		{
			m_CollapsibleHeader.GetMainPanel().FindAnyWidget("collapse_button").Show(true);
		}
	}
	
	override void MoveGridCursor( int direction )
	{
		m_FocusedContainer.MoveGridCursor( direction );
	}

	override void UpdateSpacer()
	{
		super.UpdateSpacer();
		m_RootSpacer.Update();
		if( m_Body.Count() < 3 )
		{
			m_CollapsibleHeader.GetMainPanel().FindAnyWidget("collapse_button").Show(false);
		}
		else
		{
			m_CollapsibleHeader.GetMainPanel().FindAnyWidget("collapse_button").Show(true);
		}
	}
	
	void LoadDefaultState()
	{
		m_Hidden = !ItemManager.GetInstance().GetDefaultHeaderOpenState( "VICINITY" );
		
		if( m_Hidden )
		{
			OnHide();
			Print(GetMainPanel().GetName());
			/*for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}*/
		}
		else
		{
			OnShow();
			/*for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}*/
		}

		//GetMainPanel().FindAnyWidget("opened").Show(!m_Hidden);
		//GetMainPanel().FindAnyWidget("closed").Show(m_Hidden);
	}
	
	override bool IsActive()
	{
		for ( int i = 1; i < m_Body.Count(); i++ )
		{
			Container cont = Container.Cast( m_Body[i] );
			if( cont.IsActive() )
			{
				return true;
			}
		}
		return false;
	}
	
	void SetFirstActive()
	{
		Container cont = Container.Cast( m_Body[1] );
		cont.SetActive( true );
		m_FocusedContainer = cont;
		activeIndex = 1;
	}
	
	void SetLastActive()
	{
		Container cont = Container.Cast( m_Body[Count() - 1] );
		cont.SetActive( true );
		m_FocusedContainer = cont;
		activeIndex = Count() - 1;
	}
	
	override void SetActive( bool active )
	{
		if( active )
		{
			/*Container cont = Container.Cast( m_Body[1] );
			cont.SetActive( true );
			m_FocusedContainer = cont;
			activeIndex = 1;*/
			lastIndex = Count() == 2;
		}
		else
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				Container cont = Container.Cast( m_Body[i] );
				if( cont.IsActive() )
				{
					cont.SetActive( false );
					activeIndex = 1;
				}
			}
		}
	}
	
	void CollapseButtonOnMouseButtonDown( Widget w )
	{
		if( !m_Hidden )
		{
			//OnHide();
			for (int i = 2; i < m_Body.Count(); i++)
			{
				m_Body.Get( i ).OnHide();
			}

			m_Hidden = true;
		}
		else
		{
			m_Hidden = false;
			OnShow();
		}
		
		w.FindAnyWidget("opened").Show(!m_Hidden);
		w.FindAnyWidget("closed").Show(m_Hidden);

		this.UpdateSpacer();
		( Container.Cast( m_Parent ) ).UpdateSpacer();
	}
}
