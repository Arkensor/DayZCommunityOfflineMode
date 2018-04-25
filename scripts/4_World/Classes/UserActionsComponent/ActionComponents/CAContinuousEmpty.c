class CAContinuousEmpty : CAContinuousBase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_QuantityEmptiedPerSecond;
	protected float					m_TimeToComplete;
	protected ref Param1<float>	m_SpentUnits;
	
	void CAContinuousEmpty( float quantity_emptied_per_second )
	{
		m_QuantityEmptiedPerSecond = quantity_emptied_per_second;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_SpentQuantity = item.GetWet();
		m_TargetUnits = item.GetWetMax();
		
		m_TimeToComplete = m_SpentQuantity/m_QuantityEmptiedPerSecond;
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( item.GetWet() >= item.GetWetMax() )
		{
			return UA_SETEND_2;
		}
		else
		{
		if ( m_SpentQuantity < m_TargetUnits )
		{
			m_SpentQuantity += m_QuantityEmptiedPerSecond * player.GetDeltaT();
			return UA_PROCESSING;
		}
		else
		{
			CalcAndSetQuantity(player,target,item);
			return UA_FINISHED;
		}	
	}
	}
	
	override int Cancel( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player || !item )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity(player, target, item);
		return UA_CANCEL;
	}	
	
	override float GetProgress()
	{	
		//float progress = (m_SpentQuantity*m_QuantityEmptiedPerSecond)/m_TimeToComplete;
		return (m_SpentQuantity*m_QuantityEmptiedPerSecond)/m_TimeToComplete;
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = player.GetSoftSkillManager().SubtractSpecialtyBonus( m_SpentQuantity, m_Action.GetSpecialtyWeight(), true );
			SetACData(m_SpentUnits);
		}
	}
};