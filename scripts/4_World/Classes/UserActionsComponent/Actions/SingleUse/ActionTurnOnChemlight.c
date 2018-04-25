class ActionTurnOnChemlight: ActionTurnOnWhileInHands
{
	void ActionTurnOnChemlight()
	{
		m_MessageSuccess = "I broke chemlight.";
		m_MessageFail = "It's ran out.";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
	}

	override int GetType()
	{
		return AT_TURN_ON_CHEMLIGHT;
	}

	override string GetText()
	{
		return "break";
	}
};