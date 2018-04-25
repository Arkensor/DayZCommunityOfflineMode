/**
 * \defgroup Enforce Enforce script essentials
 * @{
 */

 //! Super root of all classes in Enforce script
class Class
{
	/**
	\brief Returns true when instance is of the type, or inherited one.
	\param type Class type
	\returns \p bool true when 'clType' is the same as 'type', or inherited one.
	@code
		if (inst && inst.IsInherited(Widget))
		{
			Print("inst is inherited from Widget class!");		
		}
	@endcode
	*/
	proto native external bool IsInherited(typename type);
	
	/**
	\brief Returns name of class-type
		\param inst Class
		\returns \p string class-type
		@code
			Man player = g_Game.GetPlayer();
			string className = player.ClassName();
			Print(className);

			>> className = 'Man'
		@endcode
	*/
	proto native owned external string ClassName();
	
	/**
	\brief Returns typename of object's class
		\returns \p typename class-type
		@code
			Man player = g_Game.GetPlayer();
			typename type = player.Type();
			Print(type.ToString());

			>> 'Man'
		@endcode
	*/
	proto native external typename Type();
	
	proto external string ToString();
	
	/**
	\brief Try to safely down-cast base class to child class.
		\returns down-casted 'from' pointer when cast is successfull (classes are related), or null if casting is invalid
		@code
			// assume that Man inheites from Object
			Object obj = g_Game.GetPlayer();
			Man player = Man.Cast(obj);
			
			if (player)
			{
				// horay!
			}
		@endcode
	*/		
	proto static Class Cast(Class from);
	
	/**
	\brief Try to safely down-cast base class to child class.
		\returns \p bool true when 'from' is not null and cast successfull, false when casting is not valid or 'from' is null
		@code
			// assume that Man inheites from Object
			Object obj = g_Game.GetPlayer();
			Man player;
			
			if (Class.CastTo(player, obj))
			{
				// horay!
			}
		@endcode
	*/	
	proto static bool CastTo(out Class to, Class from);
	
	//! This function is for internal script usage
	private proto static bool SafeCastType(Class type, out Class to, Class from);
};

 //! TODO doc
class Managed
{
};

//! TODO doc
class NonSerialized
{
};

//! script representation for C++ RTTI types
typedef int[] TypeID;

//! Module containing compiled scripts.
class ScriptModule
{
	private void ~ScriptModule();

	/*!dynamic call of function
	when inst == NULL, it's global function call, otherwise it's method of class
	returns true, when success
	The call creates new thread, so it's legal to use sleep/wait
  */
	proto volatile int Call(Class inst, string function, void parm);

	/*!dynamic call of function
	when inst == NULL, it's global function call, otherwise it's method of class
	returns true, when success
	The call do not create new thread!!!!
  */
	proto volatile int CallFunction(Class inst, string function, out void returnVal, void parm);
	proto volatile int CallFunctionParams(Class inst, string function, out void returnVal, Class parms);
	proto native void Release();
	
	/**
	\brief Do load script and create ScriptModule for it
		\param parentModule Module
		\param scriptFile Script path
		\param listing ??
		\returns \p ScriptModule Loaded scripted module
		@code
			???
		@endcode
	*/
	static proto native ScriptModule LoadScript(ScriptModule parentModule, string scriptFile, bool listing);
};

//main script module (contains script.c and this file)
//ScriptModule g_Script;

class EnScript
{
	/**
	\brief Dynamic read of variable value by its name
		\param inst When inst == NULL, it's for global variable, otherwise it's class member
		\param index Is index when variable is array
		\param[out] result Variable must be of the same type!
		\returns \p int true when success
		@code
			float count = 0;

			bool success = EnScript.GetClassVar(myClass, "m_Counter", 0, count);
			Print(count);
			Print(success);

			>> count = 5
			>> success = 1
		@endcode
	*/
	static proto int GetClassVar(Class inst, string varname,int index, out void result);

	/**
	\brief Dynamic write to variable by its name
		\param inst when inst == NULL, it's for global variable, otherwise it's class member
		\param varname
		\param index Is index when variable is array
		\param input Input variable must be of the same type!
		\returns \p int Returns true(1) when success
		@code
			Print(myClass.m_Counter);

			>> m_Counter = 0

			bool success = EnScript.SetClassVar(myClass, "m_Counter", 0, 5.0);

			Print(myClass.m_Counter);
			Print(success);

			>> m_Counter = 5
			>> success = 1
		@endcode
	*/
	static proto int SetClassVar(Class inst, string varname, int index, void input);
	
	/**
	\brief Sets variable value by value in string
		\param[out] var
		\param value
		\returns int
		@code
			???
		@endcode
	*/
	static proto int SetVar(out void var, string value);

