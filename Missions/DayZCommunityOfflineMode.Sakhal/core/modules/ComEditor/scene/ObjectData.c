class ObjectData 
{
	string classname;
	vector position;
	vector orientation;

	void ObjectData( string name, vector pos, vector ori )
	{
		classname = name;
		position = pos;
		orientation = ori;
	}

	string GetClassName() 
	{
		return classname;
	}

	vector GetPosition() 
	{
		return position;
	}

	vector GetOrientation() 
	{
		return orientation;
	}
}