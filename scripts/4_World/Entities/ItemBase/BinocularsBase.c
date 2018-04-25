class BinocularsBase extends Weapon
{	
	/**
	\brief Returns number of slots for attachments corrected for weapons
	*/
	override int GetSlotsCountCorrect()
	{
		return GetInventory().GetSlotsCount();
	};
}
