class ActionDebug : ActionInstantBase
{	
	ItemBase	m_Target;
	PlayerBase 	m_Player;
	int 		m_Id;

	override int GetType()  
	{
		return AT_DEBUG;
	}
	
	override void PerformAction( PlayerBase player, Param data )
	{
		m_Player = player;
		ref Param2<ItemBase,int> ndata = Param2<ItemBase,int>.Cast( data );
		m_Target = ndata.param1;
		m_Id = ndata.param2;
		
		m_Target.OnAction(m_Id,m_Player,NULL);
	}
	
	override void WriteToContext (ParamsWriteContext ctx,ActionTarget target)
	{
		super.WriteToContext(ctx, target); //Mandatory
		
		if( m_Target )
		{
			ctx.Write(m_Target);
		}
		if ( m_Id > 0 )
		{
			ctx.Write(m_Id);
		}
	}
	
	override bool UseAcknowledgment()
	{
		return false;
	}
};