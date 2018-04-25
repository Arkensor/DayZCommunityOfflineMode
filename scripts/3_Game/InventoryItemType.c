class InventoryItemType
{
	ref array<ref AnimSoundEvent> m_animSoundEvents;

	void InventoryItemType()
	{
		LoadSoundEvents();
	}
	
	private void ~InventoryItemType();
	
	void LoadSoundEvents()
	{
		string cfgPath = "CfgVehicles " + GetName() + " AnimEvents SoundWeapon";
		
		m_animSoundEvents = new array<ref AnimSoundEvent>;

		int soundCount = GetGame().ConfigGetChildrenCount(cfgPath);
		
		if(soundCount <= 0)//try other path
		{
			cfgPath = "CfgWeapons " + GetName() + " AnimEvents SoundWeapon";
			soundCount = GetGame().ConfigGetChildrenCount(cfgPath);
		}
		
		if(soundCount <= 0)//try other path
		{
			cfgPath = "CfgMagazines " + GetName() + " AnimEvents SoundWeapon";
			soundCount = GetGame().ConfigGetChildrenCount(cfgPath);
		}
		
		if(soundCount <= 0)//try other path
		{
			cfgPath = "CfgAmmo " + GetName() + " AnimEvents SoundWeapon";
			soundCount = GetGame().ConfigGetChildrenCount(cfgPath);
		}
		
		for(int i = 0; i < soundCount; i++)
		{
			string soundName;
			GetGame().ConfigGetChildName(cfgPath, i, soundName);			
			string soundPath = cfgPath + " " + soundName + " ";
			AnimSoundEvent soundEvent = new AnimSoundEvent(soundPath);
			m_animSoundEvents.Insert(soundEvent);
		}
	}


	AnimSoundEvent GetSoundEvent(int event_id)
	{
		for(int i = 0; i < m_animSoundEvents.Count(); i++)
		{
			AnimSoundEvent soundEvent = m_animSoundEvents.Get(i);
			if(soundEvent.m_iID == event_id)
			{
				return soundEvent;
			}
		}

		return NULL;
	}


	proto native owned string GetName();
}