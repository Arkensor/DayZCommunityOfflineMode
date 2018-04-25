/**@class	WeaponStateBase
 * @brief	represent weapon state base
 *
 * Class comes with entry/update/exit hooks that can be overriden in custom states
 *
 * Class is ready for hierarchic composition, i.e. this state having a sub-machine running
 * under hood. If no m_fsm member is configured, class acts as ordinary plain
 * finite machine state.
 **/
class WeaponStateBase
{
	Weapon_Base m_weapon; /// weapon that this state relates to
	WeaponStateBase m_parentState; /// hierarchical parent state of this state (or null)
	ref WeaponFSM m_fsm; /// nested state machine (or null)
	void WeaponStateBase (Weapon_Base w = NULL, WeaponStateBase parent = NULL) { m_weapon = w; m_parentState = parent; }

	/**@fn		SetParentState
	 * @brief	allows construction of hierarchical state machine
	 **/
	void SetParentState (WeaponStateBase parent) { m_parentState = parent; }
	/**@fn		GetParentState
	 * @return	state that owns this sub-state (or null if plain state)
	 **/
	WeaponStateBase GetParentState () { return m_parentState; }

	bool HasFSM () { return m_fsm != NULL; }
	WeaponFSM GetFSM () { return m_fsm; }

	bool ProcessEvent (WeaponEventBase e)
	{
		if (HasFSM())
			return m_fsm.ProcessEvent(e);
		return false;
	}
	/**@fn		AddTransition
	 * @brief	adds transition into m_fsm transition table
	 **/
	void AddTransition (WeaponTransition t)
	{
		if (HasFSM())
			m_fsm.AddTransition(t);
		else
			Error("[wpnfsm] adding transition to state without FSM. Configure FSM first.");
	}


	/**@fn		OnEntry
	 * @brief	called upon entry to state
	 * @NOTE	if state has (non-running) sub-machine, it's started on entry
	 * @param[in] e	the event that triggered transition to this state
	 **/
	void OnEntry (WeaponEventBase e)
	{
		if (HasFSM() && !m_fsm.IsRunning())
		{
			wpnDebugPrint("[wpnfsm] { " + this.Type().ToString() + "  Has Sub-FSM! Starting submachine...");
			m_fsm.Start(e);
		}
		else
			wpnDebugPrint("[wpnfsm] { " + this.Type().ToString());
	}

	/**@fn		OnUpdate
	 * @brief ongoing behavior, performed while being in the state
	 *
	 * @NOTE: this is supposed to be the Do() operation in UML speak
	 **/
	void OnUpdate (float dt)
	{
		if (HasFSM() && m_fsm.IsRunning())
			m_fsm.GetCurrentState().OnUpdate(dt);
	}

	/**@fn		OnAbort
	 * @brief called when abort signal arrives
	 * @param[in] e	the event that triggered abort from this state
	 **/
	void OnAbort (WeaponEventBase e)
	{
		if (HasFSM() && m_fsm.IsRunning())
		{
			wpnDebugPrint("[wpnfsm] OnAbort " + this.Type().ToString() + "  Has Sub-FSM! Aborting submachine...");
			m_fsm.Abort(e);
		}
		wpnDebugPrint("[wpnfsm] } ABORTED " + this.Type().ToString());
	}

	/**@fn		OnExit
	 * @brief called on exit from state
	 * @param[in] e	the event that triggered transition from this state
	 **/
	void OnExit (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] } " + this.Type().ToString());
	}

	/**@fn		IsWaitingForActionFinish
	 * @brief	waiting for active animation action/actionType finish
	 * @return true if this state or active substate is waiting for finish signal
	 **/
	bool IsWaitingForActionFinish () { return HasFSM() && m_fsm.IsRunning() && m_fsm.GetCurrentState().IsWaitingForActionFinish(); }

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
	void OnSubMachineChanged (WeaponStateBase src, WeaponStateBase dst) { }
};


