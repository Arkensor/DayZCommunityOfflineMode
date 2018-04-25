class CAContinuousFill : CAContinuousBase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemQuantity;
	protected float 				m_QuantityFilledPerSecond;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousFill( float quantity_filled_per_second )
	{
		m_QuantityFilledPerSecond = quantity_filled_per_second;
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
		
		m_ItemQuantity = item.GetQuantity();
		m_TargetUnits = item.GetQuantityMax() - item.GetQuantity();	
		m_QuantityFilledPerSecond = player.GetSoftSkillManager().AddSpecialtyBonus( m_QuantityFilledPerSecond, m_Action.GetSpecialtyWeight(), true );
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( item.GetQuantity() >= item.GetQuantityMax() )
		{
			return UA_SETEND_2;
		}
		else
		{
			if ( m_SpentQuantity < m_TargetUnits )
			{
				m_SpentQuantity += m_QuantityFilledPerSecond * player.GetDeltaT();
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
		//float progress = m_SpentQuantity/m_TargetUnits;
		return m_SpentQuantity/m_TargetUnits;
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
		}
	}
};