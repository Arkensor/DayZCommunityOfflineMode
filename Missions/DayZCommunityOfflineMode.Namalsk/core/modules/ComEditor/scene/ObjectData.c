class objectData
 {
 	string name, group, date; int active; vector pos, ypr; float scale;
 	void objectData(string name1, string group1, string date1, int active1, vector pos1, vector ypr1, float scale1) {
        name = name1;
        if(!group1) { group1 = "None"; }
        group = group1;
        if(!date1) { date1 = getDateTime(); }
        date = date1;
        if(!active1) { active1 = 1; }
        active = active1;
        pos = pos1;
        ypr = ypr1;
        if(!scale1) { scale1 = 1; }
        scale = COM_RoundFloat(scale1);
    }

 	objectData duplicate() { return new objectData(name, group, date, active, pos, ypr, scale); }

 	string getName() { return name; }
 	void setName(string name1) { name = name1; }

 	string getGroup() { return group; }
 	void setGroup(string group1) { group = group1; }

 	string getDate() { return date; }
 	void setDate(string date1) { date = date1; }

 	int getActive() { return active; }
 	void setActive(int active1) { active = active1; }
 	bool isActive() { return active == 1; }

 	vector getPosition() { return pos; }
 	void setPosition(vector pos1) { pos = pos1; }

 	vector getOrientation() { return ypr; }
 	void setOrientation(vector ypr1) { ypr = ypr1; }

 	float getScale() { return COM_RoundFloat(scale); }
 	void setScale(float scale1) { scale = COM_RoundFloat(scale1); }

 	void printData() {
 	    scriptLog(string.Format("Name: %1 - Group: %2 - Date: %3 - Active: %4 - Pos: %5 - YPR: %6 - Scale: %7", name, group, date, active.ToString(), COM_VectorToString(pos), COM_VectorToString(ypr), scale.ToString()));
 	}

 }