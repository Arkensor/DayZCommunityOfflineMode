class ActionAttachWheels: ActionAttach
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	int m_wheel_hub;
	
	void ActionAttachWheels()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've attached the object.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		
		m_wheel_hub = -1;
	}

	override int GetType()
	{
		return AT_ATTACH_WHEELS;
	}
		
	override string GetText()
	{
		return "attach wheels";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		string test = player.GetDisplayName();
		string test2 = item.GetType();
		string test3 = targetObject.GetDisplayName();
		
		if ( player && targetObject && item )
		{
			int componentIndex = target.GetComponentIndex();
			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_1_1") )
			{
				Print( "Predni leve" );
				m_wheel_hub = 0;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_1_2") )
			{
				Print( "zadni leve" );
				m_wheel_hub = 1;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_1_3") )
			{
				Print( "prostredni leve" );
				m_wheel_hub = 2;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_1_4") )
			{
				Print( "druhe predni leve" );
				m_wheel_hub = 3;
				return true;
			}
			
			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_2_1") )
			{
				Print( "Predni prave" );
				m_wheel_hub = 4;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_2_2") )
			{
				Print( "zadni prave" );
				m_wheel_hub = 5;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_2_3") )
			{
				Print( "prostredni prave" );
				m_wheel_hub = 6;
				return true;
			}

			if( targetObject.IsActionComponentPartOfSelection(componentIndex, "wheel_2_4") )
			{
				Print( "druhe predni prave" );
				m_wheel_hub = 7;
				return true;
			}		
		}
		
 		return false;
	}

	override void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity;
		Class.CastTo(target_entity,  target.GetObject() ); // cast to ItemBase
		int slot_index	= item.GetInventory().GetSlotId();
		int test	= m_wheel_hub;
		if (!target_entity.LocalTakeEntityAsAttachmentEx( item, slot_index ))
			target_entity.LocalTakeEntityAsAttachment( item );
	}
	override void OnCompleteLoopClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI target_entity;
		Class.CastTo(target_entity,  target.GetObject() ); // cast to ItemBase
		int slot_index	= item.GetInventory().GetSlotId();
		int test	= m_wheel_hub;
		if (!target_entity.LocalTakeEntityAsAttachmentEx( item, slot_index ))
			target_entity.LocalTakeEntityAsAttachment( item );
	}
};