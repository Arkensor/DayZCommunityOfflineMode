enum DayZInfectedConstants
{
	//! anim commands
	COMMANDID_MOVE,
	COMMANDID_VAULT,
	COMMANDID_DEATH,
	COMMANDID_HIT,
	COMMANDID_ATTACK,
	
	//! mind states
	MINDSTATE_CALM,
	MINDSTATE_DISTURBED,
	MINDSTATE_ALERTED,
	MINDSTATE_CHASE,
	MINDSTATE_FIGHT
};

static int death = 0;
class DayZInfectedCommandMove
{
	proto native void SetStanceVariation(int pStanceVariation);
	proto native void SetIdleState(int pIdleState);
}

class DayZInfectedCommandVault
{
	proto native bool WasLand();
}

class DayZInfectedCommandAttack
{
	proto native bool WasHit();
}

class DayZInfected extends DayZCreatureAI
{	
	proto native DayZInfectedType GetDayZInfectedType();
	proto native DayZInfectedInputController GetInputController();	
	proto native DayZInfectedCommandMove StartCommand_Move();	
	proto native DayZInfectedCommandVault StartCommand_Vault(int pType);	
	proto native void StartCommand_Death();
	proto native void StartCommand_Hit(float pType, int pDirection);
	proto native DayZInfectedCommandAttack StartCommand_Attack(EntityAI pTarget, int pType, float pSubtype);
	
	proto native bool CanAttackToPosition(vector pTargetPosition);
	
	proto native DayZInfectedCommandMove GetCommand_Move();
	proto native DayZInfectedCommandVault GetCommand_Vault();
	proto native DayZInfectedCommandAttack GetCommand_Attack();
	
	//! server / singleplayer properties
	private int m_StanceVariation = 0;
	private int m_LastMindState = -1;
	
	private bool m_KnuckleLand = false;
	private float m_KnuckleOutTimer = 0;
	
	//-------------------------------------------------------------
	void DayZInfected()
	{
		SetEventMask(EntityEvent.INIT);
	}
	
	//-------------------------------------------------------------
	void ~DayZInfected()
	{
#ifdef PLATFORM_XBOX
		StaroyeInfectedRespawner.Event_OnInfectedDespawn(this);
#endif
	}
	
	//-------------------------------------------------------------
	override void EOnInit(IEntity other, int extra)
	{
		if( !GetGame().IsMultiplayer() || GetGame().IsServer() )
		{
			m_StanceVariation = Math.RandomInt(0, 4);
	
			DayZInfectedCommandMove moveCommand = GetCommand_Move();
			moveCommand.SetStanceVariation(m_StanceVariation);
		}
		
#ifdef PLATFORM_XBOX
		StaroyeInfectedRespawner.Event_OnInfectedSpawn(this);
#endif
	}
	
	override void EEKilled( Object killer )
	{
		super.EEKilled(killer);
		
#ifdef PLATFORM_XBOX
		StaroyeInfectedRespawner.Event_OnInfectedDespawn(this);
#endif
	}
	
	//-------------------------------------------------------------
	override AnimBootsType GetBootsType()
	{
		return AnimBootsType.Boots;
	}
	
	override bool CanBeSkinned()
	{
		return false;
	}
	//-------------------------------------------------------------
	override bool IsHealthVisible()
	{
		return false;
	}
	
	//-------------------------------------------------------------
	//!
	//! CommandHandler
	//! 

	void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//! handle death
		if( HandleDeath(pCurrentCommandID) )
		{
			return;
		}
		
		//! handle finished commands
		if (pCurrentCommandFinished)
		{			
			//! default behaviour after finish is to start move
			DayZInfectedCommandMove moveCommand = StartCommand_Move();
			moveCommand.SetStanceVariation(m_StanceVariation);
			
			return;
		};

		//! damage hits
		if( HandleDamageHit(pCurrentCommandID) )
		{
			return;
		}
		
		DayZInfectedInputController inputController = GetInputController();
		if( inputController )
		{
			if( HandleVault(pCurrentCommandID, inputController, pDt) )
			{
				return;
			}
			
			if( HandleMindStateChange(pCurrentCommandID, inputController, pDt) )
			{
				return;
			}
			
			if( FightLogic(pCurrentCommandID, inputController, pDt) )
			{
				return;
			} 
		}
	}
	
	//-------------------------------------------------------------
	//!
	//! HandleDeath
	//! 

	bool HandleDeath(int pCurrentCommandID)
	{
		if( pCurrentCommandID == DayZInfectedConstants.COMMANDID_DEATH )
		{
			return true;
		}

		if( !IsAlive() )
		{
			StartCommand_Death();
			death++;
			return true;
		}

		return false;
	}	
	
