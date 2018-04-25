/**
 * \defgroup Physics Physics system
 * @{
 */

typedef int[] dGeom;
typedef int[] dJoint;
typedef int[] dBlock;

proto native int dGetNumDynamicBodies(notnull IEntity worldEnt);
proto native IEntity dGetDynamicBody(notnull IEntity worldEnt, int index);
proto native void dSetInteractionLayer(notnull IEntity worldEntity, int mask1, int mask2, bool enable);

//!Gets global gravity
proto native vector dGetGravity(notnull IEntity worldEntity);
//!Changes global gravity
proto native void dSetGravity(notnull IEntity worldEntity, vector g);
//!Changes fixed time-slice. Default is 1/40, thus simulation runs on 40fps. With smaller values, there is more precise simulation
proto native void dSetTimeSlice(notnull IEntity worldEntity, float timeSlice);

/**
 * \defgroup RigidBody RigidBody API
 * @{
 */

//proto native int dMaterialClone(string target, string source, int material_index)
//proto native int dMaterialGetType(string source)
//proto native int dMaterialSetType(string source, int material_index)

class PhysicsGeomDef: Managed
{
	string	Name;
	dGeom		Geometry;
	vector	Frame[4] = {Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)};
	int			ParentNode = -1;
	string	MaterialName;
	int			LayerMask; //<Bit mask of layers we are in

	
	void PhysicsGeomDef(string name, dGeom geom, string materialName, int layerMask)
	{
		Name = name;
		Geometry = geom;
		MaterialName = materialName;
		LayerMask = layerMask;	
	}	
};

/*!
Creates CollisionObject from geometry embedded in attached vobject. If there is not any, false is returned
\param mass Body mass
\param layerMask Bit mask of layers that is ANDed with layers in object geometry. Use 0xffffffff if you want to keep it unmodified
*/
proto native bool dBodyCreateStatic(notnull IEntity ent, int layerMask);

/*!
Creates RigidBody from custom made geometries. The geometries are deleted when rigid body is destroyed
\param geoms array of custom made geometries
@code
	autoptr PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateBox(size), "material/default", 0xffffffff)};
	dBodyCreateStaticEx(this, geoms);
@endcode
*/
proto bool dBodyCreateStaticEx(notnull IEntity ent, PhysicsGeomDef geoms[]);

/*!
Creates RigidBody from geometry embedded in attached vobject. If there is not any, false is returned
\param mass Body mass
\param layerMask Bit mask of layers that is ANDed with layers in object geometry. Use 0xffffffff if you want to keep it unmodified
*/
proto native bool dBodyCreateDynamic(notnull IEntity ent, float mass, int layerMask);

//int SetupPhysicsGeomDefs(vobject obj, out PhysicsGeomDef defs[], int maxDefs);
//proto native int dBodyAddGeoms(IEntity body, PhysicsGeomDef defs[], int numGeoms);

/*!
Creates RigidBody from custom made geometries. The geometries are deleted when rigid body is destroyed
\param centerOfMass Offset from object-pivot to center of mass
\param mass Body mass
\param geoms array of custom made geometries
@code
	autoptr PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateBox(size), "material/default", 0xffffffff)};
	dBodyCreateDynamicEx(this, center, 1.0, geoms);
@endcode
*/
proto bool dBodyCreateDynamicEx(notnull IEntity ent, vector centerOfMass, float mass, PhysicsGeomDef geoms[]);

proto bool dBodyCreateGhostEx(notnull IEntity ent, PhysicsGeomDef geoms[]);

//!Destroys attached physics body
proto native void dBodyDestroy(notnull IEntity ent);

//!Has the entity attached physics body?
proto native bool dBodyIsSet(notnull IEntity ent);

proto native void dBodySetInteractionLayer(notnull IEntity ent, int mask);
proto native int dBodyGetInteractionLayer(notnull IEntity ent);
proto native void dBodySetGeomInteractionLayer(notnull IEntity ent, int index, int mask);
proto native int dBodyGetGeomInteractionLayer(notnull IEntity ent, int index);

//! state of a rigidbody
enum ActiveState
{
	ACTIVE,
	INACTIVE,
	ALWAYS_ACTIVE
};
	
proto native void dBodyActive(notnull IEntity ent, ActiveState activeState);
proto native void dBodyDynamic(notnull IEntity ent, bool dynamic);
proto native bool dBodyIsDynamic(notnull IEntity ent);

proto native bool dBodyIsActive(notnull IEntity ent);
proto native bool dBodyEnableGravity(notnull IEntity ent, bool enable);
proto native void dBodySetDamping(notnull IEntity ent, float linearDamping, float angularDamping);
proto native void dBodySetSleepingTreshold(notnull IEntity body, float linearTreshold, float angularTreshold);

