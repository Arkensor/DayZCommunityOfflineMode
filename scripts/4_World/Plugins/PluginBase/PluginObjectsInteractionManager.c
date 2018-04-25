class PluginObjectsInteractionManager extends PluginBase
{
	private ref 		array<Object> m_LockedObjects;
	private ref 		array<float> m_LockedObjectsDecay;
	private const float		TIME_TO_FORCED_UNLOCK = 60;
	private const float 	TICK_RATE = 10;
	private ref Timer	m_DecayTimer;
		
	void PluginObjectsInteractionManager()
	{
		m_LockedObjects = new array<Object>;
		m_LockedObjectsDecay = new array<float>;
		//TIMERDEPRECATED - timer for decaying objects 
		m_DecayTimer = new Timer();
		m_DecayTimer.Run(TICK_RATE, this, "Decay", NULL,true);
	}

	bool IsFree(Object target)
	{
		if ( target && m_LockedObjects.Count() > 0 )
		{
			for ( int i = 0; i < m_LockedObjects.Count(); i++ )
			{	
				if ( m_LockedObjects.Get(i) == target )
				{
					return false;
				}
			}
		}
		return true;
	}

	void Lock(Object target)
	{
		if ( target && !IsFree(target) )
		{
			m_LockedObjects.Insert(target);
			m_LockedObjectsDecay.Insert(0);
		}
	}

	void Release(Object target)
	{
		if ( target && m_LockedObjects.Count() > 0 )
		{
			for ( int i = 0; i < m_LockedObjects.Count(); i++ )
			{	
				if ( m_LockedObjects.Get(i) == target )
				{
					m_LockedObjects.Remove(i);
					m_LockedObjectsDecay.Remove(i);
					break;
				}
			}
		}
	}
	
	//FAILSAFE - checks periodically locked objects and releases them automaticaly if given time has passed
	void Decay()
	{
		if ( m_LockedObjectsDecay.Count() > 0 )
		{
			for ( int i = 0; i < m_LockedObjectsDecay.Count(); i++ )
			{	
				if ( m_LockedObjectsDecay.Get(i) >= TIME_TO_FORCED_UNLOCK )
				{
					m_LockedObjects.Remove(i);
					m_LockedObjectsDecay.Remove(i);
				}
				else
				{
					m_LockedObjectsDecay.Get(i) += TICK_RATE;
				}
			}
		}
	}
};
