class SmallGuts extends Edible_Base
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		const int SLOTS_ARRAY = 8;
		bool is_barrel = false;
		bool is_opened_barrel = false;
		bool slot_test = true;
		string slot_names[SLOTS_ARRAY] = { "BerryR", "BerryB", "Nails", "OakBark", "BirchBark", "Lime", "Disinfectant" };

		
		// is barrel
		if ( parent.IsKindOf("Barrel_ColorBase") )
		{
			is_barrel = true;
		}

		// is opened barrel				
		if ( is_barrel && parent.GetAnimationPhase("Lid") == 1 )
		{
			is_opened_barrel = true;
		}

		// all of the barrel slots are empty
		for ( int i = 0; i < SLOTS_ARRAY ; i++ )
		{
			if ( parent.FindAttachmentBySlotName(slot_names[i]) != NULL )
			{
				slot_test = false;
				break;
			}
		}
		
		if ( ( is_opened_barrel && slot_test ) || !is_barrel )
		{
			return true;
		}
		return false;
	}

	override bool CanDetachAttachment( EntityAI parent )
	{
		
		bool is_barrel = false;
		bool is_opened_barrel = false;
		
		// is barrel
		if ( parent.IsKindOf("Barrel_ColorBase") )
		{
			is_barrel = true;
		}

		// is opened barrel				
		if ( is_barrel && parent.GetAnimationPhase("Lid") == 1 )
		{
			is_opened_barrel = true;
		}
		
		if ( is_opened_barrel || !is_barrel )
		{
			return true;
		}
		return false;
	}
}
