class AreaDamageRegularDeferred extends AreaDamageBase
{	
	void AreaDamageRegularDeferred(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.REGULAR_DEFERRED;
	}

	void ~AreaDamageRegularDeferred() {}
}