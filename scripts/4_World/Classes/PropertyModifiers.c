class PropertyModifiers
{
	vector m_RecoilModifiers;
	float m_BarrelLength;
	//ref Timer test_timer;
	ItemBase m_OwnerItem;
	
	vector GetRecoilModifiers()
	{
		return m_RecoilModifiers;
	}
	
	void PropertyModifiers(ItemBase owner)
	{
		//test_timer = new Timer();
		//test_timer.Run(5, this, "Refresh", NULL, true);
		m_OwnerItem = owner;
		UpdateModifiers();
		m_BarrelLength = CalculateBarrelLength(owner);
	}
	
	//! Get Barrel Legth in mm
	float GetBarrelLength()
	{
		//return m_BarrelLength * 1000:
		return CalculateBarrelLength(m_OwnerItem) * 1000;
	}
	
	/*
	void Refresh()
	{
		float delka = CalculateBarrelLength(m_OwnerItem);
		//int i = 1 + 1;
	}
*/
	float CalculateBarrelLength(ItemBase owner)
	{
		vector usti_hlavne_position = owner.GetSelectionPosition( "usti hlavne" );//usti hlavne
		vector konec_hlavne_position = owner.GetSelectionPosition( "konec hlavne" );//konec hlavne
		usti_hlavne_position = owner.ModelToWorld(usti_hlavne_position);
		konec_hlavne_position = owner.ModelToWorld(konec_hlavne_position);
		return vector.Distance(usti_hlavne_position, konec_hlavne_position);
	}


	void UpdateModifiers()
	{
		GetModifierRaw(m_OwnerItem, "recoilModifier", m_RecoilModifiers);
		
		for (int i = 0; i < m_OwnerItem.GetInventory().AttachmentCount(); i++)
		{
			ItemBase attachment = ItemBase.Cast(m_OwnerItem.GetInventory().GetAttachmentFromIndex(i));
			vector temp_value;
			if( PropertyModifiers.GetModifierRaw(attachment, "recoilModifier", temp_value ) )
			{
				m_RecoilModifiers[0] = m_RecoilModifiers[0] * temp_value[0];
				m_RecoilModifiers[1] = m_RecoilModifiers[1] * temp_value[1];
				m_RecoilModifiers[2] = m_RecoilModifiers[2] * temp_value[2];
			}
		}
	}

	static bool GetModifierRaw(ItemBase item, string modifier_name, out vector value)
	{
		if( item.ConfigIsExisting(modifier_name) )
		{
			value = item.ConfigGetVector(modifier_name);
			return true;
		}
		else
		{
			value = "1 1 1";
			Debug.LogError("Config value'"+ modifier_name+"'not specified for item: " +item.GetType() );
			return false;
		}
	}
	
}