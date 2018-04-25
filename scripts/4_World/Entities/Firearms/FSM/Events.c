/**@enum	WeaponEventID
 * @brief	identifier for events.
 * mainly for rpc purposes
 **/
enum WeaponEventID
{
	UNKNOWN,
	MECHANISM,
	TRIGGER,
	TRIGGER_AUTO_START,
	TRIGGER_AUTO_END,
	LOAD1_BULLET,
	CONTINUOUS_LOADBULLET_START,
	CONTINUOUS_LOADBULLET_END,
	UNJAM,
	ATTACH_MAGAZINE,
	UNJAMMING_FAILED_TIMEOUT,
	UNJAMMING_TIMEOUT,
	DETACH_MAGAZINE,
	SWAP_MAGAZINE,
	HUMANCOMMAND_ACTION_FINISHED,
	HUMANCOMMAND_ACTION_ABORTED,
	RELOAD_TIMEOUT,
	DRY_FIRE_TIMEOUT,
	ANIMATION_EVENT,
};


/**@class	WeaponEventBase
 * @brief	represents event that triggers transition from state to state
 **/
class WeaponEventBase
{
	int m_eventID;
	DayZPlayer m_player;
	Magazine m_magazine;

	void WeaponEventBase (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = 0; m_player = p; m_magazine = m; }
	/// returns id from enum WeaponEventID
	WeaponEventID GetEventID () { return m_eventID; }
	/// returns animation event from anim engine, @see WeaponEvents
	WeaponEvents GetAnimEvent () { return -1; }
	/// returns eventID in upper 16 bits and AnimEvent in lower 16 bits
	int GetPackedType ()
	{
		int animEvent = GetAnimEvent() & 0x0000ffff;
		int evShl = m_eventID << 16;
		return evShl | animEvent;
	}

	void ReadFromContext (ParamsReadContext ctx) { }
	void WriteToContext (ParamsWriteContext ctx)
	{
		int packedType = GetPackedType();
		ctx.Write(packedType);
		ctx.Write(m_player);
		ctx.Write(m_magazine);
	}
};

/**@brief		signalize mechanism manipulation
 **/
class WeaponEventMechanism extends WeaponEventBase
{
	void WeaponEventMechanism (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.MECHANISM; }
};
/**@brief		event when trigger pressed
 **/
class WeaponEventTrigger extends WeaponEventBase
{
	void WeaponEventTrigger (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.TRIGGER; }
};
/**@brief		event when trigger pressed
 **/
class WeaponEventTriggerAutoStart extends WeaponEventBase
{
	void WeaponEventTriggerAutoStart (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.TRIGGER_AUTO_START; }
};
/**@brief		event when trigger pressed
 **/
class WeaponEventTriggerAutoEnd extends WeaponEventBase
{
	void WeaponEventTriggerAutoEnd (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.TRIGGER_AUTO_END; }
};
/**@brief		event when user wants to load 1 bullet
 **/
class WeaponEventLoad1Bullet extends WeaponEventBase
{
	void WeaponEventLoad1Bullet (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.LOAD1_BULLET; }
};
/**@brief		event when user wants to load N bullets
 **/
class WeaponEventContinuousLoadBulletStart extends WeaponEventBase
{
	void WeaponEventContinuousLoadBulletStart (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.CONTINUOUS_LOADBULLET_START; }
};
/**@brief		event when user wants to end loading of bullets
 **/
class WeaponEventContinuousLoadBulletEnd extends WeaponEventBase
{
	void WeaponEventContinuousLoadBulletEnd (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.CONTINUOUS_LOADBULLET_END; }
};
/**@brief		event when user wants to unjam weapon
 **/
class WeaponEventUnjam extends WeaponEventBase
{
	void WeaponEventUnjam (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.UNJAM; }
};
/**@brief		event when user wants to attach magazine
 **/
class WeaponEventAttachMagazine extends WeaponEventBase
{
	void WeaponEventAttachMagazine (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.ATTACH_MAGAZINE; }
};
/**@brief		triggered when unjamming timer expires
 **/
class WeaponEventUnjammingFailedTimeout extends WeaponEventBase
{
	void WeaponEventUnjammingFailedTimeout (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.UNJAMMING_FAILED_TIMEOUT; }
};
/**@brief		triggered when unjamming failed timer expires
 **/
class WeaponEventUnjammingTimeout extends WeaponEventBase
{
	void WeaponEventUnjammingTimeout (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.UNJAMMING_TIMEOUT; }
};
/**@brief		event when user wants to detach magazine
 * @param dst \p  inventory location where the magazine will be stored
 **/
class WeaponEventDetachMagazine extends WeaponEventBase
{
	ref InventoryLocation m_dst;
	void WeaponEventDetachMagazine (DayZPlayer p = NULL, Magazine m = NULL, InventoryLocation il = NULL) { m_eventID =  WeaponEventID.DETACH_MAGAZINE; m_dst = il; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		m_dst = new InventoryLocation;
		super.ReadFromContext(ctx);
		m_dst.ReadFromContext(ctx);
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		m_dst.WriteToContext(ctx);
	}
};
/**@brief		event when user wants to swap magazine
 **/
