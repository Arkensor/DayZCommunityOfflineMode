class TacticalBaconCan : Edible_Base
{
	void TacticalBaconCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		m_Opened = true;
		ReplaceEdibleWithNew("TacticalBaconCan_Opened");
	}
}
