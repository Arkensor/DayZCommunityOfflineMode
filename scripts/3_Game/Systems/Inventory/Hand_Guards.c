///@{ guards

/**@class	HandGuardBase
 * @brief	represents guard on a transition from state to state
 **/
class HandGuardBase
{
	/**@fn			GuardCondition
	 * @brief		enable or disable transition based on condition
	 * the guard is a boolean operation executed first and which can prevent the transition from firing by returning false
	 * @return		true if transition is allowed
	 **/
	bool GuardCondition (HandEventBase e) { return true; }
};


class HandGuardAnd extends HandGuardBase
{
	ref HandGuardBase m_arg0;
	ref HandGuardBase m_arg1;

	void HandGuardAnd (HandGuardBase arg0 = NULL, HandGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (HandEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) && m_arg1.GuardCondition(e);
		hndDebugPrint("[hndfsm] guard - " + m_arg0.Type() + " && " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class HandGuardNot extends HandGuardBase
{
	ref HandGuardBase m_arg0;

	void HandGuardNot (HandGuardBase arg0 = NULL) { m_arg0 = arg0; }

	override bool GuardCondition (HandEventBase e)
	{
		bool result = !m_arg0.GuardCondition(e);
		hndDebugPrint("[hndfsm] guard - ! " + m_arg0.Type() + " = " + result);
		return result;
	}
};

class HandGuardOr extends HandGuardBase
{
	ref HandGuardBase m_arg0;
	ref HandGuardBase m_arg1;

	void HandGuardOr (HandGuardBase arg0 = NULL, HandGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (HandEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) || m_arg1.GuardCondition(e);
		hndDebugPrint("[hndfsm] guard - " + m_arg0.Type() + " || " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class HandGuardHasItemInEvent extends HandGuardBase
{
	protected Man m_Player;
	void HandGuardHasItemInEvent (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		EntityAI eai = e.m_Entity;
		if (eai != NULL /* && CanTakeInHands*/)
		{
			hndDebugPrint("[hndfsm] guard - has valid entity in event");
			return true;
		}
		hndDebugPrint("[hndfsm] guard - no entity in event");
		return false;
	}
};

class HandGuardIsSameItemInHands extends HandGuardBase
{
	protected Man m_Player;
	void HandGuardIsSameItemInHands (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		if (e.m_Entity == m_Player.GetHumanInventory().GetEntityInHands())
		{
			hndDebugPrint("[hndfsm] guard - has same entity in hands");
			return true;
		}

		hndDebugPrint("[hndfsm] guard - different entity in hands");
		return false;
	}
};

class HandGuardHasDestroyedItemInHands extends HandGuardBase
{
	protected Man m_Player;
	void HandGuardHasDestroyedItemInHands (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		hndDebugSpam("[hndfsm] guard - has same entity in hands ev=" + e.m_Entity + " hnd=" + m_Player.GetHumanInventory().GetEntityInHands());
		EntityAI hnd = m_Player.GetHumanInventory().GetEntityInHands();
		if (e.m_Entity)
		{
			if (e.m_Entity == hnd)
			{
				hndDebugPrint("[hndfsm] guard - has same entity in hands");
				return true;
			}

			if (hnd == null)
			{
				hndDebugPrint("[hndfsm] guard - hands already empty");
				return true;
			}
		}
		else
		{
			hndDebugPrint("[hndfsm] guard - hands already empty and item destroyed");
			return true;
		}

		hndDebugPrint("[hndfsm] guard - destroyed entity not in hands");
		return false;
	}
};

class HandGuardHasItemInHands extends HandGuardBase
{
	protected Man m_Player;
	void HandGuardHasItemInHands (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		if (m_Player.GetHumanInventory().GetEntityInHands())
		{
			hndDebugPrint("[hndfsm] guard - has valid entity in hands");
			return true;
		}

		hndDebugPrint("[hndfsm] guard - no entity in hands");
		return false;
	}
};

class HandGuardHasRoomForItem extends HandGuardBase
{
	protected Man m_Player;
	void HandGuardHasRoomForItem (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		EntityAI eai = e.m_Entity;

		HandEventStash es = HandEventStash.Cast(e);
		if (es)
		{
			if (GameInventory.LocationCanAddEntity(es.m_Dst))
			{
				hndDebugPrint("[hndfsm] guard - has valid entity in hands");
				return true;
			}
			else
				Error("[hndfsm] HandGuardHasRoomForItem - no room at dst=" + es.m_Dst.DumpToString());
		}

		hndDebugPrint("[hndfsm] guard - no room for item in hands");
		return false;
	}
};
///@} guards


