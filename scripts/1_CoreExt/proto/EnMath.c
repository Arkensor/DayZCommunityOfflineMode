/**
 * \defgroup Math Math library
 * @{
 */

class Math
{
	static const float PI = 3.14159265358979;
	static const float PI2 = 6.28318530717958;
	static const float PI_HALF = 1.570796326794;

	static const float RAD2DEG = 57.2957795130823208768;
	static const float DEG2RAD = 0.01745329251994329577;
	
	/**
	\brief Return power of v ^ power
		\param v \p float Value
		\param power \p float Power
		\return \p float - Angle in radians
		@code
			Print( Math.Pow(2, 4) ); // (2*2*2*2)=16

			>> 16
		@endcode
	*/
	proto native static float Pow(float v, float power);

	/**
	\brief Returns a random \p int number between and min [inclusive] and max [exclusive].
		\param min \p int Range starts [inclusive]
		\param max \p int Range ends [exclusive]
		\return \p int - Random number in range
		@code
			Print( Math.RandomInt(0, 1) );	// only 0
			Print( Math.RandomInt(0, 2) );	// 0 or 1

			>> 0
			>> 1
		@endcode
	*/
	proto native static int RandomInt(int min, int max);
	
	/**
	\brief Returns a random \p int number between and min [inclusive] and max [inclusive].
		\param min \p int Range starts [inclusive]
		\param max \p int Range ends [inclusive]
		\return \p int - Random number in range
		@code
			Print( Math.RandomIntInclusive(0, 1) );	// 0 or 1
			Print( Math.RandomIntInclusive(0, 2) );	// 0, 1, 2

			>> 1
			>> 2
		@endcode
	*/
	static int RandomIntInclusive(int min, int max)
	{
		return Math.RandomInt(min, max+1);
	}

	/**
	\brief Returns a random \p float number between and min [inclusive] and max [inclusive].
		\param min \p float Range starts [inclusive]
		\param max \p float Range ends [inclusive]
		\return \p float - Random number in range
		@code
			Print( Math.RandomFloatInclusive(0, 1) );	// 0.0 .. 1.0
			Print( Math.RandomFloatInclusive(1, 2) );	// 1.0 .. 2.0

			>> 0.3
			>> 2.0
		@endcode
	*/
	static float RandomFloatInclusive(float min, float max)
	{
		int max_range = Math.Pow(2, 30); //max range
		int random_int = Math.RandomInt(0, max_range);
		float rand_float = (float)random_int / (float)max_range;
		float range = max - min;

		return min + (rand_float * range); //rand float
	}

	/**
	\brief Returns a random \p float number between and min [inclusive] and max [inclusive].
		\return \p float - Random number in range 0.0 .. 1.0
		@code
			Print( Math.RandomFloat01() );	// 0.0 .. 1.0

			>> 0.3
			>> 1.0
		@endcode
	*/
	static float RandomFloat01()
	{
		return RandomFloatInclusive(0, 1);
	}

	/**
	\brief Returns a random \p float number between and min[inclusive] and max[exclusive].
		\param min \p float Range starts [inclusive]
		\param max \p float Range ends [exclusive]
		\return \p float - Random number in range
		@code
			Print( Math.RandomFloat(0,1) );
			Print( Math.RandomFloat(0,2) );

			>> 0.597561
			>> 1.936456
		@endcode
	*/
	proto native static float RandomFloat(float min, float max);
	
	/**
	\brief Sets the seed for the random number generator.
		\param seed \p int New seed for the random number generator
		\return \p int - Returns new seed
		@code
			Print( Math.Randomize(5) );

			>> 5
		@endcode
	*/
	proto native static int Randomize(int seed);
	
	/**
	\brief Normalizes the angle (0...360)
		\param ang \p float Angle for normalizing
		\return \p float - Normalized angle
		@code
			Print( Math.NormalizeAngle(390) );
			Print( Math.NormalizeAngle(-90) );

			>> 30
			>> 270
		@endcode
	*/
	proto native static float NormalizeAngle(float ang);

