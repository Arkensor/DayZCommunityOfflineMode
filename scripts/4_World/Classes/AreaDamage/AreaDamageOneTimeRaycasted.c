class AreaDamageOneTimeRaycasted extends AreaDamageBase
{	
	void AreaDamageOneTimeRaycasted(EntityAI parent)
	{
		AreaDamageBase(parent);
		m_AreaDamageType = AreaDamageType.ONETIME_RAYCASTED;
	}

	void ~AreaDamageOneTimeRaycasted() {}
}