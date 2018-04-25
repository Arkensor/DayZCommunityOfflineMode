class WidgetEventHandler: ScriptedWidgetEventHandler
{
	private ref static WidgetEventHandler s_instance;

	protected ref map<Widget, ref Param> m_OnMouseButtonDownRegister;
	protected ref map<Widget, ref Param> m_OnMouseButtonUpRegister;
	protected ref map<Widget, ref Param> m_OnMouseWheel;
	protected ref map<Widget, ref Param> m_OnDropReceived;
	protected ref map<Widget, ref Param> m_OnDrag;
	protected ref map<Widget, ref Param> m_OnDrop;
	protected ref map<Widget, ref Param> m_OnDraggingOver;
	protected ref map<Widget, ref Param> m_OnMouseEnter;
	protected ref map<Widget, ref Param> m_OnMouseButtonLeave;
	protected ref map<Widget, ref Param> m_OnDoubleClick;
	protected ref map<Widget, ref Param> m_OnController;

	static WidgetEventHandler GetInstance()
	{
		return s_instance;
	}

	void WidgetEventHandler()
	{
		s_instance = this;
		m_OnMouseButtonDownRegister = new map<Widget, ref Param>;
		m_OnMouseButtonUpRegister = new map<Widget, ref Param>;
		m_OnMouseWheel = new map<Widget, ref Param>;
		m_OnDropReceived = new map<Widget, ref Param>;
		m_OnDrag = new map<Widget, ref Param>;
		m_OnDrop = new map<Widget, ref Param>;
		m_OnDraggingOver = new map<Widget, ref Param>;
		m_OnMouseEnter = new map<Widget, ref Param>;
		m_OnMouseButtonLeave = new map<Widget, ref Param>;
		m_OnDoubleClick = new map<Widget, ref Param>;
		m_OnController = new map<Widget, ref Param>;
	}
	
	void RegisterOnMouseButtonDown( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnMouseButtonDownRegister.Insert( w, param );
	}

	void RegisterOnMouseButtonUp( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnMouseButtonUpRegister.Insert( w, param );
	}

	void RegisterOnMouseWheel( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnMouseWheel.Insert( w, param );
	}
	
	void RegisterOnDropReceived( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnDropReceived.Insert( w, param );
	}
	
	void RegisterOnDrag( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnDrag.Insert( w, param );
	}
	
	void RegisterOnDrop( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnDrop.Insert( w, param );
	}
	
	void RegisterOnDraggingOver( Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnDraggingOver.Insert( w, param );
	}
	
	void RegisterOnMouseEnter(Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnMouseEnter.Insert( w, param );
	}
	
	void RegisterOnMouseLeave(Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnMouseButtonLeave.Insert( w, param );
	}

	void RegisterOnDoubleClick(Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnDoubleClick.Insert( w, param );
	}
	
	void RegisterOnController(Widget w, ContainerBase eventHandler, string functionName )
	{
		w.SetHandler( this );
		SetFocus( w );
		Param param = new Param2<ContainerBase, string>( eventHandler, functionName );
		m_OnController.Insert( w, param );
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnDoubleClick.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, int, int, int>( w, x, y, button );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnController( Widget w, int control, int value )
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnController.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param3<Widget, int, int>( w, control, value );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnMouseButtonLeave.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, Widget, int, int>( w, enterW, x, y );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnMouseEnter.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param3<Widget, int, int>( w, x, y );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}

	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnMouseButtonDownRegister.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, int, int, int>( w, x, y, button );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnMouseButtonUpRegister.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, int, int, int>( w, x, y, button );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnDrag.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param3<Widget, int, int>( w, x, y );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnDrop.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param3<Widget, int, int>( w, x, y );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnDraggingOver.Get( reciever ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, int, int, Widget>( w, x, y, reciever );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
	
	override bool OnDropReceived( Widget w, int x, int y, Widget reciever )
	{
		if( w.GetName() == "GridItem" )
		{
			return false;
		}
		Param p = m_OnDropReceived.Get( reciever );
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( p );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param4<Widget, int, int, Widget>( w, x, y, reciever );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}

	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		Param2<ContainerBase, string> param = Param2<ContainerBase, string>.Cast( m_OnMouseWheel.Get( w ) );
		if( param == NULL )
		{
			return false;
		}

		Param param2 = new Param3<int, int, int>( x, y, wheel );
		GetGame().GameScript.CallFunctionParams( param.param1, param.param2, NULL, param2 );

		return true;
	}
}
