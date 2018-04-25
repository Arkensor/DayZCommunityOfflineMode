/**@class		OFSMBase
 * @brief		base class for Orthogonal Finite State Machine
 *
 * stores current states (m_states) and transition table with possible transitions from each state
 * to another state via event
 **/
class OFSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
	protected ref array<ref FSMStateBase> m_States; /// current fsm state
	protected ref array<ref FSMStateBase> m_InitialStates; /// configurable initial state of the machine
	protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_Transitions; /// fsm transition table

	void OFSMBase ()
	{
		m_States = new array<ref FSMStateBase>;
		m_InitialStates = new array<ref FSMStateBase>;
		m_Transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
	}

	/**@fn			GetCurrentStates
	 * @brief		returns currently active state
	 * @return		current state the FSM is in (or NULL)
	 **/
	array<ref FSMStateBase> GetCurrentState ()
	{
		return m_States;
	}

	/**@fn			SetInitialState
	 * @brief		sets the initial_state for starting the machine
	 **/
	void SetInitialStates (array<ref FSMStateBase> initial_states)
	{
		m_InitialStates = initial_states;

		for (int s = 0; s < initial_states.Count(); ++s)
			m_States.Insert(initial_states[s]);
	}

	/**@fn			Start
	 * @brief		starts the state machine by entering the initial_state (using intial_event as argument to initial state's onEntry)
	 * @param[in]	e \p	optional event for starting the machind
	 **/
	void Start (array<ref FSMEventBase> initial_events = null)
	{
		fsmbDebugPrint("[ofsm] " + this.ToString() + "::Start(" + initial_events.ToString() + "), init_state=" + m_InitialStates.ToString());

		for (int s = 0; s < m_States.Count(); ++s)
		{
			m_States[s] = m_InitialStates[s];

			if (initial_events)
				m_States[s].OnEntry(initial_events[s]);
			else
				m_States[s].OnEntry(null);
		}
	}

	/**@fn			IsRunning
	 * @brief		returns true if machine is in running state
	 **/
	bool IsRunning ()
	{
		int sc = m_States.Count();
		if (sc)
		{
			for (int s = 0; s < sc; ++s)
				if (m_States[s] != null)
					return true;
		}
		return false;
	}

	/**@fn			Terminate
	 * @brief		terminates the state machine
	 **/
	void Terminate (array<ref FSMEventBase> terminal_events = null)
	{
		if (IsRunning())
		{
			for (int s = 0; s < m_States.Count(); ++s)
			{
				if (terminal_events)
					m_States[s].OnExit(terminal_events[s]);
				else
					m_States[s].OnExit(null);

				m_States[s] = null;
			}
		}
	}

	/**@fn			Update
	 * @brief		if machine running, call OnUpdate() on current state
	 **/
	void Update (float dt)
	{
		if (IsRunning())
		{
			for (int s = 0; s < m_States.Count(); ++s)
			{
				m_States[s].OnUpdate(dt);
			}
		}
	}
	
	/**@fn			AddTransition
	 * @brief		adds transition into transition table
	 **/
	void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
	{
		m_Transitions.Insert(t);
	}

	/**@fn			ProcessEvent
	 * @brief		instructs the state machine to process the event e
	 * @param[in]	e	\p	event that will be used to find suitable transition from current state
	 * @return		FSM_OK if transition found and allowed by guard (if any)
	 **/
	ProcessEventResult ProcessEvent (FSMEventBase e)
	{
		int count = m_Transitions.Count();
		for (int i = 0; i < count; ++i)
		{
			FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(i);
			if (row.m_event.Type() == e.Type())
			{
				for (int s = 0; s < m_States.Count(); ++s)
				{
					if (row.m_srcState.Type() == m_States[s].Type() && row.m_event.Type() == e.Type())
					{
						FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
						bool hasGuard = t.m_guard != NULL;
						if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e)))		// 1) exec guard (if any)
						{
							ProcessLocalTransition(s, t, e); // 2) process transition allowed by guard
						}
					}
				}
			}
		}
		return ProcessEventResult.FSM_NO_TRANSITION;
	}

	/**@fn			ProcessLocalTransition
	 * @brief		instructs the state machine to process the event locally - no hierarchy is crossed
	 * @param[in]	t	\p	the transition in m_transitions
	 * @param[in]	e	\p	event that will be used to process transition from current state
	 * @return		FSM_OK or FSM_TERMINATED
	 **/
	protected ProcessEventResult ProcessLocalTransition (int s, FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
	{
		fsmbDebugPrint("[ofsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());

		m_States[s].OnExit(e);			// 1) call onExit on old state

		if (t.m_action)
			t.m_action.Action(e);	// 2) execute transition action (if any)

		m_States[s] = t.m_dstState;		// 3) change state to new

		if (t.m_dstState != NULL)
		{
			m_States[s].OnEntry(e);		// 4a) call onEntry on new state
			return ProcessEventResult.FSM_OK;
		}
		else
		{
			fsmbDebugPrint("[ofsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
			return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
		}
	}
};