	/**
	\brief Debug tool for watching certain variable. Invokes debugger whenever is variable used
		\param var Certain variable for watching
		\param flags = 1 means it will break even when not modified
		\return \p void
	*/
	static proto void Watch(void var, int flags);
};



/**
\brief Sorts static array of integers(ascendically) / floats(ascendically) / strings(alphabetically)
	\param param_array \p array Array to sort
	\param num \p int How many items will be sorted in array
	\return \p void
	@code
		string	arrStr[3] = {"Dog", "Car", "Apple"};
		Sort(arrStr, 2)
		for ( int x = 0; x < 3; x++ )
		{
			Print( arrStr[x] );
		}

		>> 'Car'
		>> 'Dog'
		>> 'Apple'
	@endcode
*/
proto void Sort(void param_array[], int num);
proto void reversearray(void param_array);
proto void copyarray(void destArray, void srcArray);

/**
\brief Parses one token from input string. Result is put into token string, and type of token is returned. Input string is left-truncated by the resulting token length.
	\param[in,out] input \p string String for parse\ Output is without founded token
	\param[out] token \p string Founded string token
	\return \p int Type of token
	\verbatim
Token types:
 0 - error, no token
 1 - defined token (special characters etc. . / * )
 2 - quoted string. Quotes are removed -> TODO
 3 - alphabetic string
 4 - number
 5 - end of line -> TODO
	\endverbatim
	@code
		string input = "Hello*World";
		string token1;
		string token2;

		int result1 = ParseStringEx(input, token1);
		int result2 = ParseStringEx(input, token2);

		Print( String( "Token1 = '" + token1 + "' Type = " + result1.ToString() ) );
		Print( String( "Token2 = '" + token2 + "' Type = " + result2.ToString() ) );
		Print( input );

		>> 'Toke1 = 'Hello' Type = 3'
		>> 'Toke1 = '*' Type = 1'
	@endcode
*/
proto int ParseStringEx(inout string input, string token);

/**
\brief Parses string into array of tokens returns number of tokens
	\param input \p string String for parse
	\param[out] tokens \p array[] Parsed string in array
	\return \p int Number of tokens
	@code
		string token[2];
		int result = ParseString("Hello World", token);

		for( int i = 0; i < 2; i++ )
		{
			Print(token[i]);
		}

		>> 'Hello'
		>> 'World'
	@endcode
*/
proto int ParseString(string input, out string tokens[]);

/**
\brief Kills thread.
	\param owner Can be NULL for global threads.
	\param name Name of the first function on stack
	\return \p int ???
	@code
		???
	@endcode
*/
proto native int KillThread(Class owner, string name);

/**
Yiels execution to other threads and then it continues. Obsolete...
*/
proto volatile void Idle();

/**
\brief Debug function. Returns current function on stack of the thread
	\param owner Can be NULL for global threads
	\param name Name of the first function on stack
	\param backtrace ???
	\param linenumber ???
	\return \p string ???
	@code
		???
	@endcode
*/
proto owned string ThreadFunction(Class owner, string name, int backtrace, out int linenumber);

//!Helper for passing string expression to functions with void parameter. Example: Print(String("Hello " + var));
string String(string s)
{
	return s;
}

//!Helper for printing out string expression. Example: PrintString("Hello " + var);
void PrintString(string s)
{
	Print(s);
}

class array<Class T>
{
	proto native int Count();
	proto native void Clear();
	proto void Set(int n, T value);
	proto int Find(T value);
	proto T Get(int n);
	/*!
	Inserts element at the end of array.
	\param value
	Element to be inserted
	\return
	Position at which element is inserted
	*/
	proto int Insert(T value);
	/*!
	Inserts element at certain position and moves all elements behind
	this position by one.
	\param value
	Element to be inserted
	\param index
	Position at which element is inserted. Must be less than Array::GetCardinality()
	\return
	Number of elements after insertion
	*/
	proto int InsertAt(T value, int index);
	/**
	\brief Inserts all elements from array
		\param from \p array<T> array from which all elements will be added
		@code
			TStringArray arr1 = new TStringArray;
			arr1.Insert( "Dave" );
			arr1.Insert( "Mark" );
			arr1.Insert( "John" );
			
			TStringArray arr2 = new TStringArray;
			arr2.Insert( "Sarah" );
			arr2.Insert( "Cate" );
			
			arr1.InsertAll(arr2);
			
			for ( int i = 0; i < arr1.Count(); i++ )
			{
				Print( arr1.Get(i) );
			}
			
			delete arr2;
			delete arr1;

			>> "Dave"
			>> "Mark"
			>> "John"
			>> "Sarah"
			>> "Cate"
		@endcode
	*/
	void InsertAll(notnull array<T> from)
	{
		for ( int i = 0; i < from.Count(); i++ )
		{
			Insert( from.Get(i) );
		}
	}
	/*!
	Removes element from array. The empty position is replaced by
	last element, so removal is quite fast but do not retain order.
	\param index
	Index of element to be removed
	*/
	proto native void Remove(int index);
	/*!
	Removes element from array, but retain all elements ordered. It's
	slower than Remove
	\param index
	Index of element to be removed
	*/
	proto native void RemoveOrdered(int index);
	proto native void Swap(notnull array<T> other);
	proto int Copy(notnull array<T> from);
	proto int Init(T init[]);

