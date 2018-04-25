class CAContinuousMineRock : CAContinuousMineWood
{	
	void CAContinuousMineRock(float time_between_drops)
	{
		m_TimeBetweenEachMaterialDrop = time_between_drops;
	}
	
	override bool GetMiningData(PlayerBase player, ActionTarget target, ItemBase item )
	{
		RockBase ntarget;
		if ( Class.CastTo(ntarget, target.GetObject()) )
		{
			m_AmountOfDrops = Math.Max(1,ntarget.GetAmountOfDrops(item));
			m_Material = ntarget.GetMaterial(item);
			m_AmountOfMaterialPerDrop = Math.Max(1,ntarget.GetAmountOfMaterialPerDrop(item));
			m_DamageToMiningItemEachDrop = ntarget.GetDamageToMiningItemEachDrop(item);
			return true;
		}
		return false;
	}
};