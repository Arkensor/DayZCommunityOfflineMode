void hndDebugPrint (string s)
{
	//Print("" + s); // comment/uncomment to hide/see debug logs
}
void hndDebugSpam (string s)
{
	//Print("" + s); // comment/uncomment to hide/see debug spam
}


typedef FSMTransition<HandStateBase, HandEventBase, HandActionBase, HandGuardBase> HandTransition;


/**@class		HandFSM
 * @brief		Hand finite state machine
 **/
class HandFSM extends OFSMBase<HandStateBase, HandEventBase, HandActionBase, HandGuardBase>
{
	int GetCurrentStateID ()
	{
		if (m_States.Count() == 2)
		{
			int s0 = m_States[0].GetCurrentStateID();
			int s1 = m_States[1].GetCurrentStateID() << 8;
			return s1 | s0;
		}
		return 0;
	}

	/**@fn			SyncStateFromID
	 * @brief		load from database - reverse lookup for state from saved id
	 * @param[in]	id			the id stored in database during save
	 **/
	protected bool SyncStateFromID (int id)
	{
		if (id == 0)
			return false;

		int s0 = id & 0x000000ff;
		int s1 = id & 0x0000ff00;

		int count = m_Transitions.Count();
		bool set0 = false;
		bool set1 = false;
		for (int i = 0; i < count; ++i)
		{
			HandTransition t = m_Transitions.Get(i);
			if (!set0 && t.m_srcState && s0 == t.m_srcState.GetCurrentStateID())
			{
				m_States[0] = t.m_srcState;
				set0 = true;
			}
			if (!set1 && t.m_srcState && s1 == t.m_srcState.GetCurrentStateID())
			{
				m_States[1] = t.m_srcState;
				set1 = true;
			}
			if (set0 && set1)
				return true;
		}
		return false;
	}

	/**@fn			OnStoreLoad
	 * @brief		load state of fsm
	 **/
	void OnStoreLoad (ParamsReadContext ctx)
	{
		int id = 0;
		ctx.Read(id);
		if (SyncStateFromID(id))
			hndDebugPrint("[hndfsm] OnStoreLoad - loaded current state from id=" + id);
		else
			Print("[hndfsm] Warning! OnStoreLoad - cannot load curent hand state, id=" + id);
	}

	/**@fn			OnStoreSave
	 * @brief		save state of fsm
	 **/
	void OnStoreSave (ParamsWriteContext ctx)
	{
		int id = GetCurrentStateID();
		ctx.Write(id);
		hndDebugPrint("[hndfsm] OnStoreSave - saving current state=" + GetCurrentState() + " id=" + id);
	}

	/**@fn			NetSyncCurrentStateID
	 * @brief		Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	void NetSyncCurrentStateID (int id)
	{
		if (SyncStateFromID(id))
			hndDebugPrint("[hndfsm] NetSyncCurrentStateID - loaded current state from id=" + id);
		else
			Print("[hndfsm] NetSyncCurrentStateID called with null, ignoring request to set current fsm state.");
	}

	bool IsInLockedState ()
	{
		for (int s = 0; s < m_States.Count(); ++s)
			if (m_States[s].GetCurrentStateID() == HandStateID.Locked)
				return true;
		return false;
	}
};

