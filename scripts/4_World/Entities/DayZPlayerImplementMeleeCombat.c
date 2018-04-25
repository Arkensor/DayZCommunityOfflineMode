enum MeleeCombatHit
{
	NONE = -1,

	LIGHT,
	HEAVY,
	SPRINT,
	KICK,
}

class DayZPlayerImplementMeleeCombat
{
	// target selection
	private const float				TARGETING_ANGLE_NORMAL	= 30.0;
	private const float				TARGETING_ANGLE_SPRINT	= 15.0;
	private const float				TARGETING_MIN_HEIGHT	= -2.0;
	private const float				TARGETING_MAX_HEIGHT	= 2.0;
	private const float				TARGETING_RAY_RADIUS	= 0.15;
	private const float				TARGETING_RAY_DIST		= 5.0;

	private const float 			RANGE_EXTENDER_NORMAL	= 0.65;
	private const float 			RANGE_EXTENDER_SPRINT	= 1.35;
	
	// members
	private DayZPlayerImplement		m_DZPlayer;
	
	private Object					m_TargetObject;
	private ref array<Object> 		m_AllTargetObjects;
	private ref array<typename>		m_TargetableObjects;

	private InventoryItem			m_Weapon;
	private int						m_WeaponMode;
	private float					m_WeaponRange;
	
	private bool 					m_SprintAttack;
	
	private vector 					m_RayStart;
	private vector	 				m_RayEnd;	

	private int 					m_HitZoneIdx;
	private string					m_HitZoneName;
	private vector					m_HitPositionWS;
	
	protected MeleeCombatHit		m_HitMask;

	// ------------------------------------------------------------
	// CONSTRUCTOR
	// ------------------------------------------------------------

	void DayZPlayerImplementMeleeCombat(DayZPlayerImplement player)
	{
		Init(player);
	}
	
	void Init(DayZPlayerImplement player)
	{
		m_DZPlayer 		= player;
		
		m_HitZoneName	= "";
		m_HitZoneIdx 	= -1;
		m_HitPositionWS = "-1 -1 -1";
		
		m_TargetObject      = NULL;
		m_AllTargetObjects 	= new array<Object>;
		m_TargetableObjects = new array<typename>;
		m_TargetableObjects.Insert(DayZPlayer);
		m_TargetableObjects.Insert(DayZInfected);
		m_TargetableObjects.Insert(DayZAnimal);
	}

	void ~DayZPlayerImplementMeleeCombat() {}

	// ------------------------------------------------------------
	// PUBLIC
	// ------------------------------------------------------------

	MeleeCombatHit GetHitMask()
	{
		return m_HitMask;
	}

	void SetTargetObject(Object target)
		{ m_TargetObject = target; }
	
	void SetHitZoneIdx(int hitZone)
		{ m_HitZoneIdx = hitZone; }
	
	EntityAI GetTargetObject()
	{
		EntityAI target;
		Class.CastTo(target, m_TargetObject);

		return target;
	}

	void Update(InventoryItem weapon, MeleeCombatHit hitMask)
	{
		m_Weapon = weapon;
		m_HitMask = hitMask;
		m_SprintAttack = (hitMask & MeleeCombatHit.SPRINT) == MeleeCombatHit.SPRINT;
		m_WeaponMode = SelectWeaponMode(weapon);
		m_WeaponRange = GetWeaponRange(weapon, m_WeaponMode);
		m_AllTargetObjects.Clear();

		if( !GetGame().IsMultiplayer() || !GetGame().IsServer() )
		{
			if( !ScriptInputUserData.CanStoreInputUserData() )
			{
				Error("DayZPlayerImplementMeleeCombat - ScriptInputUserData already posted");
				return;
			}

			TargetSelection();
			HitZoneSelection();

			//! store target into input packet
			if( GetGame().IsMultiplayer() )
			{
				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write(INPUT_UDT_MELEE_TARGET);
				ctx.Write(m_TargetObject);
				ctx.Write(m_HitZoneIdx);
				ctx.Send();
			}
		}
	}
	
