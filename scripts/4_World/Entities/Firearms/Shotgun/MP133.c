class LoopedChambering_Wait4ShowBullet extends WeaponStateBase { };

class ShotgunChambering extends WeaponStateBase
{
	WeaponActions m_action;
	int m_startActionType;
	int m_endActionType;
	Magazine m_srcMagazine; /// source of the cartridge

	ref WeaponStateBase m_start;
	ref WeaponChambering_Cartridge m_chamber;
	ref LoopedChambering_Wait4ShowBullet m_w4sb;
	ref LoopedChambering_EndLoop m_end;

	void ShotgunChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int startActionType = -1, int endActionType = -1)
	{
		m_action = action;
		m_startActionType = startActionType;
		m_endActionType = endActionType;

		// setup nested state machine
		m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_startActionType);
		m_chamber = new WeaponChambering_Cartridge(m_weapon, this);
		m_w4sb = LoopedChambering_Wait4ShowBullet(m_weapon, this);
		m_end = new LoopedChambering_EndLoop(m_weapon, this, m_action, m_endActionType);
		// events
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventContinuousLoadBulletStart __lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventContinuousLoadBulletEnd   __lE_ = new WeaponEventContinuousLoadBulletEnd;
		WeaponEventAnimBulletShow  __bs_ = new WeaponEventAnimBulletShow;
		WeaponEventAnimBulletInMagazine __bM_ = new WeaponEventAnimBulletInMagazine;
		WeaponEventAnimBulletHide  __bh_ = new WeaponEventAnimBulletHide;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start  , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb, NULL, new GuardAnd(new WeaponGuardHasAmmoInLoopedState(m_chamber), new WeaponGuardChamberHasRoomForMoreThanOne(m_weapon))));
		
		// @TODO: rychla sekvence start / stop zpusobi zaseknuti animace
		//m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_end, NULL, new GuardOr(new GuardNot(new WeaponGuardHasAmmoInLoopedState(m_chamber)), new WeaponGuardChamberHasRoomForOne(m_weapon))));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bh_, m_end, NULL, new GuardOr(new GuardNot(new WeaponGuardHasAmmoInLoopedState(m_chamber)), new WeaponGuardChamberHasRoomForOne(m_weapon))));
		m_fsm.AddTransition(new WeaponTransition(m_w4sb   , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_w4sb   , __lE_, m_end));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __lE_, m_end));
		m_fsm.AddTransition(new WeaponTransition(m_end    , _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e != NULL)
		{
			if (e.m_magazine != NULL)
			{
				wpnDebugPrint("[wpnfsm] LoopedChambering, taking mag from event e.mag=" + e.m_magazine.ToString());
				m_srcMagazine = e.m_magazine;
			}
		}
		else
		{
			wpnDebugPrint("[wpnfsm] LoopedChambering (e=NULL), m_srcMagazine=" + m_srcMagazine.ToString());
		}
		wpnDebugPrint("[wpnfsm] m_srcMagazine=" + m_srcMagazine.ToString());

		m_chamber.m_srcMagazine = m_srcMagazine;

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};


/**@class	Mp133Shotgun_Base
 * @brief	basic mp5 submachine gun
 **/
class Mp133Shotgun : Rifle_Base

	void Mp133Shotgun ()
	{
		// extend Rifle_Base fsm
		ShotgunChambering lch = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		ShotgunChambering psh = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		ShotgunChambering psh2 = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);

		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __lS_ = new WeaponEventContinuousLoadBulletStart;

		// load cartridge
		m_fsm.AddTransition(new WeaponTransition(E_C, __lS_, lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(lch, _fin_, L_C));

		m_fsm.AddTransition(new WeaponTransition(E_D, __lS_, lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(lch, _fin_, L_C));

		m_fsm.AddTransition(new WeaponTransition(L_C, __lS_, psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh, _fin_, L_C));

		// @TODO: chci load z loaded_fired_out - nefunguje mi nejak ten m_eject
		m_fsm.AddTransition(new WeaponTransition(E_D, __lS_, psh2, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh2, _fin_, L_C));
	}
};

