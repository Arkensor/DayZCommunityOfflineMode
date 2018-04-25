/**																																											 /**
 * \defgroup World World
 * @{
 */

//----------------------------------------------
/**
 * \defgroup WorldCommon World
 * @{
 */

typedef int[] WorldHandle;

proto native float GetWorldTime();

/*! Sets current world. It allows to work with entities outside world processing
// return previous world
*/
proto native WorldHandle SetCurrentWorld(WorldHandle world);

//proto native void SchedulePreload(vector pos, float radius);

proto native IEntity FindEntityByName(IEntity worldEnt, string name);
proto native IEntity FindEntityByID(IEntity worldEnt, int ID);

//!returns number of active (simulated) Entities in the world
proto native int GetNumActiveEntities(IEntity worldEntity);
//!returns active entity
proto native IEntity GetActiveEntity(IEntity worldEntity, int index);
//@}

//----------------------------------------------
/**
 * \defgroup Camera Camera
 * @{
 */

enum CameraType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

//! sets which camera will be a listener (for sound engine)
proto native void SetListenerCamera(int camera);

/*!
Changes camera position
\param cam Index of camera
\param origin	position
\param angle	orientation
*/
proto native void SetCamera(int cam, vector origin, vector angle);

//! Changes camera matrix
proto native void SetCameraEx(int cam, const vector mat[4]);

//!Returns current camera transformation
proto native void GetCamera(int cam, out vector mat[4]);

proto native void SetCameraVerticalFOV(int cam, float fovy);
proto native void SetCameraFarPlane(int cam, float farplane);		//default 160000 units
proto native void SetCameraNearPlane(int cam, float nearplane);	//default 5 units

proto native void SetCameraType(int cam, CameraType type);

/*! 
\brief Post-process effect type.
\attention Keep enum names in synch with post-process effect material class names. Postfix "Effect" is added automatically.
*/
enum PostProcessEffectType
{
	None,
	UnderWater,
	SSAO,
	DepthOfField,
	HBAO,
	RotBlur,
	GodRays,
	Rain,
	FilmGrain,
	RadialBlur,
	ChromAber,
	WetDistort,
	DynamicBlur,
	ColorGrading,
	Colors,
	Glow,
	SMAA,
	FXAA,
	Median,
	SunMask,
	GaussFilter,
	SSR
};
/*!
set postprocess effect to camera
To disable effect in some prioroty ppEffect, just set effectName or name to NULL
\param cam 		number of camera
\param priority		priority of effect
\param type	type of effect
\param materialPath 		material
*/
proto native void SetCameraPostProcessEffect(int cam, int priority, PostProcessEffectType type, string materialPath);
		
//ent can be NULL for world-space coords
proto vector	ProjectVector(int cam, IEntity ent, vector vec);
proto vector	UnprojectVector(int cam, float x, float y, vector dir);

//@}

//----------------------------------------------
/**
 * \defgroup Light Light API
 * @{
 */

//!Light handle
typedef int[] HLIGHT;

enum LightType
{
	POINT, //< point light, all directional light
	SPOT,	//<	spot light, direction is determined by owner (entity)
	DIRECTIONAL,
	AMBIENT
};

enum LightFlags
{
/*!
Dynamic light. There is limit 512 dynamic lights per world and
32 per camera view. They are faster when moving and changing shape.
Also they are always attached to owner entity
*/
	DYNAMIC, 
	CASTSHADOW,
//! for cheaper dynamic lights, like muzzle flashes (might use cheaper rendering method)
	CHEAP
};

/*!
creates light
*/
proto HLIGHT AddLight(IEntity owner, LightType type, LightFlags flags, float radius, vector color);
//!removes light
proto native bool RemoveLight(HLIGHT light);
proto native bool SetLightEx(HLIGHT light, float radius, vector color);
//!sets lookup texture for projection lights
proto native bool SetLightTexture(HLIGHT light, string cubemap);
proto native int SetLightFlags(HLIGHT light, LightFlags flags);
proto native int ClearLightFlags(HLIGHT light, LightFlags flags);
//!Sets light cone in degrees (for LightType.SPOT).
proto native bool SetLightCone(HLIGHT light, float cone);
//@}


