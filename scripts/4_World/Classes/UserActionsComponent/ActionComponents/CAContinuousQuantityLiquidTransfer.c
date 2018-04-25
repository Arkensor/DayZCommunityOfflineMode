class CAContinuousQuantityLiquidTransfer : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemMaxQuantity; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	protected PlayerBase 			m_Player;
	
	void CAContinuousQuantityLiquidTransfer( float quantity_used_per_second, float time_to_progress )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Player = player;
		
		ItemBase target_item = ItemBase.Cast(target.GetObject());
		
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
		
		if ( player.GetLiquidTendencyDrain() )
		{
			if ( target_item.GetQuantity() > (item.GetQuantityMax() - item.GetQuantity()) )
			{
				m_ItemMaxQuantity = item.GetQuantityMax() - item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
			else //
			{
				m_ItemMaxQuantity = target_item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
		}
		else
		{
			if ( item.GetQuantity() > (target_item.GetQuantityMax() - target_item.GetQuantity()) )
			{
				m_ItemMaxQuantity = target_item.GetQuantityMax() - target_item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //item.GetQuantity();
			}
			else //
			{
				m_ItemMaxQuantity = item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
		}
		//m_ItemMaxQuantity = item.GetQuantityMax();
		//m_ItemQuantity = item.GetQuantity();
	}
	
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		ItemBase target_item = ItemBase.Cast(target.GetObject());
		
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
				
				/*if ( m_TimeElpased >= m_DefaultTimeStep )
				{
					CalcAndSetQuantity( player, target, item );
					//Setup( player, target, item );	//reset data after repeat
				}*/
				
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
		//float progress = ( m_ItemQuantity - m_SpentQuantity ) / m_ItemMaxQuantity;
		//float progress2 = m_ItemQuantity - m_SpentQuantity / m_ItemMaxQuantity;;
		
		//return (m_ItemQuantity - m_SpentQuantity) / m_ItemMaxQuantity;
		
		if ( m_Player.GetLiquidTendencyDrain() )
		{
			return m_SpentQuantity / m_ItemMaxQuantity;
		}
		else
		{
			return -(m_SpentQuantity / m_ItemMaxQuantity);
		}
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase target_item = ItemBase.Cast(target.GetObject());
		
		if ( GetGame().IsServer() )
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}
			
			//could move following stuff to action itself, if needed
			if ( player.GetLiquidTendencyDrain() )
			{
				Liquid.Transfer(target_item, item, m_SpentQuantity);
			}
			else
			{
				Liquid.Transfer(item, target_item, m_SpentQuantity);
			}
			//item.AddQuantity( -m_SpentQuantity, false, false );
		}
	}
}