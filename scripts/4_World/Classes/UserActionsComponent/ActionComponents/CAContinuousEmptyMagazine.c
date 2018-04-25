class CAContinuousEmptyMagazine : CAContinuousBase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_Time;
	protected float 				m_TimeToEjectOneBullet;
	protected ref Param1<float>	m_SpentUnits;
	
	void CAContinuousEmptyMagazine( float eject_time )
	{
		m_TimeToEjectOneBullet = eject_time;
		m_Time = 0;
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
		m_SpentQuantity = 0;
		Magazine itm;
		Class.CastTo(itm, item);
		m_TargetUnits = itm.GetAmmoCount();			
		
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( m_TargetUnits == 0 )
		{
			return UA_SETEND_2;
		}
		else
		{
			if ( m_SpentQuantity < m_TargetUnits )
			{
				m_Time += player.GetDeltaT();
				if ( m_Time > m_TimeToEjectOneBullet )
				{
					m_Time = 0;
					m_SpentQuantity += 1;
				}
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
			if ( item )
			{
				Magazine itm;
				Class.CastTo(itm, item);
				if ( m_SpentUnits )
				{
					m_SpentUnits.param1 = m_SpentQuantity;
					SetACData(m_SpentUnits);
				}		
				itm.ServerAddAmmoCount(-m_SpentQuantity);
			}
		}
	}
};