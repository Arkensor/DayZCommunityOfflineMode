class groupData
 {
 	string name;
 	ref array<ref Object> objects;

 	void groupData(string name1, ref array<ref Object> objects1)
 	{
 		name = name1;
 		objects = objects1;
 	}

 	string getName()
 	{
 		return name;
 	}

 	array getObjects()
 	{
 		return objects;
 	}
 }