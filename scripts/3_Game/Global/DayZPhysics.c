enum PhxInteractionLayers
{
	NOCOLLISION,

	DEFAULT,
	BUILDING,
	CHARACTER,
	VEHICLE,
	DYNAMICITEM,
	ROADWAY,
	VEHICLE_NOTERRAIN,
	CHARACTER_NO_GRAVITY,
	FIREGEOM,

	DOOR,
	RAGDOLL,

	WATERLAYER,
	TERRAIN,

	GHOST,
	WORLDBOUNDS,
	FENCE,

	AI,
	AI_NO_COLLISION,
	AI_COMPLEX,

	TINYCAPSULE,
	TRIGGER,
	TRIGGER_NOTERRAIN,

	ITEM_SMALL,
	ITEM_LARGE,

	CAMERA,
	
	TEMP
};


/*!
	Input parameters for RaycastRVProxy function.
*/
class RaycastRVParams
{
	vector begPos; //!< begin position of raycast (e.g. player position)
	vector endPos; //!< end position of raycast (e.g. player direction)

	Object ignore; //!< ignore this object in collision, used only if groundOnly is false
	Object with;   //!< ignore object with this object, otherwise collision hits, used only if groundOnly is false

	float radius; //!< radius along the ray tested

	/*!
		Sets the raycast behaviour in terms of result.
		\see CollisionFlags
	*/
	CollisionFlags flags;

	/*!
		type of intersection, possible values
		ObjIntersectFire(0),
		ObjIntersectView(1),
		ObjIntersectGeom(2),
		ObjIntersectIFire(3), 
		ObjIntersectNone(4)
	*/
	int type;

	bool sorted;     //!< used only if groundOnly = false
	bool groundOnly; //!< raycasts only ground (ignores all objects). Default value is false if not needed.

	void RaycastRVParams( vector vBeg, vector vEnd, Object pIgnore = null, float fRadius = 0.0 )
	{
		begPos = vBeg;
		endPos = vEnd;
		ignore = pIgnore;
		radius = fRadius;

		// default values
		with       = null;
		flags      = CollisionFlags.NEARESTCONTACT;
		type       = ObjIntersectView;
		sorted     = false;
		groundOnly = false;
	}
};


/*!
	Contains result data of RaycastRVProxy function.
*/
class RaycastRVResult
{
	Object obj;    //!< object,that we collide with (NULL if none), If hierLevel > 0 object is the proxy object
	Object parent; //!< if hierLevel > 0 most parent of the proxy object

	vector pos; //!< position of collision (in world coord)
	vector dir; //!< direction outside (in world coord) or (in case of line-object collision) direction and size of the intersection

	int hierLevel; //!< which hierarchy level is the collision detected at, == 0 = objects in landscape, > 0 = proxy
	int component; //!< index of component in corresponding geometry level

	bool entry; //!< is false if begining point was TriggerInsider
	bool exit;  //!< is false if end point was inside
};


class DayZPhysics
{
	/**
	\brief Raycasts world by given parameters
	\param begPos \p vector 	Begin position of raycast (e.g. player position)
	\param endPos \p vector 	End position of raycast (e.g. player direction)
	\param contactPos \p vector out, 	world position of first contact
	\param contactDir \p vector out, 	direction of first contact (available only when object is hitted)
	\param contactComponent \p int out, object 	component index (available only when object is hitted)
	\param results \p set<Object> out, 	set of objects hitted by raycast. Can be NULL if not needed
	\param with \p Object 	Ignores the object from collision. Used only when \p ground_only is false. Can be NULL if not needed
	\param ignore \p Object Ignores the object from collision. Used only when \p ground_only is false. Can be NULL if not needed 
	\param sorted \p bool 	Default value is false, used only if \p ground_only = false
	\param ground_only \bool 	raycasts only ground (ignores all objects). Default value is false if not needed.
	\param iType \p int, 	type of intersection, possible values ObjIntersectFire(0), ObjIntersectView(1), ObjIntersectGeom(2), ObjIntersectIFire(3), 
	ObjIntersectNone(4) 
	\param radius \p float 	Radius of the ray, default value set to 0 

	\returns \p bool return true if raycast hits ground or object
	@code
	// raycast test
	PlayerBase player = g_Game.GetPlayer();
	
	if (player)
	{	
		vector begPos = player.GetPosition();
		vector endPos = begPos + (player.GetDirection() * 100);
		vector contactPos;
		vector contactDir;
		int contactComponent; 
		set<Object> results = new set<Object>;
		Object with; 
		Object ignore;  
		bool sorted; 
		bool ground_only;  
		int iType;
		float radius;

		if ( GetGame().Raycast(begPos, endPos, contactPos, contactDir, contactComponent, results, with, ignore, sorted, ground_only, iType, radius) )
		{	
			Print(begPos);
			Print(endPos);
			Print(contactPos);
			Print(contactDir);
			Print(contactComponent);	
			Print(results);
			Print(with);
			Print(ignore);
			Print(sorted);
			Print(ground_only);
			Print(iType);
			Print(radius);

			Print( GetGame().Raycast(begPos, endPos, contactPos, contactDir, contactComponent, results, with, ignore, sorted, ground_only, iType, radius) );
		}
	}

		>> vector begPos = 0x00000000dd62df30 {<750.573,49.8043,2167.25>}
		>> vector endPos = 0x00000000dd62df3c {<849.589,49.8043,2181.25>}
		>> vector contactPos = 0x00000000dd62df48 {<751.068,49.8043,2167.32>}
		>> vector contactDir = 0x00000000dd62df54 {<0.987203,2.04891e-08,0.139505>}
		>> int contactComponent = 8
		>> set<Object> results = set<Object><a022d840>
		>> Object with = NULL
		>> Object ignore = NULL
		>> bool sorted = 0
		>> bool ground_only = 0
		>> int iType =   0
		>> float radius = 0
	
		>> 1
	@endcode
	*/
	proto static bool	RaycastRV(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, /*out*/ set<Object> results = NULL, Object with = NULL, Object ignore = NULL,  bool sorted = false,  bool ground_only = false,  int iType = ObjIntersectView, float radius = 0.0, CollisionFlags flags = CollisionFlags.NEARESTCONTACT);

	proto static bool	RaycastRVProxy( notnull RaycastRVParams in, out notnull array< ref RaycastRVResult> results );


	proto static bool	RayCastBullet(vector begPos, vector endPos, PhxInteractionLayers layerMask, out Object hitObject, out vector hitPosition, out vector hitNormal, out float hitFraction);

	proto static bool	SphereCastBullet(vector begPos, vector endPos, float radius, PhxInteractionLayers layerMask, out Object hitObject, out vector hitPosition, out vector hitNormal, out float hitFraction);
}