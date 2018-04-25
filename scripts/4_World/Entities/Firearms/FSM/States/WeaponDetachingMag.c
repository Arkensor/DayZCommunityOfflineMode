
// detach magazine composite state
class WeaponDetachingMag_1 extends WeaponStartAction
{ };

class WeaponDetachingMag_Store extends WeaponStateBase
{
	Magazine m_magazine; /// magazine that will be detached
	ref InventoryLocation m_dst;

	override void OnEntry (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm]  WeaponDetachingMag_Store, Detaching mag=" + m_magazine.ToString() +  "to loc=" + m_dst.DumpToString());
		super.OnEntry(e);

		if (m_magazine && m_dst)
		{
			InventoryLocation il = new InventoryLocation;
			if (m_magazine.GetInventory().GetCurrentInventoryLocation(il))
			{
				if (GameInventory.LocationRemoveEntity(il))
				{
					wpnDebugPrint("[wpnfsm] WeaponDetachingMag_Store, ok - magazine removed from inv");
				}
				else
					Error("[wpnfsm] WeaponDetachingMag_Store, error - cannot remove mag from inv");
			}
			else
				Error("[wpnfsm] WeaponDetachingMag_Store, error - cannot get curr location");
		}
		else
			Error("[wpnfsm] WeaponDetachingMag_Store, error - no magazine to load from (m_magazine=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_magazine && m_dst)
		{
			//bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			if (!GetGame().IsMultiplayer() || GetGame().IsServer())
			{
				if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_magazine))
					wpnDebugPrint("[wpnfsm] WeaponDetachingMag_Store, ok - aborting, detached magazine dropped to ground");
				else
					Error("[wpnfsm] WeaponDetachingMag_Store, error - cannot abort detaching of magazine");
			}

			m_weapon.SelectionMagazineHide(); // force hide on abort
		}

		m_magazine = NULL;
		m_dst = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		if (GameInventory.LocationAddEntity(m_dst))
		{
			wpnDebugPrint("[wpnfsm] WeaponDetachingMag_Store, ok - stored detached magazine");
		}
		else
		{
			// @TODO: drop on gnd
			Error("[wpnfsm] WeaponDetachingMag_Store, error - cannot store detached magazine!");
		}

		m_magazine = NULL;
		m_dst = NULL;
		super.OnExit(e);
	}
};

/*class WeaponDetachingMag_Store_W4T extends WeaponDetachingMag_Store
{
	override bool IsWaitingForActionFinish () { return true; }
};*/

class WeaponDetachingMag extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	Magazine m_magazine; /// detached magazine
	ref InventoryLocation m_dst; /// detached magazine destination

	ref WeaponStateBase m_start;
	ref WeaponDetachingMag_Store m_store;
	ref MagazineHide_W4T m_hideM;

	void WeaponDetachingMag (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponDetachingMag_1(m_weapon, this, m_action, m_actionType);
		m_store = new WeaponDetachingMag_Store(m_weapon, this);
		m_hideM = new MagazineHide_W4T(m_weapon, this);
		// events
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __md_ = new WeaponEventAnimMagazineDetached;
		WeaponEventBase __mh_ = new WeaponEventAnimMagazineHide;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start, __md_, m_store));
		m_fsm.AddTransition(new WeaponTransition(m_store, __mh_, m_hideM));
		m_fsm.AddTransition(new WeaponTransition(m_hideM, _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e != NULL)
		{
			WeaponEventDetachMagazine de;
			if (Class.CastTo(de, e))
			{
				wpnDebugPrint("WeaponDetachingMag type=" + typename.EnumToString(InventoryLocationType, de.m_dst.GetType()));
				m_magazine = e.m_magazine;
				m_dst = de.m_dst;
			}
		}

		m_store.m_magazine = m_magazine;
		m_store.m_dst = m_dst;
		wpnDebugPrint("WeaponDetachingMag type=" + typename.EnumToString(InventoryLocationType, m_store.m_dst.GetType()));
		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		m_dst = NULL;
		m_magazine = NULL;
		super.OnExit(e);
	}
};

