class FracturedArms: NotifierBase
{
	private const float	 	FRACTURED_TRESHOLD	= 0;
	private const float	 	CHIPPED_TRESHOLD	= 40;
	private const float	 	SPRAINED_TRESHOLD	= 80;
		
	void FracturedArms(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
		m_Active = false; 
	}


	override int GetNotifierType()
	{
		return NTF_FRACTURED_ARMS;
	}


	override void DisplayBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_FRACTURE,true);
		
		float damage = Math.Min(m_Player.GetHealth("RightArm","Health"),m_Player.GetHealth("LeftArm","Health"));
		if ( damage <= FRACTURED_TRESHOLD ) 
		{
			////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FRACTURE,DELM_LVL_1);
		}
		else if ( damage <= CHIPPED_TRESHOLD )
		{
			////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FRACTURE,DELM_LVL_2);
		}
		else if ( damage <= SPRAINED_TRESHOLD )
		{
			////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FRACTURE,DELM_LVL_3);
		}
	}

	override void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_FRACTURE,false);
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FRACTURE,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		float damage = Math.Min(m_Player.GetHealth("RightArm","Health"),m_Player.GetHealth("LeftArm","Health"));
		if ( damage <= FRACTURED_TRESHOLD ) 
		{
			m_Player.MessageImportant("I think my leg is broken");
		}
		else if ( damage <= CHIPPED_TRESHOLD )
		{
			m_Player.MessageStatus("My leg hurts");
		}
		else if ( damage <= SPRAINED_TRESHOLD )
		{
			m_Player.MessageStatus("My leg is painful");
		}
	}

	override void PlayAnimation()
	{
	//	PrintString(GetName() + " animation");
	}	

	override void PlaySound()
	{
	//	PrintString(GetName() + " sound");
	}
};