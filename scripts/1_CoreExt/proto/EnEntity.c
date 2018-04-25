typedef int[] BaseContainer;		
typedef int[] IEntitySource;
typedef int[] WidgetSource;
	
class BaseContainer	
{
	proto native owned string GetClassName();
	proto native owned string GetName();
	proto native int VarIndex(string varName);
	proto native bool IsVariableSet(int varIndex);
	proto bool IsType(int varIndex, typename type);
	proto bool Get(int varIndex, out void val);
};

class IEntitySource: BaseContainer
{
	proto native IEntitySource GetChildren();
	proto native IEntitySource GetSibling();
	proto native IEntitySource GetParent();
};

class WidgetSource: BaseContainer
{
	proto native WidgetSource GetChildren();
	proto native WidgetSource GetSibling();
	proto native WidgetSource GetParent();
};

/**
 * \defgroup EntityAPI Entity system
 * @{
 */

/**
 * \defgroup EntityAttributes Entity editor attribute system
 * @{
 */

		
		
//@}
	
 //!Entity events for event-mask, or throwing event from code
enum EntityEvent
{
	//! entity was touched by other entity
	TOUCH,

	/*!
	entity is visible, so part of the rendering
	other		WorldClass
	extra		frame index
	result	false should be rendered, true shoudn't be rendered
	*/
	VISIBLE, 
	
	/*!
	entity is not visible, will not be part of rendering
	other		WorldClass
	extra		frame index
	*/
	NOTVISIBLE, 

	/*!
	new frame event, called each frame
	other		WorldClass
	extra		frame index
	*/
	FRAME,

	/*!
	event at the end of each frame or when entity is moved during the frame
	other		WorldClass
	*/
	POSTFRAME,

	/*!
	event called after the world is created, including all entities
	other		WorldClass
	*/
	INIT,      	

//Only with MeshObject
//ANIMEND	- End of AF_ONCE animation, extra = slot number 0...11
//ANIMBLEND - Animation blended from previous one, extra = slot number 0...11


//SOUNDEND	- playing of SFL_ONCE sound has ended
// extra		= pointer to a sound handle

//USER

	JOINTBREAK,
	SIMULATE,
	POSTSIMULATE,
	PHYSICSMOVE,
	CONTACT,
	EXTRA,
	ANIMEVENT,
	SOUNDEVENT,
	PHYSICSSTEADY,
	USER,
	//!Mask of all events
	ALL
};

//!Entity flags
enum EntityFlags
{
/*!
Entity is visible. Is rendered if there is some attached object
and event EntityEvent.VISIBLE is invoked, when event mask is set.
*/
	VISIBLE,


	SOLID,	///<Is collidable by various trace methods
	TRIGGER,	///<Is not collidable, but invokes touch events.
	TOUCHTRIGGERS,	///<Interacts with triggers
	SYNCHRONIZATION_DIRTY,	///<Entity wants to synchronize (network)

	FEATURE,	///<Scene rendering hint for dominant objects that are not culled by standard way
/*!
Used by tracing methods. When tracing with TraceFlags.PASSTRANSLUCENT,
then this entity is ignored. Used for glass for example.
*/
	TRANSLUCENT,

//!Used by tracing methods. Traceable only with flag TraceFlags.WATER
	WATER,

/*!
Tells that this entity has to be actively updated by engine, its
EntityEvent.FRAME has to be called etc.
*/
	ACTIVE,

/*!
Tells that this entity will represent mostly static object, so
we can use more precise but slower methods for scene-tree linking.
Also it tells to scene tree that he can count with this entity as
tree-split hint.
*/
	STATIC,

//!Flags for custom usage and filterings.
	USER1,
	USER2,
	USER3,
	USER4,
	USER5,
	USER6
};

