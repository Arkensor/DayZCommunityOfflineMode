class ActionDrinkCan: ActionDrink
{
	void ActionDrinkCan()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINKCAN;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_DRINKCAN;
	}
	
	override int GetType()
	{
		return AT_DRINK_CAN;
	}
	
	override string GetText()
	{
		return "drink can";
	}
};
