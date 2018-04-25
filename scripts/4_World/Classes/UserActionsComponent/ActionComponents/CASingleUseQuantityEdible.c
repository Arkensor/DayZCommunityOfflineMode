class CASingleUseQuantityEdible : CASingleUseQuantity
{
	void CASingleUseQuantityEdible( float quantity_used_per_action )
	{
		m_QuantityUsedPerAction = quantity_used_per_action;
	}
	
	override void CalcAndSetQuantity(PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_QuantityUsedPerAction;
			SetACData(m_SpentUnits);	
		}
		
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		if ( ntarget )
		{
			if ( GetGame().IsServer() ) 
			{
				//item.Consume(ntarget, m_QuantityUsedPerAction);
				ntarget.Consume(item, m_QuantityUsedPerAction);
			}
		}
		else
		{
			if ( GetGame().IsServer() ) 
			{
				//item.Consume(player, m_QuantityUsedPerAction);
				player.Consume(item, m_QuantityUsedPerAction);
			}
		}
	}
};