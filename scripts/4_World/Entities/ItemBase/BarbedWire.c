class BarbedWire extends ItemBase
{
	// Sounds lists
	const static int		SOUNDS_SPARK_COUNT							= 4;
	const static int		SOUNDS_CUT_COUNT							= 3;
	const static int		SOUNDS_COLLISION_COUNT						= 4;
	const static int		SOUNDS_SHOCK_COUNT							= 4;
	const static float		RANDOM_SPARK_INTERVAL						= 5.0; // TO DO! Currently not used.
	
	const static string 	m_SoundsSpark[SOUNDS_SPARK_COUNT] 			= {"electricFenceSpark1", "electricFenceSpark2", "electricFenceSpark3", "electricFenceSpark4"};
	const static string 	m_SoundsCut[SOUNDS_CUT_COUNT] 				= {"barbedFenceCut1", "barbedFenceCut2", "barbedFenceCut3"};
	const static string 	m_SoundsCollision[SOUNDS_COLLISION_COUNT]	= {"barbedFenceCollision1", "barbedFenceCollision2", "barbedFenceCollision3", "barbedFenceCollision4"};
	const static string 	m_SoundsShock[SOUNDS_SHOCK_COUNT] 			= {"electricFenceShock1", "electricFenceShock2", "electricFenceShock3", "electricFenceShock4"};
	const static string 	m_SoundBuzzLoop 							= "electricFenceBuzzLoop1";
	
	SoundOnVehicle m_BuzzSoundLoop;
	
	ref Timer m_SparkEvent;
	private ref AreaDamageBase m_AreaDamage;
	
	private bool m_TriggerActive;
	private bool m_IsPlaced;
	
	void BarbedWire()
	{
		m_SparkEvent 	= new Timer( CALL_CATEGORY_GAMEPLAY );
		m_TriggerActive = false;
		m_IsPlaced 		= false;
	}
	
	void ~BarbedWire() {}
	
	override void OnWorkStart()
	{
		SoundBuzzLoopStart();
		if (m_TriggerActive)
			{ DestroyDamageTrigger(); }

		if (m_IsPlaced)
		{
			//TimerRandomSpark();
			CreateElectrifiedDamageTrigger();
		}
	}

	override void OnWorkStop()
	{
		SoundBuzzLoopStop();
		if (m_TriggerActive)
			{ DestroyDamageTrigger(); }
		
		if (m_IsPlaced)
			{ CreateDamageTrigger(); }
		
		m_SparkEvent.Stop();
	}

	override void OnWork( float consumed_energy ) {}

	override void OnIsPlugged(EntityAI source_device)
	{
		SoundCut();
	}

	override void OnIsUnplugged( EntityAI last_energy_source )
	{
		if (m_TriggerActive)
			{ DestroyDamageTrigger(); }
		SoundCut();
	}
	
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		HideSelection("placing");
		ShowSelection("zbytek");
		if (m_TriggerActive)
			{ DestroyDamageTrigger(); }
		GetCompEM().UnplugThis();
		GetCompEM().UnplugAllDevices();
	}
	
	override void OnPlacementComplete( Man player )
	{
		ShowAllSelections();
		HideSelection("zbytek");

		if (!GetHierarchyParent())
		{
			if (GetCompEM().IsPlugged() && GetCompEM().IsWorking() )
				{ CreateElectrifiedDamageTrigger(); }
			else
				{ CreateDamageTrigger(); }
			m_IsPlaced = true;
		}
	}
	
	// Area Damage triggers
	// ---------------------------------------------------------			
	protected void CreateElectrifiedDamageTrigger()
	{
		m_AreaDamage = new AreaDamageRegular(this);
		m_AreaDamage.SetExtents("-1 0 -0.4", "1 0.7 0.4");
		m_AreaDamage.SetLoopInterval(0.3);
		m_AreaDamage.SetHitZones({"RightLeg", "LeftLeg", "RightFoot", "LeftFoot"});
		m_AreaDamage.SetAmmoName("MeleeDamage");
		m_AreaDamage.Spawn();
		m_TriggerActive = true;						
	}
	
	protected void CreateDamageTrigger()
	{
		m_AreaDamage = new AreaDamageOneTime(this);
		m_AreaDamage.SetExtents("-1 0 -0.4", "1 0.7 0.4");
		m_AreaDamage.SetLoopInterval(0.1);
		m_AreaDamage.SetHitZones({"RightLeg", "LeftLeg", "RightFoot", "LeftFoot"});
		m_AreaDamage.SetAmmoName("MeleeDamage");
		m_AreaDamage.Spawn();
		m_TriggerActive = true;
	}
	
	protected void DestroyDamageTrigger()
	{
		m_AreaDamage.DestroyDamageTrigger();
		m_TriggerActive = false;
	}
	// ---------------------------------------------------------
	
	// Controls spawn of random sparks
	/*
	protected void TimerRandomSpark() // TO DO: Come up with randomized functionality.
	{
		if ( GetCompEM().IsSwitchedOn() )
		{
			int plugged_devices = GetCompEM().GetEnergySource().GetCompEM().GetPluggedDevicesCount();
			float rnd_time = Math.RandomFloat(0.3, RANDOM_SPARK_INTERVAL / plugged_devices + 1.0);
			m_SparkEvent.Run(rnd_time + 0.3, this, "Spark", NULL, true);
		}
	}
	*/

	// Spawns spark particle effect and plays sound.
	void Spark()
	{
		Particle.Play( ParticleList.BARBED_WIRE_SPARKS, this);
		SoundSpark();
	}
	
	
	// SOUNDS
	// ---------------------------------------------------------
	void SoundCut()
	{
		int random_index = Math.RandomInt(0, SOUNDS_CUT_COUNT);
		string sound_type = m_SoundsCut[random_index];
		PlaySound(sound_type, 50);
	}

	// Plays sound
	void SoundSpark()
	{
		int random_index = Math.RandomInt(0, SOUNDS_SPARK_COUNT);
		string sound_type = m_SoundsSpark[random_index];
		PlaySound(sound_type, 50);
	}

	// Plays sound
	void SoundBuzzLoopStart()
	{
		if (!m_BuzzSoundLoop)
		{
			m_BuzzSoundLoop = PlaySoundLoop(m_SoundBuzzLoop, 50);
		}
	}

	// Stops sound
	void SoundBuzzLoopStop()
	{
		if (m_BuzzSoundLoop)
		{
			GetGame().ObjectDelete(m_BuzzSoundLoop);
			m_BuzzSoundLoop = NULL;
		}
	}
	
	// Plays an electric shock sound
	void SoundElectricShock()
	{
		int random_index = Math.RandomInt(0, SOUNDS_SHOCK_COUNT);
		string sound_type = m_SoundsShock[random_index];
		PlaySound(sound_type, 50);
	}
	
	// Plays a collision sound
	void SoundCollision()
	{
		int random_index = Math.RandomInt(0, SOUNDS_COLLISION_COUNT);
		string sound_type = m_SoundsCollision[random_index];
		PlaySound(sound_type, 50);
	}
	// ---------------------------------------------------------

	// Area Damage Pre/Post actions	
	// ---------------------------------------------------------
	override void PreAreaDamageActions()
	{
		if ( GetCompEM().IsPlugged() && GetCompEM().IsSwitchedOn() )
		{
			Spark();
			SoundElectricShock();
		}
		SoundCollision();
	}
	
	override void PostAreaDamageActions() {}
	// ---------------------------------------------------------	
	
	
	// TODO: proper handling can be done once the ticket DAYZ-26145 is resolved
	override void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.EEItemLocationChanged(old_owner, new_owner);

		if (m_TriggerActive)
		{
			DestroyDamageTrigger();
			m_IsPlaced = false;
		}
	}
}