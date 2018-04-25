/**
 * \defgroup Visual Visual objects
 * @{
 */

//!Loads object from data, or gets it from cache. Object must be released when not used
proto native vobject GetObject(string name);

/*!
Release object. When there are not any other references, object is stored into cache and ready to be victed if necessary.
\param object Object handle
\param flag If RF_RELEASE is used, the object is evicted immediatelly, if not used by anyone else
*/
proto native void ReleaseObject(vobject object, int flag = 0);

//! Returns number of frames, if the object is animation
proto native int GetNumAnimFrames(vobject anim);

//! Returns name of visual object
proto string vtoa(vobject vobj);

/**
 * \defgroup MeshObject Mesh object (XOB)
 * @{
 */

proto int GetObjectMaterials(vobject object, string materials[]);

// dynamic model creation (for dynamic aabb triggers)
//proto void CreateModel(IEntity ent, vector mins, vector maxs);
//proto void RemoveModel(IEntity ent);

//! Dynamic MeshObject
proto vobject CreateXOB(int nsurfaces, int nverts[], int numindices[], string materials[]);
proto void UpdateVertsEx(notnull IEntity ent, int surf, vector verts[], float uv[]);
proto void UpdateIndices(vobject obj, int surf, int indices[]);

proto native void	SetBone(notnull IEntity ent, int bone, vector angles, vector trans, float scale);
proto native bool	SetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native void	SetBoneGlobal(notnull IEntity ent, int bone, vector mat[4]);
proto native bool	GetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native bool	GetBoneLocalMatrix(notnull IEntity ent, int bone, vector mat[4]);

proto native void	SetAnimFrame(notnull IEntity ent, int slot, float frame);

// BoneMask == NULL means that all bits are set
class BoneMask
{
	int Mask[8]
}

enum AnimFlags
{
	//! animation is played only once and then if freezes at the last frame. EntityEvent.ANIMEND is called
	ONCE,
/*! defaultne zustava animace pri prehravani a AF_ONCE po skonceni na posledni frame "zamrzla", dokud neni nahrazena jinou,
 nebo neni kanal vynulovan. Pokud se nastavi AF_BLENDOUT, postara se engine o vyhozeni animace sam a pro preblendovani pouzije hodnotu
 blendout. Pokud je odchycen EOnAnimEnd a byla zmenena animace na jinou, nebo byl kanal rucne vynulovan, tak se tato
 funkcnost neprovede. */
	BLENDOUT,

//! animation waits on the first frame. Frame is set by calling SetFrame()
	USER,

//! forces animation to start from begining (including blending)
	RESET,

/*! defaul framerate is from anim.def. If it is missing, parametr fps applies
It is possible to use fps parameter even when the framerate is defined in the anim.def, is using this flag
*/
	FORCEFPS,

//! EntityEvent.ANIMEND will not be called
	NOANIMEND,

//! Animhooks will not be called
	NOANIMHOOKS
};

proto native void	SetAnimSlot(notnull IEntity ent, int slot, vobject anim, float blendin, float blendout, BoneMask mask, int fps, AnimFlags flags);

// changes a mask and does the blending if the blendin is set
proto native void ChangeAnimSlotMask(notnull IEntity ent, int slot, float blendin, BoneMask mask);

// changes animation framerate to fps param
proto native void ChangeAnimSlotFPS(notnull IEntity ent, int slot, int fps);

// set mask for a channel. There are 12 chanels and mask is using first for bits 0..3
proto native void	SetAnimMask(notnull IEntity ent, int mask);

// clears chanal mask, return bits which were cleared
proto native int	ClearAnimMask(notnull IEntity ent, int mask);

//vrati nastavene bity tech kanalu, ktere maji nastavenou animaci, nejsou na konci
// a pro ktere byla nastavena vstupni maska.
//Je tim mozno se dotazat na stav vice slotu najednou
proto native int IsAnimSlotPlaying(notnull IEntity ent, int mask);
//mask - 16bitu, pro 16 anim slotu. Maximalni hodnota je tedy 0xffff!

//sets how much this morph affect object
proto native bool SetMorphState(notnull IEntity ent, string morph, float value);
//morph name
//value 0...1
//@}


/**
 * \defgroup ParticleEffect Particle effect API
 * @{
 */

enum EmitorParam
{
	//! Vector3 R/W
	CONEANGLE,
	
	//! Vector3 R/W
	EMITOFFSET,
	
	//! Float R/W
	VELOCITY,
	
	//! Float R/W
	VELOCITY_RND,
	
	//! Float R/W
	AVELOCITY,
	
	//! Float R/W
	SIZE,
	
	//! Float R/W
	STRETCH,

	//! begin with random rotation. Bool R/W
	RANDOM_ANGLE,
	
	//! rotate in random direction. Bool R/W
	RANDOM_ROT,

	//! Float R/W
	AIR_RESISTANCE,
	
	//! Float R/W
	AIR_RESISTANCE_RND,

	//! Float R/W
	GRAVITY_SCALE,

	//! Float R/W
	GRAVITY_SCALE_RND,
	
	//! Float R/W
	BIRTH_RATE,

	//! Float R/W
	BIRTH_RATE_RND,

	//! Float R/W
	LIFETIME,

	//! Float R/W
	LIFETIME_RND,

	//! Bool R/W
	LIFETIME_BY_ANIM,

	//! Bool R/W
	ANIM_ONCE,

	//! Bool R/W
	RAND_FRAME,

	//! efector's total time. Float R/W
	EFFECT_TIME,

	//! should efector repeate after time up? Bool R/W
	REPEAT,

	//! current efector's time. Float R/W
	CURRENT_TIME,

	//! number of active particles. Int R
	ACTIVE_PARTICLES,

	//! Bool R/W
	SORT,

	//! Bool R/W
	WIND,

	//! Float R/W
	SPRING
};

// return total count of active particles in all emitors
proto native int GetParticleCount(notnull IEntity ent);

// gets name of defined emitors in a particle effect
// return number of emitors and their names in an array (max defines max. emitors to return)
proto int GetParticleEmitors(notnull IEntity ent, out string emitors[], int max);

// sets a parametr of the particle emitor
// if the emitor == - 1, it sets the parameter for all emitors
proto void SetParticleParm(notnull IEntity ent, int emitor, EmitorParam parameter, void value);

//gets parameter of particle emitor
proto void GetParticleParm(notnull IEntity ent, int emitor, EmitorParam parameter, out void value);

//gets original parameter of particle emitor
proto void GetParticleParmOriginal(notnull IEntity ent, int emitor, EmitorParam parameter, out void value);

//Force-changes particle emitor position to the current one.
//Used for sudden changes of particle position to avoid spreading emitted
//particles between previous and the new one position
proto native void ResetParticlePosition(notnull IEntity ent);

//Restart particle effect to its default state. This means no 
//particles, timer reset and so on. Usefull for implementing
//particle cache.
proto native void RestartParticle(notnull IEntity ent);
//@}


//@}
