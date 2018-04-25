///@{ states

enum HandStateID
{
	UNKNOWN				=  0,
	Empty				=  1,
	Equipped			=  2,
	Locked				=  3,
	Unlocked			=  4,
}

/**@class	HandStateBase
 * @brief	represent hand state base
 *
 * Class comes with entry/update/exit hooks that can be overriden in custom states
 **/
class HandStateBase
{
	Man m_Player; /// entity that this state relates to
	void HandStateBase (Man player = null) { m_Player = player; }

	/**@fn			OnEntry
	 * @brief		called upon entry to state
	 * @param[in]	e		the event that triggered transition to this state
	 **/
	void OnEntry (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] { " + this.Type().ToString());
	}
	/**@fn			OnUpdate
	 * @brief		ongoing behavior, performed while being in the state
	 * @NOTE:		this is supposed to be the Do() operation in UML speak
	 **/
	void OnUpdate (float dt)
	{ }
	/**@fn			OnAbort
	 * @brief		called when abort signal arrives
	 * @param[in]	e		the event that triggered abort from this state
	 **/
	void OnAbort (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] } ABORTED " + this.Type().ToString());
	}
	/**@fn			OnExit
	 * @brief		called on exit from state
	 * @param[in]	e		the event that triggered transition from this state
	 **/
	void OnExit (HandEventBase e)
	{
		hndDebugPrint("[hndfsm] } " + this.Type().ToString());
	}

	/**@fn			IsWaitingForActionFinish
	 * @brief		waiting for active animation action/actionType finish
	 * @return		true if this state is waiting for finish signal
	 **/
	bool IsWaitingForActionFinish () { return false; }

	/**@fn			IsIdle
	 * @brief		idle state does not expect any animation events
	 * @return		true if this state is idle
	 **/
	bool IsIdle () { return false; }

	int GetCurrentStateID () { return 0; }
};


class HandStateEmpty : HandStateBase
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Empty; }
};

class HandStateEquipped : HandStateBase
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Equipped; }
};

class HandStateLocked : HandStateBase
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Locked; }
};

class HandStateUnlocked : HandStateBase
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Unlocked; }
};

///@} states


