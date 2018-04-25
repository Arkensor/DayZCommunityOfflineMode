class ActionCoverPlantedSeed: ActionInteractBase
{
	void ActionCoverPlantedSeed()
	{
		m_MessageSuccess = "";
		m_MessageStart = "";
	}

	override int GetType()
	{
		return AT_COVER_PLANTED_SEED;
	}

	override string GetText()
	{
		return "cover planted slot";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject  &&  targetObject.IsInherited( GardenBase) )
		{
			GardenBase garden_base = GardenBase.Cast( targetObject );
			
			float c_index = target.GetComponentIndex();		
			string selection = targetObject.GetActionComponentName(c_index);
			Slot slot = garden_base.GetSlotBySelection( selection );
			
			if (slot)
			{
				int slot_state = slot.m_State;
				bool is_slot_correct = slot.IsPlanted();
				bool is_slot_free = (slot.GetPlant() == NULL);
				bool is_slot_seeded = (slot.HasSeed());
				
				if ( is_slot_correct  &&  is_slot_free  &&  is_slot_seeded )
				{
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		bool can_complete = ActionCondition( player, target, item );
		
		if (can_complete) // Double check status of the slot after the animation
		{
			GardenBase garden_base = GardenBase.Cast( targetObject );
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			Slot slot = garden_base.GetSlotBySelection( selection );
			garden_base.CreatePlant(slot);
		}
	}
};