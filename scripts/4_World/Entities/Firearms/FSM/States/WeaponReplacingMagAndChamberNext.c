
class DetachOldMagaine extends WeaponStateBase
{
	Magazine m_oldMagazine; /// magazine that will be detached
	ref InventoryLocation m_oldSrc;

	void DetachOldMagaine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_oldMagazine = NULL;
		m_oldSrc = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_oldSrc.IsValid())
			Error("[wpnfsm] DetachOldMagaine m_oldSrc=invalid, item not in bubble?");

		if (m_oldMagazine && m_oldSrc)
		{
			if (GameInventory.LocationRemoveEntity(m_oldSrc))
				wpnDebugPrint("[wpnfsm] DetachOldMagaine, ok - old magazine removed from wpn");
			else
				Error("[wpnfsm] DetachOldMagaine, error - cannot remove old mag from wpn");
		}
		else
			Error("[wpnfsm] DetachOldMagaine, error - no magazines configured for replace (m_old=m_oldSrc=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_oldMagazine && m_oldSrc)
		{
			//bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			if (!GetGame().IsMultiplayer() || GetGame().IsServer())
			{
				if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_oldMagazine))
					wpnDebugPrint("[wpnfsm] RemoveNewMagazineFromInventory, ok - aborting, old magazine dropped to ground");
				else
					Error("[wpnfsm] RemoveNewMagazineFromInventory, error - cannot abort removal from wpn (of old mag)");
			}

			m_weapon.SelectionMagazineHide(); // force hide on abort
		}

		m_oldMagazine = NULL;
		m_oldSrc = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;
		m_oldSrc = NULL;
		super.OnExit(e);
	}
};

class RemoveNewMagazineFromInventory extends WeaponStateBase
{
	Magazine m_newMagazine; /// magazine that will be removed from inventory
	ref InventoryLocation m_newSrc;

	void RemoveNewMagazineFromInventory (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_newMagazine = NULL;
		m_newSrc = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_newSrc.IsValid())
			Error("[wpnfsm] RemoveNewMagazineFromInventory m_newSrc=invalid, item not in bubble?");

		if (m_newMagazine && m_newSrc)
		{
			if (GameInventory.LocationRemoveEntity(m_newSrc))
				wpnDebugPrint("[wpnfsm] RemoveNewMagazineFromInventory, ok - new magazine removed from inv");
			else
				Error("[wpnfsm] RemoveNewMagazineFromInventory, error - cannot new remove mag from inv");
		}
		else
			Error("[wpnfsm] RemoveNewMagazineFromInventory, error - no magazines configured for replace (m_old=m_new=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_newMagazine && m_newSrc)
		{
			//bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			//if (is_single_or_server)
			//{
				if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_newMagazine))
					wpnDebugPrint("[wpnfsm] RemoveNewMagazineFromInventory, ok - aborting, new magazine dropped to ground");
				else
					Error("[wpnfsm] RemoveNewMagazineFromInventory, error - cannot abort removal from inv (of new mag)");
			//}

			m_weapon.SelectionMagazineHide(); // force hide on abort
		}

		m_newMagazine = NULL;
		m_newSrc = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_weapon.SelectionMagazineShow();

		m_newMagazine = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}
};

class RemoveNewMagazineFromInventory_OnExitShowMag extends RemoveNewMagazineFromInventory
{
	override void OnExit (WeaponEventBase e)
	{
		m_weapon.SelectionMagazineShow();

		m_newMagazine = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}
};

class StoreOldMagazine extends WeaponStateBase
{
	Magazine m_oldMagazine; /// detached magazine that will be stored in inventory
	Magazine m_newMagazine; /// magazine that will be dropped on abort
	ref InventoryLocation m_oldDst;

