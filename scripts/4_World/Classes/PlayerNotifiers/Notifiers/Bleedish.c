class Bleedish: NotifierBase
{
	void Bleedish(NotifiersManager manager)
	{
		m_TendencyBufferSize = 8;
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
		return NTF_BLEEDISH;
	}


	override void DisplayBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_BLEEDING,true);
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_BLOOD,DELM_LVL_1);
	}

	override void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_BLEEDING,false);
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_BLOOD,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
		m_Player.MessageImportant("I feel warm blood on my clothes");
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