class WeaponEventSwapMagazine extends WeaponEventBase
{
	void WeaponEventSwapMagazine (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.SWAP_MAGAZINE; }
};
/**@brief		triggered when animation action finishes
 **/
class WeaponEventHumanCommandActionFinished extends WeaponEventBase
{
	void WeaponEventHumanCommandActionFinished (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.HUMANCOMMAND_ACTION_FINISHED; }
};
/**@brief		triggered when animation action aborts
 **/
class WeaponEventHumanCommandActionAborted extends WeaponEventBase
{
	void WeaponEventHumanCommandActionAborted (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.HUMANCOMMAND_ACTION_ABORTED; }
};
/**@brief		triggered when reload timer expires
 **/
class WeaponEventReloadTimeout extends WeaponEventBase
{
	void WeaponEventReloadTimeout (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.RELOAD_TIMEOUT; }
};
/**@brief		triggered when dry fire timer expires
 **/
class WeaponEventDryFireTimeout extends WeaponEventBase
{
	void WeaponEventDryFireTimeout (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.DRY_FIRE_TIMEOUT; }
};


class WeaponEventAnimation extends WeaponEventBase
{
	WeaponEvents m_type; /// animation event from Human.WeaponEvents
	void WeaponEventAnimation (DayZPlayer p = NULL, Magazine m = NULL) { m_eventID = WeaponEventID.ANIMATION_EVENT; }
	override int GetAnimEvent () { return m_type; }
};

/// creates weapon fsm events
WeaponEventBase WeaponEventFactory (WeaponEventID id, int aetype, DayZPlayer p = NULL, Magazine m = NULL)
{
	switch (id)
	{
		case WeaponEventID.UNKNOWN: return null;
		case WeaponEventID.MECHANISM: return new WeaponEventMechanism(p, m);
		case WeaponEventID.TRIGGER: return new WeaponEventTrigger(p, m);
		case WeaponEventID.LOAD1_BULLET: return new WeaponEventLoad1Bullet(p, m);
		case WeaponEventID.CONTINUOUS_LOADBULLET_START: return new WeaponEventContinuousLoadBulletStart(p, m);
		case WeaponEventID.CONTINUOUS_LOADBULLET_END: return new WeaponEventContinuousLoadBulletEnd(p, m);
		case WeaponEventID.UNJAM: return new WeaponEventUnjam(p, m);
		case WeaponEventID.ATTACH_MAGAZINE: return new WeaponEventAttachMagazine(p, m);
		case WeaponEventID.UNJAMMING_FAILED_TIMEOUT: return new WeaponEventUnjammingFailedTimeout(p, m);
		case WeaponEventID.UNJAMMING_TIMEOUT: return new WeaponEventUnjammingTimeout(p, m);
		case WeaponEventID.DETACH_MAGAZINE: return new WeaponEventDetachMagazine(p, m);
		case WeaponEventID.SWAP_MAGAZINE: return new WeaponEventSwapMagazine(p, m);
		case WeaponEventID.HUMANCOMMAND_ACTION_FINISHED: return new WeaponEventHumanCommandActionFinished(p, m);
		case WeaponEventID.HUMANCOMMAND_ACTION_ABORTED: return new WeaponEventHumanCommandActionAborted(p, m);
		case WeaponEventID.RELOAD_TIMEOUT: return new WeaponEventReloadTimeout(p, m);
		case WeaponEventID.DRY_FIRE_TIMEOUT: return new WeaponEventDryFireTimeout(p, m);
		case WeaponEventID.ANIMATION_EVENT: return WeaponAnimEventFactory(aetype, p, m);
	}
	return NULL;
}

/// creates animation system events
WeaponEventBase WeaponAnimEventFactory (WeaponEvents type, DayZPlayer p = NULL, Magazine m = NULL)
{
	switch (type)
	{
		case WeaponEvents.ATTACHMENT_HIDE: return new WeaponEventAnimAttachmentHide(p, m);
		case WeaponEvents.ATTACHMENT_SHOW: return new WeaponEventAnimAttachmentShow(p, m);
		case WeaponEvents.BULLET_EJECT: return new WeaponEventAnimBulletEject(p, m);
		case WeaponEvents.BULLET_HIDE: return new WeaponEventAnimBulletHide(p, m);
		case WeaponEvents.BULLET_HIDE2: return new WeaponEventAnimBulletHide2(p, m);
		case WeaponEvents.BULLET_IN_CHAMBER: return new WeaponEventAnimBulletInChamber(p, m);
		case WeaponEvents.BULLET_IN_MAGAZINE: return new WeaponEventAnimBulletInMagazine(p, m);
		case WeaponEvents.BULLET_SHOW: return new WeaponEventAnimBulletShow(p, m);
		case WeaponEvents.BULLET_SHOW2: return new WeaponEventAnimBulletShow2(p, m);
		case WeaponEvents.CANUNJAM_END: return new WeaponEventAnimCanUnjamEnd(p, m);
		case WeaponEvents.CANUNJAM_START: return new WeaponEventAnimCanUnjamStart(p, m);
		case WeaponEvents.COCKED: return new WeaponEventAnimCocked(p, m);
		case WeaponEvents.MAGAZINE_ATTACHED: return new WeaponEventAnimMagazineAttached(p, m);
		case WeaponEvents.MAGAZINE_DETACHED: return new WeaponEventAnimMagazineDetached(p, m);
		case WeaponEvents.MAGAZINE_HIDE: return new WeaponEventAnimMagazineHide(p, m);
		case WeaponEvents.MAGAZINE_SHOW: return new WeaponEventAnimMagazineShow(p, m);
		case WeaponEvents.SLIDER_OPEN: return new WeaponEventAnimSliderOpen(p, m);
		case WeaponEvents.UNJAMMED: return new WeaponEventAnimUnjammed(p, m);
		case WeaponEvents.HAMMER_UNCOCKED: return new WeaponEventAnimHammerUncocked(p, m);
		case WeaponEvents.HAMMER_COCKED: return new WeaponEventAnimHammerCocked(p, m);
		//case WeaponEvents.: return new WeaponEventAnim(p, m);
	}
	return NULL;
}

