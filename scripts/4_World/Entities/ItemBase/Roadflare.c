/*
	Description: The flare has 3 burning states during which it uses different particle effects. When it burns up, it still releases some smoke for a while.
	When the flare is dropped while its burning, it is stood up on its stands. This makes the shadows, illumination and particles stand out better.
*/

enum RoadflareBurningState
{
	NOT_BURNING,
	INITIAL_BURN,
	MAIN_BURN,
	FINAL_BURN,
	SMOKE_ONLY
};

class Roadflare : ItemBase
{
	// Burning
	static protected const int 	INITIAL_BURNING_STATE_TIME = 50;
	static protected const int 	FINAL_BURNING_STATE_TIME = 60;
	static protected vector		m_FlameLocalPos = "0 0.28 0";
	protected int				m_BurningState = RoadflareBurningState.NOT_BURNING;
	
	// Particles
	protected Particle 			m_ParInitialFire;
	protected Particle 			m_ParMainFire;
	protected Particle 			m_ParFinalFire;
	protected Particle 			m_ParJustSmoke;
	protected Particle 			m_ParRandomSparks;
	
	static protected int 		PARTICLE_INIT_FIRE = ParticleList.ROADFLARE_BURNING_INIT;
	static protected int 		PARTICLE_MAIN_FIRE = ParticleList.ROADFLARE_BURNING_MAIN;
	static protected int 		PARTICLE_FINAL_FIRE = ParticleList.ROADFLARE_BURNING_ENDING;
	static protected int 		PARTICLE_FINAL_SMOKE = ParticleList.ROADFLARE_BURNING_SMOKE;
	static protected int 		PARTICLE_RANDOM_SPARKS = ParticleList.BARBED_WIRE_SPARKS; // PLACEHOLDER
	
	// Sounds
	protected SoundOnVehicle		m_LoopSoundEntity;
	static protected const string	BURNING_SOUND = "roadflareLoop";
	static protected const int		BURNING_NOISE_RANGE = 30;
	
	// Timers
	ref Timer 					m_FinalSmokeTimer;
	ref Timer 					m_RandomSparksTimer;
	
	// Selections
	protected const string 		STANDS_FOLDED = "Sticks_Flare_Folded_1";
	protected const string 		STANDS_UNFOLDED = "Sticks_Flare_Unfolded_1";

	void Roadflare()
	{
		m_FinalSmokeTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_RandomSparksTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		RegisterNetSyncVariableFloat("m_EM.m_Energy"); // TO DO: Do not register float energy directly, but use GetEnergy0To100() instead!
	}
	
	void ~Roadflare()
	{
		if ( GetGame() )
		{
			if ( m_LoopSoundEntity != NULL )
			{
				GetGame().ObjectDelete( m_LoopSoundEntity );
			}
			
			delete m_FinalSmokeTimer;
			delete m_RandomSparksTimer;
		}
		
		DestroyAllParticles();
	}
	
	// When the flare starts burning
	override void OnWorkStart()
	{
		if ( GetGame().IsServer() )
			SwitchLight(true);
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer())
			m_LoopSoundEntity = PlaySoundLoop(BURNING_SOUND, BURNING_NOISE_RANGE);
		
		m_BurningState = RoadflareBurningState.INITIAL_BURN;
		
