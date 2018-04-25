class Bottle_Base extends Edible_Base
{
	//Particles
	protected int PARTICLE_STEAM = ParticleList.BOILING_WATER_STEAM;
	protected Particle m_ParticleSteam;
	
	//Sounds
	const string SOUND_BOILING_WATER 	= "boilingWater";
	//
	protected SoundOnVehicle m_SoundBoilingWaterLoop = NULL;
	
	void ~Bottle_Base()
	{
		//stop particles
		SteamStop();
	}
	
	//================================================================
	// PARTICLES & SOUNDS
	//================================================================
	//steam - boiling water
	void SteamStart()
	{
		if ( !m_ParticleSteam && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			vector local_pos = GetSteamPosition();
			m_ParticleSteam = Particle.Create( PARTICLE_STEAM, this, local_pos );
			m_ParticleSteam.Play();
		
			//sound
			SoundBoilingWaterStart();
		}
	}
	
	void SteamStop()
	{
		if ( m_ParticleSteam && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_ParticleSteam.Stop();
			m_ParticleSteam = NULL;
			
			//sound
			SoundBoilingWaterStop();
		}
	}
	
	//get position for steam particle
	protected vector GetSteamPosition()
	{
		vector steam_point_local_pos = GetSelectionPosition( "steam" );
		vector steam_point_world_pos = ModelToWorld( steam_point_local_pos );
		
		return WorldToModel( steam_point_world_pos );
	}

	//================================================================
	// SOUNDS
	//================================================================
	//Burning
	protected void SoundBoilingWaterStart()
	{
		if ( !m_SoundBoilingWaterLoop )
		{
			m_SoundBoilingWaterLoop = PlaySoundLoop( SOUND_BOILING_WATER, 50 );
		}
	}
	
	protected void SoundBoilingWaterStop()
	{
		if ( m_SoundBoilingWaterLoop )
		{
			GetGame().ObjectDelete( m_SoundBoilingWaterLoop );
			m_SoundBoilingWaterLoop = NULL;			
		}
	}	
}
