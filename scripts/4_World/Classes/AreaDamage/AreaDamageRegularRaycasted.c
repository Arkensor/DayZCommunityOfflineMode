class AreaDamageRegularRaycasted extends AreaDamageBase
{	
	void AreaDamageRegularRaycasted(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.REGULAR_RAYCASTED;
	}

	void ~AreaDamageRegularRaycasted() {}
}