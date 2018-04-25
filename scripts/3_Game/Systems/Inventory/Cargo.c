/**@class	Cargo
 * @brief	represents grid like storage for entities
 **/
class Cargo : Managed
{
	/// width of the cargo
	proto native int GetWidth ();
	/// height of the cargo
	proto native int GetHeight ();
	/// number of items in cargo
	proto native int GetItemCount ();
	/// get item at specific index
	proto native EntityAI GetItem (int index);
	/// get item at coordinates (row, col)
	proto native EntityAI FindEntityInCargoOn (int row, int col);
	/// return index of item in cargo (or -1 if not found)
	proto native int FindEntityInCargo (EntityAI e);

	proto native bool CanMoveEntityInCargoEx (EntityAI e, int row, int col); /// can move from current location in cargo to new one
	proto native bool CanSwapEntityInCargo (EntityAI oldItem, EntityAI newItem);

	/// get cargo owning entity
	proto native EntityAI GetParent ();

	proto void GetItemSize (int index, out int w, out int h);
	proto void GetItemPos (int index, out int x, out int y);
	
	private void Cargo();
	private void ~Cargo();
};

