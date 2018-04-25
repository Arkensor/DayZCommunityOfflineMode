class NotifierBase
{
	
	float				m_DeltaT; // time in seconds since the last tick
	ref Timer 			m_Timer1; // timer which can be used for whatever
	PlayerBase			m_Player; //the player this Notifier belongs to
	int					m_Type;
	NotifiersManager	m_Manager;
	int 				m_TendencyBufferSize = 3;//for best results, this should be somewhat aligned with modifier frequency
	const int 			TENDENCY_BUFFER_SIZE = 30;//this needs to be bigger or same size as buffer size of any invidual buffer size
	bool				m_ShowTendency;
	bool				m_Active;
	private float		m_SecsSinceLastMessage; //internal counter
	private float		m_SecsToMessage; //internal counter
	protected float		m_MinPauseBetweenMessages; //minimal amount of seconds that needs to pass till message is displayed
	protected float		m_MaxPauseBetweenMessages; //maximal amount of seconds that can pass till message is displayed
	private float		m_SecsSinceLastSound; //internal counter
	private float		m_SecsToSound; //internal counter
	protected float		m_MinPauseBetweenSounds; //minimal amount of seconds that needs to pass till sound is played
	protected float		m_MaxPauseBetweenSounds; //maximal amount of seconds that can pass till sound is played
	private float		m_SecsSinceLastAnimation; //internal counter
	private float		m_SecsToAnimation; //internal counter
	protected float		m_MinPauseBetweenAnimations; //minimal amount of seconds that needs to pass till animation played
	protected float		m_MaxPauseBetweenAnimations; //maximal amount of seconds that can pass till animation is splayed
	float				m_TickInterval;
	float				m_TickIntervalLastTick;
	float				m_TendencyBuffer[TENDENCY_BUFFER_SIZE];
	int					m_TendencyBufferWriteIterator;
	float 				m_LastTendency;
	float				m_LastMA;
	//int					m_TendencyID;
	
	PluginPlayerStatus 	m_ModulePlayerStatus;
		
	void NotifierBase(NotifiersManager manager)
	{

		//m_Timer1 = new Timer();
		m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
		m_Active = true; 
		m_Manager = manager;
		m_Player = manager.GetPlayer();
		
		m_MinPauseBetweenMessages = -1;
		m_MinPauseBetweenSounds = m_MinPauseBetweenMessages;
		m_MinPauseBetweenAnimations = m_MinPauseBetweenMessages;
		m_TickInterval = 1000;
	}

	bool IsTimeToTick(float current_time)
	{
		if( current_time > m_TickIntervalLastTick + m_TickInterval )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	
	DisplayStatus GetDisplayStatus()
	{
		return m_Player.GetDisplayStatus();
	}

	int GetNotifierType()
	{
		return m_Type;
	}

	string GetName()
	{
		return this.ClassName() + " Notifier";
	}

	bool IsActive()
	{
		if ( m_Active )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetActive( bool state )
	{
		m_Active = state;
		if ( !state ) HideBadge();
	}

	void DisplayTendency(float delta)
	{
	}

	void AddToCyclicBuffer(float value)//for tendency
	{
		m_TendencyBuffer[m_TendencyBufferWriteIterator] = value;
		m_TendencyBufferWriteIterator++;
		if( m_TendencyBufferWriteIterator == m_TendencyBufferSize )
		{
			m_TendencyBufferWriteIterator = 0;
			m_ShowTendency = true;
		}
	}
	
	float ReadFromCyclicBuffer(int index)
	{
		int indx = m_TendencyBufferWriteIterator + index;
		if( indx >= m_TendencyBufferSize)
		{
			indx = indx - m_TendencyBufferSize;
		}
		return m_TendencyBuffer[indx];
	}
	
	float GetDeltaAvaraged()//for tendency
	{
		array<float> values = new array<float>;
		for(int i = 0; i < m_TendencyBufferSize; i++)
		{
			values.Insert(ReadFromCyclicBuffer(i));
		}
		//SmoothOutFloatValues(values);
		
		//--------------------------------------------------------------------------
		
		float values_sum = 0;
		//PrintString("----------------------------");
		//PrintString("tendency:" + this.ClassName() );
		for(i = 0; i < values.Count(); i++)
		{
			float value = values.Get(i);
			values_sum += value;
			//PrintString( value.ToString() );
		}
		
		float sma = values_sum / m_TendencyBufferSize;
		float tnd = sma - m_LastMA;
		m_LastMA = sma;
		
		//PrintString("tendency:" + tnd.ToString() );
		//PrintString("----------------------------");
		return tnd;
		
		//--------------------------------------------------------------------------
		
	}
	
	/*
	float GetDeltaAvaraged()//for tendency
	{
		array<float> values = new array<float>;
		for(int i = 0; i < m_TendencyBuffer.Count(); i++)
		{
			values.Insert(ReadFromCyclicBuffer(i));
		}
		SmoothOutFloatValues(values);
		//--------------------------------------------------------------------------
		
		float values_sum = 0;
		for(int i = 0; i < m_TendencyBufferSize; i++)
		{
			float value = ReadFromCyclicBuffer(i);
			values_sum += value;
		}
		float sma = values_sum / m_TendencyBufferSize;
		float tnd = sma - m_LastMA;
		m_LastMA = sma;
		return tnd;
		
		//--------------------------------------------------------------------------
	}
	*/
	void SmoothOutFloatValues(array<float> values)
	{
		float value1;
		float value2;
		for(int i = 0; i < values.Count() - 1; i++)
		{
			value1 = values.Get(i);
			value2 = values.Get(i + 1);
			float average = (value1 + value2) / 2;
			values.Set(i, average);
			values.Set(i + 1, average);
		}
		int index = values.Count() - 1;
		values.Set(index, value2);
	}

	void OnTick(float current_time)
	{
		//------------
		/*
		float time = GetGame().GetTime();
		Print("ticking notifier "+ this.ClassName() +" for player " + m_Player.ToString() + " at time:" + time.ToString());
		*/
		//------------
		m_TickIntervalLastTick = current_time;
		DisplayBadge();
		if ( m_MinPauseBetweenMessages != -1 )
		{
			if ( m_SecsSinceLastMessage >= m_SecsToMessage )
			{
				DisplayMessage();
				m_SecsToMessage = Math.RandomInt(m_MinPauseBetweenMessages,m_MaxPauseBetweenMessages);
				m_SecsSinceLastMessage = 0;
			}
			else
			{
				m_SecsSinceLastMessage+=m_DeltaT;
			}
		}
		if ( m_MinPauseBetweenSounds != -1 )
		{
			if ( m_SecsSinceLastSound >= m_SecsToSound )
			{
				PlaySound();
				m_SecsToSound = Math.RandomInt(m_MinPauseBetweenSounds,m_MaxPauseBetweenSounds);
				m_SecsSinceLastSound = 0;
			}
			else
			{
				m_SecsSinceLastSound+=m_DeltaT;
			}
		}
		if ( m_MinPauseBetweenAnimations != -1 )
		{
			if ( m_SecsSinceLastAnimation >= m_SecsToAnimation )
			{
				PlayAnimation();
				m_SecsToAnimation = Math.RandomInt(m_MinPauseBetweenAnimations,m_MaxPauseBetweenAnimations);
				m_SecsSinceLastAnimation = 0;
			}
			else
			{
				m_SecsSinceLastAnimation+=m_DeltaT;
			}
		}
		AddToCyclicBuffer( GetObservedValue() );
		if (m_ShowTendency) DisplayTendency( GetDeltaAvaraged() );

	}
	
	protected int CalculateTendency(float delta, float inctresholdlow, float inctresholdmed, float inctresholdhigh, float dectresholdlow, float dectresholdmed, float dectresholdhigh)
	{                                        	
		int ndelta = TENDENCY_STABLE;
		if ( delta > inctresholdlow ) 	ndelta = TENDENCY_INC_LOW;
		if ( delta > inctresholdmed )  	ndelta = TENDENCY_INC_MED;
		if ( delta > inctresholdhigh ) 	ndelta = TENDENCY_INC_HIGH;	
		if ( delta < dectresholdlow )  	ndelta = TENDENCY_DEC_LOW;
		if ( delta < dectresholdmed )  	ndelta = TENDENCY_DEC_MED;
		if ( delta < dectresholdhigh ) 	ndelta = TENDENCY_DEC_HIGH;
		return ndelta;
	}
	
	protected DSLevels DetermineLevel(float value, float warning_treshold, float critical_treshold, float empty_treshold )
	{
		DSLevels level;
		if(value < warning_treshold) level = DSLevels.WARNING;
		if(value < critical_treshold) level = DSLevels.CRITICAL;
		if(value <= empty_treshold) level = DSLevels.BLINKING;
		return level;
	}
	

	protected void DisplayBadge()
	{
	}

	protected void HideBadge()
	{
	}

	protected void DisplayMessage()
	{
	}

	protected void PlaySound()
	{
	}
	
	protected void PlayAnimation()
	{
	}
	
	protected float GetObservedValue()
	{
		return 0;
	}
}