class Particle extends ScriptedEntity
{
	protected int		m_ParticleID;
	protected bool 		m_IsPlaying;	
	
	vector 				m_DefaultOri;
	vector 				m_DefaultPos;
	float 				m_MaxOriWiggle;
	float 				m_MaxOriInterval;
	ref Timer 			m_RandomizeOri;
	
	private Object 		m_ParentObject;
	private Object 		m_ParticleEffect;
	
	Effect m_MyEffectHolder;
	
	//====================================
	// Client: Constructor
	//====================================
	void Particle()
	{
		SetFlags(EntityFlags.VISIBLE, true);
		SetEventMask(EntityEvent.INIT);
		SetEventMask(EntityEvent.FRAME);
	}	
	
	//====================================
	// Client: UpdateState
	//====================================
	protected void UpdateState()
	{
		if ( m_IsPlaying == false && m_ParticleEffect)
		{
			DestroyParticleEffect();
		}
		else if ( m_IsPlaying == true && m_ParticleEffect == NULL )
		{
			CreateParticleEffect();
		}
	}
	
	//====================================
	// Client: CreateParticleEffect
	//====================================
	private void CreateParticleEffect()
	{
		if ( m_ParticleEffect == NULL )
		{
			m_ParticleEffect = GetGame().CreateObject("#particlesourceenf", Vector(0,0,0), true); // partiple source must be lowercase!
		}
		
		this.AddChild(m_ParticleEffect, -1);
		vobject vobj = NULL;
		
		if (!GetGame().IsServer()  ||  !GetGame().IsMultiplayer())
			vobj = GetObject( ParticleList.GetParticleFullPath(m_ParticleID) );
		
		m_ParticleEffect.SetObject(vobj, "");
		ReleaseObject(vobj);
	}
	
	//====================================
	// Client: DestroyParticleEffect
	//====================================	
	private void DestroyParticleEffect()
	{
		if ( m_ParticleEffect && GetGame() )
		{
			SetParameter(-1, EmitorParam.LIFETIME, 0);
			SetParameter(-1, EmitorParam.LIFETIME_RND, 0);
			SetParameter(-1, EmitorParam.REPEAT, 0);
		}
	}
	
	//====================================
	// Client: SetParameter
	//====================================
	void SetParameter(int emitter, int parameter, float value)
	{
		// m_ParticleEffect -> owner of Particle, as set in Particle CreateParticleEffect()

		SetParticleParm(m_ParticleEffect, emitter, parameter, value);
	}
	
	//====================================
	// Client: GetParameter
	//====================================
	void GetParameter(int emitter, int parameter, out float value)
	{
		// m_ParticleEffect -> owner of Particle, as set in Particle CreateParticleEffect()

		GetParticleParm(m_ParticleEffect, emitter, parameter, value);
	}
	
	//====================================
	// Client: GetParticleCount
	//====================================
	int GetParticleCount()
	{
		if (m_ParticleEffect)
		{
			return m_ParticleEffect.GetParticleCount(m_ParticleEffect);
		}
		
		return 0;
	}
	
	//====================================
	// Client: IsActive
	//====================================
	bool IsActive()
	{
		if (m_ParticleEffect)
		{
			int repeat = false;
			string emitors_array[];
			
			int emitors = GetParticleEmitors(m_ParticleEffect, emitors_array, 30);
			
			for (int i = 0; i < emitors; i++)
			{	
				GetParticleParm(m_ParticleEffect, i, EmitorParam.REPEAT, repeat);
				
				if (repeat)
				{
					break;
				}
			}
			
			if (repeat == 1)
			{
				return true;
			}
		}
		
		return false;
	}
	
	//====================================
	// Client: OnCheckAutoDelete
	//====================================	
	void OnCheckAutoDelete()
	{
		//int count = GetParticleCount();
		
		if (!IsActive()  &&  GetParticleCount() <= 0 )
		{
			OnToDelete();
			GetGame().ObjectDelete( this );
			m_ParticleEffect = NULL;
		}
	}
	
	override void EOnFrame(IEntity other, float timeslice)
	{
		OnCheckAutoDelete();
	}
	
	// Called before deletion
	void OnToDelete()
	{
		if (m_MyEffectHolder)
		{
			m_MyEffectHolder.SetParticle(NULL);
			m_MyEffectHolder.CheckLifeSpan();
		}
	}
	
	void SetEffectHolder(Effect eff)
	{
		m_MyEffectHolder = eff;
	}
	
	//! Makes the particle change direction by random_angle every random_interval seconds.
	void SetWiggle(float random_angle, float random_interval)
	{
		if ( !random_angle == 0  ||  !random_interval == 0)
		{
			m_MaxOriWiggle = random_angle;
			m_MaxOriInterval = random_interval;
			
			if (!m_RandomizeOri)
				m_RandomizeOri = new Timer( CALL_CATEGORY_GAMEPLAY );
			
			if ( !m_RandomizeOri.IsRunning() ) // Makes sure the timer is NOT running already
			{
				m_RandomizeOri.Run( Math.RandomFloat(0, m_MaxOriInterval) , this, "RandomizeOrientation", NULL, false);
			}
		}
		else if (m_RandomizeOri)
		{
			m_RandomizeOri.Stop();
			delete m_RandomizeOri;
		}
	}
	
