/*!
	Type of vehicle's fluid.

	!!! DO NOT EXTEND THIS !!!
	Maximal number of fluid types is limited to eight.
*/
enum CarFluid
{
	FUEL,
	OIL,
	BRAKE,
	COOLANT,

	USER1, //!< reserved for user / modding support
	USER2, //!< reserved for user / modding support
	USER3, //!< reserved for user / modding support
	USER4  //!< reserved for user / modding support
};



//!	Enumerated vehicle's gears.
enum CarGear
{
	REVERSE,
	NEUTRAL,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
	SIXTH,
	SEVENTH,
	EIGTH,
	NINTH,
	TENTH,
	ELEVENTH,
	TWELFTH,
	THIRTEENTH,
	FOURTEENTH,
	FIFTEENTH,
	SIXTEENTH
};



//!	Base native class for all motorized wheeled vehicles.
class Car extends Transport
{
	//!	Returns the current speed of the vehicle in km/h.
	proto native float GetSpeedometer();

	/*!
		Returns fraction value (in range <0, 1>)
		of the current state of the specified vehicle's fluid.

		\param fluid the specified fluid type
	*/
	proto native float GetFluidFraction( CarFluid fluid );

	//! Removes to the specified fluid the specified amount.
	proto native void Leak( CarFluid fluid, float amount );

	//! Adds from the specified fluid the specified amount.
	proto native void Fill( CarFluid fluid, float amount );

	//!	Returns the instance of vehicle's controller.
	proto native CarController GetController();

	//! Returns engine's rpm value.
	proto native float GetEngineRPM();

	//! Returns true when engine is running, false otherwise.
	proto native bool IsEngineOn();

	//! Starts the engine.
	proto native void EngineStart();

	//! Stops the engine.
	proto native void EngineStop();

	//! Returns true when lights are on, false otherwise.
	proto native bool IsLightsOn();

	//! Toggles the vehicle's lights state (on / off).
	proto native void SwitchLights();
};



//! Native class for controlling a wheeled vehicle (Car class).
class CarController
{
	//!	Returns the current steering value in range <-1, 1>.
	proto native float GetSteering();
	/*!
		Sets the steering value.

		\param in should be in range <-1, 1>
	*/
	proto native void SetSteering( float in );

	//!	Returns the current thrust turbo modifier value in range <0, 1>.
	proto native float GetThrustTurbo();
	//!	Returns the current thrust gentle modifier value in range <0, 1>.
	proto native float GetThrustGentle();
	//!	Returns the current thrust value in range <0, 1>.
	proto native float GetThrust();
	/*!
		Sets the thrust value.

		\param in     should be in range <0, 1>
		\param gentle should be in range <0, 1>, thrust modifier
		\param turbo  should be in range <0, 1>, thrust modifier
	*/
	proto native void SetThrust( float in, float gentle = 0, float turbo = 0 );

	//! Returns the current brake value in range <0, 1>.
	proto native float GetBrake();
	/*!
		Sets the brake value.

		\param in should be in range <0, 1>
	*/
	proto native void SetBrake( float in );

	//!	Returns index of the current gear.
	proto native int GetGear();

	proto native void ShiftUp();
	proto native void ShiftDown();
};
