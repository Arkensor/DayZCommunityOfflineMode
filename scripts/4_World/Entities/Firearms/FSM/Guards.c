/**@class	WeaponGuardBase
 * @brief	represents guard on a transition from state to state
 **/
class WeaponGuardBase
{
	/**@fn		GuardCondition
	 * @brief enable or disable transition based on condition
	 * the guard is a boolean operation executed first and which can prevent the transition from firing by returning false
	 * @return	true if transition is allowed
	 **/
	bool GuardCondition (WeaponEventBase e) { return true; }
};

class GuardAnd extends WeaponGuardBase
{
	ref WeaponGuardBase m_arg0;
	ref WeaponGuardBase m_arg1;

	void GuardAnd (WeaponGuardBase arg0 = NULL, WeaponGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (WeaponEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) && m_arg1.GuardCondition(e);
		wpnDebugPrint("[wpnfsm] guard - " + m_arg0.Type() + " && " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class GuardNot extends WeaponGuardBase
{
	ref WeaponGuardBase m_arg0;

	void GuardNot (WeaponGuardBase arg0 = NULL) { m_arg0 = arg0; }

	override bool GuardCondition (WeaponEventBase e)
	{
		bool result = !m_arg0.GuardCondition(e);
		wpnDebugPrint("[wpnfsm] guard - ! " + m_arg0.Type() + " = " + result);
		return result;
	}
};

class GuardOr extends WeaponGuardBase
{
	ref WeaponGuardBase m_arg0;
	ref WeaponGuardBase m_arg1;

	void GuardOr (WeaponGuardBase arg0 = NULL, WeaponGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (WeaponEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) || m_arg1.GuardCondition(e);
		wpnDebugPrint("[wpnfsm] guard - " + m_arg0.Type() + " || " + m_arg1.Type() + " = " + result);
		return result;
	}
};

// guards /////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WeaponGuardJammed extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardJammed (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.IsChamberJammed(mi))
		{
			wpnDebugPrint("[wpnfsm] guard - jammed");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - not jammed");
		return false;
	}
};

class WeaponGuardHasAmmo extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardHasAmmo (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		Magazine mag = m_weapon.GetMagazine(mi);
		if (mag != NULL && mag.GetAmmoCount() > 0)
		{
			wpnDebugPrint("[wpnfsm] guard - has ammo");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - no ammo");
		return false;
	}
};

class WeaponGuardHasAmmoInEvent extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardHasAmmoInEvent (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		Magazine mag = e.m_magazine;
		if (mag != NULL && mag.GetAmmoCount() > 0)
		{
			wpnDebugPrint("[wpnfsm] guard - has ammo in event");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - no ammo in event");
		return false;
	}
};


class WeaponGuardHasMag extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardHasMag (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		Magazine mag = m_weapon.GetMagazine(mi);
		if (mag != NULL)
		{
			wpnDebugPrint("[wpnfsm] guard - has magazine");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - no magazine");
		return false;
	}
};

class WeaponGuardChamberEmpty extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardChamberEmpty (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.IsChamberEmpty(mi))
		{
			wpnDebugPrint("[wpnfsm] guard - chamber empty");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - chamber not empty");
		return false;
	}
};

class WeaponGuardChamberFull extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardChamberFull (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.IsChamberFull(mi))
		{
			wpnDebugPrint("[wpnfsm] guard - chamber full");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - chamber not full");
		return false;
	}
};


class WeaponGuardChamberFiredOut extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardChamberFiredOut (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.IsChamberFiredOut(mi))
		{
			wpnDebugPrint("[wpnfsm] guard - chamber fired out");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - chamber not fired out");
		return false;
	}
};

class WeaponGuardCanAttachMag extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardCanAttachMag (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon && e.m_magazine && m_weapon.CanAttachMagazine(mi, e.m_magazine))
		{
			wpnDebugPrint("[wpnfsm] guard - can attach magazine");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - cannot attach magazine");
		return false;
	}
};

class WeaponGuardCanDetachMag extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardCanDetachMag (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon && e.m_magazine && m_weapon.GetMagazine(mi)/* && m_weapon.CanDetachMagazine(mi, e.m_magazine)*/)
		{
			wpnDebugPrint("[wpnfsm] guard - can detach magazine");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - cannot detach magazine");
		return false;
	}
};

class WeaponGuardChamberHasRoomForMoreThanOne extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardChamberHasRoomForMoreThanOne (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.GetChamberMaxCartridgeCount(mi) - m_weapon.GetChamberCartridgeCount(mi) > 1)
		{
			wpnDebugPrint("[wpnfsm] guard - chamber has room for more than 1b");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - chamber has no room for more than 1b");
		return false;
	}
};

class WeaponGuardChamberHasRoomForOne extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardChamberHasRoomForOne (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		if (m_weapon.GetChamberMaxCartridgeCount(mi) - m_weapon.GetChamberCartridgeCount(mi) == 1)
		{
			wpnDebugPrint("[wpnfsm] guard - chamber has room for 1b");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - chamber has no room for 1b");
		return false;
	}
};

class WeaponGuardHasAmmoInLoopedState extends WeaponGuardBase
{
	WeaponChambering_Cartridge m_state;
	void WeaponGuardHasAmmoInLoopedState (WeaponChambering_Cartridge state) { m_state = state; }

	override bool GuardCondition (WeaponEventBase e)
	{
		Magazine mag = m_state.m_srcMagazine;
		if (mag != NULL && mag.GetAmmoCount() > 0)
		{
			wpnDebugPrint("[wpnfsm] guard - has ammo in looped state");
			return true;
		}
		wpnDebugPrint("[wpnfsm] guard - no ammo in looped state");
		return false;
	}
};

class WeaponGuardMagazinesHaveEqualSizes extends WeaponGuardBase
{
	protected Weapon_Base m_weapon;
	void WeaponGuardMagazinesHaveEqualSizes (Weapon_Base w = NULL) { m_weapon = w; }

	override bool GuardCondition (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		Magazine mag = m_weapon.GetMagazine(mi);
		Magazine mag2 = e.m_magazine;
		if (mag != NULL && mag2 != NULL)
		{
			bool eq = magazinesHaveEqualSizes(mag, mag2);
			if (eq)
			{
				wpnDebugPrint("[wpnfsm] guard - same inventory sizes");
				return true;
			}

			wpnDebugPrint("[wpnfsm] guard - different inventory sizes");
			return false;
		}
		Error("[wpnfsm] guard - mag == NULL or mag2 == NULL, cannot perform comparison");
		return false;
	}
};

