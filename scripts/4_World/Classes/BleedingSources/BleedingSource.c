class BleedingSource
{
	vector m_Position;
	Particle m_BloodParticle;
	PlayerBase m_Player;
	
	void BleedingSource(string zone, PlayerBase player, vector position)
	{
		m_Position = position;
		m_Player = player;
		CreateBleedSymptom();
	}

	void ~BleedingSource(string zone, PlayerBase player, vector position)
	{
		RemoveBleedSymptom();
	}
	
	void CreateBleedSymptom()
	{
		if( m_Player.GetStateManager() )
		{
			m_Player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLEEDING_SOURCE );
			/*
			m_BloodParticle = Particle.Create( ParticleList.BLEEDING_SOURCE, m_Player, m_Position );
			m_BloodParticle.Play();
			*/
		}
	}

	void RemoveBleedSymptom()
	{
		if( m_Player && m_Player.GetStateManager() )
		{
			m_Player.GetStateManager().RemoveSecondaryState( StateIDs.STATE_BLEEDING_SOURCE );
		}
		if( m_BloodParticle )
		{
			m_BloodParticle.Stop();
		}
	}

	vector GetPosition()
	{
		return m_Position;
	}
}