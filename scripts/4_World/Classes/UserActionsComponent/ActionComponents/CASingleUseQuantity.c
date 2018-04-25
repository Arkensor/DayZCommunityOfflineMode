class CASingleUseQuantity : CASingleUseBase
{	
	protected float 				m_QuantityUsedPerAction;
	protected float 				m_ItemQuantity;
	protected ref Param1<float>	m_SpentUnits;

	void CASingleUseQuantity( float quantity_used_per_action )
	{
		m_QuantityUsedPerAction = quantity_used_per_action;	
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
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		

		CalcAndSetQuantity(player, target, item);
		return UA_FINISHED;	
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetGame().IsServer() )
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = player.GetSoftSkillManager().AddSpecialtyBonus( m_QuantityUsedPerAction, m_Action.GetSpecialtyWeight(), true );
				SetACData(m_SpentUnits);	
			}

			item.AddQuantity(- m_QuantityUsedPerAction,false,false);
		}
	}
};