	void ProcessHit()
	{
		if( m_TargetObject )
		{
			bool noDamage = false;
			string ammo = "Dummy_Light";
			vector hitPosWS = m_HitPositionWS;
			vector modelPos = m_TargetObject.WorldToModel(m_HitPositionWS);
			PlayerBase targetedPlayer = NULL;
			
			//! sound
			if( GetGame().IsMultiplayer() && GetGame().IsServer() ) // HOTFIX for old melee hit sounds in MP - has to be redone
			{
				hitPosWS = m_TargetObject.GetPosition();
			}

			if(m_HitZoneIdx == -1) noDamage = true;

			//! Melee Hit/Impact modifiers
			if( m_TargetObject.IsInherited(DayZPlayer) )
			{
				if( Class.CastTo(targetedPlayer, m_TargetObject) )
				{
					//! if the oponnent is in Melee Block decrease the damage
					if( targetedPlayer.GetMeleeFightLogic() && targetedPlayer.GetMeleeFightLogic().IsInBlock())
					{
						if( m_WeaponMode > 0 )
							m_WeaponMode--; // Heavy -> Light
						else
							noDamage = true;
					}
				}
			}
			
			//! in case of kick (on back or push from erc) change the ammo type to dummy 
			if((m_HitMask & MeleeCombatHit.KICK) == MeleeCombatHit.KICK)
			{
				ammo = "Dummy_Heavy";
				noDamage = true;
			}

			if(!noDamage)
			{
				//! normal hit with applied damage
				m_DZPlayer.ProcessMeleeHit(m_Weapon, m_WeaponMode, m_TargetObject, m_HitZoneIdx, hitPosWS);
			}
			else
			{
				//! play hit animation for dummy hits
				if( GetGame().IsServer() && targetedPlayer )
				{
					targetedPlayer.EEHitBy(null, 0, EntityAI.Cast(m_DZPlayer), m_HitZoneName, ammo, modelPos);
				}
			}
		}
	}
			
	// ------------------------------------------------------------
	// PRIVATE
	// ------------------------------------------------------------

	private int SelectWeaponMode(InventoryItem weapon)
	{
		if( weapon )
		{
			switch(m_HitMask)
			{
				case MeleeCombatHit.LIGHT :
					return weapon.GetMeleeMode();
				break;
				case MeleeCombatHit.HEAVY :
					return weapon.GetMeleeHeavyMode();
				break;
				case MeleeCombatHit.SPRINT :
					return weapon.GetMeleeSprintMode();
				break;
			}
		}

		switch(m_HitMask)
		{
			case MeleeCombatHit.HEAVY :
				return 1;
			break;
			case MeleeCombatHit.SPRINT :
				return 2;
			break;
		}
		return 0;
	}
	
	private float GetWeaponRange(InventoryItem weapon, int weaponMode)
	{
		if( weapon )
		{
			return weapon.GetMeleeCombatData().GetModeRange(weaponMode);
		}
		else
		{
			return m_DZPlayer.GetMeleeCombatData().GetModeRange(weaponMode);
		}
	}

	private void TargetSelection()
	{
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		vector pos = m_DZPlayer.GetPosition();
		vector dir = MiscGameplayFunctions.GetHeadingVector(player);

		float dist = m_WeaponRange + RANGE_EXTENDER_NORMAL;
		float tgtAngle = TARGETING_ANGLE_NORMAL;
		if (m_SprintAttack)
		{
			dist = m_WeaponRange + RANGE_EXTENDER_SPRINT;
			tgtAngle = TARGETING_ANGLE_SPRINT;
		}

		m_TargetObject = DayZPlayerUtils.GetMeleeTarget(pos, dir, tgtAngle, dist, TARGETING_MIN_HEIGHT, TARGETING_MAX_HEIGHT, m_DZPlayer, m_TargetableObjects, m_AllTargetObjects);

		if(IsObstructed(m_TargetObject))
			m_TargetObject = NULL;
	}

	private void HitZoneSelection()
	{
		Object cursorTarget = NULL;
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);

		// ray properties 
		vector pos;
		vector cameraDirection = GetGame().GetCurrentCameraDirection();

		MiscGameplayFunctions.GetHeadBonePos(player, pos);
		m_RayStart = pos;
		m_RayEnd = pos + cameraDirection * TARGETING_RAY_DIST;

		// raycast
		vector hitPos;
		vector hitNormal;	
		ref set<Object> hitObjects = new set<Object>;

