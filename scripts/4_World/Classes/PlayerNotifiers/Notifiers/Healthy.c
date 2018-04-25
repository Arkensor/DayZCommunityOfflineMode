class Healthy: NotifierBase
{
	/*
	private const float	 	HEALTHY_BLOOD_TRESHOLD	= 5000;
	private const float	 	HEALTHY_TRESHOLD		= 5000;
	private const float	 	HEALING_ENERGY_TRESHOLD	= 4000;
	private const float	 	HEALING_WATER_TRESHOLD	= 2500;
	private const float	 	HEALING_BLOOD_TRESHOLD	= 5000;
	*/
	private const float 	DEC_TRESHOLD_LOW 			= -0.0001;
	private const float 	DEC_TRESHOLD_MED 			= -0.7;
	private const float 	DEC_TRESHOLD_HIGH			= -1.3;
	private const float 	INC_TRESHOLD_LOW 			= 0.0001;
	private const float 	INC_TRESHOLD_MED 			= 0.7;
	private const float 	INC_TRESHOLD_HIGH			= 1.3;
	
		
	void Healthy(NotifiersManager manager)
	{
		m_TickInterval = 3000;
		m_TendencyBufferSize = 6;
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
	}

	override int GetNotifierType()
	{
		return NTF_HEALTHY;
	}

	override void DisplayBadge()
	{
		/*
		float energy = m_Player.GetStatEnergy().Get();
		float water = m_Player.GetStatWater().Get();
		float blood = m_Player.GetHealth("", "Blood");
		float health = m_Player.GetHealth("", "Health");
		if ( energy >= HEALING_ENERGY_TRESHOLD && water >= HEALING_WATER_TRESHOLD && blood >= HEALING_BLOOD_TRESHOLD && health < HEALTHY_TRESHOLD ) 
		{
			////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HEALTHY,DELM_LVL_1);
			
		}
		else if ( blood >= HEALTHY_BLOOD_TRESHOLD && health >= HEALTHY_TRESHOLD )
		{
			////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HEALTHY,DELM_LVL_2);
		}
		*/
	}
	
	override void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		//PrintString("tendency:" + tendency);
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_TDCY_HEALTH,tendency);
		
		DSLevels level = DetermineLevel( GetObservedValue(), ModifierConstants.THRESHOLD_HEALTH_WARNING, ModifierConstants.THRESHOLD_HEALTH_CRITICAL, ModifierConstants.THRESHOLD_HEALTH_EMPTY);
		GetDisplayStatus().SetLevel(eDisplayElements.DELM_TDCY_HEALTH, level);
	}

	override void HideBadge()
	{
		
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_HEALTHY,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
	}

	override void PlayAnimation()
	{
	//	PrintString(GetName() + " animation");
	}	

	override void PlaySound()
	{
	//	PrintString(GetName() + " sound");
	}
	
	override protected float GetObservedValue()
	{
		float health = m_Player.GetHealth("","Health");
		//PrintString("health:" + health);
		return health;
	}
	

};