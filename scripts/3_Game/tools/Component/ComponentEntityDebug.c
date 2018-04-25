class ComponentEntityDebug extends Component
{
	
	protected Shape				m_DebugShapeBBox;
	protected Shape				m_DebugShapeDirection;
	protected float				m_DebugShapeDirectionDist;

	// -------------------------------------------------------------------------
	override Shape DebugBBoxDraw()
	{
		if ( !GetGame().IsDebug() )
			return NULL;
		
		if ( m_DebugShapeBBox != NULL )
			m_DebugShapeBBox.Destroy();
		
		
		vector min_max[2];
		
		if(!m_ThisEntityAI.GetCollisionBox(min_max))
			return NULL;
		
		m_DebugShapeBBox = Debug.DrawBox(min_max[0], min_max[1]);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.OnDrawing, 0, true);
		
		m_ThisEntityAI.SetEventMask(EntityEvent.FRAME);

		return NULL;
	}

	// -------------------------------------------------------------------------
	override void DebugBBoxSetColor(int color)
	{
		if ( m_DebugShapeBBox )
		{
			m_DebugShapeBBox.SetColor(color);
		}
	}

	// -------------------------------------------------------------------------
	override void DebugBBoxDelete()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeBBox != NULL )
		{
			m_DebugShapeBBox.Destroy();
			m_DebugShapeBBox = NULL;
		}
		
		if ( m_DebugShapeDirection == NULL && m_DebugShapeBBox == NULL )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.OnDrawing);
		}
	}

	// -------------------------------------------------------------------------
	override Shape DebugDirectionDraw(float distance = 1)
	{	
		if ( !GetGame().IsDebug() )
		{
			return NULL;
		}
		
		if ( m_DebugShapeDirection != NULL )
		{
			m_DebugShapeDirection.Destroy();
		}
		
		vector p1 = "0 0 0";
		vector p2 = m_ThisEntityAI.GetDirection() * m_DebugShapeDirectionDist;
		m_DebugShapeDirectionDist = distance;
		m_DebugShapeDirection = Debug.DrawArrow(p1, p2);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.OnDrawing, 0, true);
		
		m_ThisEntityAI.SetEventMask(EntityEvent.FRAME);
		return NULL;
	}

	// -------------------------------------------------------------------------
	override void DebugDirectionDelete()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeDirection != NULL )
		{
			m_DebugShapeDirection.Destroy();
			m_DebugShapeDirection = NULL;
		}
		
		if ( m_DebugShapeDirection == NULL && m_DebugShapeBBox == NULL )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.OnDrawing);
		}
	}

	void OnDrawing()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeBBox != NULL || m_DebugShapeDirection != NULL )
		{		
			vector mat[4];
			m_ThisEntityAI.GetTransform(mat);
		
			if ( m_DebugShapeBBox != NULL )
			{
				m_DebugShapeBBox.SetMatrix(mat);
			}
			
			if ( m_DebugShapeDirection != NULL )
			{
				m_DebugShapeDirection.SetMatrix(mat);			
			}
		}
	}
}