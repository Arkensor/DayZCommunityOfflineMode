class SardinesCan : Edible_Base
{
	void SardinesCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		m_Opened = true;
		ReplaceEdibleWithNew("SardinesCan_Opened");
	}
}