	void RemoveItem(T value)
	{
		int remove_index = Find(value);

		if ( remove_index >= 0 )
		{
			Remove(remove_index);
		}
	}

	/**
	\brief Print all elements in array
		\return \p void
		@code
			my_array.Debug();

			>> "One"
			>> "Two"
			>> "Three"
		@endcode
	*/
	void Debug()
	{
		PrintString( "Array count: " + this.Count().ToString() );
		for ( int i = 0; i < Count(); i++ )
		{
			T item = Get(i);

			if ( item )
			{
				PrintString( "["+i.ToString()+"] => " + string.ToString(item) );
				//Print( item );
			}
		}
	}

	/**
	\brief Returns a random index of array. If Count is 0, return index is -1 .
		\return \p int Random index of array
		@code
			Print( my_array.GetRandomIndex() );

			>> 2
		@endcode
	*/
	int GetRandomIndex()
	{
		if ( Count() > 0 )
		{
			return Math.RandomInt(0, Count());
		}
		
		return -1;	
	}

	/**
	\brief Returns a random element of array
		\return \p int Random element of array
		@code
			Print( my_array.GetRandomElement() );

			>> "Three"
		@endcode
	*/
	T GetRandomElement()
	{
		return Get(GetRandomIndex());
	}

	void SwapItems(int item1_index, int item2_index)
	{
		T item1 = Get(item1_index);
		Set(item1_index, Get(item2_index));
		Set(item2_index, item1);
	}
};

//force these to compile so we can link C++ methods to them
typedef array<string> TStringArray;
typedef array<float> TFloatArray;
typedef array<int> TIntArray;
typedef array<bool> TBoolArray;
typedef array<Class> TClassArray;
typedef array<Managed> TManagedArray;
typedef array<ref Managed> TManagedRefArray;
typedef array<vector> TVectorArray;
typedef array<typename> TTypenameArray;

class set<Class T>
{
	proto native int Count();
	proto native void Clear();
	proto int Find(T value);
	proto T Get(int n);
	/*!
	Inserts element at the end of array.
	\param value
	Element to be inserted
	\return
	Position at which element is inserted
	*/
	proto int Insert(T value);
	/*!
	Inserts element at certain position and moves all elements behind
	this position by one.
	\param value
	Element to be inserted
	\param index
	Position at which element is inserted. Must be less than Array::GetCardinality()
	\return
	Number of elements after insertion
	*/
	proto int InsertAt(T value, int index);
	/*!
	Removes element from array, but retain all elements ordered.
	\param index
	Index of element to be removed
	*/
	proto native void Remove(int index);
	proto int Copy(set<T> from);
	proto native void Swap(set<T> other);
	proto int Init(T init[]);
};

//force these to compile so we can link C++ methods to them
typedef set<string> TStringSet;
typedef set<float> TFloatSet;
typedef set<int> TIntSet;
typedef set<Class> TClassSet;
typedef set<Managed> TManagedSet;
typedef set<ref Managed> TManagedRefSet;
typedef set<typename> TTypenameSet;

typedef int MapIterator;
/**
 \brief Associative array template
 \n usage:
 @code
 autoptr map<string, int> prg_count = new map<string, int>;

 // fill
 project_locations.Insert("dayz", 10);
 project_locations.Insert("arma", 20);
 project_locations.Insert("tkom", 1);

 Print(project_locations.Get("arma")); // prints '20'

 @endcode

 */
class map<Class TKey,Class TValue>
{
	/*!
		\return
		The number of elements in the hashmap.
	*/
	proto native int Count();

