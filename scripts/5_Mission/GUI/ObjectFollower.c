class ObjectFollower extends ScriptedWidgetEventHandler
{
	reference int						m_PivotYOffset;

	protected Widget					m_Root;
	protected Object					m_TargetObject;
	protected bool 						m_Visible;

	void ObjectFollower()
	{
		m_Visible = false;
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~ObjectFollower()
	{
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}

	protected void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
	}

	void SetTarget(Object obj)
	{
		m_TargetObject = obj;
	}

	override bool OnUpdate(Widget w)
	{
		if ( m_Root == w )
		{
			Update();
			return true;
		}
		
		return false;
	}
	
	void Update()
	{
		float x, y;
		
		if(m_Visible)
		{
			if(m_TargetObject)
			{
				GetOnScreenPosition(x, y);
			}
			
			m_Root.Show(true);
		}
		else
		{
			m_Root.Show(false);		
		}
	}
	
	protected void GetOnScreenPosition(out float x, out float y)
	{
		const float 							DEFAULT_HANDLE_OFFSET 	= 0.2;

		float pivotYOffset = 0.0;
		
		bool ignorePivot	= false;
		bool traceMatched	= false;
		bool isDefaultPos   = false;
		
		vector objectPos;

		if(m_TargetObject)
		{
			pivotYOffset	= m_PivotYOffset;

			traceMatched = true;
			objectPos = m_TargetObject.GetPosition();
			if ( pivotYOffset != 0.0 )
				{ objectPos[1] = objectPos[1] + pivotYOffset; }
			else
				{ objectPos[1] = objectPos[1] + DEFAULT_HANDLE_OFFSET; }
		}
		else
		{
			// default position for the action cursor
			traceMatched = false;
			isDefaultPos = true;
		}

		if ( ignorePivot && !traceMatched )
		{
			// hides the widget if we are in situation that the widget will be displayed on pivot point 
			// and we do not want to display it
			x = 0.0;
			y = 0.0;
		}
		else
		{	
			// calculate screen position from traced results
			if( isDefaultPos )
			{
				int w,h;
				GetScreenSize(w, h);
				x = w/2;
				y = h/2;
			}
			else
			{
				vector screen_pos = GetGame().GetScreenPos( objectPos );
				x = screen_pos[0];
				y = screen_pos[1];
			}
		}
		
		objectPos = "0 0 0";
		isDefaultPos = false;
	}
}