	/**
	\brief Return relative difference between angles
		\param angle1 \p float 
		\param angle2 \p float
		\return \p float Difference between angles (angle1 - angle2)
		@code
			Print( Math.DiffAngle(-45, 45) );
			Print( Math.DiffAngle(90, 80) );

			>> -90
			>> 10
		@endcode
	*/
	static float DiffAngle(float angle1, float angle2)
	{
		angle1 = Math.NormalizeAngle(angle1);
		angle2 = Math.NormalizeAngle(angle2);

		float diff = angle1 - angle2;

		if (Math.AbsFloat(diff) > 180)
		{
			if (angle1 < angle2)
				diff = angle1 + 360 - angle2;
			else
				diff = angle1 - 360 - angle2;
		}

		return diff;
	}
	
	/**
	\brief Returns absolute value
		\param f \p float Value
		\return \p float - Absolute value
		@code
			Print( Math.AbsFloat(-12.5) );

			>> 12.5
		@endcode
	*/
	proto native static float AbsFloat(float f);

	/**
	\brief Returns absolute value
		\param i \p int Value
		\return \p int - Absolute value
		@code
			Print( Math.AbsInt(-12) );

			>> 12
		@endcode
	*/
	proto native static int AbsInt(int i);
	
	/**
	\brief Returns square root
		\param val \p float Value
		\return \p float - Square of value
		@code
			Print( Math.Sqrt(25));

			>> 5
		@endcode
	*/
	proto native static float Sqrt(float val);

	/**
	\brief Returns sinus of angle in radians
		\param angle \p float Angle in radians
		\return \p float - Sinus of angle
		@code
			Print( Math.Sin(0.785398) ); // (45)

			>> 0.707107
		@endcode
	*/
	proto native static float Sin(float angle);

	/**
	\brief Returns cosinus of angle in radians
		\param angle \p float Angle in radians
		\return \p float - Cosinus of angle
		@code
			Print( Math.Cos(0.785398) ); // (45)

			>> 0.707107
		@endcode
	*/
	proto native static float Cos(float angle);

	/**
	\brief Returns tangent of angle in radians
		\param deg \p float Angle in radians
		\return \p float - Tangens of angle
		@code
			Print( Math.Tan(0.785398) ); // (45)

			>> 1
		@endcode
	*/
	proto native static float Tan(float deg);

	/**
	\brief Returns angle in radians from sinus
		\param s \p float Sinus
		\return \p float - Angle in radians
		@code
			Print( Math.Asin(0.707107) ); // (sinus 45)

			>> 0.785398
		@endcode
	*/
	proto native static float Asin(float s);

	/**
	\brief Returns angle in radians from cosinus
		\param c \p float Cosinus
		\return \p float - Angle in radians
		@code
			Print( Math.Acos(0.707107) ); // (cosinus 45)

			>> 0.785398
		@endcode
	*/
	proto native static float Acos(float c);

	/**
	\brief Returns angle in radians from tangent
		\param y \p float Tangent
		\param x \p float Tangent
		\return \p float - Angle in radians
		@code
			Print ( Math.Atan2(1, 1) );

			>> 0.785398
		@endcode
	*/
	proto native static float Atan2(float y, float x);

	/**
	\brief Returns mathematical round of value
		\param f \p float Value
		\return \p int - closest whole number to 'f'
		@code
			Print( Math.Round(5.3) );
			Print( Math.Round(5.8) );

			>> 5
			>> 6
		@endcode
	*/
	proto native static float Round(float f);

	/**
	\brief Clamps 'value' to 'min' if it is lower than 'min', or to 'max' if it is higher than 'max'
		\param value \p float Value
		\param min \p float Minimum value
		\param max \p float Maximum value
		\return \p float - Clamped value
		@code
			Print( Math.Clamp(-0.1, 0, 1) );
			Print( Math.Clamp(2, 0, 1) );
			Print( Math.Clamp(0.5, 0, 1) );
	
			>> 0
			>> 1
			>> 0.5
		@endcode
	*/
	static float Clamp(float value, float min, float max)
	{
		if ( value < min )
		{
			return min;
		}

		if ( value > max )
		{
			return max;
		}

		return value;
	}
	
