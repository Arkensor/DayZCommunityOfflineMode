/**
 * \defgroup Strings Strings
 * @{
 */
class string
{
	static const string Empty;
			
	/**
	\brief Converts string to integer
		\return \p int - Converted \p string.
		@code
			string str = "56";
			int i = str.ToInt();
			Print(i);

			>> i = 56
		@endcode
	*/
	proto native int ToInt();
	
	/**
	\brief Converts string to float
		\return \p float - Converted \p string \p in float.
		@code
			string str = "56.6";
			float f = str.ToFloat();
			Print(f);

			>> f = 56.6
		@endcode
	*/
	proto native float ToFloat();
			
	/**
	\brief Returns a vector from a string
		\return \p vector Converted s as vector
		@code
			string str = "1 0 1";
			vector v = str.ToVector();
			Print(v);

			>> v = <1,0,1>
		@endcode
	*/
	proto vector ToVector();
		
	/**
	\brief Converts string's first character to ASCII code
		\param str String for convert to ASCII code
		\return \p ascii code \p int.
		@code
			int ascii = "M".ToAcsii();
			Print(ascii);

			>> ascii = 77
		@endcode
	*/
	proto int ToAscii(string str);		
	
	/**
	\brief Returns internal type representation. Can be used in runtime, or cached in variables and used for faster inheritance checking
		\returns \p typename Type of class
		@code
			???
		@endcode
	*/
	proto native typename ToType();

	//! Return string representation of variable
	static proto string ToString(void var, bool type = false, bool name = false, bool quotes = true);
	
	/**
	\brief Substring of 'str' from 'start' position 'len' number of characters
		\param start Position in \p str
		\param len Count of characters
		\return \p string - Substring of \p str
		@code
			string str = "Hello World";
			string strSub = str.Substring(2, 5);
			Print(strSub);

			>> strSub = llo W
		@endcode
	*/
	proto string Substring(int start, int len);

	/**
	\brief Replace all occurrances of 'sample' in 'str' by 'replace'
		\param sample string to search in \p str
		\param replace string which replace \p sample in \p str
		\return \p int - number of occurrances of 'sample' in 'str' 
		@code
		string test = "If the length of the C string in source is less than num, only the content up to the terminating null-character is copied.";
		Print(test);
		int count = test.Replace("the", "*");
		Print(count);
		Print(test);

		>> string test = 'If the length of the C string in source is less than num, only the content up to the terminating null-character is copied.';
		>> int count =   4
		>> string test = 'If * length of * C string in source is less than num, only * content up to * terminating null-character is copied.'
		@endcode
	*/
	proto int Replace(string sample, string replace);

	/**
	\brief Changes string to lowercase. Returns length
		\return \p int - Length of changed string
		@code
			string str = "Hello World";
			int i = str.ToLower();
			Print(str);
			Print(i);

			>> str = hello world
			>> i = 11
		@endcode
	*/
	proto int ToLower();

	/**
	\brief Changes string to uppercase. Returns length
		\return \p int - Length of changed string
		@code
			string str = "Hello World";
			int i = str.ToUpper();
			Print(str);
			Print(i);

			>> str = HELLO WORLD
			>> i = 11
		@endcode
	*/
	proto int ToUpper();

	/**
	\brief Returns length of string
		\return \p int - Length of string
		@code
			string str = "Hello World";
			int i = str.Length();
			Print(i);

			>> i = 11
		@endcode
	*/
	proto native int Length();

	/**
	\brief Returns hash of string
		\return \p int - Hash of string
		@code
			string str = "Hello World";
			int hash = str.Hash();
			Print(hash);
		@endcode
	*/
	proto native int Hash();
	
	/**
	\brief Finds 'sample' in 'str'. Returns -1 when not found
		\param sample \p string Finding string
		\return \p int - Returns position where \p sample starts, or -1 when \p sample not found
		@code
			string str = "Hello World";
			Print( str.IndexOf( "H" ) );
			Print( str.IndexOf( "W" ) );
			Print( str.IndexOf( "Q" ) );

			>> 0
			>> 6
			>> -1
		@endcode
	*/
	proto native int IndexOf(string sample);
	
	/**
	\brief Finds last 'sample' in 'str'. Returns -1 when not found
		\param sample \p string Finding string
		\return \p int - Returns position where \p sample starts, or -1 when \p sample not found
		@code
			string str = "Hello World";
			Print( str.IndexOf( "l" ) );
			
			>> 9
		@endcode
	*/
	proto native int LastIndexOf(string sample);

