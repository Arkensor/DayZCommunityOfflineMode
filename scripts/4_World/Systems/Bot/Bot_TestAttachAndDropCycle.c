class BotEventEntityAttached : BotEventBase
{
	EntityAI m_Entity;
	void BotEventEntityAttached (PlayerBase p = NULL, EntityAI att = NULL) { m_Entity = att; }
};

class BotEventEntityDetached : BotEventBase
{
	EntityAI m_Entity;
	void BotEventEntityDetached (PlayerBase p = NULL, EntityAI att = NULL) { m_Entity = att; }
};


class BotTestAttachAndDropCycle extends BotStateBase
{
	EntityAI m_Entity;
	ref BotTestAttachAndDropCycle_Detaching m_Detaching;
	ref BotTestAttachAndDropCycle_Attaching m_Attaching;

	void BotTestAttachAndDropCycle (Bot bot = NULL, BotStateBase parent = NULL)
	{
		// setup nested state machine
		m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm

		m_Detaching = new BotTestAttachAndDropCycle_Detaching(m_Bot, this);
		m_Attaching = new BotTestAttachAndDropCycle_Attaching(m_Bot, this);
		
		// events
		BotEventBase __EntAtt__ = new BotEventEntityAttached;
		BotEventBase __EntDet__ = new BotEventEntityDetached;

		// transitions
		m_FSM.AddTransition(new BotTransition(   m_Detaching, __EntDet__, m_Attaching));
		m_FSM.AddTransition(new BotTransition(   m_Attaching, __EntAtt__, m_Detaching));

		m_FSM.SetInitialState(m_Detaching);
	}

	override void OnEntry (BotEventBase e)
	{
		m_Entity = m_Owner.GetInventory().CreateAttachment("TaloonBag_Orange");
		m_Detaching.m_Entity = m_Entity;
		m_Attaching.m_Entity = m_Entity;

		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e)
	{
		m_Entity = null;

		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);
	}
};

class BotTestAttachAndDropCycle_Detaching extends BotStateBase
{
	EntityAI m_Entity;

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }

	override void OnExit (BotEventBase e)
	{
		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		if (m_Entity)
		{
			botDebugPrint("[bot] drop item=" + m_Entity + " bot=" + m_Owner);
			
			m_Owner.PredictiveDropEntity(m_Entity);

			InventoryLocation loc = new InventoryLocation;
			if (m_Entity.GetInventory().GetCurrentInventoryLocation(loc))
			{
				if (loc.GetType() == InventoryLocationType.GROUND)
				{
					m_Bot.ProcessEvent(new BotEventEntityDetached(m_Owner, m_Entity));
				}
			}
		}
	}
};

class BotTestAttachAndDropCycle_Attaching extends BotStateBase
{
	EntityAI m_Entity;

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }

	override void OnExit (BotEventBase e)
	{
		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		if (m_Entity)
		{
			botDebugPrint("[bot] att item=" + m_Entity + " bot=" + m_Owner);
			
			if (m_Owner.GetInventory().CanAddAttachment(m_Entity))
			{
				m_Owner.PredictiveTakeEntityAsAttachment(m_Entity);
	
				InventoryLocation loc = new InventoryLocation;
				if (m_Entity.GetInventory().GetCurrentInventoryLocation(loc))
				{
					if (loc.GetType() == InventoryLocationType.ATTACHMENT)
					{
						m_Bot.ProcessEvent(new BotEventEntityAttached(m_Owner, m_Entity));
					}
				}
			}
		}
	}
};

