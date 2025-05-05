/**
\brief Stores loaded scene data information from json file. ie object positions, loot positions, duration etc

*/
class SceneInfo
{

	//ref map< ObjectData, array< LootSpot >>  m_objects = new ref map< ObjectData, array< LootSpot >>;
	string name;
	ref array< ObjectData > m_objects = new ref array< ObjectData >;
	ref array< LootSpot > m_lootSpots = new ref array< LootSpot >;

	bool persistent = false;
	int duration = 0; // in seconds

	void SceneInfo( string sceneName ) 
	{
		name = sceneName;
	}

	void ~SceneInfo() 
	{

	}

	string GetName() 
	{
		return name;
	}

/*
	ObjectData GetObjectData( vector position ) 
	{
		for(int i = 0; i<m_objects.Count();i++ ) 
		{
			ObjectData data = m_objects.GetKey( i );
			if ( data.GetPosition() == position ) 
			{
				return data;
			}
		}
		return NULL;
	}
*/
	void AddObject( Object object, vector pos ) 
	{
		m_objects.Insert( new ObjectData( object.GetType(), pos, object.GetOrientation() ));
	}

	void AddLootSpot( vector pos, LootType type ) 
	{
		m_lootSpots.Insert( new LootSpot( pos, type ) );
	}

/* Do not use below */

/*
	void AddObject( Object object ) 
	{
		AddObject( object.GetType(), object.GetPosition(), object.GetOrientation() );
	}

	void AddObject( string classname, vector position, vector orientation ) 
	{
		m_objects.Insert( new ObjectData( classname, position, orientation ), new array< LootSpot > );
	}
*/
}

// create scene (set origin point to player position)
// create object containers with loot spots
// add object container to scene