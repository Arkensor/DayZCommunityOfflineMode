class PositionData
{
	string name;
	vector pos;

	void PositionData( string name1, vector pos1 )
	{
		name = name1;
		pos = pos1;
	}

	string GetName()
	{
		return name;
	}

	vector GetPosition() 
	{
		return pos;
	}

}