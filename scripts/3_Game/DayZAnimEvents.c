/*enum AnimSurfaces
{
	cp_concrete1,
	cp_concrete2,

	cp_dirt,

	cp_broadleaf_dense1,
	cp_broadleaf_dense2,
	cp_broadleaf_sparse1,
	cp_broadleaf_sparse2,

	cp_conifer_common1,
	cp_conifer_common2,
	cp_conifer_moss1,
	cp_conifer_moss2,

	cp_grass,
	cp_grass_tall,

	cp_gravel,
	cp_rock,

	asphalt_ext,
	asphalt_int,

	asphalt_destroyed_ext,
	asphalt_destroyed_int,

	concrete_ext,
	concrete_int,

	stone_ext,
	stone_int,

	gravel_large_ext,
	gravel_large_int,

	gravel_small_ext,
	gravel_small_int,

	sand_ext,
	sand_int,

	dirt_ext,
	dirt_int,

	rubble_large_ext,
	rubble_large_int,

	rubble_small_ext,
	rubble_small_int,

	trash_ext,
	trash_int,

	grass_dry_ext,
	grass_dry_int,

	metal_thick_ext,
	metal_thick_int,

	metal_thin_ext,
	metal_thin_int,

	metal_thin_mesh_ext,
	metal_thin_mesh_int,

	asphalt_felt_ext,
	asphalt_felt_int,

	ceramic_tiles_ext,
	ceramic_tiles_int,

	lino_ext,
	lino_int,

	textile_carpet_ext,
	textile_carpet_int,

	wood_parquet_ext,
	wood_parquet_int,

	wood_planks_ext,
	wood_planks_int,

	concrete_stairs_ext,
	concrete_stairs_int,

	metal_stairs_ext,
	metal_stairs_int,

	wood_planks_stairs_ext,
	wood_planks_stairs_int
}*/

enum AnimBootsType
{
	None,
	Sneakers,
	Boots
}

enum AnimUpperBodyType
{
	None 				= 4107064,//string hash, because we can't make compile time constant
	NylonJacket			= 1228024514,
	TShirt				= 1690896446,
	WoolShirt			= 1060939383,
	HeavyJacket			= 1544363355,
	LeatherJacket		= 2008520095,
	Coat				= 3549415,
	ChemlonDress		= -1491825621,
	Ghillie				= 602194810
}

enum AnimRangedWeaponType
{
	None 		= 5727960,
	Rifle 		= 219116654,
	Shotgun 	= 1836650908
}




class AnimSoundEvent
{
	int m_iID;
	ref SoundObjectBuilder m_SoundObjectBuilder;
	ref SoundParams m_SoundParams;
	autoptr NoiseParams m_NoiseParams;

	void AnimSoundEvent(string soundPath)
	{
		m_iID = GetGame().ConfigGetInt(soundPath + "id");

		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			string soundSetName;
			GetGame().ConfigGetText(soundPath + "soundSet", soundSetName);
			m_SoundParams = new SoundParams(soundSetName);
			m_SoundObjectBuilder = new SoundObjectBuilder(m_SoundParams);
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			string noiseName;
			if(GetGame().ConfigGetText(soundPath + "noise", noiseName))
			{
				m_NoiseParams = new NoiseParams();
				m_NoiseParams.Load(noiseName);
			}
			else
			{
				//Print("AnimSoundEvent: \"" + soundPath + "\" doesn't have defined \"noise\"");
			}
		}
	}
	
	SoundObjectBuilder GetSoundBuilder()
	{
		return m_SoundObjectBuilder;
	}

	SoundObject GetSoundObject(vector position)
	{
		m_SoundObjectBuilder.UpdateEnvSoundControllers(position);
		return m_SoundObjectBuilder.BuildSoundObject();
	}
}

class AnimStepEvent
{
	int m_iID;
	string m_sSoundLookupTableName;
	StepSoundLookupTable m_soundLookupTable;
	autoptr NoiseParams m_NoiseParams;
	//autoptr array<ref AnimEffectParams> m_EffectsParams;

	void AnimStepEvent(string stepPath)
	{
		m_iID = GetGame().ConfigGetInt(stepPath + "id");

		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			GetGame().ConfigGetText(stepPath + "soundLookupTable", m_sSoundLookupTableName);
			m_soundLookupTable = AnimSoundLookupTableBank.GetInstance().GetStepTable(m_sSoundLookupTableName);
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			string noiseName;
			if(GetGame().ConfigGetText(stepPath + "noise",noiseName))
			{
				m_NoiseParams = new NoiseParams();
				m_NoiseParams.Load(noiseName);
			}
		}

		//autoptr array<string> effectNames = new array<string>;
		//m_EffectsParams = new array<ref AnimEffectParams>;
		//GetGame().ConfigGetTextArray(stepPath + "effects", effectNames);
	}
	
	SoundObjectBuilder GetSoundBuilder(int surfaceHash)
	{
		return m_soundLookupTable.GetSoundBuilder(surfaceHash);
	}
}

class AnimDamageEvent
{
	int m_iID;
	autoptr AnimDamageParams m_DamageParams;

	void AnimDamageEvent(string damagePath)
	{
		m_iID = GetGame().ConfigGetInt(damagePath + "id");
		
		string damageName;
		GetGame().ConfigGetText(damagePath + "damage", damageName);
		m_DamageParams = new AnimDamageParams(damageName);
	}
}

class AnimDamageParams
{
	string m_sName;
	string m_sBoneName;
	string m_sAmmoName;	
	float m_fRadius;
	float m_fDuration;

	static const string DAMAGE_CFG_CLASS = "CfgDamages ";
	void AnimDamageParams(string damageName)
	{
		m_sName = damageName;
		string damagePath = DAMAGE_CFG_CLASS + damageName + " ";

		GetGame().ConfigGetText(damagePath + "bone", m_sBoneName);
		GetGame().ConfigGetText(damagePath + "ammo", m_sAmmoName);
		m_fRadius = GetGame().ConfigGetFloat(damagePath + "radius");
		m_fDuration = GetGame().ConfigGetFloat(damagePath + "duration");
	}
}

class AnimEffectParams
{
	string m_sName;

	static const string EFFECT_CFG_CLASS = "CfgEffects ";
	void AnimEffectParams(string effectName)
	{
		m_sName = effectName;
		string effectPath = EFFECT_CFG_CLASS + effectName + " ";
		//TODO load params
	}
}
