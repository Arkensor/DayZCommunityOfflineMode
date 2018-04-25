class Entity extends ObjectTyped
{
	//! Return animation phase of animation on object.
	proto native float GetAnimationPhase(string animation);
	
	//! Process animation on object. Animation is defined in config file. Wanted animation phase is set to phase.
	proto native void SetAnimationPhase(string animation, float phase);

	//! Returns skeleton's bone index of named proxy selection.
	proto native int GetBoneIndex( string proxySelectionName );

	//! Returns proxy object that corresponds given bone inside skeleton.
	proto native Entity GetBoneObject( int boneIndex );
	
	//! Turns on/off invisibility
	proto native void SetInvisible(bool invisible);
};
