class SpaghettiCan : Edible_Base
{
	void SpaghettiCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		m_Opened = true;
		ReplaceEdibleWithNew("SpaghettiCan_Opened");
	}
}
