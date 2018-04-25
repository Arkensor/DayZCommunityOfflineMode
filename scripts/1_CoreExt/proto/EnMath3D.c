/**
 * \defgroup Math3DAPI Math3D library
 * @{
 */

/**
\brief Vector constructor from components
    \param x \p float x component
	\param y \p float y component
	\param z \p float z component
	\return \p vector resulting vector
	@code
		Print( Vector(1, 2, 3) );

		>> <1,2,3>
	@endcode
*/
proto native vector Vector(float x, float y, float z);

enum ECurveType
{
	CatmullRom,
	NaturalCubic,
	UniformCubic
};

class Math3D
{
	//-----------------------------------------------------------------
	static vector ClipLine(vector start, vector end, vector norm, float d)
	{
		vector vec;
		float d1, d2;

		d1 = start * norm;
		d1 = d1 - d;

		d2 = end * norm;
		d2 = d2 - d;

		d = d1 - d2;

		if(d1 < 0)
			d1 = d1 + 0.1;
		else
			d1 = d1 - 0.1;

		d = d1 / d;
		
		//clamp d<0, 1>
		if (d < 0) d = 0;
		if (d > 1) d = 1;

		vec = end - start;
		vec = vec * d;

		return vec + start;
	}

	proto static float IntersectRaySphere(vector raybase, vector raycos, vector center, float radius);
	proto static float IntersectRayBox(vector start, vector end, vector mins, vector maxs);
	
	/**
	\brief Creates rotation matrix from angles
		\param ang \p vector which contains angles
		\param[out] mat \p vector created rotation matrix
		@code
			vector mat[3];
			YawPitchRollMatrix( "70 15 45", mat );
			Print( mat );

			>> <0.330366,0.0885213,-0.939693>,<0.458809,0.854988,0.241845>,<0.824835,-0.511037,0.241845>
		@endcode
	*/
	proto static void YawPitchRollMatrix(vector ang, out vector mat[3]);

	/**
	\brief Creates rotation matrix from direction and up vector
		\param dir \p vector direction vector
		\param up \p vector up vector
		\param[out] mat \p vector[4] created rotation matrix
		@code
			vector mat[4];
			vector dir = "1 0 1";
			vector up = "0 1 0";
			DirectionAndUpMatrix( dir, up, mat );
			Print( mat );

			>> <0.707107,0,-0.707107>,<0,1,0>,<0.707107,0,0.707107>,<0,0,0>
		@endcode
	*/
	proto static void DirectionAndUpMatrix(vector dir, vector up, out vector mat[4]);

	/**
	\brief Transforms matrix
		\param mat0 \p vector[4] first matrix
		\param mat1 \p vector[4] second matrix
		\param[out] res \p vector[4] result of first and second matrix multiplication
		@code
			vector mat0[4] = { "2 0 0 0", "0 3 0 0", "0 1 0 0", "0 0 0 1" }; // scale matrix
			vector mat1[4] = { "1 0 0 0", "0 1 0 0", "0 1 0 0", "2 4 1 3" }; // translation matrix
			vector res[4];
			Math3D.MatrixMultiply4(mat0, mat1, res)
			Print( res );

			>> <2,0,0>,<0,3,0>,<0,3,0>,<4,13,0>
		@endcode
	*/
	proto static void MatrixMultiply4(vector mat0[4], vector mat1[4], out vector res[4]);

	/**
	\brief Transforms rotation matrix
		\param mat0 \p vector[3] first matrix
		\param mat1 \p vector[3] second matrix
		\param[out] res \p vector[3] result of first and second matrix multiplication
		@code
			vector mat0[3] = { "1.5 2.5 0", "0.1 1.3 0", "0 0 1" }; // rotation matrix
			vector mat1[3] = { "1 0.4 0", "0 1 0", "0 1.3 2.7" }; // rotation matrix
			vector res[3];
			Math3D.MatrixMultiply3(mat0, mat1, res)
			Print( res );

			>> <1.54,3.02,0>,<0.1,1.3,0>,<0.13,1.69,2.7>
		@endcode
	*/
	proto static void MatrixMultiply3(vector mat0[3], vector mat1[3], out vector res[3]);

