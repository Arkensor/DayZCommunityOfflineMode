class AreaDamageRegular extends AreaDamageBase
{	
	void AreaDamageRegular(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.REGULAR;
	}

	void ~AreaDamageRegular() {}
}