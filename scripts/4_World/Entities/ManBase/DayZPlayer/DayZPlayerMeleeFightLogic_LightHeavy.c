//! Light / Heavy punches

class DayZPlayerMeleeFightLogic_LightHeavy
{
	protected DayZPlayerImplement					m_DZPlayer;
	protected ref DayZPlayerImplementMeleeCombat	m_MeleeCombat;
	protected MeleeCombatHit						m_HitMask;

	protected bool									m_ComboRangeMissed;
	protected bool									m_RateLimited;
	protected bool									m_IsInBlock;

	void DayZPlayerMeleeFightLogic_LightHeavy(DayZPlayerImplement player)
	{
		Init(player);
	}
	
	void Init(DayZPlayerImplement player)
	{
		m_DZPlayer 			= player;
		m_MeleeCombat		= m_DZPlayer.GetMeleeCombat();

		m_ComboRangeMissed 	= false;
		m_RateLimited 		= true;
		m_IsInBlock 		= false;
		m_HitMask			= MeleeCombatHit.NONE;
	}

	void ~DayZPlayerMeleeFightLogic_LightHeavy() {}

	bool IsInBlock()
	{
		return m_IsInBlock;
	}


	MeleeCombatHit GetAttackTypeFromInputs(HumanInputController pInputs)
	{
		if (pInputs.IsMeleeFastAttackModifier() && m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
		{
			return MeleeCombatHit.HEAVY;
		}
		else
		{
			return MeleeCombatHit.LIGHT;
		}
	}

	
	
	bool Process(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState)
	{
		InventoryItem itemInHands = InventoryItem.Cast(pEntityInHands);

		if (pInputs.IsUseButtonDown())
		{
			EntityAI target;

			bool isWeapon = itemInHands && itemInHands.IsInherited(Weapon);
			bool isNotMeleeWeapon = itemInHands && !itemInHands.IsMeleeWeapon(); // TODO: allowed for everything that is not disabled in config (primarily for anim testing)

			if (isWeapon)
			{
				//! no melee for firearm weapons right now - will be connected later
				return false;
			}

			//! if the item in hands cannot be used as melee weapon
			if (isNotMeleeWeapon)
			{
				return false;
			}

			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				//! first attack in raised erc (light or heavy if modifier is used)
				if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
				{
					m_HitMask = GetAttackTypeFromInputs(pInputs);
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					target = m_MeleeCombat.GetTargetObject();
					m_DZPlayer.StartCommand_Melee2(target, m_HitMask == MeleeCombatHit.HEAVY);
					if(m_HitMask == MeleeCombatHit.HEAVY)
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
					return true;
				}
				//! kick from raised pne
				else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
				{
					m_HitMask = MeleeCombatHit.KICK;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					target = m_MeleeCombat.GetTargetObject();
					m_DZPlayer.StartCommand_Melee2(target, false);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					return true;
				}
				//! sprint attack in erc stance
				else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && m_DZPlayer.IsSprintFull())
				{
					m_HitMask = MeleeCombatHit.SPRINT;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					target = m_MeleeCombat.GetTargetObject();
					m_DZPlayer.StartCommand_Melee2(target, false);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					return true;
				}
			}
			//! combo hits - when we are already in Melee command and clicking UseButton
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
			{
				HumanCommandMelee2 hmc2 = m_DZPlayer.GetCommand_Melee2();

				// if(m_RateLimited || m_ComboRangeMissed)
				//	return true;

				if (hmc2 && hmc2.IsInComboRange())
				{
					//! select if the next attack will light or heavy (based on key modifier)
					m_HitMask = GetAttackTypeFromInputs(pInputs);					
					if (m_HitMask == MeleeCombatHit.HEAVY)
					{
						hmc2.ContinueCombo(true);
    					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					}
					else
					{
						hmc2.ContinueCombo(false);
    					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
					}

					return true;
				}
				else
				{
					m_ComboRangeMissed = true;
					return true;
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			HumanCommandMelee2 hmc2a = m_DZPlayer.GetCommand_Melee2();
			if (hmc2a)
			{
				//! on anim event Hit received
				if (hmc2a.WasHit())
				{
					//! re-target (enemy can be already away)
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					//! procees hit - selection of cfg 'ammo' type
					m_MeleeCombat.ProcessHit();

					//! reset - prepared for next hit
					m_MeleeCombat.SetTargetObject(null);
					m_MeleeCombat.SetHitZoneIdx(-1);
					m_ComboRangeMissed = false;
					//m_RateLimited = false;
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{			
			HumanCommandMove cm = m_DZPlayer.GetCommand_Move();


			int 	roll = pInputs.IsMeleeLREvade();

			//! evades in raised erc stance while moving
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT && roll != 0)
			{
				//! not enough stamina to do evades
				if (!m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE)) return false;

				float angle;
				if (roll == 1)
				{
					angle = -90;	// left
				}
				else
				{
					angle = 90;		// right
				}

				// start melee evade
				cm.StartMeleeEvadeA(angle);
				m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
			}


			//! stand up when crouching and raised pressed
			if (pInputs.IsWeaponRaised() && pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				// Print(pMovementState.m_iStanceIdx);
				cm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
			}

			//! blocks in raised erc/pro stance
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT || pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				float angle2;

				if (cm.GetCurrentInputAngle(angle2) && (angle2 < -130.0 || angle2 > 130))
				{
					cm.SetMeleeBlock(true);
					m_IsInBlock = true;
				}
				else
				{
					cm.SetMeleeBlock(false);
					m_IsInBlock = false;
				}
			}
			else
			{
				cm.SetMeleeBlock(false);
				m_IsInBlock = false;
			}
		}

		return false;
	}
}