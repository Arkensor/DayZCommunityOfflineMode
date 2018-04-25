class PeachesCan : Edible_Base
{
	void PeachesCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		m_Opened = true;
		ReplaceEdibleWithNew("PeachesCan_Opened");
	}
}
