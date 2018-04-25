class CAContinuousQuantityExtinguish : CAContinuousQuantityRepeat
{
	private const float ROUGH_SPECIALTY_WEIGHT		= 0.02;
	private float 		m_WetnessGainMultiplier 	= 1.0;
	
	void CAContinuousQuantityExtinguish( float quantity_used_per_second, float time_to_repeat, float wetness_gain_multiplier )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeToRepeat = time_to_repeat;
		m_WetnessGainMultiplier = wetness_gain_multiplier;
	}
		
	override void CalcAndSetQuantity( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsServer() ) 
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData( m_SpentUnits );
			}
			m_SpentQuantity = Math.Floor( m_SpentQuantity );
			
			Object targetObject = target.GetObject();
			if ( targetObject )
			{
				FireplaceBase fireplace_target = FireplaceBase.Cast( targetObject );
				
				//add wetness to fireplace target
				float wetness = fireplace_target.GetWet() + ( player.GetSoftSkillManager().AddSpecialtyBonus( m_SpentQuantity, m_Action.GetSpecialtyWeight(), true ) / 1000 * m_WetnessGainMultiplier );
				wetness = Math.Clamp( wetness , 0, 1 );
				fireplace_target.SetWet( wetness );
				
				//subtract quantity from water source
				item.AddQuantity( -m_SpentQuantity );
				
				//add soft skill speciality
				if ( item.GetQuantityMax() > 0 )
				{
					player.GetSoftSkillManager().AddSpecialty( ROUGH_SPECIALTY_WEIGHT * ( m_SpentQuantity / item.GetQuantityMax() ) );
				}
			}
		}
	}
}