WeaponEventBase CreateWeaponEventFromContext (ParamsReadContext ctx)
{
	int packedType;
	ctx.Read(packedType);
	DayZPlayer player;
	ctx.Read(player);
	Magazine magazine;
	ctx.Read(magazine);

	WeaponEventID eventID = packedType >> 16;
	WeaponEvents animEvent = packedType & 0x0000ffff;
	WeaponEventBase b = WeaponEventFactory(eventID, animEvent, player, magazine);
	b.ReadFromContext(ctx);
	return b;
}

class WeaponEventAnimAttachmentHide extends WeaponEventAnimation
{
	void WeaponEventAnimAttachmentHide (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.ATTACHMENT_HIDE; }
};

class WeaponEventAnimAttachmentShow extends WeaponEventAnimation
{
	void WeaponEventAnimAttachmentShow (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.ATTACHMENT_SHOW; }
};

class WeaponEventAnimBulletEject extends WeaponEventAnimation
{
	void WeaponEventAnimBulletEject (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_EJECT; }
};

class WeaponEventAnimBulletHide extends WeaponEventAnimation
{
	void WeaponEventAnimBulletHide (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_HIDE; }
};

class WeaponEventAnimBulletHide2 extends WeaponEventAnimation
{
	void WeaponEventAnimBulletHide2 (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_HIDE2; }
};

class WeaponEventAnimBulletInChamber extends WeaponEventAnimation
{
	void WeaponEventAnimBulletInChamber (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_IN_CHAMBER; }
};

class WeaponEventAnimBulletInMagazine extends WeaponEventAnimation
{
	void WeaponEventAnimBulletInMagazine (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_IN_MAGAZINE; }
};

class WeaponEventAnimBulletShow extends WeaponEventAnimation
{
	void WeaponEventAnimBulletShow (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_SHOW; }
};

class WeaponEventAnimBulletShow2 extends WeaponEventAnimation
{
	void WeaponEventAnimBulletShow2 (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.BULLET_SHOW2; }
};

class WeaponEventAnimCanUnjamEnd extends WeaponEventAnimation
{
	void WeaponEventAnimCanUnjamEnd (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.CANUNJAM_END; }
};

class WeaponEventAnimCanUnjamStart extends WeaponEventAnimation
{
	void WeaponEventAnimCanUnjamStart (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.CANUNJAM_START; }
};

class WeaponEventAnimCocked extends WeaponEventAnimation
{
	void WeaponEventAnimCocked (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.COCKED; }
};

class WeaponEventAnimMagazineAttached extends WeaponEventAnimation
{
	void WeaponEventAnimMagazineAttached (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.MAGAZINE_ATTACHED; }
};

class WeaponEventAnimMagazineDetached extends WeaponEventAnimation
{
	void WeaponEventAnimMagazineDetached (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.MAGAZINE_DETACHED; }
};

class WeaponEventAnimMagazineHide extends WeaponEventAnimation
{
	void WeaponEventAnimMagazineHide (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.MAGAZINE_HIDE; }
};

class WeaponEventAnimMagazineShow extends WeaponEventAnimation
{
	void WeaponEventAnimMagazineShow (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.MAGAZINE_SHOW; }
};

class WeaponEventAnimSliderOpen extends WeaponEventAnimation
{
	void WeaponEventAnimSliderOpen (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.SLIDER_OPEN; }
};

class WeaponEventAnimUnjammed extends WeaponEventAnimation
{
	void WeaponEventAnimUnjammed (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.UNJAMMED; }
};


class WeaponEventAnimHammerUncocked extends WeaponEventAnimation
{
	void WeaponEventAnimHammerUncocked (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.HAMMER_UNCOCKED; }
};

class WeaponEventAnimHammerCocked extends WeaponEventAnimation
{
	void WeaponEventAnimHammerCocked (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.HAMMER_COCKED; }
};

/*
class WeaponEventAnim extends WeaponEventAnimation
{
	void WeaponEventAnim (DayZPlayer p = NULL, Magazine m = NULL) { m_type = WeaponEvents.; }
};
*/

