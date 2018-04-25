//WIP

class Effect
{
	protected int       		m_ParticleID = -1;
    protected string    		m_SoundName;
    protected vector    		m_Position;
    protected vector    		m_Orientation;
	protected Particle 			m_Particle;
	protected SoundOnVehicle 	m_Sound;
	
	void ~Effect()
	{
		SEffectManager.EffectUnregister(this);
		Stop();
	}
	
	void Effect()
	{
		
	}
	
	void AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		GetParticle().AddAsChild(obj, local_pos, local_ori);
	}
	
	//! Set orientation in degrees (yaw, pitch, roll)
	void SetOrientation( vector ori )
	{
		m_Orientation = ori;
	}
	
	vector GetOrientation()
	{
		return m_Orientation;
	}
	
	Particle GetParticle()
	{
		return m_Particle;
	}
	
	void SetParticle(Particle p)
	{
		m_Particle = p;
	}
	
	SoundOnVehicle GetSound()
	{
		return m_Sound;
	}
	
	void GetLight()
	{
		return; // TO DO!
	}
	
	void GetDecal()
	{
		return; // TO DO!
	}
	
	// Plays all elements this effects consists of
    void Play()
    {
		if (m_ParticleID > 0)
		{
			Particle p;
			p = Particle.Play(m_ParticleID, GetPosition());
			
			p.SetOrientation( GetOrientation() );
			p.SetEffectHolder(this);
			
			SetParticle(p);
		}
		
		if (m_SoundName != "")
		{
			//PlaySoundInWorld(m_SoundName, 50); // TO DO: SUCH FUNCTION DOESN'T EXIST! SOMEONE NEEDS TO CREATE ENGINE SUPPORT FOR IT!
			
			// TEMPORAL HACK: Because we can't play sounds at world coordinates, we play them on player.
			if (!GetGame().IsMultiplayer())
			{
				Object plr = GetGame().GetPlayer();
				GetGame().CreateSoundOnObject(plr, m_SoundName, 50, false, true);
			}
			
			
		}
	}
	
	void CheckLifeSpan()
	{
		if (!m_Particle  &&  !m_Sound)
		{
			//Print(this);
			delete this;
		}
	}
 
    void SetPosition( vector pos )
    {
        m_Position = pos;
    }
	
	void SetParticleID( int id )
    {
        m_ParticleID = id;
    }
	
	void SetSoundName(string snd)
	{
		m_SoundName = snd;
	}
	
	vector GetPosition()
	{
		return m_Position;
	}
	
    void Stop()
    {
		if (GetParticle())
		{
			GetParticle().Stop();
			m_Particle = NULL;
		}
		
		if (GetSound())
		{
			GetGame().ObjectDelete(GetSound());
			m_Sound = NULL;
		}
    }
}

class EffGrenadeExplosion : Effect
{
	void EffGrenadeExplosion()
	{
		m_ParticleID = ParticleList.EXPLOSION_TEST;
 		m_SoundName = "BigBoom";
	}
}

class SEffectManager
{
	protected static ref array<ref Effect> m_Effects = new array<ref Effect>;
	
	void SEffectManager()
	{
		m_Effects = new array<ref Effect>;
	}
	
	static private void EffectRegister(Effect eff)
	{
		for ( int i = 0; i < m_Effects.Count(); ++i )
		{
			Effect eff_to_update = m_Effects.Get(i);
			eff_to_update.CheckLifeSpan();
		}
		
		m_Effects.Insert( eff );
	}
	
	
	static Effect PlayInWorld(Effect eff, vector pos, vector ori = "0 0 0")
	{
		eff.SetPosition( pos );
		eff.SetOrientation( ori );
		
		eff.Play();
		EffectRegister(eff);
		
		return eff;
	}
	
	
	
	static Effect PlayOnObject(Effect eff, Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		eff.Play();
		eff.AttachTo( obj, local_pos, local_ori);
		EffectRegister(eff);
		
		return eff;
	}
	
	static void StopAllEffects()
    {
        for ( int i = m_Effects.Count(); i > 0; --i )
		{
			Effect e = m_Effects.Get(i);
			e.Stop();
		}
		int m_Effects_count_check = m_Effects.Count();
		Print(m_Effects_count_check);
    }
	
	static void EffectUnregister(Effect eff)
    {
		for ( int i = 0; i < m_Effects.Count(); ++i )
		{
			Effect eff_to_delete = m_Effects.Get(i);
			
			if (eff == eff_to_delete)
			{
				m_Effects.Remove(i);
			}
		}
    }
	
	static array<ref Effect> GetAllEffects()
	{
		return m_Effects;
	}
}