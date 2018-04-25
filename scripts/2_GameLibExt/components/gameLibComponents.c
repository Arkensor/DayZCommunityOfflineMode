#ifdef COMPONENT_SYSTEM
//Generic components from GameLib (script side of c++ classes)

typedef int[] IEntityComponentSource;
class IEntityComponentSource: BaseContainer
{
};

//!Entity touch event types
enum TouchEvent
{
	ON_ENTER,
	ON_STAY,
	ON_EXIT
};

//!Builtin component types
//TypeID MeshObjectTypeID;
//TypeID HierarchyTypeID;
//TypeID RigidBodyTypeID;
//TypeID SphereGeometryTypeID;
//TypeID BoxGeometryTypeID;


class GenericComponent : Managed
{
	/**
	* Gets current eventmask of the component.
	* \return Returns bitmask of events the component accepts
	*/
	proto native int GetEventMask();
	
	/**
	* Sets eventmask. Component accepts only events which has set bits in eventmask.
	* Bits are or'ed with existing bitmask. See enf::EntityEvents.
	* When this method is called in the constructor of the component, it will not properly set the eventmask to the parent entity. You may consider OnComponentInsert event.
	* \param mask Mask of those bits, which will be set.
	* \return Returns bitmask of events the component accepts (result of mask|GetEventMask())
	*/
	proto native int SetEventMask(IEntity owner, int mask);
	
	/**
	* Clears bitmask. Component accepts only events which has set bits in eventmask.
	* Only bits set in the mask are cleared. See enf::EntityEvents
	* \param mask Mask of those bits, which will be cleared.
	* \return returns these bits that were set before and now are cleared.
	*/
	proto native int ClearEventMask(IEntity owner, int mask);

	/**
	* Activate component and calls EOnActivate().
	*/
	proto native void Activate(IEntity owner);

	/**
	* Deactivate component and calls EOnDectivate().
	*/
	proto native void Deactivate(IEntity owner);

	/**
	* Returns activity state.
	* \return Returns true, if component is active.
	*/
	proto native bool IsActive();
	
	//! Constructor
	protected void GenericComponent(IEntityComponentSource src, IEntity ent);	
}

class GenericComponentClass
{
	bool DependsOn(typename otherClass, TypeID otherTypeID) {}
}

/**
* Parent class for all components created in script.
* Every ScriptComponent is being created in Entity's constructor and may receive following events
* 1. OnComponentInsert is being called when component is created. This is last event Workbench sends in World Editor edit mode.
* 2. EOnInit is being called after all components have been inserted and if the component registered event mask EV_INIT
* 3. EOnActivate is being called if the entity was flagged as TFL_ACTIVE and if the component is active. The component is active by default.
* 4. EOnXXXs are being called base on event mask component registered
* 5. EOnDeactivate is being called when Deactivate is called or when the component is being to be removed. Component must be active to be deactivated.
* 6. OnComponentRemove is being called after a component is removed.
* 7. EOnDelete is being called after entity is going to be destroyed.
*/
class ScriptComponent : GenericComponent
{
	/*
	Event when owner entity is touched
	\param owner
	Touched entity
	\param extra
	Bitmask of touch types TODO
	*/
	private void EOnTouch(IEntity owner, int extra);

	/*!
	Event after component is initialized. At this point all entity's components have recieved OnComponentInsert.
	\param owner
	\param extra
	Number of entity
	*/
	private void EOnInit(IEntity owner, int extra);

	/*!
	Extra event of various functional extensions. ATM it's used
	by Trigger when some insider is leaving
	\param owner
	owner actor of event
	\param extra
	Extra value of event
	*/
	private void EOnExtra(IEntity owner, int extra);

	/*!
	Event when we are out of visibility
	\param owner
	\param extra
	Frame number
	*/
	private void EOnNotVisible(IEntity owner, int extra);

	/*!
	Event when we are visible
	\param owner
	\param extra
	Frame number
	*/
	private void EOnVisible(IEntity owner, int extra);

	/*!
	Event every frame
	\param owner
	\param timeSlice
	Time passed since last frame
	*/
	private void EOnFrame(IEntity owner, float timeSlice);

	/*!
	Even after physics update
	\param owner
	\param extra
	Frame number
	*/
	private void EOnPostFrame(IEntity owner, int extra);

	/*!
	Event from animation system
	\param owner
	\param extra
	extra data
	*/
	private void EOnAnimEvent(IEntity owner, AnimEvent extra);

	/*!
	Event from sound system
	\param owner
	\param extra
	extra data
	*/
	private void EOnSoundEvent(IEntity owner, SoundEvent extra);

	/*!
	Event after simulated by physics engine (once per frame)
	\param owner
	\param timeslice
	Time slice of simulation step
	*/
	private void EOnPostSimulate(IEntity owner, float timeslice);

	/*!
	Event before simulated by physics engine (called from sub-iterations!
	\param owner
	\param timeslice
	Time slice of simulation step
	*/
	private void EOnSimulate(IEntity owner, float timeslice);

	/*!
	Event when joint attached to RigidBody of this entity is broken
	\param owner
	owner Entity attached to the joint
	\param extra
	Not used ATM
	*/
	private void EOnJointBreak(IEntity owner, int extra);
	/*!
	Event when physics engine has moved with this Entity
	\param owner
	World Entity
	\param extra
	Not used ATM
	*/
	private void EOnPhysicsMove(IEntity owner, int extra);

	/*!
	Event when physics engine registered contact with owner RigidBody
	\param owner
	\param contact
	Structure describing the contact
	*/
	private void EOnContact(IEntity owner, Contact extra);

	/**
    * Event when component is activated.
    */
    private void EOnActivate(IEntity owner);

    /**
    * Event when component is deactivated.
    */
    private void EOnDeactivate(IEntity owner);

	/*!
	Event when a component is created and added to Entity.
	\param owner Entity into which component is added
	\param other Component which is being added into Entity
	*/
	private void OnComponentInsert(IEntity owner, ScriptComponent other);

	/*!
	Event when a component is being removed from Entity.
	\param owner Entity from which component is being removed
	\param other Component which is being removed from the Entity
	*/
	private void OnComponentRemove(IEntity owner, ScriptComponent other);

	/*!
	Called when Entity is being to be destroyed (deleted) or component to be deleted (see Game::DeleteScriptComponent).
	\param owner Entity which owns the component
	*/
	private void OnDelete(IEntity owner);
}

typedef int[] SoundHandle;

class SignalInput
{
	string m_name;
	float m_value;
	
	void SignalInput()
	{
		m_value = 0;
	}
};	

class BaseSoundComponent : GenericComponent
{
	/* Get list of 'events'. */
	proto native int GetEventNames(out array<string> events);
	/* Get list of 'signals. '*/
	proto native int GetSignalNames(out array<string> signals);
	/* Convert signal name to index. */
	proto native int GetSignalIndex(string name);
	/* Set signal value by 'name'. */
	proto native void SetSignalValueName(string signal, float value);
	/* Set signal value by 'index'. */
	proto native void SetSignalValue(int index, float value);
	/* Play 'event'. */
	proto native SoundHandle Play(string name);
	/* Play sounds based on triggers. */
	proto native SoundHandle Update();
	/* Terminate 'sound'. */
	proto native void Terminate(SoundHandle handle);
	/* Check if 'sound' is played. */
	proto native bool IsPlayed(SoundHandle handle);
	/* Validate handle. */
	proto native bool IsHandleValid(SoundHandle handle);
	/* Set sound position. */
	proto native void SetTransform(vector[] transf);
	/* Enable debug mode. */
	proto native void SetDebug(bool value);
};

#endif