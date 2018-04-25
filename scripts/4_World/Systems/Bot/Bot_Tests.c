class BotTestSwapWithCorpse : BotStateBase
{
};

class BotTestSwapInternal : BotStateBase
{
	EntityAI m_Entity1;
	EntityAI m_Entity2;

	void BotTestSwapInternal (Bot bot = NULL, BotStateBase parent = NULL)
	{ }
	
	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
		EntityAI j = m_Owner.GetInventory().CreateAttachment("Jeans_Blue");
		m_Entity1 = j.GetInventory().CreateEntityInCargo("Rag");
		EntityAI s = m_Owner.GetInventory().CreateInInventory("TacticalShirt_Grey");
		m_Entity2 = s.GetInventory().CreateEntityInCargo("Roadflare");
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);
		
		if (m_Entity1 && m_Entity2)
		{
			if (m_Owner.GetInventory().CanSwapEntities(m_Entity1, m_Entity2))
			{
				botDebugPrint("[bot] -> swap item=" + m_Entity1 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity1, m_Entity2);
			}
			else if (m_Owner.GetInventory().CanSwapEntities(m_Entity2, m_Entity1))
			{
				botDebugPrint("[bot] <- swap item=" + m_Entity2 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity2, m_Entity1);
			}
		}
	}
};


class BotTestSwapInternalC2H : BotStateBase
{
	EntityAI m_Entity1;
	EntityAI m_Entity2;

	void BotTestSwapInternalC2H (Bot bot = NULL, BotStateBase parent = NULL)
	{ }
	
	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
		EntityAI j = m_Owner.GetInventory().CreateAttachment("Jeans_Blue");
		m_Entity1 = j.GetInventory().CreateEntityInCargo("Rag");
		EntityAI s = m_Owner.GetInventory().CreateInInventory("TacticalShirt_Grey");
		m_Entity2 = s.GetInventory().CreateEntityInCargo("Roadflare");
		m_Owner.PredictiveTakeEntityToHands(m_Entity2);
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);
		
		if (m_Entity1 && m_Entity2)
		{
			if (m_Owner.GetInventory().CanSwapEntities(m_Entity1, m_Entity2))
			{
				botDebugPrint("[bot] -> swap item=" + m_Entity1 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity1, m_Entity2);
			}
			else if (m_Owner.GetInventory().CanSwapEntities(m_Entity2, m_Entity1))
			{
				botDebugPrint("[bot] <- swap item=" + m_Entity2 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity2, m_Entity1);
			}
		}
	}
};


class BotTestSwapG2H : BotStateBase
{
	EntityAI m_Entity1;
	EntityAI m_Entity2;

	void BotTestSwapG2H (Bot bot = NULL, BotStateBase parent = NULL)
	{ }
	
	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
		EntityAI j = m_Owner.GetInventory().CreateAttachment("Jeans_Blue");
		m_Entity1 = j.GetInventory().CreateEntityInCargo("Rag");
		EntityAI s = m_Owner.GetInventory().CreateInInventory("TacticalShirt_Grey");
		m_Entity2 = s.GetInventory().CreateEntityInCargo("Roadflare");
		m_Owner.PredictiveDropEntity(m_Entity2);
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);
		
		/*if (m_Entity1 && m_Entity2)
		{
			if (m_Owner.GetInventory().CanSwapEntities(m_Entity1, m_Entity2))
			{
				botDebugPrint("[bot] -> swap item=" + m_Entity1 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity1, m_Entity2);
			}
			else if (m_Owner.GetInventory().CanSwapEntities(m_Entity2, m_Entity1))
			{
				botDebugPrint("[bot] <- swap item=" + m_Entity2 + " bot=" + m_Owner);
				m_Owner.PredictiveSwapEntities(m_Entity2, m_Entity1);
			}
		}*/
	}
};

