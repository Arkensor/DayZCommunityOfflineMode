class Compass : ItemCompass
{
	void Compass()
	{
		m_Opened = false;
	}

	override void Open()
	{
		m_Opened = true;
		SetAnimationPhase("cover",1);
		//super.Open();
	}

	override void Close()
	{
		m_Opened = false;
		SetAnimationPhase("cover",0);
		//super.Close();
	}
}