//----------------------------------------------
/**
 * \defgroup WorldTrace Trace&Visibility API
 * @{
 */

enum TraceFlags
{
	BONES, //< tests collision geometries around bones of animated objects
	ENTS, //< tests entities
	WORLD, //< tests world bounding box
	ONLY_PHYSICS,
	WATER, //< tests collision with water surface
	PASSTRANSLUCENT,//< Do not intersects with entities with EntityFlags.TRANSLUCENT set
	RAGDOLLS, //< tests ragdolls
	VISTEST, //< performs visibility test first. Not necessary for entities receiving EntityEvent.VISIBLE, because there is a certainty that a camera will see them
	NOTRACE,
	TRANSPARENT_OCCLUDERS
};

enum TraceShape
{
  LINE,
  BOX,
  OBB,
  SPHERE
};

//! collision and tracing
class TraceContact
{
	float	Fraction;
	int		Content;
	int		Surfparm;
	int		MaterialFlags;
	int		Triangle;
	int		SurfaceID;
	owned string	MaterialName;
	owned string	OriginalMaterialName;
	float		Plane[4];
	vector	Point;
}

proto native bool TraceLineToEntity(IEntity ent, vector start, vector end, out TraceContact contact);

//bool FilterCallback(Class target [, vector rayorigin, vector raydirection])

class TraceParam: Managed
{
	vector		Start;
	vector		End;
	int			LayerMask = 0xffffffff;
	TraceFlags	Flags;
	IEntity		Exclude;
	
};

class TraceSphere: TraceParam
{
	float			Radius;	
};

class TraceBox: TraceParam
{
	vector		Mins;
	vector		Maxs;
};

class TraceOBB: TraceBox
{
	vector		Mat[3];
};

/*!
traces line start->end, return 0..1 if trace was sucessfull.
// It take bboux from ent
// flag like in P2PVisibilityEx will be added
//OUTPUT:
\param cent		[out] traced entity
\param plane	[out] traced polygon plane (X,Y,Z,D)
\param surfparm	 [out] traced surface parameters
\returns	value 0...1, percentage of a path traveled
*/
proto volatile float TraceMove(TraceParam param, out IEntity cent, out float plane[4], out int surfparm, func filtercallback);


/*!
tests visibility
\param from
\param to
\param flags
//TraceFlags.VISTEST	- 
//TraceFlags.DETAIL		- test agains detail brushes
//TraceFlags.ENT			- test against brush entities
//TraceFlags.NOTRACE	- doesn't test geometry (has meaning in conjuction with s TraceFlags.VISTEST)
\returns		true is visible/false not visibel
*/
proto native int P2PVisibilityEx(vector from, vector to,int flags);

//! finds all entities in a radius
proto int SphereQuery(vector origin, float radius, out IEntity visents[], int ents, int fmask);

/*!tests if bbox is visible according to view-frustum and PVS
\param flags & 1 - test also PVS
\returns true/false is/isn't visible
*/
proto native bool IsBoxVisible(vector mins, vector maxs, int flags);

/*!finds all visible entities (rought, according to a visibility. It is convinient for entitie selection where we want to do more precise visibility test)
\param origin		-		position it is looked from
\param look		-		look direction
\param angle		-		view angle (usuably 90). -1 if we do not care about the view angle
\param ents		-		array to which entities will be stored
\param maxents	-		length of the array (prevents overflowing the array)
\param fmask		-		flag mask (SetFlags()). Entity must have all flags set.
						it is possible to use reserved flags like EntityFlags.USER1, EntityFlags.USER2, EntityFlags.USER6 for fast finding of entities in custom categories
*/
proto int		VisEntities(vector origin, vector look, float angle, float radius, out IEntity ents[2], int maxents, int fmask);

/*!
Object that handles visibility on GPU. Used for coronas etc.
*/
class OcclusionQuery
{
	proto private void ~OcclusionQuery();

	/*!
	return Query result
	\returns  -1 result is not ready yet, try it later.
  >0 point is visible
 ==0 point is not visible
	*/
	proto native int GetResult();

	//!Sets world position
	proto native void SetPosition(vector pos);
	//!Destroys the object
	proto native void Destroy();
};

