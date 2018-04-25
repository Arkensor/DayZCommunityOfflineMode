class HumanSteakMeat extends Edible_Base
{
	void HumanSteakMeat()
	{
		InsertAgent(AGT_BRAIN, 1);
	}
	override bool IsMeat()
	{
		return true;
	}
}
