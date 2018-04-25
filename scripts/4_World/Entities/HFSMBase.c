void fsmDebugPrint (string s)
{
	Print("" + s); // comment/uncomment to hide/see debug logs
}
void fsmDebugSpam (string s)
{
	//Print("" + s); // comment/uncomment to hide/see debug spam
}


/**@class		HFSMBase
 * @brief		base class for hierarchic finite state machine
 *
 * stores current state (m_State) and transition table with possible transitions from each state
 * to another state via event
 *
 * each state can have nested state machine, thus creating hierarchy
 **/
class HFSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
	protected ref FSMStateBase m_State; /// current fsm state
	protected FSMStateBase m_OwnerState; /// state that owns this fsm (or null if root)
	protected ref FSMStateBase m_InitialState; /// configurable initial state of the machine
	protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_Transitions; /// fsm transition table
	protected bool m_HasCompletions;

	void HFSMBase (FSMStateBase ownerState = NULL)
	{
		m_OwnerState = ownerState;
		m_Transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
		m_HasCompletions = false;
	}

	/**@fn		GetCurrentState
	 * @return	returns currently active state within this machine (i.e. not hierarchic state)
	 **/
	FSMStateBase GetCurrentState ()
	{
		return m_State;
	}
	/**@fn		GetOwnerState
	 * @return	returns state that is owner of this fsm submachine. returns null if this is a root machine.
	 **/
	FSMStateBase GetOwnerState ()
	{
		return m_OwnerState;
	}
	/**@fn		GetHierarchyPath
	 * @brief		returns hierarchic state (path to root) of a state
	 * @param[in] state \p state the path is starting
	 * @param[out] path  \p  current hierarchic state
	 * @return	true if current state returned in path argument, false otherwise
	 **/
	bool GetHierarchyPath (WeaponStateBase state, out ref array<WeaponStateBase> path)
	{
		WeaponStateBase curr = state;
		while (curr)
		{
		  path.Insert(curr);
		  curr = curr.GetParentState();
		}
		return path.Count() > 0;
	}

	/**@fn		SetInitialState
	 * @param[in] initial_state \p state the machine will be entered to after ::Start()
	 * @param[in] initial_event \p event that will be used to start the machine
	 **/
	void SetInitialState (FSMStateBase initial_state)
	{
		m_InitialState = initial_state;
	}
	/**@fn		SetTransitions
	 * @brief	sets the transition table at once (in case it's prepared ahead)
	 * @note	alternative is to use AddTransition one by one
	 **/
	void SetTransitions (array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> arr)
	{
		m_Transitions = arr;
	}
	/**@fn		AddTransition
	 * @brief	adds transition into transition table
	 **/
	void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
	{
		m_Transitions.Insert(t);
		//fsmDebugSpam("[hfsm] +++ ow=" + this.GetOwnerState() + " this=" + this + " t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
		if (t.m_event == NULL)
		{
			Print("Warning (performance): FSM " + this + " has completion transition for src=" + t.m_srcState + " ---NULL----|> dst=" + t.m_dstState);
			m_HasCompletions = true;
		}
	}

	/**@fn		Start
	 * @brief	starts the state machine by entering the initial_state (using intial_event as argument to initial state's onEntry)
	 * @param[in] e \p optional event for starting the machind
	 **/
	void Start (FSMEventBase initial_event = NULL)
	{
		fsmDebugPrint("[hfsm] " + this.ToString() + "::Start(" + initial_event.ToString() + "), init_state=" + m_InitialState.ToString());

		m_State = m_InitialState;
		m_State.OnEntry(initial_event);

		if (m_HasCompletions)
			ProcessCompletionTransitions();
	}
	/**@fn		IsRunning
	 * @brief	returns true if machine is in running state
	 **/
	bool IsRunning () { return m_State != NULL; }
	/**@fn		Terminate
	 * @brief	terminates the state machine
	 **/
	void Terminate (FSMEventBase terminal_event = NULL)
	{
		fsmDebugPrint("[hfsm] " + this.ToString() + "::Terminate(" + terminal_event.ToString() + ")");
		if (IsRunning())
		{
			m_State.OnExit(terminal_event);
			m_State = NULL;
		}
	}
	void Abort (FSMEventBase abort_event = NULL)
	{
		fsmDebugPrint("[hfsm] " + this.ToString() + "::Abort(" + abort_event.ToString() + ")");
		if (IsRunning())
		{
			m_State.OnAbort(abort_event);
			m_State = NULL;
		}
	}
	/**@fn		Update
	 * @brief	if machine running, call OnUpdate() on current state
	 **/
	void Update (float dt)
	{
		if (IsRunning())
			m_State.OnUpdate(dt);
	}

	protected ProcessEventResult ProcessAbortTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
	{
		fsmDebugPrint("[hfsm] (local abort) state=" + t.m_srcState.ToString() + "-------- ABORT event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());

		m_State.OnAbort(e);			// 1) call onAbort on old state

		if (t.m_action)
			t.m_action.Action(e);	// 2) execute transition action (if any)

		auto tmp = t.m_srcState.GetParentState();
		if (tmp == t.m_dstState.GetParentState())
		{
			m_State = t.m_dstState;		// 3) change state to new (or NULL)

			if (t.m_dstState != NULL)
			{
				m_State.OnEntry(e);		// 4a1) call onEntry on new state (see 4a2) )
				return ProcessEventResult.FSM_OK;
			}
			else
			{
				fsmDebugPrint("[hfsm] abort & terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
				return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
			}
		}
		else
		{
			m_State = NULL;
			return ProcessEventResult.FSM_ABORTED; // 4c) or signal abort to parent (with appropriate transition)
		}
	}

	/**@fn		FindAbortDestinationState
	 * @brief	i
	 * @param[in]	e	\p	event that will be used to find suitable transition from current state
	 * @return	FSM_OK if transition found and allowed by guard (if any)
	 * @return	FSM_NO_TRANSITION if no unguarded transition found in submachine or local machine
	 * @return	FSM_TERMINATED state machine has terminated
	 * @return	FSM_ABORTED state machine has aborted
	 **/
	FSMStateBase FindAbortDestinationState (FSMEventBase e)
	{
		if (GetOwnerState())
			fsmDebugPrint("[hfsm] SUB! " + GetOwnerState().Type().ToString() + "::FindAbortDestinationState(" + e.Type().ToString() + ")");
		else
			fsmDebugPrint("[hfsm] root::FindAbortDestinationState(" + e.Type().ToString() + ")");

		// 1) look in submachine first (if any)
		if (m_State.HasFSM())
		{
			HFSMBase<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> a = m_State.GetFSM();
			FSMStateBase abort_dst = a.FindAbortDestinationState(e);

			if (abort_dst)
			{
				return abort_dst;
			}
		}

		// 2) local transitions
		int i = FindFirstUnguardedTransition(e);
		if (i == -1)
		{
			fsmDebugPrint("[hfsm] abort event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
			return NULL;
		}

		FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
		return t.m_dstState;
	}
	
	/**@fn		ProcessAbortEvent
	 * @brief	instructs the hierarchical state machine to process the event e
	 * @param[in]	e	\p	event that will be used to find suitable transition from current state
	 * @return	FSM_OK if transition found and allowed by guard (if any)
	 * @return	FSM_NO_TRANSITION if no unguarded transition found in submachine or local machine
	 * @return	FSM_TERMINATED state machine has terminated
	 * @return	FSM_ABORTED state machine has aborted
	 **/
	FSMStateBase ProcessAbortEvent (FSMEventBase e, out ProcessEventResult result)
	{
		if (GetOwnerState())
			fsmDebugPrint("[hfsm] SUB! " + GetOwnerState().Type().ToString() + "::ProcessAbortEvent(" + e.Type().ToString() + ")");
		else
			fsmDebugPrint("[hfsm] root::ProcessAbortEvent(" + e.Type().ToString() + ")");

		// 1) look in submachine first (if any)
		if (m_State.HasFSM())
		{
			HFSMBase<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> a = m_State.GetFSM();
			ProcessEventResult subfsm_res;
			FSMStateBase abort_dst = a.ProcessAbortEvent(e, subfsm_res);

			switch (subfsm_res)
			{
				case ProcessEventResult.FSM_OK:
				{
					fsmDebugPrint("[hfsm] event processed by sub machine=" + m_State.ToString());
					result = subfsm_res;		// 1.1) submachine accepted event
					return NULL;
				}
				case ProcessEventResult.FSM_ABORTED:
				{
					fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString());

					if (GetOwnerState() == abort_dst.GetParentState())
					{
						fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString() + " & abort destination reached.");
						m_State = abort_dst;
						m_State.OnEntry(e);		// 1.2) submachine aborted, call onEntry on new state (cross-hierarchy transition)
						result = ProcessEventResult.FSM_OK;
						return NULL;
					}
					else
					{
						result = ProcessEventResult.FSM_ABORTED; // 1.3) submachine has aborted, look for destination state in parent
						return NULL;
					}

					break;
				}
				case ProcessEventResult.FSM_TERMINATED:
				{
					break; // submachine has finished, look for local transitions from exited submachine
				}
				case ProcessEventResult.FSM_NO_TRANSITION:
				{
					fsmDebugPrint("[hfsm] aborted (but no transition) sub machine=" + m_State.ToString());
					break; // submachine has no transition, look for transitions in local machine
				}
			}
		}

		// 2) local transitions
		int i = FindFirstUnguardedTransition(e);
		if (i == -1)
		{
			fsmDebugPrint("[hfsm] abort event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
			result = ProcessEventResult.FSM_NO_TRANSITION;
			return NULL;
		}

		FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
		ProcessEventResult res = ProcessAbortTransition(t, e);
		result = res;
		switch (res)
		{
			case ProcessEventResult.FSM_OK:
			{
				//fsmDebugSpam("[hfsm] abort event processed by machine=" + m_State.ToString());
				return NULL; // machine accepted event
			}
			case ProcessEventResult.FSM_ABORTED:
			{
				fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString() + " will fall-through to dst=" + t.m_dstState);
				return t.m_dstState; // store destination state for parent(s)
			}

			case ProcessEventResult.FSM_TERMINATED:
			{
				fsmDebugPrint("[hfsm] aborted & terminated sub machine=" + m_State.ToString());
				break; // submachine has finished, look for local transitions from exited submachine
			}
			case ProcessEventResult.FSM_NO_TRANSITION:
			{	
				break; // submachine has no transition, look for transitions in local machine
			}
		}
		return NULL;
	}


	/**@fn		ProcessEvent
	 * @brief	instructs the hierarchical state machine to process the event e
	 * @param[in]	e	\p	event that will be used to find suitable transition from current state
	 * @return	FSM_OK if transition found and allowed by guard (if any)
	 * @return	FSM_NO_TRANSITION if no unguarded transition found in submachine or local machine
	 * @return	FSM_TERMINATED state machine has terminated
	 **/
	ProcessEventResult ProcessEvent (FSMEventBase e)
	{
		if (GetOwnerState())
			fsmDebugPrint("[hfsm] SUB!::" + GetOwnerState().Type().ToString() + "::ProcessEvent(" + e.Type().ToString() + ")");
		else
			fsmDebugPrint("[hfsm] root::ProcessEvent(" + e.Type().ToString() + ")");

		// 1) completion transitions have priority (if any)
		if (m_HasCompletions)
			ProcessCompletionTransitions();

		// 2) submachine then (if any)
		if (m_State.HasFSM())
		{
			ProcessEventResult subfsm_res = m_State.ProcessEvent(e);

			switch (subfsm_res)
			{
				case ProcessEventResult.FSM_OK:
				{
					fsmDebugSpam("[hfsm] event processed by sub machine=" + m_State.ToString());
					return subfsm_res; // submachine accepted event
				}
				case ProcessEventResult.FSM_TERMINATED:
				{
					break; // submachine has finished, look for local transitions from exited submachine
				}
				case ProcessEventResult.FSM_NO_TRANSITION:
				{	
					break; // submachine has no transition, look for transitions in local machine
				}
			}
		}

		// 3) local transitions
		int i = FindFirstUnguardedTransition(e);
		if (i == -1)
		{
			fsmDebugPrint("[hfsm] event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
			return ProcessEventResult.FSM_NO_TRANSITION;
		}

		FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(i);
		ProcessEventResult res;
		if (row.m_dstState != NULL)
		{
			// this is regular transition
			if (row.m_srcState.GetParentState() == row.m_dstState.GetParentState())
				res = LocalTransition(i, e); // transition is within this state machine
			else
				Error("cross-hierarchy transition or misconfigured transition detected!");
				//res = HierarchicTransition(i, e); // transition has to cross hierarchy
		}
		else
		{
			// this is terminating transition
			if (row.m_srcState.GetParentState() == GetOwnerState())
				res = LocalTransition(i, e); // terminating transition is within this state machine
			else
				Error("cross-hierarchy transition or misconfigured transition detected!");
				//res = HierarchicTransition(i, e); // source node crosses hierarchy (terminate lies always within this machine)
		}
		return res;
	}

	protected int FindFirstUnguardedTransition (FSMEventBase e)
	{
		FSMStateBase curr_state = m_State;

		int count = m_Transitions.Count();
		for (int i = 0; i < count; ++i)
		{
			FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
			if ((t.m_srcState == curr_state) && (t.m_event != NULL) && (t.m_event.Type() == e.Type()))
			{
				//fsmDebugSpam("[hfsm] [" + i + "/" + count + "] *** matched! t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
				bool hasGuard = t.m_guard != NULL;
				if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e)))		// 1) exec guard (if any)
				{
					return i;
				}
			}
			/*else
			{
				fsmDebugSpam("[hfsm][" + i + "/" + count + "] ... matching  t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
			}*/
		}
		return -1;
	}

	protected int FindFirstCompletionTransition ()
	{
		if (IsRunning())
		{
			FSMStateBase curr_state = m_State;

			int count = m_Transitions.Count();
			for (int i = 0; i < count; ++i)
			{
				FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);

				//fsmDebugPrint("[hfsm] (local) matching state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
				if ((t.m_srcState.Type() == curr_state.Type()) && (t.m_event == NULL))
				{
					bool hasGuard = t.m_guard != NULL;
					if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(NULL)))		// 1) exec guard (if any)
					{
						return i;
					}
				}
			}
		}
		return -1;
	}


	/**@fn		ProcessLocalTransition
	 * @brief	instructs the state machine to process the event locally - no hierarchy is crossed
	 * @param[in]	t	\p	the transition in m_Transitions
	 * @param[in]	e	\p	event that will be used to process transition from current state
	 * @return	FSM_OK or FSM_TERMINATED
	 **/
	protected ProcessEventResult ProcessLocalTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
	{
		fsmDebugPrint("[hfsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());

		m_State.OnExit(e);			// 1) call onExit on old state

		if (t.m_action)
			t.m_action.Action(e);	// 2) execute transition action (if any)

		m_State = t.m_dstState;		// 3) change state to new

		if (t.m_dstState != NULL)
		{
			m_State.OnEntry(e);		// 4a) call onEntry on new state

			if (GetOwnerState())
				GetOwnerState().OnSubMachineChanged(t.m_srcState, t.m_dstState);	// 5) notify owner state about change in submachine

			return ProcessEventResult.FSM_OK;
		}
		else
		{
			fsmDebugPrint("[hfsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());

			if (GetOwnerState())
				GetOwnerState().OnSubMachineChanged(t.m_srcState, NULL);	// 5) notify owner state about change in submachine
			return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
		}
	}
	/**@fn		LocalProcessEvent
	 * @brief	instructs the state machine to process the event e
	 * @param[in]	i	\p	index of the transition in m_Transitions
	 * @param[in]	e	\p	event that will be used to process transition from current state
	 * @return	FSM_OK or FSM_TERMINATED
	 **/
	protected ProcessEventResult LocalTransition (int i, FSMEventBase e)
	{
		FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
		ProcessEventResult ret = ProcessLocalTransition(t, e);
		return ret;
	}

	protected ProcessEventResult ProcessCompletionTransitions ()
	{
		int completionIdx = FindFirstCompletionTransition();
		while (completionIdx != -1)
		{
			FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(completionIdx);
			ProcessEventResult res;
			if (row.m_dstState != NULL)
			{
				// this is regular completion transition
				if (row.m_srcState.GetParentState() == row.m_dstState.GetParentState())
					res = LocalTransition(completionIdx, NULL); // transition is within this state machine
				else
					Error("cross-hierarchy transition or misconfigured transition detected!");
					//res = HierarchicTransition(completionIdx, NULL); // transition has to cross hierarchy
			}
			else
			{
				// this is terminating completion transition
				if (row.m_srcState.GetParentState() == GetOwnerState())
					res = LocalTransition(completionIdx, NULL); // terminating transition is within this state machine
				else
					Error("cross-hierarchy transition or misconfigured transition detected!");
					//res = HierarchicTransition(completionIdx, NULL); // source node crosses hierarchy (terminate lies always within this machine)
			}

			completionIdx = FindFirstCompletionTransition();
		}
		return ProcessEventResult.FSM_NO_TRANSITION;
	}
};

/** cross-hierarchy transitions (for future use):

	protected bool RemoveCommonParents (out ref array<WeaponStateBase> src_path, out ref array<WeaponStateBase> dst_path)
	{
		//for (int j = 0; j < src_path.Count(); ++j) fsmDebugPrint("[hfsm] curr state src_path[" + j + "] = " + src_path[j].ToString());
		//for (int k = 0; k < dst_path.Count(); ++k) fsmDebugPrint("[hfsm] next state dst_path[" + k + "] = " + dst_path[k].ToString());
		while ((src_path.Count() > 0) && (dst_path.Count() > 0) && (src_path.Get(src_path.Count() - 1) == dst_path.Get(dst_path.Count() - 1)))
		{
			src_path.Remove(src_path.Count() - 1);
			dst_path.Remove(dst_path.Count() - 1);
		}
		//for (int je = 0; je < src_path.Count(); ++je) fsmDebugPrint("[hfsm] curr state src_path[" + je + "] = " + src_path[je].ToString());
		//for (int ke = 0; ke < dst_path.Count(); ++ke) fsmDebugPrint("[hfsm] next state dst_path[" + ke + "] = " + dst_path[ke].ToString());
	}
	protected ProcessEventResult HierarchicTransition (int i, FSMEventBase e)
	{
		fsmDebugPrint("[hfsm] { " + this.ToString() + "::HierarchicTransition(" + e.Type().ToString() + ")");

		FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);

		array<WeaponStateBase> src_path = new array<WeaponStateBase>;
		array<WeaponStateBase> dst_path = new array<WeaponStateBase>;
		if (GetHierarchyPath(t.m_srcState, src_path))
		{
			if (t.m_dstState == NULL)
			{
				Error("cross hierarchy terminate not yet implemented");
				WeaponStateBase curr_state = GetCurrentState();

				//for (int ex = 0; ex < src_path.Count(); ++ex) { fsmDebugPrint("[hfsm] exiting level=" + ex); src_path[ex].OnExit(e); }
				//for (int en = dst_path.Count(); en --> 0;)
				//for (int en = 0; en < dst_path.Count(); ++en) { fsmDebugPrint("[hfsm] entering level=" + en); int ii = dst_path.Count() - 1 - en; dst_path[ii].OnEntry(e); }
				fsmDebugPrint("[hfsm] } " + this.ToString() + "::HierarchicTransition(" + e.Type().ToString() + ") --> terminated");
				return ProcessEventResult.FSM_TERMINATED;
			}
			else if (GetHierarchyPath(t.m_dstState, dst_path))
			{
				Error("cross hierarchy event implemented, not fully debugged");
				RemoveCommonParents(src_path, dst_path);

				for (int ex = 0; ex < src_path.Count(); ++ex)
				{
					src_path[ex].OnExit(e); // 1) call OnExit on all nodes on src_path ()
				}

				//if (t.m_action)
				//	t.m_action.Action(e);	// 2) execute transition action (if any)

				//m_State = t.m_dstState; // 4) change state to new
				
				//for (int en = dst_path.Count(); en --> 0;)
				for (int en = 0; en < dst_path.Count(); ++en)
				{
					int ii = dst_path.Count() - 1 - en;
					dst_path[ii].OnEntry(e);	// 5) call OnEntry on all nodes on dst_path (@NOTE: reverse order)
				}

				fsmDebugPrint("[hfsm] } " + this.ToString() + "::HierarchicTransition(" + e.Type().ToString() + ") --> OK");
				return ProcessEventResult.FSM_OK;
			}
			else
			{
				Error("unhandled case, what did you do?");
			}
		}

		fsmDebugPrint("[hfsm] } " + this.ToString() + "::HierarchicTransition(" + e.Type().ToString() + ") --> no transition");
		return ProcessEventResult.FSM_NO_TRANSITION;
	}
*/

