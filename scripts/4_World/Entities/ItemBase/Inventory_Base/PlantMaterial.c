class PlantMaterial extends Inventory_Base
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		const int SLOTS_ARRAY1 = 6;
		const int SLOTS_ARRAY2 = 7;
		bool is_barrel = false;
		bool is_opened_barrel = false;
		bool slot_test1 = true;
		bool slot_test2 = true;
		string slot_names1[SLOTS_ARRAY1] = { "Nails", "OakBark", "BirchBark", "Lime", "Disinfectant", "Guts" };
		string slot_names2[SLOTS_ARRAY2] = { "BerryR", "BerryB", "Nails", "OakBark", "BirchBark", "Lime", "Disinfectant" };

		
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

		// all of the barrel attachment slots are empty (first set of ingredients)
		for ( int idx1 = 0; idx1 < SLOTS_ARRAY1 ; idx1++ )
		{
			if ( parent.FindAttachmentBySlotName(slot_names1[idx1]) != NULL )
			{
				slot_test1 = false;
				break;
			}
		}

		// all of the barrel attachment slots are empty (second set of ingredients)	
		for ( int idx2 = 0; idx2 < SLOTS_ARRAY2 ; idx2++ )
		{
			if ( parent.FindAttachmentBySlotName(slot_names2[idx2]) != NULL )
			{
				slot_test2 = false;
				break;
			}
		}
		
		if ( ( is_opened_barrel && slot_test1 ) || ( is_opened_barrel && slot_test2) || !is_barrel )
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