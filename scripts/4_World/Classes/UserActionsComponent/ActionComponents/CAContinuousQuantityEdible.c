class CAContinuousQuantityEdible : CAContinuousQuantityRepeat
{
	protected float 	m_InitItemQuantity;
	protected float 	m_SpentQuantityTotal;
	
	void CAContinuousQuantityEdible( float quantity_used_per_second, float time_to_repeat )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeToRepeat = time_to_repeat;
		m_InitItemQuantity = -1;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		super.Setup(player, target, item);
		
		if (m_InitItemQuantity < 0)
		{
			m_InitItemQuantity = item.GetQuantity();
		}
	}
	
	override void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		//Print("Munch!");
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
			
			m_SpentQuantityTotal += m_SpentQuantity;
		}
		//m_SpentQuantity = Math.Floor(m_SpentQuantity);

		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		if ( ntarget )
		{
			if ( GetGame().IsServer() ) 
			{
				//item.Consume(targetObject, m_SpentQuantity);
				ntarget.Consume(item, m_SpentQuantity);
				
			}
		}
		else
		{
			if ( GetGame().IsServer() ) 
			{
				//item.Consume(player, m_SpentQuantity);
				player.Consume(item, m_SpentQuantity);
			}
		}
	}
	
	override float GetProgress()
	{
		//return super.GetProgress();
		if (m_SpentQuantity > 0)
		{
			return Math.Clamp((1 - (m_InitItemQuantity - m_SpentQuantity - m_SpentQuantityTotal ) / m_InitItemQuantity), 0, 1);
		}
		else
		{
			return Math.Clamp((1 - (m_InitItemQuantity - m_SpentQuantityTotal ) / m_InitItemQuantity), 0, 1);
		}
	}
};