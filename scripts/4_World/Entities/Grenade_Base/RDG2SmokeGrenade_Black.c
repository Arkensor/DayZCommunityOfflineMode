class RDG2SmokeGrenade_Black : ItemGrenade
{
	protected SoundOnVehicle		m_LoopSoundEntity;
	static protected const string	BURNING_SOUND = "roadflareLoop";
	protected Particle 				m_ParMainFire;
	static protected vector			m_FlameLocalPos = "0 0.28 0";
	
	void RDG2SmokeGrenade_Black()
	{
		RegisterNetSyncVariableFloat("m_EM.m_Energy"); // TO DO: Do not register float energy directly, but use GetEnergy0To100() instead!
	}
	
	void ~RDG2SmokeGrenade_Black()
	{
		if ( GetGame() )
		{
			if ( m_LoopSoundEntity != NULL )
			{
				GetGame().ObjectDelete( m_LoopSoundEntity );
			}
		}
	}
	
	// When the flare starts burning
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer())
			m_LoopSoundEntity = PlaySoundLoop(BURNING_SOUND, 30);
		
		m_ParMainFire = Particle.Play(ParticleList.RDG2, this, m_FlameLocalPos);
	}
	
	// When the flare stops burning
	override void OnWorkStop()
	{
		DestroyParticle( m_ParMainFire );
		
		if ( m_LoopSoundEntity != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
		
		Explode();
	}
	
	// Destroys the given particle
	void DestroyParticle( Particle p )
	{
		if (p)
		{
			p.Stop();
		}
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		if ( GetCompEM().IsWorking() )
		{
			vector ori_rotate = "180 0 0";
			vector ori_standing = ori_rotate + GetOrientation();
			SetOrientation(ori_standing);
		}
	}
	
	override void OnActivatedByTripWire()
	{
		GetCompEM().SwitchOn();
	}
};