static int getDeath(){
	return death;
}

static void resetDeath(){
	death = 0;
}
	
	//-------------------------------------------------------------
	//!
	//! HandleVault
	//! 
	
	int m_ActiveVaultType = -1;
	
	int GetVaultType(float height)
	{
		if( height <= 0.6 )
			return 0;
		else if( height <= 1.1 )
			return 1;
		else if( height <= 1.6 )
			return 2;
		else 
			return 3;
	}
	
	bool HandleVault(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if( pCurrentCommandID == DayZInfectedConstants.COMMANDID_VAULT )
		{
			DayZInfectedCommandVault vaultCmd = GetCommand_Vault();
			if( vaultCmd && vaultCmd.WasLand() )
			{
				m_KnuckleOutTimer = 0;
				m_KnuckleLand = true;
			}
			if( m_KnuckleLand )
			{
				m_KnuckleOutTimer += pDt;
				if( m_KnuckleOutTimer > 2.0 )
					StartCommand_Vault(-1);
			}
			
			return true;
		}
		
		if( pInputController.IsVault() )
		{
			float vaultHeight = pInputController.GetVaultHeight();
			int vaultType = GetVaultType(vaultHeight);
			m_KnuckleLand = false;
			StartCommand_Vault(vaultType);
			return true; 
		}
		
		return false;
	}
	
	//-------------------------------------------------------------
	//!
	//! Mind state change
	//! 

	bool HandleMindStateChange(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		int mindState = pInputController.GetMindState();
		if( m_LastMindState != mindState )
		{
			DayZInfectedCommandMove moveCommand = GetCommand_Move();
			switch( mindState )
			{
			case DayZInfectedConstants.MINDSTATE_CALM:
				if( moveCommand )
					moveCommand.SetIdleState(0);
				break;

			case DayZInfectedConstants.MINDSTATE_DISTURBED:
				if( moveCommand )
					moveCommand.SetIdleState(1);
				break;
			
			case DayZInfectedConstants.MINDSTATE_CHASE:
				if( moveCommand && (m_LastMindState < DayZInfectedConstants.MINDSTATE_CHASE) )
					moveCommand.SetIdleState(2);
				break;
			}
			
			m_LastMindState = mindState;
			m_AttackCooldownTime = 0.0;
		}
		return false;
	}
	
	//-------------------------------------------------------------
	//!
	//! Combat
	//! 
	
	EntityAI m_ActualTarget = NULL;
	float m_AttackCooldownTime = 0;	
	DayZInfectedAttackType m_ActualAttackType = NULL;
	
	bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if( pCurrentCommandID == DayZInfectedConstants.COMMANDID_MOVE )
		{
			// we attack only in chase & fight state
			int mindState = pInputController.GetMindState();
			if( mindState == DayZInfectedConstants.MINDSTATE_CHASE )
			{
				return ChaseAttackLogic(pCurrentCommandID, pInputController, pDt);
			}
			else if( mindState == DayZInfectedConstants.MINDSTATE_FIGHT )
			{
				return FightAttackLogic(pCurrentCommandID, pInputController, pDt);
			}
		}
		else if( pCurrentCommandID == DayZInfectedConstants.COMMANDID_ATTACK )
		{
			DayZInfectedCommandAttack attackCommand = GetCommand_Attack();
			if( attackCommand && attackCommand.WasHit() )
			{
				if( m_ActualTarget != NULL )
				{
					bool blocked = false;
					PlayerBase playerTarget = PlayerBase.Cast(m_ActualTarget);
					if( playerTarget )
						blocked = playerTarget.GetMeleeFightLogic() && playerTarget.GetMeleeFightLogic().IsInBlock();

					vector targetPos = m_ActualTarget.GetPosition();
					if( !blocked && vector.DistanceSq(targetPos, this.GetPosition()) < 1.5 * 1.5 )
					{
						DamageSystem.CloseCombatDamage(this, m_ActualTarget, -1, m_ActualAttackType.m_AmmoType, targetPos);
					}
				}
			}
			
			return true;
		}
				
		return false;
	}
	
	bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();
		if( m_ActualTarget == NULL )
			return false;
	
		vector targetPos = m_ActualTarget.GetPosition();
		if( !CanAttackToPosition(targetPos) )
			return false;
		
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);
		
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
		if( m_ActualAttackType )
		{
			StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}
		
		return false;
	}
	
	bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();

		if( m_AttackCooldownTime > 0 )
		{
			m_AttackCooldownTime -= pDt;
			return false;
		}
					
		if( m_ActualTarget == NULL )
			return false;
	
		vector targetPos = m_ActualTarget.GetPosition();
		if( !CanAttackToPosition(targetPos) )
			return false;
		
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		
		int pitch = GetAttackPitch(m_ActualTarget);
		
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, targetDist, pitch);
		if( m_ActualAttackType )
		{
			StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}

		return false;
	}

	int GetAttackPitch(EntityAI target)
	{
		float attackRefPosY;
		
		// TODO: move it to some virtual method
		if( target.IsMan() )
		{
			DayZPlayer targetPlayer = NULL;			
			Class.CastTo(targetPlayer, target);
			
			// bone index should be cached in Type
			int boneIndex = targetPlayer.GetBoneIndexByName("Head");
			attackRefPosY = targetPlayer.GetBonePositionWS(boneIndex)[1];
		}
		else
		{
			attackRefPosY = target.GetPosition()[1];
		}

		// Now we have only erect stance, we need to get head position later too
		float headPosY = GetPosition()[1];
		headPosY += 1.8;
				
		float diff = Math.AbsFloat(attackRefPosY - headPosY);
		
		if( diff < 0.3 )
			return 0;
		
		if( headPosY > attackRefPosY )
			return -1;
		else
			return 1;
	}
		
	//-------------------------------------------------------------
	//!
	//! Damage hits
	//! 
	
	bool m_DamageHitToProcess = false;
	float m_DamageHitType = 0;
	int m_DamageHitDirection = 0;
	
	bool HandleDamageHit(int pCurrentCommandID)
	{
		if( m_DamageHitToProcess )
		{
			StartCommand_Hit(m_DamageHitType, m_DamageHitDirection);
			
			m_DamageHitToProcess = false;
			return true;
		}

		return pCurrentCommandID == DayZInfectedConstants.COMMANDID_HIT;
	}

	//! selects animation type and direction based on damage system data
	bool EvaluateDamageHitAnimation(EntityAI pSource, string pComponent, string ammoType, out float pAnimType, out int pAnimHitDir)
	{
		pAnimType = GetGame().ConfigGetInt("cfgAmmo " + ammoType + " hitAnimation");
				
		//! direction
		float dirAngle = ComputeHitDirectionAngle(pSource);
		pAnimHitDir = TranslateHitDirectionToDirectionIndex(dirAngle);

		if( pAnimType == 1 )
		{
			pAnimHitDir += 4;
		}
		else
		{
			if( pComponent == "Torso" ) // body
			{
				pAnimHitDir += 4;
			}			
			else if( pComponent == "Head" ) // head		
			{
			}
			else
			{
				pAnimHitDir += 8;
			}			
		}
				
		return true;
	}
	
	float ComputeHitDirectionAngle(EntityAI pSource)
	{
		vector targetDirection = GetDirection();
		vector toSourceDirection = (pSource.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		float dirAngle = Math.Acos(cosFi) * Math.RAD2DEG;
		if( cross[1] < 0 )
			dirAngle = -dirAngle;
		
		return dirAngle;
	}
	
	int TranslateHitDirectionToDirectionIndex(float dir)
	{
		if( dir >= -45 && dir <= 45 ) // front
			return 0;
		if( dir > 45 && dir < 135 ) // right
			return 2;
		if( dir > -135 && dir < -45 ) // left
			return 3;
		
		return 1; // back
	}
	
	//-------------------------------------------------------------
	//!
	//! Events from damage system
	//! 

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, ammo, modelPos);
		
		if( EvaluateDamageHitAnimation(source, component, ammo, m_DamageHitType, m_DamageHitDirection) )
		{
			m_DamageHitToProcess = true;
		}
	}
	
	override void EEHitByRemote(int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, ammo, modelPos);
	}
}