	void StoreOldMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_oldMagazine = NULL;
		m_newMagazine = NULL;
		m_oldDst = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		if (m_oldMagazine && m_oldDst)
		{
			if (m_oldDst && m_oldDst.IsValid() && GameInventory.LocationCanAddEntity(m_oldDst))
			{
				if (GameInventory.LocationAddEntity(m_oldDst))
				{
					wpnDebugPrint("[wpnfsm] StoreOldMagazine, ok - old magazine stored in inventory");
					return;
				}
				else
				{
					wpnDebugPrint("[wpnfsm] StoreOldMagazine, error - old magazine NOT stored in inventory, trying to find another location...");
				}
			}

			InventoryLocation newDst = new InventoryLocation;
			if (e.m_player.GetInventory().FindFreeLocationForEx(m_oldMagazine, FindInventoryLocationType.ANY, m_oldDst, newDst)) /* @NOTE: excluding m_oldDst from candidates */
			{
				wpnDebugPrint("[wpnfsm] WeaponReplacingMagAndChamberNext, selected newDst=" + newDst.DumpToString());
				if (GameInventory.LocationAddEntity(newDst))
				{
					wpnDebugPrint("[wpnfsm] StoreOldMagazine, ok - old magazine stored in inventory");
					return;
				}
				else
				{
					wpnDebugPrint("[wpnfsm] StoreOldMagazine, error - old magazine NOT stored in inventory, trying to find another location...");
				}

			}

			{
				wpnDebugPrint("[wpnfsm] WeaponReplacingMagAndChamberNext, dropping on ground");
				vector m4[4];
				Math3D.MatrixIdentity4(m4);
				GameInventory.PrepareDropEntityPos(e.m_player, m_oldMagazine, m4);
				InventoryLocation il_mag = new InventoryLocation;
				il_mag.SetGround(m_oldMagazine, m4);
				GameInventory.LocationAddEntity(il_mag);
			}
		}
		else
			Error("[wpnfsm] StoreOldMagazine, error - no magazines configured for replace (m_old=m_new=NULL)");
	}
	
	override void OnAbort (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] StoreOldMagazine, aborting, old magazine already stored");
		m_weapon.SelectionMagazineHide();

		//bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
		//if (is_single_or_server)
		//{
			if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_newMagazine))
				wpnDebugPrint("[wpnfsm] StoreOldMagazine, ok - aborting, new magazine dropped to ground");
			else
				Error("[wpnfsm] StoreOldMagazine, error - cannot abort removal from wpn (of new mag)");
		//}

		m_oldMagazine = NULL;
		m_newMagazine = NULL;
		m_oldDst = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;
		m_newMagazine = NULL;
		m_oldDst = NULL;
		super.OnExit(e);
	}
};

class AttachNewMagazine extends WeaponStateBase
{
	Magazine m_newMagazine; /// new magazine from inventory that will be attached
	ref InventoryLocation m_newDst;

	void AttachNewMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_newMagazine = NULL;
		m_newDst = NULL;
	}
	
	override bool IsWaitingForActionFinish()
	{
		return true;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		if (m_newMagazine && m_newDst)
		{
			if (GameInventory.LocationAddEntity(m_newDst))
			{
				wpnDebugPrint("[wpnfsm] AttachNewMagazine, ok - attached new magazine");
			}
			else
			{
				wpnDebugPrint("[wpnfsm] AttachNewMagazine, error - cannot attach new magazine!");
			}
		}
		else
			Error("[wpnfsm] AttachNewMagazine, error - no magazines configured for replace (m_new=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] AttachNewMagazine, aborting, but attached new magazine already in place");
		m_newMagazine = NULL;
		m_newDst = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;
		m_newDst = NULL;
		super.OnExit(e);
	}
};

class AttachNewMagazine_W4T extends AttachNewMagazine
{
	override bool IsWaitingForActionFinish () { return true; }
};


