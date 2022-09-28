class OldObjectData
 {
 	string name;
 	string group;
    string date;
 	int active;
 	vector pos;
 	vector ypr;
 	void OldObjectData(string name1, string group1, string date1, int active1, vector pos1, vector ypr1) {
        name = name1;
        group = group1;
        date = date1;
        active = active1;
        pos = pos1;
        ypr = ypr1;
    }
 	string GetType()
 	{
 		return name;
 	}
 	string getGroup()
 	{
 		return group;
 	}
 	vector GetPosition()
 	{
 		return pos;
 	}
 	vector GetOrientation()
 	{
 		return ypr;
 	}

 }