class AreaDamageOneTimeRaycastedDeferred extends AreaDamageBase
{	
	void AreaDamageOneTimeRaycastedDeferred(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.ONETIME_RAYCASTED_DEFERRED;
	}

	void ~AreaDamageOneTimeRaycastedDeferred() {}
}