/*!
If both maxMotion and shapeCastRadius is >=0 then the continuous collision detection is enabled.
If you want to disable it, use -1
\param maxMotion max motion threshold when sphere-cast is performed, to find time of impact. It should be
little bit less than size of the geometry to catch the situation when tunelling can happen
\param sphereCastRadius The radius of the largest possible sphere, that is completelly inside the body geometry.
*/
proto native void dBodyEnableCCD(notnull IEntity body, float maxMotion, float sphereCastRadius);
/*!
Sets scale factor for all impulses/velocities/forces. Default is <1,1,1>. Zero any axis if you want to do 2D physics
*/
proto native void dBodySetLinearFactor(notnull IEntity body, vector linearFactor);

//!returns center of mass offset
proto native vector dBodyGetCenterOfMass(notnull IEntity body);

/**
\brief Returns linear velocity
	\param ent \p IEntity entity which origin will be set
	\param mat \p vector[4] matrix to be set
	\return \p vector linear velocity
	@code
		Man player = g_Game.GetPlayer();
		Print( GetVelocity(player) );

		>> <0,0,0>
	@endcode
*/
proto native vector GetVelocity(notnull IEntity ent);

/**
\brief Sets linear velocity (for Rigid bodies)
	\param ent \p entity which velocity will be set
	\param vel \p velocity vector to be set
*/
proto native void SetVelocity(notnull IEntity ent, vector vel);

/**
\brief Disables collisions between two entities
*/
proto native dBlock dBodyCollisionBlock(notnull IEntity ent1, notnull IEntity ent2);
proto native void dBodyRemoveBlock(notnull IEntity worldEntity, dBlock block);

proto native void dBodySetInertiaTensorV(notnull IEntity body, vector v);
proto native void dBodySetInertiaTensorM(notnull IEntity body, vector m[3]);

proto native float dBodyGetMass(notnull IEntity ent);
proto native void dBodySetMass(notnull IEntity body, float mass);

/**
\brief Applies impuls on a pos position in world coordinates
*/
proto void dBodyApplyImpulseAt(notnull IEntity body, vector impulse, vector pos);

/**
\brief Applies impuls on a rigidbody (origin)
*/
proto void dBodyApplyImpulse(notnull IEntity body, vector impulse);

/**
\brief Applies constant force on a rigidbody (origin)
*/
proto void dBodyApplyForce(notnull IEntity body, vector force);

/**
\brief Applies constant force on a position
*/
proto void dBodyApplyForceAt(notnull IEntity body, vector pos, vector force);

proto native void dBodyApplyTorque(notnull IEntity body, vector torque);

/**
\brief Gets angular velocity for a rigidbody
*/
proto vector dBodyGetAngularVelocity(notnull IEntity body);

/**
\brief Changed an angular velocity
	\param body \p Rigid body
	\param angvel \p Angular velocity, rotation around x, y and z axis (not yaw/pitch/roll)
*/
proto void dBodySetAngularVelocity(notnull IEntity body, vector angvel);

/**
\brief Sets target transformation. If timeslice == dt (simulation step delta time), it will happen in next step, otherwise in time = timeslice
*/
proto native void dBodySetTargetMatrix(notnull IEntity body, vector matrix[4], float timeslice);
proto native float dBodyGetKineticEnergy(notnull IEntity body);

proto native vector dBodyGetVelocityAt(notnull IEntity body, vector globalpos);
//@}

/**
 * \defgroup Geometry Geometry API definition
 * @{
 */

//! Creates box geometry
proto native dGeom dGeomCreateBox(vector size);

//! Creates sphere geometry
proto native dGeom dGeomCreateSphere(float radius);

//! Creates capsule geometry
proto native dGeom dGeomCreateCapsule(float radius, vector extent);

//! Creates cylinder geometry
proto native dGeom dGeomCreateCylinder(float radius, vector extent);

//! Destroys geometry
proto native void dGeomDestroy(dGeom geom);

//proto native int dBodyAddGeom(notnull IEntity body, dGeom geom, vector frame[4], string material, int interactionLayer);
// find a geometry by its name and returns its index or -1 if the geometry wasn't found
proto native int dBodyGetGeom(notnull IEntity ent, string name);
// returns number of geometries of the entity
proto native int dBodyGetNumGeoms(notnull IEntity ent);
//@}

/**
 * \defgroup Vehicle Vehicle API definition
 * @{
 */
class VehicleWheelParams
{
	float	SuspensionStiffness;
	float	SuspensionCompression;
	float	SuspensionDamping;
	float	MaxSuspensionTravel;
	float RollInfluence;
};

