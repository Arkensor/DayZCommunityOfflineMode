// Register all particles below!

class ParticleList
{
	ref static map<int, string> m_ParticlePaths;
	static int m_lastID = 0;
	
	// REGISTER ALL PARTICLES BELOW:
	
	static const int PARTICLE_TEST					= RegisterParticle("smoke_small_generator_01");
	
	// FIREPLACE
	// Normal fireplace
	static const int CAMP_FIRE_START				= RegisterParticle("fire_small_camp_01_start");
	static const int CAMP_SMALL_FIRE 				= RegisterParticle("fire_small_camp_01");
	static const int CAMP_NORMAL_FIRE				= RegisterParticle("fire_medium_camp_01");
	static const int CAMP_SMALL_SMOKE 				= RegisterParticle("smoke_small_camp_01");
	static const int CAMP_NORMAL_SMOKE				= RegisterParticle("smoke_medium_camp_01");
	static const int CAMP_FIRE_END					= RegisterParticle("fire_small_camp_01_end");
	static const int CAMP_STEAM_2END				= RegisterParticle("steam_medium_camp_2end");
	// Fireplace indoor
	static const int HOUSE_FIRE_START				= RegisterParticle("fire_small_house_01_start");
	static const int HOUSE_SMALL_FIRE 				= RegisterParticle("fire_small_house_01");
	static const int HOUSE_SMALL_SMOKE 				= RegisterParticle("smoke_small_house_01");
	static const int HOUSE_NORMAL_FIRE				= RegisterParticle("fire_medium_house_01");
	static const int HOUSE_NORMAL_SMOKE				= RegisterParticle("smoke_medium_house_01");
	static const int HOUSE_FIRE_END					= RegisterParticle("fire_small_house_01_end");
	static const int HOUSE_FIRE_STEAM_2END			= RegisterParticle("steam_medium_house_2end");
	// Fireplace in barrel with holes
	static const int BARREL_FIRE_START				= RegisterParticle("fire_small_barrel_01_start");
	static const int BARREL_SMALL_FIRE 				= RegisterParticle("fire_small_barrel_01");
	static const int BARREL_SMALL_SMOKE 			= RegisterParticle("smoke_small_barrel_01");
	static const int BARREL_NORMAL_FIRE				= RegisterParticle("fire_medium_barel_01");
	static const int BARREL_NORMAL_SMOKE			= RegisterParticle("smoke_medium_barrel_01");
	static const int BARREL_FIRE_END				= RegisterParticle("fire_small_barrel_01_end");
	static const int BARREL_FIRE_STEAM_2END			= RegisterParticle("steam_medium_camp_2end");
	
	// COOKING
	static const int BOILING_WATER_STEAM			= RegisterParticle("steam_small_cook_01");
	
	// TORCH
	static const int TORCH							= RegisterParticle("fire_small_torch_01");
	
	// ROADFLARE
	static const int ROADFLARE_BURNING_INIT			= RegisterParticle("fire_small_roadflare_red_04");
	static const int ROADFLARE_BURNING_MAIN			= RegisterParticle("fire_small_roadflare_red_01");
	static const int ROADFLARE_BURNING_ENDING		= RegisterParticle("fire_small_roadflare_red_02");
	static const int ROADFLARE_BURNING_SMOKE		= RegisterParticle("fire_small_roadflare_red_03");
	
	
	// SMOKE GRENADES
	static const int RDG2 							= RegisterParticle("POI_orange_long_2xspeed");
	
	// ELECTRICITY	        
	static const int POWER_GENERATOR_SMOKE			= RegisterParticle("smoke_small_generator_01");
	static const int BARBED_WIRE_SPARKS				= RegisterParticle("electro_shortc2");

	// PLAYER
	static const int BLEEDING_SOURCE				= RegisterParticle("blood_test");
	static const int BLOOD_SPLAT					= RegisterParticle("blood_medium_01_upscaled");
	