	/**
	\brief Returns floor of value
		\param f \p float Value
		\return \p int - Floor of value
		@code
			Print( Math.Floor(5.3) );
			Print( Math.Floor(5.8) );

			>> 5
			>> 5
		@endcode
	*/
	proto native static float Floor(float f);

	/**
	\brief Returns ceil of value
		\param f \p float Value
		\return \p int - Ceil of value
		@code
			Print( Math.Ceil(5.3) );
			Print( Math.Ceil(5.8) );

			>> 6
			>> 6
		@endcode
	*/
	proto native static float Ceil(float f);
		
	/**
	\brief Returns smaller of two given values
		\param x \p float Value
		\param y \p float Value
		\return \p float  - min value
		@code
			Print( Math.Min(5.3, 2.8) );

			>> 2.8
		@endcode
	*/
	static float Min(float x, float y)
	{
		if ( x <= y )
		{
			return x;
		}
		else
		{
			return y;
		}
	}
	
	/**
	\brief Returns bigger of two given values
		\param x \p float Value
		\param y \p float Value
		\return \p float  - max value
		@code
			Print( Math.Max(5.3, 2.8) );

			>> 5.3
		@endcode
	*/
	static float Max(float x, float y)
	{
		if ( x >= y )
		{
			return x;
		}
		else
		{
			return y;
		}
	}
	
	/**
	\brief Returns the binary (base-2) logarithm of x.
		\param x \p float Value whose logarithm is calculated.
		\return \p float The binary logarithm of x: log2x.
				\n If x is negative, it causes a domain error:
				\n If x is zero, it may cause a pole error (depending on the library implementation).
		@code
			Print( Math.Log2(1.0) ); 
	
			>> 0.0
		@endcode
	*/
	proto native static float Log2(float x);

	
	/**
	\brief Returns PI value (3.14159265359)
		\return \p float  - 3.14159265359
		@code
			Print( Math.PI );

			>> 5.3
		@endcode
	*/
	
	 /**
 \brief Linearly interpolates between 'a' and 'b' given 'time'.
  \param a \p float Start
  \param b \p float End
  \param time \p float Time [value needs to be between 0..1 for correct results, no auto clamp applied]
  \return \p float - The interpolated result between the two float values. 
  @code
   Print( Math.Lerp(3, 7, 0.5) );
   >> 5
  @endcode
 */
 
	static float Lerp(float a, float b, float time)
	{
	 return (b - a) * time + a;
	}


 /**
 \brief Calculates the linear value that produces the interpolant value within the range [a, b], it's an inverse of Lerp.
  \param a \p float Start
  \param b \p float End
  \param value \p float value
  \return \p float - the time given the position between 'a' and 'b' given 'value', there is no clamp on 'value', to stay between [0..1] use 'value' between 'a' and 'b'
  @code
   Print( Math.InverseLerp(3, 7, 5) );
   >> 0.5
  @endcode
 */
 
	static float InverseLerp(float a, float b, float value)
	{
		return (value - a) / (b - a);
	}
	//--------------------------------------------------------------------------
	//-------------------------------- filters ---------------------------------
	//--------------------------------------------------------------------------

	/**
	\brief Does the CD smoothing function - easy in | easy out / S shaped smoothing 
		\param val \p actual value 
		\param target \p value we are reaching for -> Target
		\param velocity \p float[1] - array of ONE member - some kind of memory and actual accel/decel rate, need to be zeroed when filter is about to be reset
		\param smoothTime \p smoothing parameter, 0.1 .. 0.4 are resonable values, 0.1 is sharp, 0.4 is very smooth
		\param maxVelocity \p maximal value change when multiplied by dt
		\param dt \p delta time 

		\return \p float smoothed/filtered value 

		@code
			val = EnfMath.SmoothCD(val, varTarget, valVelocity, 0.3, 1000, dt);
		@endcode
	*/

	proto native static float SmoothCD(float val, float target, inout float velocity[], float smoothTime, float maxVelocity, float dt);



};

//@}