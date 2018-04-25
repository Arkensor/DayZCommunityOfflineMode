class DayZCreatureAIType
{
	ref array<ref AnimSoundEvent> m_animSoundEvents;
	ref array<ref AnimStepEvent> m_animStepEvents;
	ref array<ref AnimDamageEvent> m_animDamageEvents;

	string m_CfgPath;
	string m_AnimEventsCfgPath;

	void DayZCreatureAIType()
	{
		m_CfgPath = "CfgVehicles " + GetName() + " ";
		m_AnimEventsCfgPath = m_CfgPath	+ "AnimEvents ";
		LoadParams();
	}
	
	void LoadAnimSoundEvents()
	{
		m_animSoundEvents = new array<ref AnimSoundEvent>;
		
		string soundsCfgPath = m_AnimEventsCfgPath + "Sounds ";

		int soundCount = GetGame().ConfigGetChildrenCount(soundsCfgPath);
		for(int i = 0; i < soundCount; i++)
		{
			string soundName;
			GetGame().ConfigGetChildName(soundsCfgPath, i, soundName);			
			string soundPath = soundsCfgPath + soundName + " ";
			AnimSoundEvent soundEvent = new AnimSoundEvent(soundPath);
			m_animSoundEvents.Insert(soundEvent);
		}
	}
	
	void LoadAnimStepEvents()
	{
		m_animStepEvents = new array<ref AnimStepEvent>;	
		string stepsCfgPath = m_AnimEventsCfgPath + "Steps ";
		int stepsCount = GetGame().ConfigGetChildrenCount(stepsCfgPath);

		for(int i = 0; i < stepsCount; i++)
		{
			string stepName;
			GetGame().ConfigGetChildName(stepsCfgPath, i, stepName);			
			string stepPath = stepsCfgPath + stepName + " ";
			AnimStepEvent stepEvent = new AnimStepEvent(stepPath);
			m_animStepEvents.Insert(stepEvent);
		}
	}
	
	void LoadAnimDamageEvents()
	{
		m_animDamageEvents = new array<ref AnimDamageEvent>;

		string damagesCfgPath = m_AnimEventsCfgPath + "Damages ";
		int damagesCount = GetGame().ConfigGetChildrenCount(damagesCfgPath);

		for(int i = 0; i < damagesCount; i++)
		{
			string damageName;
			GetGame().ConfigGetChildName(damagesCfgPath, i, damageName);			
			string damagePath = damagesCfgPath + damageName + " ";
			AnimDamageEvent damageEvent = new AnimDamageEvent(damagePath);
			m_animDamageEvents.Insert(damageEvent);
		}
	}

	private void LoadParams()
	{
		LoadAnimSoundEvents();
		LoadAnimStepEvents();
		LoadAnimDamageEvents();
	}

	AnimStepEvent GetStepEvent(int event_id)
	{
		for(int i = 0; i < m_animStepEvents.Count(); i++)
		{
			AnimStepEvent stepEvent = m_animStepEvents.Get(i);
			if(stepEvent.m_iID == event_id)
			{
				return stepEvent;
			}
		}

		return NULL;
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

	AnimDamageEvent GetDamageEvent(int event_id)
	{
		for(int i = 0; i < m_animDamageEvents.Count(); i++)
		{
			AnimDamageEvent damageEvent = m_animDamageEvents.Get(i);
			if(damageEvent.m_iID == event_id)
			{
				return damageEvent;
			}
		}

		return NULL;
	}

	proto native owned string GetName();
}