		if ( DayZPhysics.RaycastRV( m_RayStart, m_RayEnd, m_HitPositionWS, hitNormal, m_HitZoneIdx, hitObjects, NULL, player, false, false, ObjIntersectIFire, TARGETING_RAY_RADIUS ) )
		{
			if( hitObjects.Count() )
			{
				cursorTarget = hitObjects.Get(0);
				if ( cursorTarget == m_TargetObject )
					m_HitZoneName = cursorTarget.GetDamageZoneNameByComponentIndex(m_HitZoneIdx);
			}
		}
		else
			m_HitZoneIdx = -1;
	}

	private bool IsObstructed(Object object)
	{
		// check direct visibility of object (obstruction check)
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.VEHICLE|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.ITEM_SMALL|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		int hitComponentIndex;
		float hitFraction;
		vector start, end, hitNormal, hitPosObstructed;
		Object hitObject = NULL;
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);

		if(object)
		{
			MiscGameplayFunctions.GetHeadBonePos(player, start);
			end = start + MiscGameplayFunctions.GetHeadingVector(player) * vector.Distance(player.GetPosition(), object.GetPosition());

			return DayZPhysics.RayCastBullet( start, end, collisionLayerMask, hitObject, hitPosObstructed, hitNormal, hitFraction);
		}

		return false;
	}
	
