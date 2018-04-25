class Stuffed: NotifierBase
{
	void Stuffed(NotifiersManager manager)
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
		return NTF_STUFFED;
	}

	override void DisplayBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_STUFFED,true);
	}

	override void HideBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_STUFFED,false);
	}

	override void DisplayMessage()
	{
	}

	override void PlayAnimation()
	{
	}	

	override void PlaySound()
	{
	}
};