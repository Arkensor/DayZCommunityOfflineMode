class AreaDamageOneTimeDeferred extends AreaDamageBase
{	
	void AreaDamageOneTimeDeferred(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.ONETIME_DEFERRED;
	}

	void ~AreaDamageOneTimeDeferred() {}
}