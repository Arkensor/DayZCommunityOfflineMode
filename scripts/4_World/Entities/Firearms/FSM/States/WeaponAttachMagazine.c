// load magazine composite state (attach + load chamber)
class WeaponAttachingMagStartAction extends WeaponStartAction
{ };

class RemoveNewMagazineFromInventory_OnEntryShowMag extends RemoveNewMagazineFromInventory
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_weapon.SelectionMagazineShow();
	}
};


class WeaponAttachMagazine extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	Magazine m_newMagazine; /// source of the cartridge

	ref WeaponStateBase m_start;
	ref RemoveNewMagazineFromInventory_OnEntryShowMag m_getNew;
	ref AttachNewMagazine m_attach;
	ref WeaponChamberFromAttMag_W4T m_chamber;
	ref WeaponCharging_CK m_onCK;

	void WeaponAttachMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponAttachingMagStartAction(m_weapon, this, m_action, m_actionType);
		m_getNew = new RemoveNewMagazineFromInventory_OnEntryShowMag(m_weapon, this);
		m_attach = new AttachNewMagazine(m_weapon, this);
		m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
		m_onCK = new WeaponCharging_CK(m_weapon, this);

		// events: MS, MA, BE, CK
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __ms_ = new WeaponEventAnimMagazineShow;
		WeaponEventBase __ma_ = new WeaponEventAnimMagazineAttached;
		//WeaponEventBase __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase __ck_ = new WeaponEventAnimCocked;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		m_fsm.AddTransition(new WeaponTransition(   m_start, __ms_, m_getNew));
		m_fsm.AddTransition(new WeaponTransition(  m_getNew, __ma_, m_attach));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_chamber, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new WeaponGuardHasAmmo(m_weapon)))); // when opened, there is no __be_ event
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new GuardNot(new WeaponGuardHasAmmo(m_weapon)))));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_chamber, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_onCK, _fin_, NULL));
		

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		m_newMagazine = e.m_magazine;
		m_getNew.m_newMagazine = m_newMagazine;
		m_getNew.m_newSrc = new InventoryLocation;
		m_getNew.m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_getNew.m_newSrc);

		InventoryLocation il = new InventoryLocation;
		il.SetAttachment(m_weapon, m_newMagazine, InventorySlots.MAGAZINE);
		//wpnDebugPrint("[wpnfsm] WeaponAttach, attaching mag=" + m_newMagazine.ToString() +  "to loc=" + il.ToString());
		m_attach.m_newMagazine = m_newMagazine;
		m_attach.m_newDst = il;

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (WeaponEventBase e)
	{
		m_newMagazine = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;

		super.OnExit(e);
	}
};

