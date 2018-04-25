class CAContinuousQuantity : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemMaxQuantity;
	protected float 				m_QuantityUsedPerSecond;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousQuantity( float quantity_used_per_second )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_SpentQuantity = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
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
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(player, target, item);
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
		
		CalcAndSetQuantity(player,target,item);
		return UA_INTERRUPT;
	}	
	
	override float GetProgress()
	{
		//float progress = m_SpentQuantity/m_ItemQuantity;
		return m_SpentQuantity/m_ItemQuantity;
	}
	
	//---------------------------------------------------------------------------
	
	
	void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetGame().IsServer() )
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = player.GetSoftSkillManager().AddSpecialtyBonus( m_SpentQuantity, m_Action.GetSpecialtyWeight(), true );
				SetACData(m_SpentUnits);
			}
			
			item.AddQuantity(- m_SpentQuantity,false,false);
		}
	}
};