	/*!
		Clears the hash map.
	*/
	proto native void Clear();
	/*!
		Search for an element with the given key.
			
		\param key
		The key of the element to find
		\return
		Pointer to element data if found, NULL otherwise.
	*/
	proto TValue Get(TKey key);
	/*!
		Search for an element with the given key.
			
		\param key
		The key of the element to find
		\param val
		result is stored to val
		\return
		returns True if given key exist.
	*/
	proto bool Find(TKey key, out TValue val);
	/*!
	Return the i:th element in the map.
	Note: This operation is O(n) complexity. Use with care!

	\param index
	The position of the element in the map
	\return
	The element on the i:th position
	*/
	proto TValue GetElement(int index);
	/*!
	Return the i:th element key in the map.
	Note: This operation is O(n) complexity. Use with care!

	\param i
	The position of the element key in the map
	\return
	Return key of i-th element
	*/
	proto TKey GetKey(int i);
	/*!
	Sets value of element with given key. If element with key not exists, it is created.
	Note: creating new elements is faster using Insert function.
	*/
	proto void Set(TKey key, TValue value);
	/*!
	Removes element with given key.
	*/
	proto void Remove(TKey key);
	/*!
	Removes i:th element with given key.
	Note: This operation is O(n) complexity. Use with care!
	\param i
	The position of the element key in the map
	*/
	proto void RemoveElement(int i);
	/*!
	Returns if map contains element with given key.
	*/
	proto bool Contains(TKey key);
	/*!
	Insert new element into hash map.
		
	\param key
	Key of element to be inserted.
	\param value
	Data of element to be inserted.
	*/
	proto bool Insert(TKey key, TValue value);
	proto int Copy(map<TKey,TValue> from);
	
	bool ReplaceKey(TKey old_key, TKey new_key)
	{
		if (Contains(old_key))
		{
			Set(new_key, Get(old_key));
			Remove(old_key);
			return true;
		}	
		return false;
	}
	
	TKey GetKeyByValue(TValue value)
	{
		TKey ret;
		for (int i = 0; i < Count(); i++)
		{
			if (GetElement(i) == value) 
			{
				ret = GetKey(i);
				break;
			}
		}

		return ret;
	}

	proto native MapIterator Begin();
	proto native MapIterator End();
	proto native MapIterator Next(MapIterator it);
	proto TKey GetIteratorKey(MapIterator it);
	proto TValue GetIteratorElement(MapIterator it);
};

typedef map<int, float>				TIntFloatMap;
typedef map<int, int>					TIntIntMap;
typedef map<int, string>			TIntStringMap;
typedef map<int, Class>				TIntClassMap;
typedef map<int, Managed>			TIntManagedMap;
typedef map<int, ref Managed>	TIntManagedRefMap;
typedef map<int, typename>		TIntTypenameMap;
typedef map<int, vector>			TIntVectorMap;

typedef map<string, float>		TStringFloatMap;
typedef map<string, int>			TStringIntMap;
typedef map<string, string>		TStringStringMap;
typedef map<string, Class>		TStringClassMap;
typedef map<string, Managed>	TStringManagedMap;
typedef map<string, ref Managed>	TStringManagedRefMap;
typedef map<string, typename>	TStringTypenameMap;
typedef map<string, vector>		TStringVectorMap;

typedef map<Class, float>		TClassFloatMap;
typedef map<Class, int>			TClassIntMap;
typedef map<Class, string>		TClassStringMap;
typedef map<Class, Class>		TClassClassMap;
typedef map<Class, Managed>		TClassManagedMap;
typedef map<Class, ref Managed>		TClassManagedRefMap;	
typedef map<Class, typename>	TClassTypenameMap;
typedef map<Class, vector>		TClassVectorMap;

typedef map<typename, float>		TTypeNameFloatMap;
typedef map<typename, int>			TTypeNameIntMap;
typedef map<typename, string>		TTypeNameStringMap;
typedef map<typename, Class>		TTypeNameClassMap;
typedef map<typename, Managed>		TTypeNameManagedMap;
typedef map<typename, ref Managed>	TTypeNameManagedRefMap;
typedef map<typename, typename>		TTypeNameTypenameMap;
typedef map<typename, vector>		TTypeNameVectorMap;

typedef map<Managed, float>		TManagedFloatMap;
typedef map<Managed, int>		TManagedIntMap;
typedef map<Managed, string>	TManagedStringMap;
typedef map<Managed, Class>		TManagedClassMap;
typedef map<Managed, Managed>	TManagedManagedMap;
typedef map<Managed, ref Managed>	TManagedManagedRefMap;
typedef map<Managed, typename>	TManagedTypenameMap;
typedef map<Managed, vector>	TManagedVectorMap;

typedef map<ref Managed, float>	TManagedRefFloatMap;
typedef map<ref Managed, int>		TManagedRefIntMap;
typedef map<ref Managed, string>	TManagedRefStringMap;
typedef map<ref Managed, Class>		TManagedRefClassMap;
typedef map<ref Managed, Managed>	TManagedRefManagedMap;
typedef map<ref Managed, ref Managed>	TManagedRefManagedRefMap;
typedef map<ref Managed, typename>	TManagedRefTypenameMap;
typedef map<ref Managed, vector>	TManagedRefVectorMap;

 //@}
