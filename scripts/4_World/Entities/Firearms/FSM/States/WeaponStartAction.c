/**@class 	WeaponStartAction
 * @brief	simple class starting animation action specified by m_action and m_actionType
 */
class WeaponStartAction extends WeaponStateBase
{
	WeaponActions m_action; /// action to be played
	int m_actionType; /// specific action sub-type

	void WeaponStartAction (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		if (e.m_player)
		{
			HumanCommandWeapons hcw = e.m_player.GetCommandModifier_Weapons();
			if (hcw)
			{
				hcw.StartAction(m_action, m_actionType);
		
				if (hcw.GetRunningAction() == m_action && hcw.GetRunningActionType() == m_actionType)
					wpnDebugPrint("HCW: playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished());
				else
					Error("HCW: NOT playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished());
			}
			else
				wpnDebugPrint("---: remote playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType));
		}
		else
		{
			wpnDebugPrint("---: warning, no player wants to play A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType));
		}
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};