		// Disabled temporarily, need new particle
		//m_RandomSparksTimer.Run( Math.RandomFloatInclusive(1, 3), this, "OnRandomSparks", NULL, false);	
	
	}
	
	void OnRandomSparks()
	{
		if ( GetCompEM().IsWorking() )
		{
			Particle.Play( PARTICLE_RANDOM_SPARKS, this, m_FlameLocalPos);
			m_RandomSparksTimer.Run( Math.RandomFloatInclusive(0, 5), this, "OnRandomSparks", NULL, false);
		}
	}
	
	// Every second of the flare burning
	override void OnWork(float consumed_energy)
	{
		GetCompEM().Synch();
		
		float burning_time = GetCompEM().GetEnergyMax() - GetCompEM().GetEnergy();
		
		if (m_BurningState == RoadflareBurningState.INITIAL_BURN)
		{
			SetTemperature(30, true);
			
			if (burning_time >= INITIAL_BURNING_STATE_TIME)
			{
				m_BurningState = RoadflareBurningState.MAIN_BURN;
			}
		}
		else if(m_BurningState == RoadflareBurningState.MAIN_BURN)
		{
			SetTemperature(50, true);
			
			if ( burning_time >= GetCompEM().GetEnergyMax() - FINAL_BURNING_STATE_TIME )
			{
				m_BurningState = RoadflareBurningState.FINAL_BURN;
			}
		}
		else if(m_BurningState == RoadflareBurningState.FINAL_BURN)
		{
			SetTemperature(40, true);
		}
		
		if ( GetGame().IsServer() )
		{
			// Burn containers of this roadflare. This might be removed if lit roadflare can't be put into inventory.
			EntityAI container_EAI = GetHierarchyParent();
			
			if ( container_EAI  &&  container_EAI.IsInherited(ItemBase)  &&  !container_EAI.IsInherited(TripwireTrap) )
			{
				ItemBase container_IB = ItemBase.Cast( container_EAI );
				container_IB.SetTemperature(100);
				
				int c_size = container_IB.GetItemSize();
				if (c_size == 0)
					c_size = 1;
				
				container_IB.AddHealth("","",-10/c_size);
			}
		}
		
		UpdateActiveParticles();
	}
	
	// When the flare stops burning
	override void OnWorkStop()
	{
		m_BurningState = RoadflareBurningState.SMOKE_ONLY;
		UpdateActiveParticles();
		m_FinalSmokeTimer.Run(60, this, "StopSmoking", NULL, false);
		
		if ( m_LoopSoundEntity != NULL && GetGame() != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
		
		
		if ( GetGame().IsServer() )
		{
			SwitchLight(false);
			SetHealth("","",0);
		}
	}
	
	// Updates all (in)active particles
	void UpdateActiveParticles()
	{
		if ( GetGame().IsServer()  &&  GetGame().IsMultiplayer() )
			return;
		
		switch (m_BurningState)
		{
			case RoadflareBurningState.NOT_BURNING:			
				
				DestroyAllParticles();
				break;
				
			case RoadflareBurningState.INITIAL_BURN:			
				
				if (!m_ParInitialFire)
				{
					DestroyAllParticles();
					m_ParInitialFire = Particle.Play( PARTICLE_INIT_FIRE, this, m_FlameLocalPos);
					m_ParInitialFire.SetWiggle( 15, 0.3);
				}
				break;
				
			case RoadflareBurningState.MAIN_BURN:
				
				if (!m_ParMainFire)
				{
					m_ParMainFire = Particle.Play( PARTICLE_MAIN_FIRE, this, m_FlameLocalPos);
					m_ParMainFire.SetWiggle( 10, 0.3);
				}
				
				DestroyParticle(m_ParInitialFire);
				
				break;	
				
			case RoadflareBurningState.FINAL_BURN:
				
				if (!m_ParFinalFire)
				{
					DestroyAllParticles();
					m_ParFinalFire = Particle.Play( PARTICLE_FINAL_FIRE, this, m_FlameLocalPos);
					m_ParFinalFire.SetWiggle( 5, 0.3);
				}
				break;
				
			case RoadflareBurningState.SMOKE_ONLY:			
				
				if (!m_ParJustSmoke)
				{
					DestroyAllParticles();
					m_ParJustSmoke = Particle.Play( PARTICLE_FINAL_SMOKE, this, m_FlameLocalPos);
					m_ParJustSmoke.SetWiggle( 3, 0.3);
				}
				break;
		}
	}
	
	// Destroys the given particle
	void DestroyParticle( Particle p )
	{
		if (p)
		{
			p.SetWiggle(0,0);
			p.Stop();
		}
	}
	
	// Destroys all particles
	void DestroyAllParticles()
	{
		DestroyParticle(m_ParInitialFire);
		DestroyParticle(m_ParMainFire);
		DestroyParticle(m_ParFinalFire);
		DestroyParticle(m_ParJustSmoke);
	}
	
	// Stop releasing final smoke
	void StopSmoking()
	{
		m_BurningState = RoadflareBurningState.NOT_BURNING;
		UpdateActiveParticles();
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		if (m_BurningState != RoadflareBurningState.NOT_BURNING)
		{
			HideSelection(STANDS_FOLDED);
			ShowSelection(STANDS_UNFOLDED);
			
			vector ori_rotate = "180 32 0";
			//vector ori_standing = ori_rotate + GetOrientation(); // TO DO: Use ori_standing when item drop starts working again.
			SetOrientation(ori_rotate);
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( m_BurningState != RoadflareBurningState.NOT_BURNING )
		{
			return false;
		}
		
		return true;
	}
	
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		HideSelection(STANDS_UNFOLDED);
		ShowSelection(STANDS_FOLDED);
	}
	
	override void OnActivatedByTripWire()
	{
		GetCompEM().SwitchOn();
	}
};