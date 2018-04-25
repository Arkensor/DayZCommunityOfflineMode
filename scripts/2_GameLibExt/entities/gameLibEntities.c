//Generic entities from GameLib (script side of c++ classes)

#ifdef COMPONENT_SYSTEM
class GenericEntity extends IEntity
{
	//native method implemented on c++ side
	proto native void Show(bool show);
	
	/*!
	Finds first occurance of the coresponding component.
	\param typeName type of the component
	*/
	proto native GenericComponent FindComponent(typename typeName);
	
	/*!
	inserts instance of the script component to the entity.
	Calls OnComponentInsert on all entity's components to inform that this new component was inserted. The new instance have to FindScriptComponents in order to work with them.
	Calls EOnInit on the component if component sets EV_INIT mask.
	Calls EOnActivate on the component if the component is active (default)
	\param component instance
	*/
	proto native void InsertComponent(GenericComponent component);
	
	/*!
	Removes component from entity. Doesn't delete the entity.
	Calls EOnDeactivate on the component.
	Calls OnComponentRemove on all entity's components to inform that this new component was removed.
	\param component instance
	*/
	proto native void RemoveComponent(GenericComponent component);
	
	/*!
	Removes and deletes component from entity.
	Calls EOnDeactivate on the component.
	Calls OnComponentRemove on all entity's components to inform that this new component was removed.
	Calls OnDelete on the component.
	\param component instance
	*/
	proto native void DeleteComponent(GenericComponent component);

#ifdef WORKBENCH
	/*!
	Called after updating world in Workbench. The entity must be selected.
	*/
	void _WB_AfterWorldUpdate(float timeSlice) {};
#endif
}

class GenericWorldEntity extends GenericEntity
{
}

class GenericTerrainEntity extends GenericEntity
{
}

class LightEntity extends GenericEntity
{
	/*!
	Sets diffuse part of light.
	\param color Color of light
	*/
	proto native external void SetDiffuseColor(int color);
	proto native external int GetDiffuseColor();
	
	proto native external void SetRadius(float radius);
	proto native external float GetRadius();
	
	/*!
	Sets cone of light. It's meaningful for LT_SPOT only!
	\param angle
	*/
	proto native external void SetConeAngle(float angle);
	/*!
	Gets cone of light. It's meaningful for LT_SPOT only!
	\return Cone of light
	*/
	proto native external float GetConeAngle();
	
	proto native external void SetCastShadow(bool enable);
	proto native external bool IsCastShadow(bool enable);
}

class GenericWorldLightEntity extends GenericEntity
{
}

class GenericWorldFogEntity extends GenericEntity
{
}

class BasicEntity extends GenericEntity
{
}

class WorldEntityClass
{
}

class WorldEntity extends GenericWorldEntity
{
}

class ModelEntity extends BasicEntity
{
}

enum CharacterMovement
{
	MOVEMENTTYPE_IDLE,
	MOVEMENTTYPE_WALK,
	MOVEMENTTYPE_RUN,
	MOVEMENTTYPE_SPRINT
};

enum CharacterStance
{
	STANCE_ERECT,
	STANCE_CROUCH,
	STANCE_PRONE,
	STANCE_ERECT_RAISED,
	STANCE_CROUCH_RAISED,
	STANCE_PRONE_RAISED
};

class CharacterEntity extends BasicEntity
{
	proto native void Teleport(vector transform[4]);
	proto native CharacterMovement GetCurrentMovement();
	proto native CharacterStance GetCurrentStance();
}

class BasicCamera extends BasicEntity
{
}

class VRHandEntity extends GenericEntity
{
}

#endif