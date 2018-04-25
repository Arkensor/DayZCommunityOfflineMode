class CAContinuousLoadMagazine : CAContinuousRepeat
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemQuantity;
	protected float					m_ItemMaxQuantity;
	protected float 				m_SourceQuantity;
	protected float 				m_QuantityFilledPerSecond;
	//protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousLoadMagazine( float time_to_complete_action )
	{
		m_DefaultTimeToComplete = time_to_complete_action;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		super.Setup( player, target, item);
		m_SpentQuantity = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		
		Magazine itm;
		Class.CastTo(itm, item);
		m_ItemQuantity = itm.GetAmmoCount();
		m_ItemMaxQuantity = itm.GetAmmoMax();
		Magazine trg;
		Class.CastTo(trg,  target.GetObject() );
		m_SourceQuantity = trg.GetAmmoCount();
		m_TargetUnits = itm.GetAmmoMax();		
	}
};