	/**
	\brief Invert-transforms matrix
		\param mat0 \p vector[4] first matrix
		\param mat1 \p vector[4] second matrix
		\param[out] res \p vector[4] inverse result of first and second matrix multiplication
		@code
			vector mat0[4] = { "2 0 0", "0 3 0", "0 0 1", "0 0 0" }; // scale matrix
			vector mat1[4] = { "1 0 0", "0 1 0", "0 0 1", "2 4 1" }; // translation matrix
			vector res[4];
			Math3D.MatrixInvMultiply4(mat0, mat1, res)
			Print( res );

			>> <2,0,0>,<0,3,1>,<0,3,1>,<4,12,4>
		@endcode
	*/
	proto static void MatrixInvMultiply4(vector mat0[4], vector mat1[4], out vector res[4]);

	/**
	\brief Invert-transforms rotation matrix
		\param mat0 \p vector[3] first matrix
		\param mat1 \p vector[3] second matrix
		\param[out] res \p vector[3] result of first and second matrix multiplication
		@code
			vector mat0[3] = { "1.5 2.5 0", "0.1 1.3 0", "0 0 1" }; // rotation matrix
			vector mat1[3] = { "1 0.4 0", "0 1 0", "0 1.3 2.7" }; // rotation matrix
			vector res[3];
			Math3D.MatrixInvMultiply3(mat0, mat1, res)
			Print( res );

			>> <2.5,0.62,0>,<2.5,1.3,0>,<3.25,1.69,2.7>
		@endcode
	*/
	proto static void MatrixInvMultiply3(vector mat0[3], vector mat1[3], out vector res[3]);

	/**
	\brief Creates identity matrix
	    \param[out] mat \p created identity matrix
		@code
	        vector mat[4];
	    	Math3D.MatrixIdentity4( mat );
	    	Print( mat );

			>> <1,0,0>,<0,1,0>,<0,0,1>,<0,0,0>
		@endcode
	*/
	static void MatrixIdentity4(out vector mat[4])
	{
		mat[0] = "1 0 0";
		mat[1] = "0 1 0";
		mat[2] = "0 0 1";
		mat[3] = vector.Zero;
	}
	
	/**
	\brief Creates identity matrix
	    \param[out] mat \p created identity matrix
		@code
	        vector mat[3];
	    	Math3D.MatrixIdentity3( mat );
	    	Print( mat );

			>> <1,0,0>,<0,1,0>,<0,0,1>
		@endcode
	*/
	static void MatrixIdentity3(out vector mat[3])
	{
		mat[0] = "1 0 0";
		mat[1] = "0 1 0";
		mat[2] = "0 0 1";
	}

	/**
	\brief Creates scale matrix
	    \param scale \p scale coeficient
	    \param[out] mat \p created scale matrix
		@code
	        vector mat[3];
	    	Math3D.ScaleMatrix( 2.5, mat );
	    	Print( mat );

			>> <2.5,0,0>,<0,2.5,0>,<0,0,2.5>
		@endcode
	*/
	static void ScaleMatrix(float scale, out vector mat[3])
	{
		vector v0, v1, v2;
		v0[0] = scale;
		v1[1] = scale;
		v2[2] = scale;
		mat[0] = v0;
		mat[1] = v1;
		mat[2] = v2;
	}

	/**
	\brief Creates identity quaternion
		\param[out] q \p float[4] created identity quaternion
		@code
			float q[4];
			Math3D.QuatIdentity( q );
			Print( q );

			>> {0,0,0,1}
		@endcode
	*/
	static void QuatIdentity(out float q[4])
	{
		q[0] = 0;
		q[1] = 0;
		q[2] = 0;
		q[3] = 1;
	}

	/**
	\brief Copies quaternion
	    \param s \p float[4] quaternion to copy
		\param[out] d \p float[4] created quaternion copy
		@code
			float s[4] = { 2, 3, 4, 1 };
			float d[4];
			Math3D.QuatCopy( s, d );
			Print( d );

			>> {2,3,4,1}
		@endcode
	*/
	static void QuatCopy(float s[4], out float d[4])
	{
		d[0] = s[0];
		d[1] = s[1];
		d[2] = s[2];
		d[3] = s[3];
	}