	/**
	\brief Finds 'sample' in 'str' from 'start' position. Returns -1 when not found
		\param start \p int Start from position
		\param sample \p string Finding string expression
		\return \p int - Length of string \p s
		@code
			string str = "Hello World";
			Print( str.IndexOfFrom( 3, "H" ) );
			Print( str.IndexOfFrom( 3, "W" ) );
			Print( str.IndexOfFrom( 3, "Q" ) );

			>> -1
			>> 6
			>> -1
		@endcode
	*/
	proto native int IndexOfFrom(int start, string sample);

	/**
	\brief Retunrs true if sample is substring of string
		\param sample \p string Finding string expression
		\return \p bool true if sample is substring of string
		@code
			string str = "Hello World";
			Print( str.IndexOfFrom( 3, "Hello" ) );
			Print( str.IndexOfFrom( 3, "Mexico" ) );
			
			>> true
			>> false
		@endcode
	*/
	bool Contains(string sample)
	{
		return value.IndexOf(sample) != -1;
	}

	/**
	\brief Returns trimmed string with removed leading and trailing whitespaces
		\return \p string - Trimmed string
		@code
			string str = " Hello World "
			Print( str );
			Print( str.Trim() );

			>> ' Hello World '
			>> 'Hello World'
		@endcode

	*/
	proto string Trim();

	/**
	\brief Removes leading and trailing whitespaces in string. Returns length
		\return \p int - Count of chars
		@code
			string str = " Hello World ";
			int i = str.TrimInPlace();
			Print(str);
			Print(i);

			>> str = 'Hello World'
			>> i = 11
		@endcode
	*/
	proto int TrimInPlace();
	
	/**
	\brief Parses one token from input string. Result is put into token string, and type of token is returned. Input string is left-truncated by the resulting token length.
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

			int result1 = input.ParseStringEx(token1);
			int result2 = input.ParseStringEx(token2);

			Print( "Token1 = '" + token1 + "' Type = " + result1.ToString() ) );
			Print( "Token2 = '" + token2 + "' Type = " + result2.ToString() ) );
			
			>> 'Toke1 = 'Hello' Type = 3'
			>> 'Toke1 = '*' Type = 1'
		@endcode
	*/
	proto int ParseStringEx(out string token);

	/**
	\brief Parses string into array of tokens returns number of tokens
		\param[out] tokens \p array[] Parsed string in array
		\return \p int Number of tokens
		@code
			string token[2];
			string str = "Hello World";
			int result = str.ParseString(token);

			for( int i = 0; i < 2; i++ )
			{
				Print(token[i]);
			}

			>> 'Hello'
			>> 'World'
		@endcode
	*/
	proto int ParseString(out string tokens[]);
	
	/**
	\brief Splits string into array of strings separated by 'sample'
		\param sample \p string Strings separator
		\return \p TStringArray Array with strings
		@code
			TStringArray strs = new TStringArray;
			EnString.Split( "The quick brown fox jumps over the lazy dog", " ", strs );
			
			for ( int i = 0; i < strs.Count(); i++ )
			{
				Print( strs.Get(i) );
			}

			>> 'The'
			>> 'quick'
			>> 'brown'
			>> 'fox'
			>> 'jumps'
			>> 'over'
			>> 'the'
			>> 'lazy'
			>> 'dog'
		@endcode
	*/
	void Split(string sample, out array<string> output)
	{
		int txt_len	= 0;
		int sep_pos	= -1;
		int nxt_sep_pos = 0;
		string text = "";

		bool line_end = false;
		while (line_end == false)
		{
			sep_pos = sep_pos + txt_len + 1;
			nxt_sep_pos = value.IndexOfFrom(sep_pos, sample);
			if ( nxt_sep_pos == -1 )
			{
				nxt_sep_pos = value.Length();
				line_end = true;
			}

			txt_len = nxt_sep_pos - sep_pos;
			if ( txt_len > 0 )
			{
				text = value.Substring(sep_pos, txt_len);
				output.Insert(text);
			}
		}
	}

	/**
	\brief Gets n-th character from string
		\param index character index
		\return \p string character on index-th position in string
		@code
			string str = "Hello World";
			Print( str[4] ); // Print( str.Get(4) );

			>> 'o'
		@endcode
	*/ 			
	string Get(int index)
	{
		return value.Substring(index, 1);
	}
	
	/**
	\brief Gets n-th character from string
		\param index character index
		\return \p string character on index-th position in string
		@code
		int a = 5;
		float b = 5.99;
		string c = "beta";
		string 	test = string.Format("Ahoj %1 = %3 , %2", a, b, c);
		Print(test);	
		>> 'Ahoj 5 = 'beta' , 5.99'
		@endcode
	*/ 			
	static proto string Format(string fmt, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
};

//@}