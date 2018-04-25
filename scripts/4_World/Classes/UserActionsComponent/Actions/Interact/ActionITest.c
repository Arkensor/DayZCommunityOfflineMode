class ActionITest: ActionInteractBase
{
	void ActionITest()
	{
		m_MessageSuccess = "I have taken one.";
		m_MessageStart = "I started taking onesss.";
		m_MessageStartFail = "Its' empty";
	}
	
	override int GetType()
	{
		return AT_ITEST;
	}

	override string GetText()
	{
		return "take one";
	}
};