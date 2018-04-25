class CattleProd : ItemBase
{
	protected int m_MeleeMode;
	protected int m_MeleeHeavyMode;
	protected int m_MeleeSprintMode;

	void CattleProd()
	{
		m_MeleeMode = 0;
		m_MeleeHeavyMode = 1;
		m_MeleeSprintMode = 2;
	}

	override void OnWorkStart()
	{
		//! melee modes in cfg (switched on state)
		m_MeleeMode = 3;
		m_MeleeHeavyMode = 4;
		m_MeleeSprintMode = 5;
	}
	
	override void OnWorkStop()
	{
		//! melee modes in cfg (switched off state)
		m_MeleeMode = 0;
		m_MeleeHeavyMode = 1;
		m_MeleeSprintMode = 2;
	}
	
	override int GetMeleeMode()
	{
		return m_MeleeMode;
	}

	override int GetMeleeHeavyMode()
	{
		return m_MeleeHeavyMode;
	}
	
	override int GetMeleeSprintMode()
	{
		return m_MeleeSprintMode;
	}
}