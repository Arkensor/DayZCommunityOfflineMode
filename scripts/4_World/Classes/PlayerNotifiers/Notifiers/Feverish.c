class Feverish: NotifierBase
{
	void Feverish(NotifiersManager manager)
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
		return NTF_FEVERISH;
	}

	override void DisplayBadge()
	{
	//	Print("FEVER");
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FEVER,DELM_LVL_1);
	}

	override void HideBadge()
	{
		
		////GetDisplayStatus().SetStatus(eDisplayElements.DELM_NTFR_FEVER,DELM_LVL_0);
	}

	override void DisplayMessage()
	{
	 //   m_Player.MessageStatus("I'm burning up");
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