	// GUNS
	static const int GUN_MUZZLE_FLASH_SVD_STAR		= RegisterParticle("weapon_shot_Flame_3D_4star");
	static const int GUN_SOLO_MUZZLE_FLASH			= RegisterParticle("weapon_shot_Flame_3D");
	static const int GUN_PARTICLE_TEST				= RegisterParticle("weapon_shot_fnx_01");
	static const int GUN_PARTICLE_CASING			= RegisterParticle("weapon_shot_chamber_smoke");
	static const int GUN_PARTICLE_CASING_RAISE		= RegisterParticle("weapon_shot_chamber_smoke_raise");
	static const int GUN_FNX						= RegisterParticle("weapon_shot_fnx_01");
	static const int GUN_FNX_SUPPRESSED				= RegisterParticle("weapon_shot_fnx_02");
	static const int GUN_CZ75						= RegisterParticle("weapon_shot_cz75_01");
	static const int GUN_AKM						= RegisterParticle("weapon_shot_akm_01");
	static const int GUN_AKM_SUPPRESSED				= RegisterParticle("weapon_shot_akm_02");
	static const int GUN_IZH18						= RegisterParticle("weapon_shot_izh18_01");
	static const int GUN_IZH18_SUPPRESSED			= RegisterParticle("weapon_shot_izh18_02");
	static const int GUN_MP5K						= RegisterParticle("weapon_shot_mp5k_01");
	static const int GUN_MP5K_SUPPRESSED			= RegisterParticle("weapon_shot_mp5k_02");
	static const int GUN_MP5K_COMPENSATOR			= RegisterParticle("weapon_shot_mp5k_02");
	static const int GUN_UMP45						= RegisterParticle("weapon_shot_ump45_01");
	static const int GUN_M4A1						= RegisterParticle("weapon_shot_m4a1_01");
	
	static const int GUN_CZ61						= RegisterParticle("weapon_shot_cz61_01");
	static const int GUN_LONG_WINDED_SMOKE			= RegisterParticle("weapon_shot_winded_smoke");
	static const int IMPACT_TEST					= RegisterParticle("bullet_impact_placeholder");
	static const int IMPACT_TEST2					= RegisterParticle("_test_orientation_02");
	static const int IMPACT_TEST_ENTER				= RegisterParticle("impacts/_test_impact_enter_debug");
	static const int IMPACT_TEST_RICOCHET			= RegisterParticle("impacts/_test_impact_ricochet_debug");
	static const int IMPACT_TEST_EXIT				= RegisterParticle("impacts/_test_impact_exit_debug");
	static const int SMOKING_BARREL					= RegisterParticle("smoking_barrel");
	
	
	
	// EXPLOSIONS
	static const int EXPLOSION_TEST					= RegisterParticle("explosion_placeholder");
	
	
	
	static int RegisterParticle (string file_name)
	{
		return RegisterParticle( GetPathToParticles() , file_name);
	}
	
	//! Registers a particle and returns its ID
	static int RegisterParticle (string root_path, string file_name)
	{
		if (!m_ParticlePaths)
			m_ParticlePaths = new map<int, string>;
		
		m_ParticlePaths.Insert(++m_lastID, root_path + file_name);
		return m_lastID;
	}
	
	static int GetRegisteredParticlesCount()
	{
		return m_lastID;
	}
	
	//! Returns particle's ID based on its relative path (without .ptc suffix)
	static string GetParticlePath(int particle_id)
	{
		return m_ParticlePaths.Get(particle_id);
	}
	
	//! Returns particle's full path (with .ptc suffix) based on its ID
	static string GetParticleFullPath(int particle_id)
	{
		string path = m_ParticlePaths.Get(particle_id);
		return path + ".ptc";
	}
	
	//! Returns particle's ID based on the filename (without .ptc suffix)
	static int GetParticleID(string particle_file)
	{
		return m_ParticlePaths.GetKeyByValue(particle_file);
	}
	
	//! Returns base bath to all particles
	static string GetPathToParticles()
	{
		return "graphics/particles/";
	}
}

// Example how to register particles from a mod
/*
modded class ParticleList
{
	static const int MODDED_PARTICLE = RegisterParticle( "modded_particles_folder/" , "moddedpart");
}
*/