//@}

//----------------------------------------------
/**
 * \defgroup Decals Decals API
 * @{
 */
typedef int[] hDecal;

/*!
Creates single visual mark, e.g. from shots
//when lifetime=0, pointer to decal is returned, that can be removed by RemoveDecal then
\param entity		entity where the landmark should be created
\param origin		first point of the decal, nothing is done now
\param project	projection direction (length is far clipping distance)
\param nearclip	near clipping distance
\param materialName Material used for decal
\param lifetime	Lifetime in seconds
\param flags Not used ATM
\return Decal pointer or null
*/
proto native hDecal CreateDecal(IEntity entity, vector origin, vector project, float nearclip, float angle, float size, string materialName, float lifetime, int flags);

proto native void RemoveDecal(hDecal decal);

/*!
Creates continous visual mark, e.g. from wheel when a car is moving on the ground
\param entity		entity where the landmark should be created (only terrain is supported ATM)
\param origin		first point of the decal, nothing is done now
\param normal		normal of surface
\param edgesize	Edge size of decal
\param lifetime	Lifetime in seconds
\param materialName Material used for decal
\param prev			Previous decal, we are connecting to
\param alpha		translucency of point
\return Decal pointer or null
*/
proto native hDecal CreateLandMarkDecal(IEntity entity, vector origin, vector normal, float edgeSize, float lifeTime, string materialName, hDecal prevDecal, float alpha);


/*!
is it possible to add new point to landmark decal?
\param lmDecal		entity to add new landmark point
\param entity		entity to add new landmark point
\param mat			material of decal
\param newpoint 	new point to add
\return
	LMD_ERROR 	= error when adding new point (invalid decal)
	LMD_VALID 	= can add new point
	LMD_DIFF_ENT 	= new point is on different entity
	LMD_TOO_FAR	= new point is too far from previous point
*/
proto native int CanAddToLandMarkDecal(hDecal lmDecal, IEntity entity, string mat, vector newPoint);

/*!
add new point to decal, internally, new point is added when previous point is in some
distance or the angle is more than some threshold
\param lmDecal		entity to add new landmark point
\param point		contact point
\param normal		normal of contact
\param alpha		translucency in point
\return true if everything was OK, false if not. In this case, the application MUST not used later the pointer to decal, it's finalized internally !
*/
proto native bool AddPointToLandMarkDecal(hDecal lmDecal, vector point, vector normal, float alpha);

/*!
finalize landmark adding, e.g. when entity lose contact with ground -> the pointer to decal
should have only world and entity if it has something to render, otherwise it's destroyed here
\param lmDecal		entity to add new landmark point
\param addAlpha	if to add last point with transition to zero alpha
\param alphaDist	distance to add last point
*/
proto native void FinalizeLandMarkDecal(hDecal lmDecal, bool addAlpha, float alphaDist);

/*!
return if landmark was finalized
\param lmDecal		decal to test
*/
proto native bool IsLandMarkFinalized(hDecal lmDecal);

/*!
return last landmark point or -65535.0 in all components
\param lmDecal		decal to test
*/
proto native vector GetLastLandMarkPoint(hDecal lmDecal);

/*!
set global parameters for landmark generation
\param minSegmentLength		minimum length segment, when new point is added (4 default), when is less, just the end position is on the fly updated
\param maxSegmentLength		maximum segment length, when length is bigger, the path is finished
\param degAngle				angle in degrees, when is more, the path is finished
*/
proto native void SetGlobalLandMarkParams(float minSegmentLength, float maxSegmentLength, float degAngle);


//@}


//----------------------------------------------
/**
 * \defgroup Ocean Ocean API
 * @{
 */

/*!
Is ocean availabled
*/
proto native bool IsOcean();

/*!
Get water ocean height at given point
\param worldX		world x position
\param worldZ		world z position
*/
proto native float GetOceanHeight(float worldX, float worldZ);


/*!
Get water ocean height and displacement at given point, returns vector(displaceX, height, displaceZ)
\param worldX		world x position
\param worldZ		world z position
*/
proto native vector GetOceanHeightAndDisplace(float worldX, float worldZ);



//@}



//@}
 
