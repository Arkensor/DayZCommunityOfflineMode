/**@class		AbilityRecord
 * @brief		pair ( action, actionType )
 **/
class AbilityRecord
{
	int m_action; /// corresponds to Human::actions == RELOAD, MECHANISM, ...
	int m_actionType; /// corresponds to Human::actionTypes == CHAMBERING_ONEBULLET_CLOSED, MECHANISM_CLOSED...
	void AbilityRecord (int a, int at) { m_action = a; m_actionType = at; }
};

typedef FSMTransition<WeaponStateBase, WeaponEventBase, WeaponActionBase, WeaponGuardBase> WeaponTransition; /// shorthand

/**@class		Weapon_Base
 * @brief		script base for all weapons
 *
 * @NOTE: this class is bound to core-config "Weapon_Base" config class
 **/
class Weapon_Base extends Weapon
{
	protected ref WeaponFSM m_fsm;	/// weapon state machine
	protected ref array<ref AbilityRecord> m_abilities;		/// weapon abilities
	protected int m_weaponAnimState; /// animation state the weapon is in, -1 == uninitialized
	protected bool m_receivedSyncFromRemote; /// if remote weapon, this flag waits for first stable state sync
	protected ref PropertyModifiers m_PropertyModifierObject;
	protected int m_RecoilSeed;
	
	// Overheating effect
	bool 					m_IsOverheatingEffectActive;
	float					m_OverheatingShots;
	ref Timer 				m_CheckOverheating;
	static const int 		OVERHEAT_EFFECT_SHOTS = 5; // After these many shots, the overheating effect begins
	static const int 		OVERHEAT_EFFECT_SHOTS_LIMIT = 60; // Limits the number of shots that will be tracked
	static const float 		OVERHEAT_EFFECT_SHOTS_DECAY_INT = 1; // Timer's interval for decrementing overheat effect's lifespan
	ref array <Particle> 	m_OverheatingParticles;

	void Weapon_Base ()
	{
		m_weaponAnimState = -1;
		m_receivedSyncFromRemote = false;
		m_abilities = new array<ref AbilityRecord>;
		InitStateMachine();
	}

	void InitStateMachine () { }

	bool CanProcessAction (int action, int actionType)
	{
		return false; // @TODO
	}
	/**@fn		HasActionAbility
	 * @brief	query if weapon supports action and actionType
	 * @param[in]	action	\p	one of Human.actions (i.e. RELOAD, MECHANISM, ...)
	 * @param[in]	actionType	\p	one of Human.actionTypes (i.e. CHAMBERING_ONEBULLET_CLOSED, MECHANISM_CLOSED...)
	 * @return	true if weapon supports operation
	 **/
	bool HasActionAbility (int action, int actionType)
	{
		int count = GetAbilityCount();
		for (int i = 0; i < count; ++i)
		{
			AbilityRecord rec = GetAbility(i);
			if (rec.m_action == action && rec.m_actionType == actionType)
				return true;
		}
		return false;
	}
	/**@fn		GetAbilityCount
	 * @return number of stored abilities
	 **/
	int GetAbilityCount () { return m_abilities.Count(); }
	/**@fn		GetAbility
	 * @param[in]	index	\p	index into m_abilities storage
	 * @return ability record
	 **/
	AbilityRecord GetAbility (int index) { return m_abilities.Get(index); }

	/**@fn		CanProcessWeaponEvents
	 * @return	true if weapon has running fsm
	 **/
	bool CanProcessWeaponEvents () { return m_fsm && m_fsm.IsRunning(); }

	/**@fn		GetCurrentState
	 * @brief		returns currently active state
	 * @return	current state the FSM is in (or NULL)
	 **/
	WeaponStateBase GetCurrentState () { return m_fsm.GetCurrentState(); }

	/**@fn		IsWaitingForActionFinish
	 * @brief	returns true if state machine started playing action/actionType and waits for finish
	 **/
	bool IsWaitingForActionFinish ()
	{
		return CanProcessWeaponEvents() && GetCurrentState().IsWaitingForActionFinish();
	}

