class actionData
 {
 	string action
 	int id;
 	string name;
 	vector pos, ypr; float scale;
 	string group, date; int active;
 	ref objectData data;
 	void actionData(string action1, int id1, string name1, vector pos1, vector ypr1, float scale1, string group1, string date1, int active1, ref objectData data1) {
 	    action = action1;
 	    id = id1;
        name = name1;
        pos = pos1;
        ypr = ypr1;
        scale = COM_RoundFloat(scale1);
        group = group1;
        date = date1;
        active = active1;
        data = data1;
    }

 	objectData getData() { return data; }
 	void setData(objectData data1) { data = data1; }

 	string getAction() { return action; }
 	void setAction(string action1) { action = action1; }

 	int getID() { return id; }
 	void setID(int id1) { id = id1; }

 	string getName() { return name; }
 	void setName(string name1) { name = name1; }

 	vector getPosition() { return pos; }
 	void setPosition(vector pos1) { pos = pos1; }

 	vector getOrientation() { return ypr; }
 	void setOrientation(vector ypr1) { ypr = ypr1; }

 	float getScale() { return COM_RoundFloat(scale); }
 	void setScale(float scale1) { scale = COM_RoundFloat(scale1); }

 	string getGroup() { return group; }
 	void setGroup(string group1) { group = group1; }

 	string getDate() { return date; }
 	void setDate(string date1) { date = date1; }

 	int getActive() { return active; }
 	void setActive(int active1) { active = active1; }
 	bool isActive() { return active == 1; }

 	void printData() {
 	    scriptLog(string.Format("Action: %1 - ID: %2 - Name: %3 - Pos: %4 - YPR: %5 - Scale: %6 - Group: %7 - Date: %8 - Active: %9", action, id.ToString(), name, COM_VectorToString(pos), COM_VectorToString(ypr), scale.ToString(), group, date, active.ToString()));
 	    auto oData = getData(); if(oData) { oData.printData(); }
 	}

 }