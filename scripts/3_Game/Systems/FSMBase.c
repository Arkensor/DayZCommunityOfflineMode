void fsmbDebugPrint (string s)
{
	//Print("" + s); // comment/uncomment to hide/see debug logs
}
void fsmbDebugSpam (string s)
{
	//Print("" + s); // comment/uncomment to hide/see debug spam
}


/**@class	FSMTransition
 * @brief	represents transition src ---- event[guard]/action ----|> dst
 **/
class FSMTransition<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
	ref FSMStateBase m_srcState;
	ref FSMEventBase m_event; // @NOTE: NULL event means "completion transition" in UML speak
	ref FSMStateBase m_dstState; // @NOTE: NULL dst state == UML terminate pseudonode
	ref FSMActionBase m_action;
	ref FSMGuardBase m_guard;

	void FSMTransition (FSMStateBase src, FSMEventBase e, FSMStateBase dst, FSMActionBase a = NULL, FSMGuardBase g = NULL)
	{
		m_srcState = src;
		m_event = e;
		m_dstState = dst;
		m_action = a;
		m_guard = g;
	}
};

enum ProcessEventResult
{
	FSM_OK,
	FSM_TERMINATED,
	FSM_ABORTED,
	FSM_NO_TRANSITION,
};

/**@class		FSMBase
 * @brief		base class for finite state machine
 *
 * stores current state (m_state) and transition table with possible transitions from each state
 * to another state via event
 **/
class FSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
	protected ref FSMStateBase m_state; /// current fsm state
	protected ref FSMStateBase m_initialState; /// configurable initial state of the machine
	protected ref FSMEventBase m_initialEvent; /// configurable initial event to start the machine (null by default)
	protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_transitions; /// fsm transition table

	void FSMBase ()
	{
		m_transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
	}

	/**@fn			GetCurrentState
	 * @brief		returns currently active state
	 * @return		current state the FSM is in (or NULL)
	 **/
	FSMStateBase GetCurrentState ()
	{
		return m_state;
	}

	/**@fn			SetInitialState
	 * @brief		sets the initial_state for starting the machine
	 **/
	void SetInitialState (FSMStateBase initial_state)
	{
		m_initialState = initial_state;
	}

	/**@fn			Start
	 * @brief		starts the state machine by entering the initial_state (using intial_event as argument to initial state's onEntry)
	 * @param[in]	e \p	optional event for starting the machind
	 **/
	void Start (FSMEventBase initial_event = NULL)
	{
		fsmbDebugPrint("[fsm] " + this.ToString() + "::Start(" + initial_event.ToString() + "), init_state=" + m_initialState.ToString());

		m_state = m_initialState;
		m_state.OnEntry(initial_event);
	}

	/**@fn			IsRunning
	 * @brief		returns true if machine is in running state
	 **/
	bool IsRunning () { return m_state != NULL; }

	/**@fn			Terminate
	 * @brief		terminates the state machine
	 **/
	void Terminate (FSMEventBase terminal_event = NULL)
	{
		if (IsRunning())
		{
			m_state.OnExit(terminal_event);
			m_state = NULL;
		}
	}

	/**@fn			Update
	 * @brief		if machine running, call OnUpdate() on current state
	 **/
	void Update (float dt)
	{
		if (IsRunning())
			m_state.OnUpdate(dt);
	}
	
	/**@fn			AddTransition
	 * @brief		adds transition into transition table
	 **/
	void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
	{
		m_transitions.Insert(t);
	}

	/**@fn			ProcessEvent
	 * @brief		instructs the state machine to process the event e
	 * @param[in]	e	\p	event that will be used to find suitable transition from current state
	 * @return		FSM_OK if transition found and allowed by guard (if any)
	 **/
	ProcessEventResult ProcessEvent (FSMEventBase e)
	{
		FSMStateBase curr_state = m_state;

		int count = m_transitions.Count();
		for (int i = 0; i < count; ++i)
		{
			FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_transitions.Get(i);
			if (row.m_srcState.Type() == curr_state.Type() && row.m_event.Type() == e.Type())
			{
				FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_transitions.Get(i);
				bool hasGuard = t.m_guard != NULL;
				if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e)))		// 1) exec guard (if any)
				{
					ProcessLocalTransition(t, e); // 2) process transition allowed by guard
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
	protected ProcessEventResult ProcessLocalTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
	{
		fsmbDebugPrint("[fsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());

		m_state.OnExit(e);			// 1) call onExit on old state

		if (t.m_action)
			t.m_action.Action(e);	// 2) execute transition action (if any)

		m_state = t.m_dstState;		// 3) change state to new

		if (t.m_dstState != NULL)
		{
			m_state.OnEntry(e);		// 4a) call onEntry on new state
			return ProcessEventResult.FSM_OK;
		}
		else
		{
			fsmbDebugPrint("[fsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
			return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
		}
	}
};

