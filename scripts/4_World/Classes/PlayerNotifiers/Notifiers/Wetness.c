class Wetness: NotifierBase
{
	private const float	 	DRENCHED_TRESHOLD	= 0.8;
	private const float	 	SOAKED_TRESHOLD		= 0.5;
	private const float	 	WET_TRESHOLD		= 0.25;
	private const float	 	DAMP_TRESHOLD		= 0.05;
		
	void Wetness(NotifiersManager manager)
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
		return NTF_WETNESS;
	}

	override void DisplayBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_WET,true);
		PlayerStat<float> wet_stat = m_Player.GetStatWet();
		float wetness = 0;
		
		if (wet_stat)
			wetness = wet_stat.Get();
	
		if ( wetness >= DRENCHED_TRESHOLD ) 
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_WETNESS,DELM_LVL_4); 
		}
		else if ( wetness >= SOAKED_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_WETNESS,DELM_LVL_3); 
		}
		else if ( wetness >= WET_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_WETNESS,DELM_LVL_2); 
		}
		else if ( wetness >= DAMP_TRESHOLD )
		{
			//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_WETNESS,DELM_LVL_1); 
		}
	}

	override void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_WET,false );
		//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_WETNESS,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		if (!m_Player)
			return;
		
		PlayerStat<float> wet_stat = m_Player.GetStatWet();
		float wetness = 0;
		
		if (wet_stat)
		{
			wetness = wet_stat.Get();
		}
		
		if ( wetness >= DRENCHED_TRESHOLD ) 
		{
			m_Player.MessageStatus("I am completely drenched");
		}
		else if ( wetness >= SOAKED_TRESHOLD )
		{
			m_Player.MessageStatus("I am soaked through");
		}
		else if ( wetness >= WET_TRESHOLD )
		{
			m_Player.MessageStatus("My body feels wet");
		}
		else if ( wetness >= DAMP_TRESHOLD )
		{
			m_Player.MessageStatus("I feel damp");
		}
	}

	override void PlayAnimation()
	{
		//PrintString(GetName() + " animation");
	}	

	override void PlaySound()
	{
		//PrintString(GetName() + " sound");
	}
};