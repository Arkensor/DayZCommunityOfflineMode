/*
	Author: Boris Vacula
	For documentation go to: DayZ Confluence -> How-to articles -> Weapon muzzle flash particle system configuration
	This system plays effect(s) on any weapon that is fired/jammed/ruined/...
*/

class WeaponParticlesBase
{
	bool			m_IlluminateWorld;
	bool			m_IgnoreIfSuppressed;
	int 			m_OverrideParticle;
	int 			m_OnlyWithinHealthLabelMin;
	int 			m_OnlyWithinHealthLabelMax;
	string 			m_OverrideDirectionPoint;
	string 			m_OnlyIfBulletIs;
	string 			m_OnlyIfWeaponIs;
	string 			m_OverridePoint;
	vector 			m_OverrideDirectionVector;
	vector 			m_PositionOffset;
	
	//======================================
	//  		PRELOAD EVERYTHING
	//======================================
	
	void WeaponParticlesBase(ItemBase muzzle_owner, string config_OnFire_entry)
	{
		// ignoreIfSuppressed
		m_IgnoreIfSuppressed = GetGame().ConfigGetFloat(config_OnFire_entry + " " + "ignoreIfSuppressed");
		
		// illuminateWorld
		m_IlluminateWorld = GetGame().ConfigGetFloat(config_OnFire_entry + " " + "illuminateWorld");
		
		// onlyIfWeaponIs
		m_OnlyIfWeaponIs = "";
		GetGame().ConfigGetText( config_OnFire_entry + " onlyIfWeaponIs", m_OnlyIfWeaponIs);
		
		// onlyIfBulletIs
		m_OnlyIfBulletIs = "";
		GetGame().ConfigGetText( config_OnFire_entry + " onlyIfBulletIs", m_OnlyIfBulletIs);
		
		// onlyWithinHealthLabel[]
		array<float> health_limit = new array<float>;
		GetGame().ConfigGetFloatArray( config_OnFire_entry + " onlyWithinHealthLabel", health_limit);
		
		if (health_limit.Count() == 2)
		{
			m_OnlyWithinHealthLabelMin = health_limit.Get(0);
			m_OnlyWithinHealthLabelMax = health_limit.Get(1);
		}
		else
		{
			m_OnlyWithinHealthLabelMin = -1;
			m_OnlyWithinHealthLabelMax = -1;
		}
		
		// overridePoint
		m_OverridePoint = "";
		GetGame().ConfigGetText(config_OnFire_entry + " " + "overridePoint", m_OverridePoint);
		
		if (m_OverridePoint == "")
			m_OverridePoint = "Usti hlavne"; // default memory point name
		
		// overrideParticle
		string particle_name = "";
		GetGame().ConfigGetText( config_OnFire_entry + " overrideParticle" , particle_name);
		
		if (particle_name != "")
		{
			string path = ParticleList.GetPathToParticles();
			m_OverrideParticle = ParticleList.GetParticleID(path + particle_name);
		}
		else
		{
			m_OverrideParticle = -1;
		}
		
		// overrideDirectionPoint
		m_OverrideDirectionPoint = "";
		GetGame().ConfigGetText( config_OnFire_entry + " overrideDirectionPoint" , m_OverrideDirectionPoint);
		
		if (m_OverrideDirectionPoint == "")
		{
			// overrideDirectionVector
			vector test_ori = GetGame().ConfigGetVector( config_OnFire_entry + " overrideDirectionVector");
			
			if (test_ori != Vector(0, 0, 0))
			{
				m_OverrideDirectionVector = test_ori;
			}
		}
		
		// positionOffset[]
		array<float> v = new array<float>;
		GetGame().ConfigGetFloatArray( config_OnFire_entry + " positionOffset" , v);
		
		if (v.Count() == 3)
		{
			float v1 = v.Get(0);
			float v2 = v.Get(1);
			float v3 = v.Get(2);
			m_PositionOffset = Vector(v1, v2, v3);
		}
	}
	
	
	
	//======================================
	//			  PLAY PARTICLES
	//======================================
	
