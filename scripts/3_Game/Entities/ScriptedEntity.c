class ScriptedEntity extends EntityAI
{
	/**
	\brief Sets collision box for object
	@param mins min values of box
	@param maxs max values of box
	\note This function is obsolete, use rather SetCollisionBox()
	*/
	proto native void SetClippingInfo(vector mins, vector maxs, float radius);
	
	/**
	\brief Sets collision box for object
	@param mins min values of box
	@param maxs max values of box
	\n usage :
	@code
	vector mins = "-1 -1 -1";
	vector maxs = "1 1 1";
	SetCollisionBox(mins, maxs);
	@endcode
	*/
	proto native void SetCollisionBox(vector mins, vector maxs);
};