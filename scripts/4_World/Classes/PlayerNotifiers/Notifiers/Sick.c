class Sick: NotifierBase
{
	void Sick(NotifiersManager manager)
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
		return NTF_SICK;
	}

	override void DisplayBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_SICK,true);
		//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_SICK,DELM_LVL_1);
	}

	override void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_SICK,false);
		//GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_SICK,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		m_Player.MessageStatus("I feel sick");
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