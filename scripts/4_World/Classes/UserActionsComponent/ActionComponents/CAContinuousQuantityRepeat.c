class CAContinuousQuantityRepeat : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemMaxQuantity;
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_DefaultTimeToRepeat;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousQuantityRepeat( float quantity_used_per_second, float time_to_repeat )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeToRepeat = time_to_repeat;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TimeElpased = 0;
		m_SpentQuantity = 0;
		
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}		
		
		m_ItemMaxQuantity = item.GetQuantityMax();
		m_ItemQuantity = item.GetQuantity();
	}
	
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 )
		{
			return UA_SETEND_2;
		}
		else
		{
			if ( m_SpentQuantity < m_ItemQuantity )
			{
				m_SpentQuantity += m_QuantityUsedPerSecond * player.GetDeltaT();
				m_TimeElpased += player.GetDeltaT();
				
				if ( m_TimeElpased >= m_DefaultTimeToRepeat )
				{
					CalcAndSetQuantity( player, target, item );
					Setup( player, target, item );	//reset data after repeat
				}
				
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity( player, target, item );
				return UA_FINISHED;
			}
		}
	}
	
	override int Cancel( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( player, target, item );
		return UA_INTERRUPT;
	}	
	
	override float GetProgress()
	{
		//float progress = ( m_ItemQuantity - m_SpentQuantity ) / m_ItemMaxQuantity;
		return ( m_ItemQuantity - m_SpentQuantity ) / m_ItemMaxQuantity;
	}
	
	//---------------------------------------------------------------------------
	
	
	void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetGame().IsServer() )
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}
			
			item.AddQuantity( -m_SpentQuantity, false, false );
		}
	}
}