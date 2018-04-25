//this class allows for creation and manipulation of arrays which hold unique IDs, and allow for bitwise-like operation over these IDs
//essentially, this extends single INT to any legth when it comes to holding and manipulating individual bits

const int BIT_INT_SIZE = 32;
class BitArray
{	
	static int IDToIndex(int id)
	{
		int m_NumOfItems = BitArray.IDToNumOfItems(id);
		int index 	= (id - (m_NumOfItems * BIT_INT_SIZE)) - 1;
		return index;
	}

	static int IDToMask(int id)
	{
		int index 			= BitArray.IDToIndex(id);
		int bit 			= 1 << index;
		return bit;
	}
	//! Creates an array which will be able to accomodate 'size' number of unique IDs
	static array<int> CreateMaskArray(int size)
	{
		int m_NumOfItems 		= BitArray.IDToNumOfItems(size);
		array<int> bit_array 	= new array<int>;

		for(int i = 0;i <= m_NumOfItems;i++)
		{
			bit_array.Insert(0);
		}
		
		return bit_array;
	}
	//! Returns number of bits this array can hold
	static int GetBitCount( array<int> bit_array )
	{
		return ( (bit_array.Count() ) * BIT_INT_SIZE );
	}

	//! Returns number of INTs required to hold this ID minus 1(ie for 32, the result is 0, for 33, the result is 1)
	static int IDToNumOfItems(int id)
	{
		int m_NumOfItems = id / BIT_INT_SIZE;
		if( id % BIT_INT_SIZE == 0 ) m_NumOfItems--;
		return m_NumOfItems;
	}
	
	static array<int> CreateArrayWithID(int id)
	{
		int m_NumOfItems 	= BitArray.IDToNumOfItems(id);//TODO: check for too big id
		int bit 			= BitArray.IDToMask(id);
		
		array<int> bit_array = new array<int>;
		
		for(int i = 0;i <= m_NumOfItems;i++)
		{
			bit_array.Insert(0);
		}
		bit_array.Set(m_NumOfItems, bit);
		return bit_array;
	}
	
	static bool IsMaskContainID(array<int> bit_array, int id)
	{
		if( BitArray.IDToNumOfItems( id ) > bit_array.Count() )
		{
			return false;
		}
		else
		{
			int index_mask 		=  BitArray.IDToMask(id);
			int num_of_items 	= BitArray.IDToNumOfItems(id);
			int array_mask 		= bit_array.Get( num_of_items );
			
			if ( index_mask & array_mask )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	static bool AddIDToMask(array<int> bit_array, int id)
	{
		if( BitArray.IDToNumOfItems(id) > bit_array.Count() )
		{
			return false;
		}
		else
		{
			int num_of_items 	= BitArray.IDToNumOfItems(id);
			int bit 			= BitArray.IDToMask(id);
			int new_mask 		= bit_array.Get(num_of_items) | bit;
			bit_array.Set(num_of_items, new_mask);
			return true;
		}
	}
};