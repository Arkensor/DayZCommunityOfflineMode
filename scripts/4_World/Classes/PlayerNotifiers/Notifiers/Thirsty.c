class Thirsty: NotifierBase
{
	private const float 	HYDRATED_TRESHOLD 			= 3500;
	private const float 	THIRSTY_TRESHOLD 			= 2500;
	private const float 	VERY_THIRSTY_TRESHOLD 		= 1500;
	private const float 	FATALLY_THIRSTY_TRESHOLD 	= 0;
	private const float 	DEC_TRESHOLD_LOW 			= -0.01;
	private const float 	DEC_TRESHOLD_MED 			= -0.2;
	private const float 	DEC_TRESHOLD_HIGH			= -0.85;
	private const float 	INC_TRESHOLD_LOW 			= 0.01;
	private const float 	INC_TRESHOLD_MED 			= 0.2;
	private const float 	INC_TRESHOLD_HIGH			= 0.85;
		
	void Thirsty(NotifiersManager manager)
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
		return NTF_THIRSTY;
	}

	
	override void DisplayTendency(float delta)
	{
		//PrintString(delta.ToString());
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_TDCY_WATER,tendency);
		
		DSLevels level = DetermineLevel( GetObservedValue(), ModifierConstants.THRESHOLD_WATER_WARNING, ModifierConstants.THRESHOLD_WATER_CRITICAL, ModifierConstants.THRESHOLD_WATER_EMPTY);
		GetDisplayStatus().SetLevel(eDisplayElements.DELM_TDCY_WATER, level);
	}
	
	override void DisplayBadge()
	{
		float water = m_Player.GetStatWater().Get();
		if (water >= HYDRATED_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_THIRST,DELM_LVL_1);
		}
		else if (water <= THIRSTY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_THIRST,DELM_LVL_2);
		}
		else if (water <= VERY_THIRSTY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_THIRST,DELM_LVL_3);
		}
		else if (water <= FATALLY_THIRSTY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_THIRST,DELM_LVL_4);
		}
	}

	override void HideBadge()
	{
		
		//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_THIRST,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		float water = m_Player.GetStatWater().Get();
		if ( water <= THIRSTY_TRESHOLD )
		{
			m_Player.MessageStatus("I feel thirsty");
		}
		else if ( water <= VERY_THIRSTY_TRESHOLD )
		{
			m_Player.MessageStatus("I really need to drink");
		}
		else if ( water <= FATALLY_THIRSTY_TRESHOLD )
		{
			m_Player.MessageImportant("I'm dying of dehydration");
		}
	}

	override void PlayAnimation()
	{
		float water = m_Player.GetStatWater().Get();
		if ( water <= VERY_THIRSTY_TRESHOLD )
		{
		//	PrintString(GetName() + " grumbly animation");
		}
		else if ( water <= FATALLY_THIRSTY_TRESHOLD )
		{
		//	PrintString(GetName() + " thirst dying animation");
		}
	}
	
	override void PlaySound()
	{
	//	PrintString(GetName() + " sound");
	}
	
	override protected float GetObservedValue()
	{
		return m_Player.GetStatWater().Get();
	}
};