	void OnActivate(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			// Handle effect's parameters
			if ( PrtTest.m_GunParticlesState ) // Check if particles are enambled by debug
			{
				if ( !suppressor  ||  !(m_IgnoreIfSuppressed) ) // ignoreIfSuppressed
				{
					if ( CheckHealthCondition( muzzle_owner.GetHealthLabel() ) ) // onlyWithinHealthLabel
					{
						if ( m_OnlyIfBulletIs == ""  ||  m_OnlyIfBulletIs == ammoType ) // onlyIfBulletIs
						{
							if ( m_OnlyIfWeaponIs == ""  ||  m_OnlyIfWeaponIs == weapon.GetType() ) // onlyIfWeaponIs
							{
								// Get particle ID
								int particle_id = CheckParticleOverride(ammoType);
								
								// Get position of the particle
								vector local_pos = muzzle_owner.GetSelectionPosition(m_OverridePoint);
								local_pos += m_PositionOffset;
								
								// Set orientation of the particle
								vector particle_ori = CheckOrientationOverride(local_pos, muzzle_owner);
								
								Particle p = Particle.Play( particle_id, muzzle_owner, local_pos, particle_ori );
								
								Weapon_Base weapon_WB = Weapon_Base.Cast(weapon);
								OnParticleCreated(weapon_WB, p);
								
								// HACK: This temporarily fixes DAYZ-30268 until New Hierarchy is implemented + propper Lights API is developed
								if (m_IlluminateWorld)
								{
									vector global_pos = muzzle_owner.ModelToWorld(local_pos + Vector(-0.2, 0, 0));
									Object light = g_Game.CreateObject( "Light", global_pos, true );
								}
							}
						}
					}
				}
			}
		}
	}
	
	void OnParticleCreated(Weapon_Base weapon, Particle p)
	{
		
	}
	
	void OnDeactivate(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			Weapon_Base weapon_WB = Weapon_Base.Cast(weapon);
			weapon_WB.KillAllOverheatingParticles();
		}
	}
	
	
	//==============================================
	//  		 HANDLE CONFIG PARAMETERS          
	//==============================================
	
	
	// OnlyWithinHealthLabelMin & OnlyWithinHealthLabelMax
	bool CheckHealthCondition(int health_label)
	{
		if (m_OnlyWithinHealthLabelMin != -1  ||  m_OnlyWithinHealthLabelMax != -1) // Check if health condition was even defined
		{
			if ( (health_label >= m_OnlyWithinHealthLabelMin)  &&  (health_label <= m_OnlyWithinHealthLabelMax) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else // health condition was not defined so ignore it.
		{
			return true; 
		}
	}
	
	// muzzleFlashParticle
	int CheckParticleOverride(string ammoType)
	{
		int particle_id = -1;
		
		if (m_OverrideParticle == -1)
		{
			string particle_file = "";
			string cfg_path = "CfgAmmo " + ammoType + " muzzleFlashParticle";
			
			// Config is accessed only once because the data is saved into a map for repeated access.
			GetGame().ConfigGetText( cfg_path, particle_file);
			
			if (particle_file == "")
			{
				string ammo_particle_error = "ERROR! Cannot spawn particle effect because item " + ammoType + " is missing config parameter muzzleFlashParticle!";
				DPrint(ammo_particle_error);
			}
			else
			{
				particle_id = ParticleList.GetParticleID( ParticleList.GetPathToParticles() + particle_file);
				m_OverrideParticle = particle_id;
			}
		}
		else
		{
			particle_id = m_OverrideParticle;
		}
		
		return particle_id;
	}
	
	// OverrideDirectionPoint & OverrideDirectionVector
	vector CheckOrientationOverride(vector local_pos, ItemBase muzzle_owner)
	{
		vector particle_ori = "0 0 0";
		if (m_OverrideDirectionPoint != "")
		{
			vector target_pos = muzzle_owner.GetSelectionPosition(m_OverrideDirectionPoint);
			target_pos = vector.Direction(local_pos, target_pos);
			particle_ori = target_pos.VectorToAngles();
		}
		else
		{
			if (m_OverrideDirectionVector != Vector(0, 0, 0))
			{
				particle_ori = m_OverrideDirectionVector;
			}
		}
		
		return particle_ori;
	}
}

// FIRE particles
class WeaponParticlesOnFire : WeaponParticlesBase {}

// OVERHEATING particles
class WeaponParticlesOnOverheating: WeaponParticlesBase 
{
	override void OnParticleCreated(Weapon_Base weapon, Particle p)
	{
		weapon.RegisterOverheatingParticle(p);
	}
}


//class WeaponParticlesOnJammed : WeaponParticlesBase {} // MAYBE TO DO: Create functionality that would use this
