
/**@class		InventorySlots
 * @brief		provides access to slot configuration
 **/
class InventorySlots
{
	/**@NOTE: engine pre-populates this class with first N slots from DZ/data/config.cpp
	 * i.e. currently defined values are
	 *   InventorySlots.INVALID  (has value -1)
	 *   InventorySlots.SHOULDER (0)
	 *   InventorySlots.MELEE
	 *   InventorySlots.BOW
	 *   InventorySlots.HEADGEAR
	 *   InventorySlots.MASK
	 *   InventorySlots.EYEWEAR
	 *   InventorySlots.HANDS
	 *   InventorySlots.GLOVES
	 *   InventorySlots.ARMBAND
	 *   InventorySlots.VEST
	 *   InventorySlots.BODY
	 *   InventorySlots.BACK
	 *   InventorySlots.HIPS
	 *   InventorySlots.LEGS
	 *   InventorySlots.FEET
	 *   InventorySlots.MAGAZINE
	 *   InventorySlots.MAGAZINE2
	 *   InventorySlots.MAGAZINE3
	 *   InventorySlots.DRIVER
	 **/

	/**@fn		GetSlotIdFromString
	 * @brief	converts string to slot_id
	 * @param[in]	slot_name	\p	slot name to be find.
	 * @return		slot id or InventorySlots.INVALID
	 *
	 * @example 
	 *		int slot = InventorySlots.GetSlotIdFromString("FireWood");
	 *		if (slot != InventorySlots.INVALID)
	 *
	 * @NOTE the example looks in the DZ/data/config.cpp and searches for class entry Slot_##slot_name
	 *		class Slot_Firewood { whatever };
	 *
	 * i.e. it does NOT look at name= or displayName= attributes of the entry!
	 **/
	static proto native int GetSlotIdFromString (string slot_name);
	/**@fn		GetStackMaxForSlotId
	 * @param[in]	slot_id	\p	queried slot
	 * @return	stackMax attribute from corresponding entry (or default value == 1)
	 **/
	static proto native int GetStackMaxForSlotId (int slot_Id);
	/**@fn		GetSelectionForSlotId
	 * @param[in]	slot_id	\p	queried slot
	 * @param[out]	selection	\p	selection filled with attribute from corresponding entry
	 * @return	true if slot valid
	 **/
	static proto bool GetSelectionForSlotId (int slot_Id, out string selection);
	/**@fn		IsSlotIdValid
	 * @brief	verifies existence of the slot id
	 * @return	true if slot valid
	 **/
	static proto native bool IsSlotIdValid (int slot_Id);
	/**@fn		GetBoneNameForSlotId
	 * @param[in]	slot_id	\p	queried slot
	 * @param[out]	bone_name	\p	selection filled with attribute from corresponding entry
	 * @return	true if slot valid
	 **/	
	static proto bool GetBoneNameForSlotId (int slot_Id, out string bone_name);
	/**@fn		GetBoneIndexForSlotId
	 * @param[in]	slot_id	\p	queried slot
	 * @return	stackMax attribute from corresponding entry (or default value == 1)
	 **/
	//static proto native int GetBoneIndexForSlotId (int slot_Id);
};


