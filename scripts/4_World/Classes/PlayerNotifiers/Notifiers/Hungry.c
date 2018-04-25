class Hungry: NotifierBase
{
	protected const float	ENERGIZED_TRESHOLD			= 2000;
	protected const float	HUNGRY_TRESHOLD 	 		= 1000;
	protected const float	VERY_HUNGRY_TRESHOLD	 	= 500;
	protected const float	FATALLY_HUNGRY_TRESHOLD 	= 0;
	protected const float 	DEC_TRESHOLD_LOW 			= -0.01;
	protected const float 	DEC_TRESHOLD_MED 			= -0.35;
	protected const float 	DEC_TRESHOLD_HIGH			= -0.57;
	protected const float 	INC_TRESHOLD_LOW 			= 0.01;
	protected const float 	INC_TRESHOLD_MED 			= 0.35;
	protected const float 	INC_TRESHOLD_HIGH			= 0.57;
	
	void Hungry(NotifiersManager manager)
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
		return NTF_HUNGRY;
	}

	override void DisplayTendency(float delta)
	{
		//PrintString("delta:"+delta.ToString());
		int tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_TDCY_ENERGY,tendency);
		
		DSLevels level = DetermineLevel( GetObservedValue(), ModifierConstants.THRESHOLD_ENERGY_WARNING, ModifierConstants.THRESHOLD_ENERGY_CRITICAL, ModifierConstants.THRESHOLD_ENERGY_EMPTY);
		GetDisplayStatus().SetLevel(eDisplayElements.DELM_TDCY_ENERGY, level);
	}

	override void DisplayBadge()
	{
		float energy = m_Player.GetStatEnergy().Get();
		if (energy >= ENERGIZED_TRESHOLD) 
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HUNGER,DELM_LVL_4);
		}
		else if (energy <= HUNGRY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HUNGER,DELM_LVL_3);
		}
		else if (energy <= VERY_HUNGRY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HUNGER,DELM_LVL_2);
		}
		else if (energy <= FATALLY_HUNGRY_TRESHOLD)
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HUNGER,DELM_LVL_1);
		}
	}

	override void HideBadge()
	{
		}

	override void DisplayMessage()
	{
		float energy = m_Player.GetStatEnergy().Get();
		if ( energy <= HUNGRY_TRESHOLD )
		{
			m_Player.MessageStatus("My stomach grumbles");
		}
		else if ( energy <= VERY_HUNGRY_TRESHOLD )
		{
			m_Player.MessageStatus("I'm extremely hungry");
		}
		else if ( energy <= FATALLY_HUNGRY_TRESHOLD )
		{
			m_Player.MessageImportant("I'm dying of starvation");
		}
	}

	override void PlayAnimation()
	{
		float energy = m_Player.GetStatEnergy().Get();
		if ( energy <= VERY_HUNGRY_TRESHOLD )
		{
			//PrintString(GetName() + " grumbly animation");
		}
		else if ( energy <= FATALLY_HUNGRY_TRESHOLD )
		{
			//PrintString(GetName() + " hunger dying animation");
		}
	}

	override void PlaySound()
	{
		//GetGame().CreateSoundOnObject(m_Player, "Character_Mad", 3, false);
	}
	
	override float GetObservedValue()
	{
		return m_Player.GetStatEnergy().Get();
	}
};