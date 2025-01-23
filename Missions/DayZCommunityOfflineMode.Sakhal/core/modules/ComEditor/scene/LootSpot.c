class LootSpot
{
	LootType type;
	vector position;

	void LootSpot( vector pos, LootType lootType ) 
	{
		position = pos;
		type = lootType;
	}

	vector GetPosition() 
	{
		return position;
	}

	LootType GetType() 
	{
		return type;
	}
}

enum LootType 
{
	INDUSTRIAL, // tools
	VEHICLE, // vehicle parts
	MEDICAL, // medical items
	CIVILIAN, // civ items - axe, can opener, knife, etc
	FOOD,
	MILITARY
}