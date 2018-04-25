class CAContinuousTransferQuantity : CAContinuousBase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemQuantity;
	protected float 				m_SourceQuantity;
	protected float 				m_QuantityFilledPerSecond;
	protected float					m_TimeToComplete;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousTransferQuantity( float quantity_filled_per_second )
	{
		m_QuantityFilledPerSecond = quantity_filled_per_second;
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
		m_ItemQuantity = item.GetQuantity();
		ItemBase trg;
		if ( Class.CastTo(trg, target.GetObject()) )
		{
			m_SourceQuantity = trg.GetQuantity();
		}
		m_TargetUnits = item.GetQuantityMax();	
		m_TimeToComplete= (Math.Min((m_TargetUnits-m_ItemQuantity),m_SourceQuantity))/m_QuantityFilledPerSecond;
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{		
		if ( !player )
		{
			return UA_ERROR;
		}
		ItemBase trg;
		Class.CastTo(trg, target.GetObject());
		if ( item.GetQuantity() >= item.GetQuantityMax() || trg.GetQuantity() == 0 )
		{
			return UA_SETEND_2;
		}
		else
		{
		if ( m_SpentQuantity < m_TargetUnits && m_SpentQuantity < m_SourceQuantity )
		{
			m_SpentQuantity += m_QuantityFilledPerSecond * player.GetDeltaT();
			float val = m_ItemQuantity-m_SpentQuantity;
			if ( m_Action ) m_Action.SendMessageToClient(player, val.ToString());
			return UA_PROCESSING;
		}
		else
		{
			CalcAndSetQuantity(player,target,item);
			return UA_FINISHED;
		}
	}
	}
	
	override override int Cancel( PlayerBase player, ActionTarget target, ItemBase item )
	{		
		if ( !player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity(player, target, item);
		return UA_CANCEL;
	}
		
	override float GetProgress()
	{	
		float progress = (m_SpentQuantity*m_QuantityFilledPerSecond)/m_TimeToComplete;
		return (m_SpentQuantity*m_QuantityFilledPerSecond)/m_TimeToComplete;
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
			item.AddQuantity(m_SpentQuantity);
			ItemBase trg;
			if ( Class.CastTo(trg, target.GetObject()) )
			{
				trg.AddQuantity(-m_SpentQuantity,false,false);
			}
		}
	}
};