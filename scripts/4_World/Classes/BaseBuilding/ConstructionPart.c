class ConstructionPart
{
	string 	m_PartName;
	string 	m_SlotName;
	bool 	m_IsBuilt;
	
	void ConstructionPart ( string part_name, string slot_name, bool is_built )
	{
		m_PartName = part_name;
		m_SlotName = slot_name;
		m_IsBuilt = is_built;
	}
	
	string GetPartName()
	{
		return m_PartName;
	}
	
	string GetSlotName()
	{
		return m_SlotName;
	}
	
	bool IsBuilt()
	{
		return m_IsBuilt;
	}
	
	void SetIsBuilt( bool is_built )
	{
		m_IsBuilt = is_built;
	}
}
