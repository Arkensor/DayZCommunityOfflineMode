class PileOfWoodenPlanks extends ItemBase
{	
	// Shows/Hides selections. Call this in init or after every quantity change.
	void UpdateSelections()
	{
		// Show/Hide selections according to the current quantity
		if ( this )
		{
			float quantity = GetQuantity();
			float quantity_max = GetQuantityMax();
			Print(quantity);
			Print(quantity_max);
			if ( quantity > GetQuantityMax() *0.66 )
			{
				// Show 3/3 amount of planks
				SetAnimationPhase ( "stage_3", 0 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity > quantity_max *0.33  &&  quantity <= quantity_max *0.66 )
			{
				// Show 2/3 amount of planks
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 0 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity > 0  &&  quantity <= quantity_max *0.33 )
			{
				// Show 1/3 amount of planks
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 0 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity == 0 )
			{
				// Show 0 planks. Object should be deleted now.
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
		}
	}
	
	// Removes a number of long planks. Note that one (4m) long plank has the length of 3x normal planks!
	// Returns the number of removed long planks
	int RemovePlanks( int needed_planks )
	{
		// Make sure to not give more long planks than we have available
		int available_planks = needed_planks;
		if ( available_planks > GetQuantity() )
		{
			available_planks = GetQuantity();
		}
		
		// Remove long planks from this object
		AddQuantity( -available_planks, true ); // Autodelete enabled
		
		// Visual feedback
		UpdateSelections();
		
		// Return the number of removed long planks
		return available_planks;
	}
}