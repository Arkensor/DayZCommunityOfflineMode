class TunaCan : Edible_Base
{
	void TunaCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		m_Opened = true;

		ReplaceEdibleWithNew("TunaCan_Opened");
	}
}
