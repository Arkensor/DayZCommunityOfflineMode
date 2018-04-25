class Warmth: NotifierBase
{
	private const float	 	HYPERTHERMIA_TRESHOLD		= 40;
	private const float	 	HOT_TRESHOLD				= 38;
	private const float	 	OVERHEATING_TRESHOLD		= 37.1;
	private const float	 	COLD_TRESHOLD				= 35.8;
	private const float	 	FREEZING_TRESHOLD			= 35;
	private const float	 	HYPOTHERMIA_TRESHOLD		= 34.5;
	private const float 	DEC_TRESHOLD_LOW 			= -0.001;
	private const float 	DEC_TRESHOLD_MED 			= -0.2;
	private const float 	DEC_TRESHOLD_HIGH			= -0.3;
	private const float 	INC_TRESHOLD_LOW 			= 0.001;
	private const float 	INC_TRESHOLD_MED 			= 0.2;
	private const float 	INC_TRESHOLD_HIGH			= 0.3;
		
	void Warmth(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
	}

	override int GetNotifierType()
	{
		return NTF_WARMTH;
	}

	override void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_TDCY_TEMPERATURE,tendency);
	}

	override void DisplayBadge()
	{
		float temperature = m_Player.GetStatTemperature().Get();
		if ( temperature >= HYPERTHERMIA_TRESHOLD ) 
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_1);
		}
		else if ( temperature >= HOT_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_2);
		}
		else if ( temperature >= OVERHEATING_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_3);
		}
		else if ( temperature <= COLD_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_4);
		}
		else if ( temperature <= FREEZING_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_5);
		}
		else if ( temperature <= HYPOTHERMIA_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_6);
		}
	}

	override void HideBadge()
	{
		//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_TEMPERATURE,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		float temperature = m_Player.GetStatTemperature().Get();
		if ( temperature >= HYPERTHERMIA_TRESHOLD ) 
		{
			m_Player.MessageImportant("I am hyperthermic");
		}
		else if ( temperature >= HOT_TRESHOLD )
		{
			m_Player.MessageStatus("I am sweating");
		}
		else if ( temperature >= OVERHEATING_TRESHOLD )
		{
			m_Player.MessageStatus("I am overheating");
		}
		else if ( temperature <= COLD_TRESHOLD )
		{
			m_Player.MessageStatus("I am shaking");
		}
		else if ( temperature <= FREEZING_TRESHOLD )
		{
			m_Player.MessageStatus("I am freezing");
		}
		else if ( temperature <= HYPOTHERMIA_TRESHOLD )
		{
			m_Player.MessageImportant("I am hypothermic");
		}
	}

	override void PlayAnimation()
	{
		float temperature = m_Player.GetStatTemperature().Get();
		if ( temperature >= HOT_TRESHOLD ) 
		{
		//	PrintString(GetName() + " sweat wiping animation");
		}
		else if ( temperature <= FREEZING_TRESHOLD )
		{
		//   PrintString(GetName() + " shaking animation");
		}
	}

	override void PlaySound()
	{
		//PrintString(GetName() + " sound");
	}
	
	override protected float GetObservedValue()
	{
		return m_Player.GetStatTemperature().Get();
	}
};