	//! Stops randomized wiggle
	void StopWiggle()
	{
		SetWiggle(0,0);
	}
	
	// Randomizes orientation within m_MaxOriWiggle
	void RandomizeOrientation()
	{
		if (m_ParentObject)
		{
			if ( !m_RandomizeOri.IsRunning() )
			{
				m_RandomizeOri.Run( Math.RandomFloat(0, m_MaxOriInterval) , this, "RandomizeOrientation", NULL, false);
			}
			
			Object old_parent = m_ParentObject;
			AddAsChild( NULL );
			
			float r1 = Math.RandomFloat(0, m_MaxOriWiggle*2) - m_MaxOriWiggle;
			float r2 = Math.RandomFloat(0, m_MaxOriWiggle*2) - m_MaxOriWiggle;
			float r3 = Math.RandomFloat(0, m_MaxOriWiggle*2) - m_MaxOriWiggle;
			
			vector new_ori = m_DefaultOri + Vector( r1, r2, r3 );
			AddAsChild( old_parent, m_DefaultPos, new_ori );
		}
	}
	
	
	/** \name Create a particle
		You can create a particle either at some position, or create it as an attachment on some object.
	*/
	
	/**
	\brief [server] Creates a particle emitter and attaches it on the given object
		\param particle_id \p int Particle ID registered in ParticleList.c
		\param parent_obj \p Object Instance on which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yawn, Roll in degrees) (Optional)
		\return \p Particle Created particle instance
	*/
	static Particle Create( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
	{
		if (!parent_obj)
			Error("ERROR when creating a particle! Parameter parent_obj is NULL! Remove this parameter if you don't want to attach this particle on anything!");
		
		
		vector global_pos = parent_obj.GetPosition();
		Particle p = Create(particle_id, global_pos);
		p.AddAsChild(parent_obj, local_pos, local_ori);
		p.m_DefaultOri = local_ori;
		
		return p;
	}
	
	/**
	\brief [server] Creates a particle emitter on the given position
		\param particle_id \p int Particle ID registered in ParticleList.c
		\param global_pos \p Vector Position where the particel will be created
		\param global_ori \p vector Orientation (Pitch, Yawn, Roll in degrees) (Optional)
		\return \p Particle Created particle instance
	*/
	static Particle Create( int particle_id, vector global_pos, vector global_ori = "0 0 0" )
	{
		bool is_local = !( GetGame().IsMultiplayer() && GetGame().IsServer() );
		Particle p = Particle.Cast( GetGame().CreateObject("Particle", global_pos, is_local) );
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			p.SetSource(particle_id);
			p.SetOrientation(global_ori);
		}
		return p;
	}
	
	
	
	/** \name Play/Stop particle
		You can use the following Play(...) functions to (create and) activate a particle in 1 line of your script.
	*/
	
	
	/**
	\brief [server] Creates a particle emitter, attaches it on the given object and activates it
		\param particle_id \p int Particle ID registered in ParticleList.c
		\param parent_obj \p Object Instance on which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yaw, Roll in degrees) (Optional)
		\return \p Particle Created particle instance
	*/
	static Particle Play( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
	{
		Particle p = Create(particle_id, parent_obj, local_pos, local_ori);
		p.Play();
		
		return p;
	}
	
	/**
	\brief [server] Creates a particle emitter on the given position and activates it
		\param particle_id \p int Particle ID registered in ParticleList.c
		\param global_pos \p Vector Position where the particel will be created
		\return \p Particle Created particle instance
	*/
	static Particle Play( int particle_id, vector global_pos)
	{
		Particle p = Create(particle_id, global_pos);
		p.Play();
		
		return p;
	}
	
	//! [server] Plays the current particle. The optional parameter changes this particle for the new one.
	void Play(int particle_id = -1)
	{
		if ( particle_id > -1 )
		{
			SetSource(particle_id);
		}
		
		m_IsPlaying = true;

		UpdateState();
	}
	
	//! [server] Stops generating particles. Emitter is later automatically removed from memory on clients when its particle count is 0.
	void Stop()
	{
		m_IsPlaying = false;
		UpdateState();
	}
	
	/**
	\brief [server] Attaches this particle onto some object. If NULL value is provided then the particle will be detached from the current parent.
		\param parent \p Object Parent onto which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yawn, Roll in degrees) (Optional)
	*/
	void AddAsChild(Object parent, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		if (parent  &&  !parent.ToDelete()) // AddAsChild method is sometimes called from a timer. Due to that it is necesarry to use ToDelete() here to check if the parent object is flagged for deletion or not on client, because sometimes this code is executed before the parent's destructor from where this would normally be handled.
		{
			SetPosition(local_pos);
			SetOrientation(local_ori);
			m_ParentObject = parent;
			m_DefaultPos = local_pos;
			parent.AddChild(this, -1); // Pivot does not work in DayZ so we use -1.
		}
		else
		{
			if (m_ParentObject  &&  !m_ParentObject.ToDelete())
			{
				m_ParentObject.RemoveChild(this);
				SetPosition( m_ParentObject.ModelToWorld( GetPosition() ) ); // Move the detached particle to its last position on the parent. Otherwise it will be around [0,0,0].
				m_ParentObject = NULL;
			}
		}
	}
	
	//! [server] Sets particle id
	void SetSource(int particle_id)
	{
		m_ParticleID = particle_id;
	}
}