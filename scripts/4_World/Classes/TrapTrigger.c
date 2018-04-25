class TrapTrigger extends Trigger
{
	TrapBase m_ParentObj;

	override void OnEnter( Object obj )
	{
		if ( g_Game.IsServer()  &&  obj.IsInherited(EntityAI))
		{
			if ( m_ParentObj && m_ParentObj.IsActive() == true ) 
			{
				EntityAI victim = EntityAI.Cast( obj );
				m_ParentObj.SnapOnObject(victim);
			}
		}
	}
	
	void SetParentObject( TrapBase obj )
	{
		if ( g_Game.IsServer() )
		{
			m_ParentObj = obj;
		}
	}
}
