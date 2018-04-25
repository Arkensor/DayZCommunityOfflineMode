class AreaDamageOneTime extends AreaDamageBase
{	
	void AreaDamageOneTime(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.ONETIME;
	}

	void ~AreaDamageOneTime() {}
}