	bool IsIdle ()
	{
		return CanProcessWeaponEvents() && GetCurrentState().IsIdle();
	}

	/**@fn	ProcessWeaponEvent
	 * @brief	weapon's fsm handling of events
	 * @NOTE: warning: ProcessWeaponEvent can be called only within DayZPlayer::HandleWeapons (or ::CommandHandler)
	 **/
	bool ProcessWeaponEvent (WeaponEventBase e)
	{
		SyncEventToRemote(e);

		if (m_fsm.ProcessEvent(e) == ProcessEventResult.FSM_OK)
			return true;

		//wpnDebugPrint("FSM refused to process event (no transition): src=" + GetCurrentState().ToString() + " event=" + e.ToString());
		return false;
	}
	/**@fn	ProcessWeaponAbortEvent
	 * @NOTE: warning: ProcessWeaponEvent can be called only within DayZPlayer::HandleWeapons (or ::CommandHandler)
	 **/
	bool ProcessWeaponAbortEvent (WeaponEventBase e)
	{
		SyncEventToRemote(e);
		
		ProcessEventResult aa;
		m_fsm.ProcessAbortEvent(e, aa);
		return aa == ProcessEventResult.FSM_OK;
	}

	bool CanChamberBullet (int muzzleIndex, Magazine mag)
	{
		return CanChamberFromMag(muzzleIndex, mag) && !IsChamberFull(muzzleIndex));
	}

	void SetWeaponAnimState (int state)
	{
		m_weaponAnimState = state;
	}
	void ResetWeaponAnimState ()
	{
		fsmDebugSpam("[wpnfsm] resetting anim state: " + typename.EnumToString(PistolAnimState, m_weaponAnimState) + " --> " + typename.EnumToString(PistolAnimState, -1));
		m_weaponAnimState = -1;
	}
	int GetWeaponAnimState () { return m_weaponAnimState; }

	override void EEFired (int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			// MUZZLE FLASH EFFECT
			ItemBase suppressor = GetAttachedSuppressor();
			ItemBase.PlayFireParticles(this, ammoType, this, suppressor, "CfgWeapons" );
	
			if (suppressor)
			{
				ItemBase.PlayFireParticles(this, ammoType, suppressor, NULL, "CfgVehicles" );
			}
			
			// OVERHEATING EFFECT
			IncreaseOverheating(this, ammoType, this, suppressor, "CfgWeapons");
		}
		
		#ifdef DEVELOPER
		MiscGameplayFunctions.UnlimitedAmmoDebugCheck(this);
		#endif
	}
	
	void IncreaseOverheating(ItemBase weapon, string ammoType, ItemBase muzzle_owner, ItemBase suppressor, string config_to_search)
	{
		m_OverheatingShots++;
		
		if (!m_CheckOverheating)
				m_CheckOverheating = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		m_CheckOverheating.Stop();
		m_CheckOverheating.Run(OVERHEAT_EFFECT_SHOTS_DECAY_INT, this, "OnOverheatingDecay");
		
		CheckOverheating(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
	}
	
	void CheckOverheating(ItemBase weapon = null, string ammoType = "", ItemBase muzzle_owner = null, ItemBase suppressor = null, string config_to_search = "")
	{
		if (m_OverheatingShots >= OVERHEAT_EFFECT_SHOTS  &&  !IsOverheatingEffectActive())
		{
			StartOverheating(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
		}
		
		if (m_OverheatingShots < OVERHEAT_EFFECT_SHOTS  &&  IsOverheatingEffectActive())
		{
			StopOverheating(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
		}
		
		if (m_OverheatingShots > OVERHEAT_EFFECT_SHOTS_LIMIT)
		{
			m_OverheatingShots = OVERHEAT_EFFECT_SHOTS_LIMIT;
		}
	}
	
	bool IsOverheatingEffectActive()
	{
		return m_IsOverheatingEffectActive;
	}
	
	void OnOverheatingDecay()
	{
		m_OverheatingShots--;
		
		if (m_OverheatingShots == 0)
		{
			m_CheckOverheating.Stop();
		}
		else
		{
			if (!m_CheckOverheating)
				m_CheckOverheating = new Timer( CALL_CATEGORY_GAMEPLAY );
			
			m_CheckOverheating.Stop();
			m_CheckOverheating.Run(OVERHEAT_EFFECT_SHOTS_DECAY_INT, this, "OnOverheatingDecay");
		}
		
		CheckOverheating(this, "", this);
	}

	void StartOverheating(ItemBase weapon = null, string ammoType = "", ItemBase muzzle_owner = null, ItemBase suppressor = null, string config_to_search = "")
	{
		m_IsOverheatingEffectActive = true;
		ItemBase.PlayOverheatingParticles(this, ammoType, this, suppressor, "CfgWeapons" );
	}
	
	void StopOverheating(ItemBase weapon = null, string ammoType = "", ItemBase muzzle_owner = null, ItemBase suppressor = null, string config_to_search = "")
	{
		m_IsOverheatingEffectActive = false;
		ItemBase.StopOverheatingParticles(weapon, ammoType, muzzle_owner, suppressor, config_to_search);
	}
	
	void RegisterOverheatingParticle(Particle p)
	{
		if (!m_OverheatingParticles)
			m_OverheatingParticles = new array <Particle>;
		
		m_OverheatingParticles.Insert(p);
	}
	
	void KillAllOverheatingParticles()
	{
		if (m_OverheatingParticles)
		{
			for (int i = m_OverheatingParticles.Count(); i > 0; i--)
			{
				int id = i - 1;
				Particle p = m_OverheatingParticles.Get(id);
				
				if (p)
				{
					p.Stop();
					m_OverheatingParticles.Remove(id);
				}
			}
			
			delete m_OverheatingParticles;
		}
	}
	
	void SyncSelectionState (bool has_bullet, bool has_mag)
	{					
		if (has_bullet)
			SelectionBulletShow();
		else
			SelectionBulletHide();
	
		if (has_mag)
			SelectionMagazineShow();
		else
			SelectionMagazineHide();
	}

	void OnStableStateEntry ()
	{
		PlayerBase p = PlayerBase.Cast(GetHierarchyParent());
		if (p && p.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			int mi = GetCurrentMuzzle();

			float ammoDamage = 0.0;
			string ammoTypeName;
			GetCartridgeInfo(mi, ammoDamage, ammoTypeName);

			InventoryLocation loc = new InventoryLocation;
			Magazine mag = GetMagazine(mi);
			if (mag)
				mag.GetInventory().GetCurrentInventoryLocation(loc);

			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;

			ctx.Write(INPUT_UDT_WEAPON_REMOTE_SYNC);
			ctx.Write(GetCurrentStateID());
			ctx.Write(ammoDamage);
			ctx.Write(ammoTypeName);
			loc.WriteToContext(ctx);

			p.StoreInputForRemotes(ctx);
			
		}
		if (p)
		{
			p.OnWeaponActionEnd();
		}
		//MWBREAK
	}

	void OnSyncFromRemote (ParamsReadContext ctx)
	{
		if (m_receivedSyncFromRemote)
		{
			wpnDebugSpam("[wpnfsm] already initialized, ignoring sync-from-remote packet");
			return;
		}
		else
		{
			wpnDebugSpam("[wpnfsm] first sync-from-remote packet");
			m_receivedSyncFromRemote = true;

			int currStateID = -1;
			if (!ctx.Read(currStateID))
			{
				Error("Weapon::SyncRemote - cannot read currStateID!");
				return;
			}

			float ammoDamage = 0.0;
			if (!ctx.Read(ammoDamage))
			{
				Error("Weapon::SyncRemote - cannot read ammoDamage!");
				return;
			}
			string ammoTypeName;
			if (!ctx.Read(ammoTypeName))
			{
				Error("Weapon::SyncRemote - cannot read ammoTypeName!");
				return;
			}

			InventoryLocation loc = new InventoryLocation;
			if (!loc.ReadFromContext(ctx))
			{
				Error("Weapon::SyncRemote - cannot read mag!");
				return;
			}

			if (currStateID != GetCurrentStateID())
			{
				wpnDebugPrint("[wpnfsm] first sync-from-remote packet and state is different, syncing!");
				NetSyncCurrentStateID(currStateID);

				int mi = GetCurrentMuzzle();
				LoadChamber(mi, ammoDamage, ammoTypeName);

				if (loc.IsValid())
				{
					Magazine mag = Magazine.Cast(loc.GetItem());
					InventoryLocation src = new InventoryLocation;
					mag.GetInventory().GetCurrentInventoryLocation(src);

					GameInventory.LocationSyncMoveEntity(src, loc);
				}
				else
					Error("Weapon::SyncRemote - inventory location of mag is not valid!");
			}
		}
	}

	override void OnStoreLoad (ParamsReadContext ctx)
	{
		super.OnStoreLoad(ctx);
		if (m_fsm)
		{
			m_fsm.OnStoreLoad(ctx);
		}
		else
		{
			int dummy = 0;
			ctx.Read(dummy);
		}
	}
	
	void AfterStoreLoad ()
	{
		if (m_fsm)
		{
			int mi = GetCurrentMuzzle();
			Magazine mag = GetMagazine(mi);
			bool has_mag = mag != null;
			bool has_bullet = !IsChamberEmpty(mi);
			SyncSelectionState(has_bullet, has_mag);
		}
	}

	override void OnStoreSave (ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		if (m_fsm)
			m_fsm.OnStoreSave(ctx);
		else
		{
			int dummy = 0;
			ctx.Write(dummy);
		}
	}

	/**@fn		GetCurrentStateID
	 * @brief	tries to return identifier of current stable state
	 **/
	int GetCurrentStateID ()
	{
		if (m_fsm)
			return m_fsm.GetCurrentStateID();
		return 0;
	}

	/**@fn		RandomizeFSMState
	 * @brief	Engine callback - loot randomization of FSM's state. not intended to direct use.
	 **/
	void RandomizeFSMState ()
	{
		if (m_fsm)
		{
			int mi = GetCurrentMuzzle();
			Magazine mag = GetMagazine(mi);
			bool has_mag = mag != null;
			bool has_bullet = !IsChamberEmpty(mi);
			bool has_jam = IsChamberJammed(mi);
			m_fsm.RandomizeFSMState(has_bullet, has_mag, has_jam);
			SyncSelectionState(has_bullet, has_mag);
		}
	}

	/**@fn		NetSyncCurrentStateID
	 * @brief	Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	void NetSyncCurrentStateID (int id)
	{
		if (m_fsm)
		{
			m_fsm.NetSyncCurrentStateID(id);
			
			WeaponStableState state = WeaponStableState.Cast(GetCurrentState());
			if (state)
			{
				SyncSelectionState(state.HasBullet(), state.HasMagazine());
				state.SyncAnimState();
			}
		}
	}

	/**
	 * @brief	Load posible actions for wepon from config
	 **/
	override void SetUserActions ()
	{
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " ContinuousActions", m_ContinuousActions);
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " SingleUseActions", m_SingleUseActions);
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " InteractActions", m_InteractActions);
	}
	/**
	 * @brief Returns number of slots for attachments corrected for weapons
	 **/
	override int GetSlotsCountCorrect ()
	{
		int ac = GetInventory().AttachmentCount();
		int	sc = GetInventory().GetSlotsCount() + GetMuzzleCount();
		if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
		return sc;
	};

	PropertyModifiers GetPropertyModifierObject () { return m_PropertyModifierObject; }

	bool CreatePropertyModifierObject ()
	{
		if (!m_PropertyModifierObject)
		{
			m_PropertyModifierObject = new PropertyModifiers(this);
			return true;
		}
		return false;
	}

	bool RemovePropertyModifierObject ()
	{
		if (m_PropertyModifierObject)
		{
			delete m_PropertyModifierObject;
			return true;
		}
		return false;
	}
	
	void SetRecoilSeed (int seed) { m_RecoilSeed = seed; }
	int GetRecoilSeed () { return m_RecoilSeed; }

	override void OnInventoryEnter (Man player)
	{
		super.OnInventoryEnter(player);

		CreatePropertyModifierObject();
	}
	
	override void OnInventoryExit (Man player)
	{
		super.OnInventoryExit(player);

		RemovePropertyModifierObject();
	}

	override void EEItemAttached (EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if ( GetPropertyModifierObject() ) GetPropertyModifierObject().UpdateModifiers();

		Magazine mag;
		if( !Magazine.CastTo(mag,item) )
			return;

		PlayerBase player;
		if ( PlayerBase.CastTo(player, GetHierarchyParent()) )
			if ( player.GetItemInHands() == this )
				if ( player.GetWeaponManager().IsRunning() )
					return;

		float damage;
		string type;
		if ( IsChamberEmpty( GetCurrentMuzzle()) )
		{
			if ( mag.LocalAcquireCartridge(damage, type) )
			{
				//SelectionBulletShow();
				LoadChamber( GetCurrentMuzzle(), damage, type );
			}
		}

		RandomizeFSMState();
	}

	override void EEItemDetached (EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if ( GetPropertyModifierObject() ) GetPropertyModifierObject().UpdateModifiers();

		Magazine mag;
		if( !Magazine.CastTo(mag,item) )
			return;

		PlayerBase player;
		if ( PlayerBase.CastTo(player,GetHierarchyParent()) )
		{
			if( player.GetItemInHands() == this )
			{
				if(player.GetWeaponManager().IsRunning())
					return;
			}
		}

		RandomizeFSMState();
	}

	bool IsRemoteWeapon ()
	{
		InventoryLocation il = new InventoryLocation;
		if (GetInventory().GetCurrentInventoryLocation(il))
		{
			EntityAI parent = il.GetParent();
			DayZPlayer dayzp = DayZPlayer.Cast(parent);
			if (il.GetType() == InventoryLocationType.HANDS && dayzp)
			{
				bool remote = dayzp.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_REMOTE;
				return remote;
			}
		}
		return true;
	}

	void OnEventFromRemote (ParamsReadContext ctx)
	{
		WeaponEventBase e = CreateWeaponEventFromContext(ctx);
		if (e)
		{
			PlayerBase player = PlayerBase.Cast(GetHierarchyParent());
			player.GetWeaponManager().SetRunning(true);

			fsmDebugSpam("[wpnfsm] recv event from remote: created event=" + e);
			if(e.GetEventID() == WeaponEventID.HUMANCOMMAND_ACTION_ABORTED)
			{
				ProcessEventResult aa;
				m_fsm.ProcessAbortEvent(e, aa);
			}
			else
			{
				m_fsm.ProcessEvent(e);
			}
			player.GetWeaponManager().SetRunning(false);
		}
	}

	void SyncEventToRemote (WeaponEventBase e)
	{
		DayZPlayer p = DayZPlayer.Cast(GetHierarchyParent());
		if (p && p.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;

			ctx.Write(INPUT_UDT_WEAPON_REMOTE_EVENT);
			e.WriteToContext(ctx);

			//wpnDebugPrint("[wpnfsm] send 2 remote: sending e=" + e + " id=" + e.GetEventID() + " p=" + e.m_player + "  m=" + e.m_magazine);
			p.StoreInputForRemotes(ctx);
		}
	}

	RecoilBase SpawnRecoilObject ()
	{
		return new DefaultRecoil(this);
	}

	int GetWeaponSpecificCommand (int weaponAction, int subCommand) { return subCommand; }

	bool CanFire ()
	{
		if (!IsChamberEmpty(GetCurrentMuzzle()) && !IsChamberFiredOut(GetCurrentMuzzle()) && !IsChamberJammed(GetCurrentMuzzle()))
			return true;
		return false;
	}
	
	override int GetItemWeight()
	{
		int AttachmentWeight = 0;
		float item_wetness = GetWet();
		int AttachmentsCount = GetInventory().AttachmentCount();
		int ConfWeight = ConfigGetInt("weight");
		
		for (int i = 0; i < AttachmentsCount; i++)
		{
			AttachmentWeight += GetInventory().GetAttachmentFromIndex(i).GetItemWeight();
		}
		
		return Math.Round((item_wetness + 1) * (ConfWeight + AttachmentWeight));
	}
};

