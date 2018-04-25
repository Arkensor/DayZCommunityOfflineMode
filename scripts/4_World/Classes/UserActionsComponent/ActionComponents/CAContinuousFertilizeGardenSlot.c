class CAContinuousFertilizeGardenSlot : CAContinuousQuantity
{
	protected float m_SlotFertilizerNeed;
	protected float	m_TimeToComplete;

	void CAContinuousFertilizeGardenSlot( float quantity_used_per_second )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_TimeToComplete = 0;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		GardenBase target_GB;		
		if ( Class.CastTo(target_GB,  target.GetObject() ) )
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
			
			if ( item ) 
				m_ItemQuantity = item.GetQuantity();
			
			if ( target_GB ) 
			{
				string selection = target_GB.GetActionComponentName(target.GetComponentIndex());
				Slot slot = target_GB.GetSlotBySelection( selection );
				
				
				string item_type = item.GetType();
				float consumed_quantity = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture ConsumedQuantity" ) / 2;
				
				float max = slot.GetFertilizerQuantityMax();
				
				m_SlotFertilizerNeed = max - consumed_quantity;
				Print(m_SlotFertilizerNeed);
			}
			
			m_TimeToComplete = (Math.Min(m_SlotFertilizerNeed,m_ItemQuantity))/m_QuantityUsedPerSecond;
		}
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
			if ( m_SpentQuantity < m_ItemQuantity  &&  m_SpentQuantity < m_SlotFertilizerNeed )
			{
				m_SpentQuantity += m_QuantityUsedPerSecond * player.GetDeltaT();
				float val = player.GetSoftSkillManager().AddSpecialtyBonus( m_SpentQuantity, m_Action.GetSpecialtyWeight(), true );
				GardenBase garden_base;
				Class.CastTo(garden_base,  target.GetObject() );
				string selection = garden_base.GetActionComponentName(target.GetComponentIndex());
				
				if (GetGame().IsServer())
				{
					item.AddQuantity( -m_SpentQuantity );
					m_Action.SendMessageToClient(player, garden_base.Fertilize( player, item, val, selection ));
				}

				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(player, target, item);
				return UA_FINISHED;
			}
		}
	}
	
	override float GetProgress()
	{	
		//float progress = (m_SpentQuantity*m_QuantityUsedPerSecond)/m_TimeToComplete;
		return (m_SpentQuantity*m_QuantityUsedPerSecond)/m_TimeToComplete;
	}
};