/*!
Internal ancestor of all Entity implementations.
*/
class IEntity: Managed
{
//DO NOT INSERT ANYTHING BELOW - order of event methods matters!
/** \name Event methods
 Event method stubs. Reimplement these in inherited entities to receive event calls			
*/
//@{
	event private void EOnTouch(IEntity other, int extra) //!EntityEvent.TOUCH
	{
	}
	event private void EOnInit(IEntity other, int extra) //!EntityEvent.INIT
	{
	}
	event private void EOnExtra(IEntity other, int extra)	//!EntityEvent.EXTRA
	{
	}
	event private void EOnNotVisible(IEntity other, int extra) //!EntityEvent.NOTVISIBLE
	{
	}
	event private void EOnFrame(IEntity other, float timeSlice) //!EntityEvent.FRAME
	{
	}
	event private int  EOnVisible(IEntity other, int extra) //!EntityEvent.VISIBLE
	{
	}
	event private void EOnPostFrame(IEntity other, int extra) //!EntityEvent.POSTFRAME
	{
	}
	event private void EOnWorldProcess(IEntity other, int extra) //!EntityEvent.WORLDPROCESS
	{
	}
	event private void EOnAnimEvent(IEntity other, AnimEvent extra) //!EntityEvent.ANIMEVENT
	{
	}
	event private void EOnSoundEvent(IEntity other, SoundEvent extra) //!EntityEvent.SOUNDEVENT
	{
	}
	event private void EOnSimulate(IEntity other, float dt) //!EntityEvent.SIMULATE
	{
	}
	event private void EOnPostSimulate(IEntity other, float timeSlice)	//!EntityEvent.POSTSIMULATE
	{
	}
	event private void EOnJointBreak(IEntity other, int extra) //!EntityEvent.JOINTBREAK
	{
	}
	event private void EOnPhysicsMove(IEntity other, int extra) //!EntityEvent.PHYSICSMOVE
	{
	}
	event private void EOnContact(IEntity other, Contact extra) //!EntityEvent.CONTACT
	{
	}
	private void EOnUser0(IEntity other, int extra) //!EntityEvent.EV_USER+0
	{
	}
	private void EOnUser1(IEntity other, int extra) //!EntityEvent.EV_USER+1
	{
	}
	private void EOnUser2(IEntity other, int extra) //!EntityEvent.EV_USER+2
	{
	}
	private void EOnUser3(IEntity other, int extra) //!EntityEvent.EV_USER+3
	{
	}
	private void EOnUser4(IEntity other, int extra) //!EntityEvent.EV_USER+4
	{
	}
	private void EOnDummy020(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy021(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy022(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy023(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy024(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy025(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy026(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy027(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy028(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy029(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy030(IEntity other, int extra)	//!Placeholder
	{
	}
	private void EOnDummy031(IEntity other, int extra)	//!Placeholder
	{
	}
//@}
//DO NOT INSERT ANYTHING ABOVE - order of event methods matters!

	/** \name Transformation methods
		Setting and getting of entity transformation
	*/
	//@{

	/**
	\brief Returns transformation of Entity. It returns only so much vectors as array is big
		\param mat \p vector[1...4] matrix to be get
		@code
			Man player = g_Game.GetPlayer();
	
			vector mat[4];
			player.GetTransform(mat);
			Print( mat );
	
			>> <0.989879,-0,0.141916>,<0,1,0>,<-0.141916,0,0.989879>,<2545.08,15.6754,2867.49>
		@endcode
	*/
	proto external void GetTransform(out vector mat[]);

	/**
	\brief Returns one row of Entity transformation matrix
		\param axis \p int matrix axis. Can be 0..3
		\return \p vector axis row of Entity matrix
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetTransformAxis(0) );
			Print( player.GetTransformAxis(1) );
			Print( player.GetTransformAxis(2) );
			Print( player.GetTransformAxis(3) );
	
			>> <-0.386781,0,0.922171>
			>> <0,1,0>
			>> <-0.922171,0,-0.386782>
			>> <2551.34,15.6439,2856.72>
		@endcode
	*/
	proto native external vector GetTransformAxis(int axis);

	/**
	\brief Sets entity transformation
		\param mat \p vector[4] matrix to be set
		@code
			vector mat[4];
			Math3D.MatrixIdentity( mat )
	
			Man player = g_Game.GetPlayer();
			player.SetTransform( mat );
	
			vector outmat[4];
			player.GetTransform(outmat );
			Print( outmat );
	
			>> <1,0,0>,<0,1,0>,<0,0,1>,<0,0,0>
		@endcode
	*/
	proto native external void SetTransform(vector mat[4]);

	/**
	\brief Returns origin of Entity
		\return \p vector entity origin
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetOrigin() );
	
			>> <2577.02,15.6837,2924.27>
		@endcode
	*/
	proto native external vector GetOrigin();
	
	/**
	\brief Returns orientation of Entity in world space (Yaw, Pitch, Roll)
		\return \p vector entity orientation
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetYawPitchRoll() );
	
			>> <180,-76.5987,180>
		@endcode
	*/
	proto native external vector GetYawPitchRoll();

	/**
	\brief Same as GetYawPitchRoll, but returns rotation vector around X, Y and Z axis.
	*/
	proto native external vector GetAngles();

	/**
	\brief Returns local orientation when it's in hierarchy (Yaw, Pitch, Roll)
		\return \p vector local orientation
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetLocalYawPitchRoll() );
	
			>> <180,-57.2585,180>
		@endcode
	*/
	proto native external vector GetLocalYawPitchRoll();

	/**
	\brief Same as GetLocalYawPitchRoll, but returns rotation vector around X, Y and Z axis.
	*/
	proto native external vector GetLocalAngles();

	/**
	\brief Sets angles for entity (Yaw, Pitch, Roll)
		\param angles \p vector angles to be set
		@code
			Man player = g_Game.GetPlayer();
			player.SetYawPitchRoll("180 50 180" );
			Print( player.GetYawPitchRoll() );
	
			>> <-180,50,-180>
		@endcode
	*/
	proto native external void SetYawPitchRoll(vector angles);

	/**
	\brief Same as SetYawPitchRoll, but sets rotation around X, Y and Z axis.
	*/
	proto native external void SetAngles(vector angles);
	
	/**
	\brief Sets origin for entity
		\param orig \p vector origin to be set
		@code
			Man player = g_Game.GetPlayer();
			player.SetOrigin("2550 10 2900" );
			Print( player.GetOrigin() );
	
			>> <2550,10,2900>
		@endcode
	*/
	proto native external void SetOrigin(vector orig);

	proto native external float GetScale();
	proto native external void SetScale(float scale);
	/**
	\brief Transforms local vector to world space
	    \param vec \p vector local space vector to transform
		\return \p vector world space vector
		@code
			Man player = g_Game.GetPlayer();
			Print( player.VectorToParent("1 2 3") );

			>> <2.89791,2,1.26575>
		@endcode
	*/
	proto native external vector VectorToParent(vector vec);

	/**
	\brief Transforms local position to world space
		\param coord \p vector local position to transform
		\return \p vector position in world space
		@code
			Man player = g_Game.GetPlayer();
			Print( player.CoordToParent("1 2 3") );

			>> <2549,17.6478,2857>
		@endcode
	*/
	proto native external vector CoordToParent(vector coord);

	/**
	\brief Transforms world space vector to local space
		\param vec \p vector world space vector to transform
		\return \p vector local space vector
		@code
			Man player = g_Game.GetPlayer();
			Print( player.VectorToLocal("2 1 5") );

			>> <-0.166849,1,5.38258>
		@endcode
	*/
	proto native external vector VectorToLocal(vector vec);

	/**
	\brief Transforms world space position to local space
		\param coord \p vector world space position to transform
		\return \p vector position in local space
		@code
			Man player = g_Game.GetPlayer();
			Print( player.CoordToLocal("500 10 155") );

			>> <15254,-54.2004,8745.53>
		@endcode
	*/
	proto native external vector CoordToLocal(vector coord);

	//@}


	/** \name Name/ID methods		
	*/
	//@{

	/**
	\brief Return unique entity ID
		\return \p int unique entity ID
		@code
			ItemBase apple = g_Game.CreateObject( "FruitApple", String2Vector("0 10 0"), false );
			Print( apple.GetID() );

			>> 0
		@endcode
	*/
	proto native int GetID();

	/**
	\brief Set unique entity ID
		\param id \p int unique entity ID to be set
		@code
			ItemBase apple = g_Game.CreateObject( "Fruit_Apple", String2Vector("0 10 0"), false );
			apple.SetID(101);
			Print( apple.GetID() );

			>> 101
		@endcode
	*/
	proto native void SetID(int id);
	
	proto native void SetName(string name);
	proto native external owned string GetName();
	//@}
	

	/** \name Hierarchy methods
		Scene hierarchy management
	*/
	//@{

	//!Add Entity to hierarchy. Pivot is pivot index, or -1 for center of parent.
	proto native external int AddChild(notnull IEntity child, int pivot);
	//!Remove Entity from hierarchy
	proto native external void RemoveChild(notnull IEntity child);

	//!Returns pointer to parent Entity in hierarchy
	proto native IEntity GetParent();
	//!Returns pointer to first child Entity in hierarchy
	proto native IEntity GetChildren();
	//!Returns pointer to next child Entity on the same hierarchy
	proto native IEntity GetSibling();
	//@}

	/**
	\brief Returns local bounding box of model on Entity
		\param[out] mins \p vector minimum point of bounding box
		\param[out] maxs \p vector maximum point of bounding box
		@code
			Man player = g_Game.GetPlayer();
	
			vector mins, maxs;
			player.GetBounds(mins, maxs );
	
			Print( mins );
			Print( maxs );
	
			>> <0,0,0>
			>> <0,0,0>
		@endcode
	*/
	proto external void GetBounds(out vector mins, out vector maxs);
	
	/**
	\brief Returns quantized world-bound-box of Entity
		\param[out] mins \p vector minimum point of bounding box
		\param[out] maxs \p vector maximum point of bounding box
		@code
			Man player = g_Game.GetPlayer();
	
			vector mins, maxs;
			player.GetWorldBounds( mins, maxs );
	
			Print( mins );
			Print( maxs );
	
			>> <2547.2,15.5478,2852.85>
			>> <2548.8,17.5478,2855.05>
		@endcode
	*/
	proto external void GetWorldBounds(out vector mins, out vector maxs);
	
	/** \name Simulation/handling properties
	Flags that affects simulation and entity handling behavior
	*/
	//@{
	/**
	\brief Returns Entity flags
		\return \p EntityFlags entity flags
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetFlags() );
	
			>> 1610612745
		@endcode
	*/
	proto native external EntityFlags GetFlags();

	/**
	\brief Sets Entity flags. It's OR operation, not rewrite. Returns previous flags
		\param flags \p int flags to be set
		\param recursively flags will be recursively applied to children of hierarchy too
		\return \p int previous flags
		@code
			Man player = g_Game.GetPlayer();
			player.SetFlags(EntityFlags.VISIBLE|EntityFlags.SOLID );
			Print( player.GetFlags() );
	
			>> 1610612747
		@endcode
	*/
	proto native external EntityFlags SetFlags(EntityFlags flags, bool recursively);

	/**
	\brief Clear Entity flags. Returns cleared flags
		\param flags \p int flags to be set
		\param recursively flags will be recursively applied to children of hierarchy too
		\return \p int cleared flags
		@code
			Man player = g_Game.GetPlayer();
			player.ClearFlags(EntityFlags.VISIBLE|EntityFlags.SOLID );
			Print( player.GetFlags() );
	
			>> 1610612744
		@endcode
	*/
	proto native external EntityFlags ClearFlags(EntityFlags flags, bool recursively);


	/**
	\brief Returns current event mask
		\return \p int current event mask
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetEventMask() );
	
			>> 0
		@endcode
	*/
	proto native external EntityEvent GetEventMask();

	/**
	\brief Sets event mask
		\param e combined mask of one or more members of EntityEvent enum
		@code
			Man player = g_Game.GetPlayer();
			Print( player.GetEventMask() );
			player.SetEventMask( EntityEvent.VISIBLE );
			Print( player.GetEventMask() );
	
			>> 0
			>> 128
		@endcode
	*/
	proto native external EntityEvent SetEventMask(EntityEvent e );

	/**
	\brief Clears event mask
		\param e \p int event mask
		\return \p int event mask
		@code
			Man player = g_Game.GetPlayer();
			player.SetEventMask(EntityEvent.VISIBLE );
			Print( player.GetEventMask() );
			player.ClearEventMask(EntityEvent.ALL );
			Print( player.GetEventMask() );
	
			>> 128
			>> 0
		@endcode
	*/
	proto native external EntityEvent ClearEventMask(EntityEvent e);
	
	//!Dynamic event invokation. Parameters are the same as in IEntity::EOnXXXX() methods
	proto external volatile void SendEvent(notnull IEntity actor, EntityEvent e, void extra);

//@}

/** \name Visual component methods
 Manipulation with visual component - model, particle effect etc			
*/
//@{

	/**
	\brief Sets the visual object to this entity. Reference is added and released upon entity destruction
	\param object handle to object got by GetObject()
	\param options String, dependant on object type.
	//Only supported one for XOB objects:
	//$remap 'original material name' 'new material'; [$remap 'another original material name' 'anothernew material']
	*/
	proto native external void SetObject(vobject object, string options);
	
	/**
	\brief Returns visual object set to this Entity. No reference is added
	*/
	proto native vobject GetVObject();

	//!Updates animation (either xob, or particle, whatever)
	proto native external int Animate(float speed, int loop);
	//!Updates animation (either xob, or particle, whatever)
	proto native external int AnimateEx(float speed, int loop, out vector lin, out vector ang);

	//!Sets visibility mask for cameras, where Entity will be rendered
	proto native external int SetCameraMask(int mask);	
//@}

	/**
	\brief When called, the Entity is excluded from consequent TraceMove/TraceLine
	*/
	proto native external void FilterNextTrace();

	/*!
	Updates entity state/position. Should be called when you want to manually commit position changes etc
	before trace methods etc. Entity is updated automatically at the end and the beginning of simulation step,
	when it has EntityFlags.TFL_ACTIVE flag set.
	\returns mask with flags
	//	EntityFlags.UPDATE - hierarchy has been updated
	//	EntityFlags.UPDATE_MDL - model hierarchy has been updated
	*/
	proto native external int Update();
		
#ifdef COMPONENT_SYSTEM		
	//! Constructor
	protected void IEntity(IEntitySource src, IEntity parent);	
#endif		
};

#ifdef ENF_DONE
// Set fixed LOD. -1 for non-fixed LOD
proto native void SetFixedLOD(IEntity ent, int lod);
//Sets the texture that can be referenced from material as $renderview
//and connects it with camera cam_index. Size iz recommended size of
//rendertarget (0 is default)
proto native void SetRenderView(IEntity ent, int cam_index, int width, int height);
proto void GetRenderView(IEntity ent, out int cam_index, out int width, out int height);
#endif


/**
 * \defgroup EntityAttributes Entity editor attribute system
 * @{
 */

class ParamEnum: Managed
{
	string m_Key;
	string m_Value;
	string m_Desc;
	
	void ParamEnum(string key, string value, string desc = "")
	{
		m_Key = key;
		m_Value = value;
		m_Desc = desc;
	}
}

class ParamEnumArray: array<ref ParamEnum>
{
	static ParamEnumArray FromEnum(typename e)
	{
		ParamEnumArray params = new ParamEnumArray();	
		int cnt = e.GetVariableCount();
		int val;

		for (int i = 0; i < cnt; i++)
		{
			if (e.GetVariableType(i) == int && e.GetVariableValue(NULL, i, val))
			{
				params.Insert(new ParamEnum(e.GetVariableName(i), val.ToString()));
			}
		}
				
		return params;
	}	
}

// -------------------------------------------------------------------------
class Attribute
{
	string m_DefValue;
	string m_UiWidget; ///< can be "editbox", "combobox", "spinbox", "slider", "font", "fileeditbox", "colorPicker", "flags", "resourceNamePicker"
	string m_RangeScale; ///< defined as "MIN_VALUE MAX_VALUE STEP" eg. "1 100 0.5"
	string m_Desc;
	ref ParamEnumArray m_Enums; ///< Only ints and floats are currently supported. Array can be defined this way: { ParamEnum("Choice 1", "1"), ParamEnum("Choicen 2", "2") }
	
	void Attribute(string defvalue, string uiwidget, string desc = "", string rangescale = "", ParamEnumArray enums = NULL)
	{
		m_DefValue = defvalue;
		m_UiWidget = uiwidget;
		m_RangeScale = rangescale;
		m_Desc = desc;
		m_Enums = enums;
	}
}

class EditorAttribute
{
	string m_Style; ///< can be "box", "sphere", "cylinder", "pyramid", "diamond" or custom style name
	string m_Category; ///< folder structure eg. StaticEntities/Walls
	string m_Description; ///< class purpose description
	vector m_SizeMin; ///< min vector of a bounding box
	vector m_SizeMax; ///< max vector of a bounding box
	string m_Color;
	string m_Color2;
	bool m_Visible;
	bool m_Insertable;
	bool m_DynamicBox;
	
	void EditorAttribute(string style, string category, string description, vector sizeMin, vector sizeMax, string color, string color2 = "0 0 0 0", bool visible = true, bool insertable = true, bool dynamicBox = false)
	{
		m_Style = style;
		m_Category = category;
		m_Description = description;
		m_SizeMin = sizeMin;
		m_SizeMax = sizeMax;
		m_Color = color;
		m_Color2 = color2;
		m_Visible = visible;
		m_Insertable = insertable;
		m_DynamicBox = dynamicBox;
	}
}
//@}

//@}