proto native bool dBodyCreateVehicle(notnull IEntity ent, vector center, float mass, int layerMask);
proto native void dVehicleAddWheel(notnull IEntity ent, vector offset, vector suspensionDir, vector axleDir, float wheelRadius, VehicleWheelParams params, string wheelMat);
proto native void dVehicleApplyEngineForce(notnull IEntity ent, int wheel, float force);
proto native void dVehicleSetBrake(notnull IEntity ent, int wheel, float brake);
proto native void dVehicleSetSteering(notnull IEntity ent, int wheel, float steering);
proto native float dVehicleGetSteering(notnull IEntity ent, int wheel);
proto native float dVehicleGetWheelAngle(notnull IEntity ent, int wheel);
proto native float dVehicleGetWheelAngularVelocity(notnull IEntity ent, int wheel);
proto native float dVehicleGetWheelSuspensionLength(notnull IEntity ent, int wheel);
proto native vector dVehicleGetWheelEndPosition(notnull IEntity ent, int wheel);
proto native IEntity dVehicleHasWheelContact(notnull IEntity ent, int wheel);

class WheelContactInfo
{
	int		Type;
	vector	EndPos;
	float		SuspensionLength;
	float		Fraction;
	vector	ContactNormal;
	owned string	Mat;
	IEntity	ContactEntity;
};

proto native bool dVehicleGetWheelContact(int wheel, notnull WheelContactInfo info);
//@}

/**
 * \defgroup Constraints Constraints API definition
 * @{
 */

proto native dJoint dJointCreateHinge(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector axis1, vector point2, vector axis2, bool block, float breakThreshold);
proto native dJoint dJointCreateHinge2(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreateSlider(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreateBallSocket(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector point2, bool block, float breakThreshold);
proto native dJoint dJointCreateFixed(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector point2, bool block, float breakThreshold);
proto native dJoint dJointCreateConeTwist(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreate6DOF(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreate6DOFSpring(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native void dJointDestroy(dJoint joint);

//only hinge joint
proto native void dJointHingeSetLimits(dJoint joint, float low, float high, float softness, float biasFactor, float relaxationFactor);
proto native void dJointHingeSetAxis(dJoint joint, vector axis);
proto native void dJointHingeSetMotorTargetAngle(dJoint joint, float angle, float dt, float maxImpulse);

//only cone-twist joint
proto native void dJointConeTwistSetAngularOnly(dJoint joint, bool angularOnly);
// setLimit(), a few notes:
// _softness:
//		0->1, recommend ~0.8->1.
//		describes % of limits where movement is free.
//		beyond this softness %, the limit is gradually enforced until the "hard" (1.0) limit is reached.
// _biasFactor:
//		0->1?, recommend 0.3 +/-0.3 or so.
//		strength with which constraint resists zeroth order (angular, not angular velocity) limit violation.
// __relaxationFactor:
//		0->1, recommend to stay near 1.
//		the lower the value, the less the constraint will fight velocities which violate the angular limits.
proto native void dJointConeTwistSetLimit(dJoint joint, int limitIndex, float limitValue);
proto native void dJointConeTwistSetLimits(dJoint joint, float _swingSpan1, float _swingSpan2, float _twistSpan, float _softness, float _biasFactor, float _relaxationFactor);

//only 6DOF & 6DOFSpring.
/*!
    - free means upper < lower,
    - locked means upper == lower
    - limited means upper > lower
    - axis: first 3 are linear, next 3 are angular
*/
proto native void	dJoint6DOFSetLinearLimits(dJoint joint, vector linearLower, vector linearUpper);
proto native void	dJoint6DOFSetAngularLimits(dJoint joint, vector angularLower, vector angularUpper);
proto native void dJoint6DOFSetLimit(dJoint joint, int axis, float lo, float hi);

//when stiffness == -1 && damping == -1, spring is disabled
proto native void dJoint6DOFSpringSetSpring(dJoint joint, int axis, float stiffness, float damping);

//only slider joint
proto native void dJointSliderSetLinearLimits(dJoint joint, float lowerLimit, float upperLimit);
proto native void dJointSliderSetAngularLimits(dJoint joint, float lowerLimit, float upperLimit);
proto native void dJointSliderSetDirLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetDirAngular(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetLimLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetLimAngular(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetOrthoLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetOrthoAngular(dJoint joint, float softness, float restitution, float damping);
//if force == 0, motor is off
proto native void dJointSliderSetLinearMotor(dJoint joint, float velocity, float force);
proto native void dJointSliderSetAngularMotor(dJoint joint, float velocity, float force);
proto native float dJointSliderGetLinearPos(dJoint joint);
proto native float dJointSliderGetAngularPos(dJoint joint);
//@}

//-----------------------------------------------------------------
typedef int[] dMaterial;

class Contact
{
	dMaterial	Material1;
	dMaterial	Material2;
	int			MaterialIndex1;
	int			MaterialIndex2;
	float		Impulse;
	float		RelativeNormalVelocityBefore;
	float		RelativeNormalVelocityAfter;
	vector		Normal
	vector		Position
	vector		RelativeVelocityBefore;
	vector		RelativeVelocityAfter;

	proto native vector GetNormalImpulse();
	proto native float GetRelativeVelocityBefore(vector vel);
	proto native float GetRelativeVelocityAfter(vector vel);
};
//@}
