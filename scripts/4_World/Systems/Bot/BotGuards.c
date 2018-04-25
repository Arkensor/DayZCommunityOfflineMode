/**@class	BotGuardBase
 * @brief	represents guard on a transition from state to state
 **/
class BotGuardBase
{
	/**@fn		GuardCondition
	 * @brief enable or disable transition based on condition
	 * the guard is a boolean operation executed first and which can prevent the transition from firing by returning false
	 * @return	true if transition is allowed
	 **/
	bool GuardCondition (BotEventBase e) { return true; }
};

class BotGuardAnd extends BotGuardBase
{
	ref BotGuardBase m_arg0;
	ref BotGuardBase m_arg1;

	void BotGuardAnd (BotGuardBase arg0 = NULL, BotGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (BotEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) && m_arg1.GuardCondition(e);
		botDebugPrint("[botfsm] guard - " + m_arg0.Type() + " && " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class BotGuardNot extends BotGuardBase
{
	ref BotGuardBase m_arg0;

	void BotGuardNot (BotGuardBase arg0 = NULL) { m_arg0 = arg0; }

	override bool GuardCondition (BotEventBase e)
	{
		bool result = !m_arg0.GuardCondition(e);
		botDebugPrint("[botfsm] guard - ! " + m_arg0.Type() + " = " + result);
		return result;
	}
};

class BotGuardOr extends BotGuardBase
{
	ref BotGuardBase m_arg0;
	ref BotGuardBase m_arg1;

	void BotGuardOr (BotGuardBase arg0 = NULL, BotGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (BotEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) || m_arg1.GuardCondition(e);
		botDebugPrint("[botfsm] guard - " + m_arg0.Type() + " || " + m_arg1.Type() + " = " + result);
		return result;
	}
};