// replace mag (== eject curr mag + attach another + store the old one) and chamber next (if possible)
class WeaponReplacingMagAndChamberNext extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;

	ref WeaponStartAction m_start;
	ref DetachOldMagaine m_detach;
	ref RemoveNewMagazineFromInventory_OnExitShowMag m_getNew;
	ref StoreOldMagazine m_storeOld;
	ref AttachNewMagazine_W4T m_attach;
	ref WeaponChamberFromAttMag_W4T m_chamber;
	ref WeaponCharging_CK m_onCK;

	// substates configuration
	Magazine m_oldMagazine; /// magazine that will be detached
	Magazine m_newMagazine; /// magazine that will be attached
	ref InventoryLocation m_oldSrc;
	ref InventoryLocation m_newSrc;
	ref InventoryLocation m_oldDst;
	ref InventoryLocation m_newDst;

	void WeaponReplacingMagAndChamberNext (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;
		m_newMagazine = NULL;

		// setup nested state machine
		m_start = new WeaponStartAction(m_weapon, this, m_action, m_actionType);
		m_detach = new DetachOldMagaine(m_weapon, this);
		m_getNew = new RemoveNewMagazineFromInventory_OnExitShowMag(m_weapon, this);
		m_storeOld = new StoreOldMagazine(m_weapon, this);
		m_attach = new AttachNewMagazine_W4T(m_weapon, this);
		m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
		m_onCK = new WeaponCharging_CK(m_weapon, this);

		// events
		WeaponEventBase __md_ = new WeaponEventAnimMagazineDetached;
		WeaponEventBase __mh_ = new WeaponEventAnimMagazineHide;
		WeaponEventBase __ms_ = new WeaponEventAnimMagazineShow;
		WeaponEventBase __ma_ = new WeaponEventAnimMagazineAttached;
		WeaponEventBase __ck_ = new WeaponEventAnimCocked;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(   m_start, __md_, m_detach));
		m_fsm.AddTransition(new WeaponTransition(  m_detach, __mh_, m_getNew));
		m_fsm.AddTransition(new WeaponTransition(  m_getNew, __ms_, m_storeOld));
		m_fsm.AddTransition(new WeaponTransition(m_storeOld, __ma_, m_attach));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_chamber, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new WeaponGuardHasAmmo(m_weapon))));
		//m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new GuardNot(new WeaponGuardHasAmmo(m_weapon)))));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK));
		m_fsm.AddTransition(new WeaponTransition( m_attach, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_chamber, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_onCK, _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}
	
	override void OnEntry (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		m_oldMagazine = m_weapon.GetMagazine(mi);
		m_newMagazine = e.m_magazine;

		wpnDebugPrint("[wpnfsm] WeaponReplacingMagAndChamberNext, m_oldMagazine=" + m_oldMagazine + " m_newMagazine=" + m_newMagazine);

		if (m_oldMagazine && m_newMagazine)
		{
			m_oldSrc = new InventoryLocation;
			m_newSrc = new InventoryLocation;
			m_newDst = new InventoryLocation;
			m_oldDst = new InventoryLocation;

			m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_newSrc);
			m_oldMagazine.GetInventory().GetCurrentInventoryLocation(m_oldSrc);

			m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_newDst);
			m_newDst.CopyLocationFrom(m_oldSrc);

			if (magazinesHaveEqualSizes(m_oldMagazine, m_newMagazine))
			{
				m_oldMagazine.GetInventory().GetCurrentInventoryLocation(m_oldDst);
				m_oldDst.CopyLocationFrom(m_newSrc);

			}
			else
			{
				wpnDebugPrint("[wpnfsm] WeaponReplacingMagAndChamberNext, inequal sizes, state will handle this");
			}

			m_detach.m_oldMagazine = m_oldMagazine;
			m_detach.m_oldSrc = m_oldSrc;

			m_getNew.m_newMagazine = m_newMagazine;
			m_getNew.m_newSrc = m_newSrc;

			m_storeOld.m_oldMagazine = m_oldMagazine;
			m_storeOld.m_oldDst = m_oldDst;
			m_storeOld.m_newMagazine = m_newMagazine;

			m_attach.m_newMagazine = m_newMagazine;
			m_attach.m_newDst = m_newDst;
		}
		else
			Error("[wpnfsm] WeaponReplacingMagAndChamberNext, error - no magazines configured for replace (m_old=m_new=NULL)");

		super.OnEntry(e); // @NOTE: super after submachine init (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;
		m_newMagazine = NULL;
		m_oldDst = NULL;
		m_newDst = NULL;
		m_oldSrc = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}
};

