class AreaDamageRegularRaycastedDeferred extends AreaDamageBase
{	
	void AreaDamageRegularRaycastedDeferred(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.REGULAR_RAYCASTED_DEFERRED;
	}

	void ~AreaDamageRegularRaycastedDeferred() {}
}