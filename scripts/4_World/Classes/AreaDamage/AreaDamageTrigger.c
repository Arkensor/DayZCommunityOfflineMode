class AreaDamageTrigger extends Trigger
{
	private EntityAI	  	m_AreaTriggerParent;
	private AreaDamageType	m_AreaDamageType;

	private bool			m_Looping;
	private bool			m_Deferred;
	private bool			m_Raycasted;
	
	private int				m_TriggerUpdateMs;
	private float			m_LoopingInterval;
	private float 			m_DeferredInterval;
	
	private vector			m_ExtentMin;
	private vector			m_ExtentMax;

	ref TStringArray 	m_HitZones;
	ref TStringArray 	m_RaycastSources;
	string					m_AmmoName;
	
	protected ref Timer		m_LoopTimer;
	protected ref Timer		m_DeferTimer;

	void AreaDamageTrigger()
	{
		m_Looping 			= false;
		m_Deferred 			= false;
		m_Raycasted			= false;
		m_LoopingInterval 	= 0.1;
		m_DeferredInterval 	= 0.1;
		m_TriggerUpdateMs   = 100; 
		m_AreaDamageType 	= AreaDamageType.REGULAR;
		m_HitZones			= new TStringArray;
		m_HitZones.Insert("Head");
		m_RaycastSources	= new TStringArray;
		m_RaycastSources 	= {"0.0 0.1 0.0", "0.2 0.1 0.2", "-.2 0.1 0.2", "0.2 0.1 -.2", "-.2 0.1 -.2"};

		m_AmmoName 			= "MeleeDamage";
	}

	void ~AreaDamageTrigger() {}
	
	// Sets parent object for this trigger
	void SetParentObject(EntityAI parent)
	{
		if ( GetGame().IsServer() )
		{
			m_AreaTriggerParent = parent;
		}
	}
	
	void SetAreaDamageType(int area_dmg_type )
	{
		AreaDamageSelector(area_dmg_type);
	}
	
	void SetLoopTime(float time)
	{
		m_LoopingInterval = time;
	}
	
	void SetDeferTime(float time)
	{
		m_DeferredInterval = time;
	}
	
	void SetRaycastSources(array<string> raycast_sources)
	{
		m_RaycastSources = raycast_sources;
	}
	
	void SetHitZones(array<string> hitzones)
	{
		m_HitZones = hitzones;
	}

	void SetAmmoName(string ammo_name)
	{
		m_AmmoName = ammo_name;
	}
	
	protected void PreDamageActions()
	{
		m_AreaTriggerParent.PreAreaDamageActions();
	}

	protected void PostDamageActions()
	{
		m_AreaTriggerParent.PostAreaDamageActions();
	}

	override protected void UpdateInsiders(int timeout)
	{
		super.UpdateInsiders(m_TriggerUpdateMs);
	}
	
	override void OnEnter( Object obj )
	{
		super.OnEnter( obj );

		if ( g_Game.IsServer() )
		{
			EntityAI victim;
			Class.CastTo(victim, obj);
			// start main timer
			m_LoopTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			
			// Run timer for non-deferred AreaDamageTypes only
			if ( m_AreaDamageType == AreaDamageType.REGULAR || m_AreaDamageType == AreaDamageType.REGULAR_RAYCASTED || m_AreaDamageType == AreaDamageType.ONETIME || m_AreaDamageType == AreaDamageType.ONETIME_RAYCASTED)
			{
				PreDamageActions();
				m_LoopTimer.Run( m_LoopingInterval, this, "SnapOnObject", new Param1<EntityAI>(victim), m_Looping );
			}
			
			// Run timer for deferred AreaDamageTypes only
			if ( m_AreaDamageType == AreaDamageType.REGULAR_DEFERRED || m_AreaDamageType == AreaDamageType.REGULAR_RAYCASTED_DEFERRED || m_AreaDamageType == AreaDamageType.ONETIME_DEFERRED || m_AreaDamageType == AreaDamageType.ONETIME_RAYCASTED_DEFERRED )
			{
				PreDamageActions();
				m_DeferTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
				m_DeferTimer.Run( m_DeferredInterval, this, "LoopTimer", new Param1<EntityAI>(victim), m_Looping );
			}
		}
	}
	
	override void OnLeave(Object obj)
	{
		super.OnLeave( obj );

		// stop all running timers
		if ( m_DeferTimer && m_DeferTimer.IsRunning() )
			{ m_DeferTimer.Stop(); }
		if ( m_LoopTimer && m_LoopTimer.IsRunning() )
			{ m_LoopTimer.Stop(); }
	}
	
	protected string GetRandomHitZone(array<string> hitzones)
	{
		string hitzone;
		Math.Randomize(-1);
		int idx = Math.RandomInt( 0, 100 ) % hitzones.Count();
		hitzone = hitzones[idx];

		return hitzone;
	}
	
	protected string GetRaycastedHitZone(ManBase victim, array<string> raycast_sources_str)
	{
		
		// Vertical raycast start positions:    Center,      North East,    North West,    South East,  South West
		//vector raycast_sources[5] = {"0.0 0.1 0.0", "0.2 0.1 0.2", "-.2 0.1 0.2", "0.2 0.1 -.2", "-.2 0.1 -.2"};
		
		string hitzone;
		vector contact_pos;
		vector contact_dir;
		int contactComponent;
		bool IsSteppedOn = false;
		
		ref set<Object> victims = new set<Object>;
		
		array<vector> raycast_sources = new TVectorArray;

		// convert Array of string to array of Vectors
		for ( int v = 0; v < raycast_sources_str.Count(); ++v)
		{
			raycast_sources.Insert(raycast_sources_str[v].ToVector());
		}
		
		for ( int i = 0; i < raycast_sources.Count(); ++i )
		{
			vector raycast_start_pos = ModelToWorld( raycast_sources.Get(i) );
			vector raycast_end_pos = "0 0.5 0" + raycast_start_pos;
			
			#ifdef DEVELOPER
			Debug.DrawArrow( raycast_start_pos, raycast_end_pos );
			#endif
			DayZPhysics.RaycastRV( raycast_start_pos, raycast_end_pos, contact_pos, contact_dir, contactComponent, victims , NULL, this, true, false, ObjIntersectIFire);
			
			for ( int j = 0; j < victims.Count(); ++j )
			{
				local Object contact_obj = victims.Get(j);
				
				if ( contact_obj.IsInherited(Man) )
				{
					IsSteppedOn = true;
					break;
				}
			}
			
			if (IsSteppedOn)
			{
				hitzone = victim.GetDamageZoneNameByComponentIndex(contactComponent);
				break;
			}
		}
		
		if(IsSteppedOn)
		{
			return hitzone;
		}
		else
		{
			// Damage random leg since we don't know what part of player's body was caught in the trap.
			string dmg_zone_rnd = "LeftFoot";
			if ( Math.RandomIntInclusive(0, 1) == 1 )
				dmg_zone_rnd = "RightFoot";
			return dmg_zone_rnd;
		}
	}

	// just for running Loop timer throught Defer timer
	protected void LoopTimer(EntityAI victim)
	{
		m_LoopTimer.Run( m_LoopingInterval, this, "SnapOnObject", new Param1<EntityAI>(victim), m_Looping );	
	}
	
	protected void SnapOnObject( EntityAI victim )
	{
		string hitzone;
		ManBase victim_MB;
		Class.CastTo(victim_MB, victim);
		
		if ( !m_Raycasted )
			{ hitzone = GetRandomHitZone(m_HitZones); }
		else
			{ hitzone = GetRaycastedHitZone( victim_MB, m_RaycastSources ); }
		
		if ( victim_MB.IsInherited(Man) )
		{
			victim_MB.ProcessDirectDamage(DT_CUSTOM, this, hitzone, "MeleeDamage", "0 0 0", 1);
			PostDamageActions();
		}
	}
	
	protected void AreaDamageSelector(int area_dmg_type)
	{
		m_AreaDamageType = area_dmg_type;
		switch (area_dmg_type)
		{
			case AreaDamageType.REGULAR:
				m_Looping = true;
				m_Deferred = false;
				m_Raycasted = false;
				break;
			case AreaDamageType.REGULAR_DEFERRED:
				m_Looping = true;
				m_Deferred = true;
				m_Raycasted = false;
				break;
			case AreaDamageType.REGULAR_RAYCASTED:
				m_Looping = true;
				m_Deferred = false;
				m_Raycasted = true;
				break;
			case AreaDamageType.REGULAR_RAYCASTED_DEFERRED:
				m_Looping = true;
				m_Deferred = true;
				m_Raycasted = true;
				break;
			case AreaDamageType.ONETIME:
				m_Looping = false;
				m_Deferred = false;
				m_Raycasted = false;
				break;
			case AreaDamageType.ONETIME_DEFERRED:
				m_Looping = false;
				m_Deferred = true;
				m_Raycasted = false;
				break;
			case AreaDamageType.ONETIME_RAYCASTED:
				m_Looping = false;
				m_Deferred = false;
				m_Raycasted = true;
				break;
			case AreaDamageType.ONETIME_RAYCASTED_DEFERRED:
				m_Looping = false;
				m_Deferred = true;
				m_Raycasted = true;
				break;
		}
	}
}