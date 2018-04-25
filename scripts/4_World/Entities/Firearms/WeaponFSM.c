/**@class		WeaponFSM
 * @brief		weapon finite state machine
 **/
class WeaponFSM extends HFSMBase<WeaponStateBase, WeaponEventBase, WeaponActionBase, WeaponGuardBase>
{
	/**@fn		FindStateForID
	 * @brief	load from database - reverse lookup for state from saved id
	 * @param[in]	id	the id stored in database during save
	 **/
	WeaponStableState FindStateForID (int id)
	{
		if (id == 0)
			return null;

		int count = m_Transitions.Count();
		for (int i = 0; i < count; ++i)
		{
			WeaponTransition trans = m_Transitions.Get(i);
			WeaponStableState state = WeaponStableState.Cast(trans.m_srcState);
			if (state && id == state.GetCurrentStateID())
				return state;
		}
		return null;
	}

	/**@fn		OnStoreLoad
	 * @brief	load state of fsm
	 **/
	void OnStoreLoad (ParamsReadContext ctx)
	{
		int id = 0;
		ctx.Read(id);
		WeaponStableState state = FindStateForID(id);
		if (state)
		{
			wpnDebugPrint("[wpnfsm] OnStoreLoad - loading current state=" + state + " id=" + id);
			m_State = state;
		}
		else
		{
			Print("[wpnfsm] Warning! OnStoreLoad - cannot load curent weapon state, id=" + id);
		}
	}

	/**@fn		GetCurrentStateID
	 * @brief	tries to return identifier of current stable state
	 *
	 * if the weapon is in stable state, i.e. not reloading, firing, detaching, ... than
	 * the identifier of the stable state is used directly.
	 * otherwise if the weapon is in unstable state, than the function uses abort information
	 * to determine the closest stable state that is coherent with weapon's state.
	 *
	 * @return	integer id that will be stored to database
	 **/
	int GetCurrentStateID ()
	{
		// 1) if current state is stable state then return ID directly
		WeaponStableState state = WeaponStableState.Cast(GetCurrentState());
		if (state)
			return state.GetCurrentStateID();

		// 2) otherwise find closest stable state (by looking on abort event)
		WeaponStateBase abort_dst = FindAbortDestinationState(new WeaponEventHumanCommandActionAborted(null));
		WeaponStableState closest_stable_state = WeaponStableState.Cast(abort_dst);
		Print("[wpnfsm] Save occured in fsm transition! current state=" + GetCurrentState() + " closes stable state=" + closest_stable_state + " id=" + closest_stable_state.GetCurrentStateID());
		if (closest_stable_state)
			return closest_stable_state.GetCurrentStateID();

		Print("[wpnfsm] Warning! Save occured in fsm transition! GetCurrentStateID - cannot find closest weapon stable state.");
		return 0;
	}

	/**@fn		OnStoreSave
	 * @brief	save state of fsm
	 **/
	void OnStoreSave (ParamsWriteContext ctx)
	{
		int id = GetCurrentStateID();
		wpnDebugPrint("[wpnfsm] OnStoreSave - saving current state=" + GetCurrentState() + " id=" + id);
		ctx.Write(id);
	}

	/**@fn		NetSyncCurrentStateID
	 * @brief	Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	void NetSyncCurrentStateID (int id)
	{
		WeaponStateBase state = FindStateForID(id);
		if (state)
		{
			wpnDebugPrint("[wpnfsm] NetSyncCurrentStateID - loading current state=" + state + " id=" + id);
			m_State = state;
		}
		else
			Print("[wpnfsm] NetSyncCurrentStateID called with null, ignoring request to set current fsm state.");
	}

	/**@fn		RandomizeFSMState
	 * @brief	Engine callback - loot randomization of FSM's state. not intended to direct use.
	 **/
	void RandomizeFSMState (bool hasBullet, bool hasMagazine, bool isJammed)
	{
		array<WeaponStableState> candidates = new array<WeaponStableState>;
		int tc = m_Transitions.Count();
		for (int i = 0; i < tc; ++i)
		{
			WeaponTransition trans = m_Transitions.Get(i);
			WeaponStableState state = WeaponStableState.Cast(trans.m_srcState);
			if (state && state.HasBullet() == hasBullet && state.HasMagazine() == hasMagazine && state.IsJammed() == isJammed)
				candidates.Insert(state);
		}

		int cc = candidates.Count();
		if (cc)
		{
			int randomIndex = Math.RandomInt(0, cc);
			WeaponStableState selected = candidates.Get(randomIndex);
			m_State = selected;
			wpnDebugPrint("[wpnfsm] RandomizeFSMState - randomized current state=" + m_State + " id=" + selected.GetCurrentStateID());
			selected.SyncAnimState();
		}
		else
		{
			wpnDebugPrint("[wpnfsm] RandomizeFSMState - warning - cannot randomize, no states available");
		}
	}
};

