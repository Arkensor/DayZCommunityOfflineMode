class ItemVariableFlags
{
	static const int NONE 					= 0;
	static const int FLOAT			 		= 0x0001;
	static const int STRING		 			= 0x0002;
	static const int BOOL 					= 0x0004;
};

class CashedObjectsParams
{	
	static ref Param1<int> 		PARAM1_INT;//CashedObjectsParams.PARAM1_INT
	static ref Param1<bool> 	PARAM1_BOOL;//CashedObjectsParams.PARAM1_BOOL
	static ref Param1<float> 	PARAM1_FLOAT;//CashedObjectsParams.PARAM1_FLOAT
	static ref Param1<string> 	PARAM1_STRING;//CashedObjectsParams.PARAM1_STRING
	
	static ref Param2<int,int> 		PARAM2_INT_INT;//CashedObjectsParams.PARAM2_INT_INT
	static ref Param2<int,float> 	PARAM2_INT_FLOAT;//CashedObjectsParams.PARAM2_INT_FLOAT
	static ref Param2<int,string> 	PARAM2_INT_STRING;//CashedObjectsParams.PARAM2_INT_STRING
	static ref Param2<string,float> PARAM2_STRING_FLOAT;//CashedObjectsParams.PARAM2_STRING_FLOAT
	static ref Param2<string,string> PARAM2_STRING_STRING;//CashedObjectsParams.PARAM2_STRING_STRING
	
	static void Init()
	{
		PARAM1_INT = new Param1<int>(0);
		PARAM1_BOOL = new Param1<bool>(false);
		PARAM1_FLOAT = new Param1<float>(0);
		PARAM1_STRING = new Param1<string>("");
		
		PARAM2_INT_INT = new Param2<int,int>(0,0);
		PARAM2_INT_FLOAT = new Param2<int,float>(0,0);
		PARAM2_INT_STRING = new Param2<int,string>(0,"");
		PARAM2_STRING_FLOAT = new Param2<string,float>("",0);
		PARAM2_STRING_STRING = new Param2<string,string>("","");
	
	}
};


class CashedObjectsArrays//don't forget to .Clear() your cache object before using it
{	
	static void Init()
	{
		ARRAY_STRING 	= new TStringArray;
		ARRAY_FLOAT 	= new TFloatArray;
		ARRAY_INT 		= new TIntArray;
	}
	
	static ref TStringArray ARRAY_STRING;//CashedObjectsArrays.ARRAY_STRING
	static ref TFloatArray ARRAY_FLOAT;//CashedObjectsArrays.ARRAY_FLOAT
	static ref TIntArray ARRAY_INT;//CashedObjectsArrays.ARRAY_INT
};