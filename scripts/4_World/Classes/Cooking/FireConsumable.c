class FireConsumable
{
	ItemBase m_Item;
	float m_Energy;
	float m_RemainingEnergy;
	
	void FireConsumable ( ItemBase item, float energy )
	{
		m_Item = item;
		m_Energy = energy;
		m_RemainingEnergy = energy;
	}
	
	//Item
	ItemBase GetItem()
	{
		return m_Item;
	}
	
	//Energy
	float GetEnergy()
	{
		return m_Energy;
	}
	
	//Remaining energy
	float GetRemainingEnergy()
	{
		return m_RemainingEnergy;
	}
	
	void SetRemainingEnergy( float energy )
	{
		m_RemainingEnergy = energy;
	}
}
