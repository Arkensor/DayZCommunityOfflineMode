//-----------------------------------------------------------------------------
/**
 \brief Serialization general interface. Serializer API works with:
	- primitive types: int, float, string, bool, vector
	- dynamic containers: array, set, map
	- static arrays
	- complex types: classes
 \note Serializer provides deep serialization (it serialize class memebers and their members etc). To avoid serialization of certain class variable, use NonSerialized attribute.
 \par usage:
 @code
	class MyData
	{
		int m_id;
		autoptr map<string, float> m_values;
	
		[NonSerialized()]
		string m_dbg; // I don't want to serialize this variable
	}

	void Serialize(Serializer s)
	{
		int statArray[4] = {6,9,2,3};
		array<int> dynArray = {8,5,6,4};
		autoptr MyData data = new MyData();
	
		data.m_id = 965;
		data.m_values = map<string, float>;
		data.m_values.Insert("value1", 5.98);
		data.m_values.Insert("value2", 4.36);
	
		s.Write(10);
		s.Write("Hello");
		s.Write(statArray);
		s.Write(dynArray);
		s.Write(data);
	}

	void Deserialize(Serializer s)
	{
		int statArray[4];
		array<int> dynArray;
		MyData data;
		int someInt;
		string someString;	
	
		s.Read(someInt);
		s.Read(someString);
		s.Read(statArray);
		s.Read(dynArray);
		s.Read(data);
	}
 
 @endcode
 */
class Serializer: Managed
{
	proto bool Write(void value_out);
	proto bool Read(void value_in);
	
	protected void Serializer() {}
	protected void ~Serializer() {}
};

/**
 \brief Serializer API implementation for File IO. See Serializer for more info.
 \n usage:
 @code
void TestSave()
{
	FileSerializer file = new FileSerializer();
	string names[3] = {"alpha", "beta", "gama"};
	
	if (file.Open("test.save", FileMode.WRITE))
	{
		file.Write(10);
		file.Write("lalala");
		file.Write(name);
		file.Close();
	}
}
	
void TestLoad()
{
	FileSerializer file = new FileSerializer();
	int intVal;
	string stringVal;
	string names[3];
		
	if (file.Open("test.save", FileMode.READ))
	{
		file.Read(intVal);
		file.Read(stringVal);
		file.Read(names);
		file.Close();
	}
}	
 
 @endcode
 */
class FileSerializer: Serializer
{
	void FileSerializer() {}
	void ~FileSerializer() {}
	
	proto native bool Open(string path, FileMode mode = FileMode.READ);
	proto native bool IsOpen();
	proto native void Close();
}
	
