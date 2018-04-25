class BakedBeansCan : Edible_Base
{
	void BakedBeansCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{	
		m_Opened = true;
		ReplaceEdibleWithNew("BakedBeansCan_Opened");
	}
}