#ifdef DEVELOPER
	// ------------------------------------------------------------
	// DEBUG
	// ------------------------------------------------------------
	private ref array<Shape> dbgConeShapes = new array<Shape>();
	private ref array<Shape> dbgTargets = new array<Shape>();
	private ref array<Shape> hitPosShapes = new array<Shape>();
	
	void Debug(InventoryItem weapon, MeleeCombatHit hitMask)
	{
		bool show_targets = DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_SHOW_TARGETS);
		bool draw_targets = DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DRAW_TARGETS);
		bool draw_range = DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DRAW_RANGE);

		if( show_targets || draw_targets || draw_range )
			Update(weapon, hitMask);

		ShowDebugMeleeTarget(show_targets);
		DrawDebugTargets(show_targets);
		DrawDebugMeleeHitPosition(draw_targets);
		DrawDebugMeleeCone(draw_range);
	}

	void SetBlockingStance(bool enabled)
	{
		HumanInputController hic = m_DZPlayer.GetInputController();
		HumanCommandMove hcm = m_DZPlayer.GetCommand_Move();
		if(enabled)
		{
			if(hic)
			{
				hic.OverrideMovementSpeed(true, 2);
				hic.OverrideMovementAngle(true, 180);
			}
			if(hcm)
			{
				hcm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
				hcm.SetMeleeBlock(true);
			}
		}
		else
		{
			if(hcm)
			{
				hcm.SetMeleeBlock(false);
				hcm.ForceStance(DayZPlayerConstants.STANCEIDX_ERECT);
				hcm.ForceStance(-1);
			}
			if(hic)
			{
				hic.OverrideMovementSpeed(false, 0);
				hic.OverrideMovementAngle(false, 0);
			}
		}
	}

	void SetMeleeFight(bool enabled)
	{
		HumanInputController hic = m_DZPlayer.GetInputController();
		HumanCommandMove hcm = m_DZPlayer.GetCommand_Move();
		HumanCommandMelee hcme = m_DZPlayer.GetCommand_Melee();

		if(enabled)
		{
			if(hic)
			{
				if(hcm)
					hcm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
				hic.OverrideRaise(true, true);
			}
		}
		else
		{
			if(hic)
			{
				hic.OverrideMovementSpeed(false, 0);
				hic.OverrideMovementAngle(false, 0);
				hic.OverrideRaise(true, false);
			}
		}
	}

	//! shows target in DbgUI 'window'
	private void ShowDebugMeleeTarget(bool enabled)
	{
		int windowPosX = 0;
		int windowPosY = 500;

		DbgUI.BeginCleanupScope();
		DbgUI.Begin("Melee Target", windowPosX, windowPosY);
		if (enabled )
		{
			if ( m_TargetObject && m_TargetObject.IsMan() )
			{
				DbgUI.Text("Character: " + m_TargetObject.GetDisplayName());
				DbgUI.Text("HitZone: " + m_HitZoneName);
				DbgUI.Text("HitPosWS:" + m_HitPositionWS);
			}
		}
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}

	//! shows debug sphere above the target
	private void DrawDebugTargets(bool enabled)
	{
		vector w_pos, w_pos_sphr, w_pos_lend;
		Object obj;

		if ( enabled )
		{
			CleanupDebugShapes(dbgTargets);

			for (int i = 0; i < m_AllTargetObjects.Count(); i++ )
			{
				if ( m_TargetObject && m_AllTargetObjects.Count() )
				{
					obj = m_AllTargetObjects.Get(i);
					w_pos = obj.GetPosition();
					// sphere pos tweaks
					w_pos_sphr = w_pos;
					w_pos_sphr[1] = w_pos_sphr[1] + 1.8;
					// line pos tweaks
					w_pos_lend = w_pos;
					w_pos_lend[1] = w_pos_lend[1] + 1.8;
					
					if ( m_AllTargetObjects.Get(i) == m_TargetObject )
					{
						dbgTargets.Insert( Debug.DrawSphere(w_pos_sphr, 0.05, COLOR_RED, ShapeFlags.NOOUTLINE) );
						dbgTargets.Insert( Debug.DrawLine(w_pos, w_pos_lend, COLOR_RED) );
					}
					else
					{
						dbgTargets.Insert( Debug.DrawSphere(w_pos_sphr, 0.05, COLOR_YELLOW, ShapeFlags.NOOUTLINE) );
						dbgTargets.Insert( Debug.DrawLine(w_pos, w_pos_lend, COLOR_YELLOW) );
					}
				}
			}
		}
		else
			CleanupDebugShapes(dbgTargets);
	}
		
	private void DrawDebugMeleeCone(bool enabled)
	{
		// "cone" settings
		vector start, end, endL, endR;
		float playerAngle;
		float xL,xR,zL,zR;
		float dist = m_WeaponRange + RANGE_EXTENDER_NORMAL;
		float tgtAngle = TARGETING_ANGLE_NORMAL;
		
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		if (m_SprintAttack)
		{
			dist = m_WeaponRange + RANGE_EXTENDER_SPRINT;
			tgtAngle = TARGETING_ANGLE_SPRINT;
		}

		if (enabled)
		{
			CleanupDebugShapes(dbgConeShapes);

			start = m_DZPlayer.GetPosition();
			playerAngle = MiscGameplayFunctions.GetHeadingAngle(player);
			
			endL = start;
			endR = start;
			xL = dist * Math.Cos(playerAngle + Math.PI_HALF + tgtAngle * Math.DEG2RAD); // x
			zL = dist * Math.Sin(playerAngle + Math.PI_HALF + tgtAngle * Math.DEG2RAD); // z
			xR = dist * Math.Cos(playerAngle + Math.PI_HALF - tgtAngle * Math.DEG2RAD); // x
			zR = dist * Math.Sin(playerAngle + Math.PI_HALF - tgtAngle * Math.DEG2RAD); // z
			endL[0] = endL[0] + xL;
			endL[2] = endL[2] + zL;
			endR[0] = endR[0] + xR;
			endR[2] = endR[2] + zR;

			dbgConeShapes.Insert( Debug.DrawLine(start, endL, COLOR_BLUE ) );
			dbgConeShapes.Insert( Debug.DrawLine(start, endR, COLOR_BLUE) ) ;
			dbgConeShapes.Insert( Debug.DrawLine(endL, endR, COLOR_BLUE  ) );
		}
		else
			CleanupDebugShapes(dbgConeShapes);		
	}	

	private void DrawDebugMeleeHitPosition(bool enabled)
	{
		if (enabled && m_TargetObject)
		{
			CleanupDebugShapes(hitPosShapes);
			hitPosShapes.Insert( Debug.DrawSphere(m_HitPositionWS, TARGETING_RAY_RADIUS, COLOR_YELLOW, ShapeFlags.NOOUTLINE|ShapeFlags.TRANSP) );
		}
		else
			CleanupDebugShapes(hitPosShapes);
	}

	private void CleanupDebugShapes(array<Shape> shapes)
	{
		for ( int it = 0; it < shapes.Count(); ++it )
		{
			Debug.RemoveShape( shapes[it] );
			shapes.Remove(it);
		}
	}
#endif
}