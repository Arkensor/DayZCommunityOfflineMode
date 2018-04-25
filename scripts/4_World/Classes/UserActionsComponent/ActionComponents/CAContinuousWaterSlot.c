class CAContinuousWaterSlot : CAContinuousQuantity
{
	protected float m_PlantThirstyness;
	protected float	m_TimeToComplete;

	void CAContinuousWaterSlot( float quantity_used_per_second )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		GardenBase target_GB;
			
		if ( Class.CastTo(target_GB, target.GetObject()) )
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
			
				m_PlantThirstyness = slot.GetWaterUsage() - slot.GetWater();
			}
			
			m_TimeToComplete = (Math.Min(m_PlantThirstyness,m_ItemQuantity))/m_QuantityUsedPerSecond;
			
			if (m_TimeToComplete <= 0.1) // Division by zero prevention
			{
				m_TimeToComplete = 0.1;
			}
		}
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		GardenBase target_GB;
		Class.CastTo(target_GB,  target.GetObject() );
		
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
			string selection = target_GB.GetActionComponentName(target.GetComponentIndex());
			Slot slot = target_GB.GetSlotBySelection( selection );
			
			if ( slot  &&  m_SpentQuantity < m_ItemQuantity )
			{
				m_SpentQuantity += m_QuantityUsedPerSecond * player.GetDeltaT();
				
				if ( GetGame().IsServer() )
				{
					float water = player.GetSoftSkillManager().AddSpecialtyBonus( m_SpentQuantity, m_Action.GetSpecialtyWeight(), true );
					m_Action.SendMessageToClient(player, slot.GiveWater( item, water ));
					item.AddQuantity(- m_SpentQuantity);
				}
				
				m_SpentQuantity = 0;
				return UA_PROCESSING;
			}
			else
			{
				//CalcAndSetQuantity(player, target, item);
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