// Added scale.
// - Brandon10x15

class ObjectData
{
	string name;
	vector pos;
	vector ypr;
	float scale;

	void ObjectData( string name1, vector pos1, vector ypr1, float scale1 )
	{
		name = name1;
		pos = pos1;
		ypr = ypr1;
		scale = scale1;
	}

	string GetClassName() 
	{
		return name;
	}

	vector GetPosition() 
	{
		return pos;
	}

	vector GetOrientation()
	{
		return ypr;
	}

	float GetScale()
	{
		return scale;
	}
}