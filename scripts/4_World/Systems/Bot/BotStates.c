/**@class	BotStateBase
 * @brief	represent weapon state base
 *
 * Class comes with entry/update/exit hooks that can be overriden in custom states
 *
 * Class is ready for hierarchic composition, i.e. this state having a sub-machine running
 * under hood. If no m_FSM member is configured, class acts as ordinary plain
 * finite machine state.
 **/
class BotStateBase
{
	PlayerBase m_Owner; /// man that this state belongs to
	Bot m_Bot; /// bot that this state belongs to
	BotStateBase m_ParentState; /// hierarchical parent state of this state (or null)
	ref BotFSM m_FSM; /// nested state machine (or null)
	void BotStateBase (Bot bot = NULL, BotStateBase parent = NULL) { m_Bot = bot; m_Owner = bot.m_Owner; m_ParentState = parent; }

	/**@fn		SetParentState
	 * @brief	allows construction of hierarchical state machine
	 **/
	void SetParentState (BotStateBase parent) { m_ParentState = parent; }
	/**@fn		GetParentState
	 * @return	state that owns this sub-state (or null if plain state)
	 **/
	BotStateBase GetParentState () { return m_ParentState; }

	bool HasFSM () { return m_FSM != NULL; }
	BotFSM GetFSM () { return m_FSM; }

	bool ProcessEvent (BotEventBase e)
	{
		if (HasFSM())
			return m_FSM.ProcessEvent(e);
		return false;
	}

	/**@fn		AddTransition
	 * @brief	adds transition into m_FSM transition table
	 **/
	void AddTransition (FSMTransition<BotStateBase, BotEventBase, BotActionBase, BotGuardBase> t)
	{
		if (HasFSM())
			m_FSM.AddTransition(t);
		else
			Error("[botfsm] adding transition to state without FSM. Configure FSM first.");
	}


	/**@fn		OnEntry
	 * @brief	called upon entry to state
	 * @NOTE	if state has (non-running) sub-machine, it's started on entry
	 * @param[in] e	the event that triggered transition to this state
	 **/
	void OnEntry (BotEventBase e)
	{
		if (HasFSM() && !m_FSM.IsRunning())
		{
			botDebugPrint("[botfsm] { " + this.Type().ToString() + "  Has Sub-FSM! Starting submachine...");
			m_FSM.Start(e);
		}
		else
			botDebugPrint("[botfsm] { " + this.Type().ToString());
	}

	/**@fn		OnUpdate
	 * @brief ongoing behavior, performed while being in the state
	 *
	 * @NOTE: this is supposed to be the Do() operation in UML speak
	 **/
	void OnUpdate (float dt)
	{
		if (HasFSM() && m_FSM.IsRunning())
			m_FSM.GetCurrentState().OnUpdate(dt);
	}

	/**@fn		OnAbort
	 * @brief called when abort signal arrives
	 * @param[in] e	the event that triggered abort from this state
	 **/
	void OnAbort (BotEventBase e)
	{
		if (HasFSM() && m_FSM.IsRunning())
		{
			botDebugPrint("[botfsm] OnAbort " + this.Type().ToString() + "  Has Sub-FSM! Aborting submachine...");
			m_FSM.Abort(e);
		}
		botDebugPrint("[botfsm] } ABORTED " + this.Type().ToString());
	}

	/**@fn		OnExit
	 * @brief called on exit from state
	 * @param[in] e	the event that triggered transition from this state
	 **/
	void OnExit (BotEventBase e)
	{
		botDebugPrint("[botfsm] } " + this.Type().ToString());
	}

	/**@fn		IsWaitingForActionFinish
	 * @brief	waiting for active animation action/actionType finish
	 * @return true if this state or active substate is waiting for finish signal
	 **/
	bool IsWaitingForActionFinish () { return HasFSM() && m_FSM.IsRunning() && m_FSM.GetCurrentState().IsWaitingForActionFinish(); }

	/**@fn		IsIdle
	 * @brief	idle state does not expect any animation events
	 * @return	true if this state is idle
	 **/
	bool IsIdle () { return false; }

	/**@fn		OnSubMachineChanged
	 * @brief	called when sub-machine has changed its state
	 * @param[in]	src		from state (previous)
	 * @param[in]	dst		to state (current)
	 **/
	void OnSubMachineChanged (BotStateBase src, BotStateBase dst) { }
};

class BotStateIdle : BotStateBase
{
	void BotStateIdle (Bot bot = NULL, BotStateBase parent = NULL) { m_Bot = bot; m_Owner = m_Bot.m_Owner; m_ParentState = parent; }

	override bool IsIdle () { return true; }
};