	/**
	\brief Converts rotation matrix to quaternion
	    \param mat \p vector[3] rotation matrix
		\param[out] d \p float[4] created quaternion copy
		@code
			vector mat[3];
			vector rot = "70 15 45";
			rot.RotationMatrixFromAngles( mat );
			float d[4];
			Math3D.MatrixToQuat( mat, d );
			Print( d );

			>> {0.241626,0.566299,-0.118838,0.778973}
		@endcode
	*/
	proto static void MatrixToQuat(vector mat[3], out float d[4]);

	
	//! Converts quaternion to rotation matrix
	proto static void QuatToMatrix(float q[4], out vector mat[3]);

	/**
	\brief Returns angles of rotation matrix
		\param mat \p vector[3] rotation matrix
		\return \p vector roll, pitch, yaw angles
		@code
			vector mat[3];
			Math3D.RollPitchYawMatrix( "70 15 45", mat );
			vector ang = Math3D.MatrixToAngles( mat );
			Print( ang );

			>> <70,15,-45>
		@endcode
	*/
	proto static vector MatrixToAngles(vector mat[3]);
	
	/**
	\brief Linear interpolation between q1 and q2 with weight 'frac' (0...1)
	    \param[out] qout \p float[4] result quaternion
		\param q1 \p float[4] first quaternion
		\param q2 \p float[4] second quaternion
		\param frac \p float interpolation weight
		@code
			float q1[4] = { 1, 1, 1, 1 };
			float q2[4] = { 2, 2, 2, 1 };
			float qout[4];
			Math3D.QuatLerp( qout, q1, q2, 0.5 );
			Print( qout );

			>> {1.5,1.5,1.5,1}
		@endcode
	*/
	proto static void QuatLerp(out float qout[4], float q1[4], float q2[4], float frac);

	/**
	\brief Multiplies quaternions
	    \param[out] qout \p float[4] result quaternion
		\param q1 \p float[4] first quaternion
		\param q2 \p float[4] second quaternion
		@code
			float q1[4] = { 1, 2, 3, 1 };
			float q2[4] = { 2, 2, 2, 1 };
			float qout[4];
			Math3D.QuatMultiply( qout, q1, q2 );
			Print( qout );

			>> {2,4,6,1}
		@endcode
	*/
	proto static void QuatMultiply(out float qout[4], float q1[4], float q2[4]);

	//! Returns Angles vector from quaternion
	proto static vector QuatToAngles(float q[4]);
	
	/**
	\brief Returns 1, when bounding boxes intersects
	    \param mins1 \p vector minimum point of first bounding box
		\param maxs1 \p vector maximum point of first bounding box
		\param mins2 \p vector minimum point of second bounding box
		\param maxs2 \p vector maximum point of second bounding box
		\return \p int 1 if boundig boxes intersects, otherwise 0
		@code
			vector mins1 = "1 1 1";
			vector maxs1 = "3 3 3";
			vector mins2 = "2 2 2";
			vector maxs2 = "4 4 4";
			Print( Math3D.CheckBoundBox(mins1, maxs1, mins2, maxs2) );

			>> 1
		@endcode
	*/
	static int CheckBoundBox(vector mins1, vector maxs1,vector mins2, vector maxs2)
	{
		if (mins1[0] > maxs2[0] || mins1[1] > maxs2[1] || mins1[2] > maxs2[2] || maxs1[0] < mins2[0] || maxs1[1] < mins2[1] || maxs1[2] < mins2[2])
			return false;

		return true;
	}
	
	/**
	\brief Returns randon normalized direction
	    \return \p vector 
		@code
			Print( Math3D.GetRandomDir() );

			>>vector ret =  0x0000000007c1a1c0 {<0.422565,0,-0.906333>}
		@endcode
	*/
	static vector GetRandomDir()
	{
		float x = Math.RandomFloatInclusive(-1, 1);
		float y = Math.RandomFloatInclusive(-1, 1);
		float z = Math.RandomFloatInclusive(-1, 1);
			
		return Vector(x, y, z).Normalized();
	}
	
	
	/**
	\brief Computes curve
	 \return \p vector 
		@code
			auto points = new array<vector>();
			points.Insert( Vector( 0, 0, 0) );
	    	points.Insert( Vector( 5, 0, 0) );
	    	points.Insert( Vector( 8, 3, 0) );
	    	points.Insert( Vector( 6, 1, 0) );
		
			float t = 0.5;
			vector result = Math3D.Curve(ECurveType.CatmullRom, t, points);
		@endcode
	*/
	proto static native vector Curve(ECurveType type, float param, notnull array<vector> points);
};
//@}