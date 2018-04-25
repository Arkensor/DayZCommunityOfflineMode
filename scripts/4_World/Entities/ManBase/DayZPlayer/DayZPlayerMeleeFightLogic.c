class DayZPlayerMeleeFightLogic
{
	protected DayZPlayerImplement					m_DZPlayer;
	protected ref DayZPlayerImplementMeleeCombat	m_MeleeCombat;
	protected MeleeCombatHit						m_HitMask;

	protected float									m_ComboHitCount;

	protected bool									m_ComboRangeMissed;
	protected bool									m_RateLimited;
	protected bool									m_IsInBlock;

	void DayZPlayerMeleeFightLogic(DayZPlayerImplement player)
	{
		Init(player);
	}
	
	void Init(DayZPlayerImplement player)
	{
		m_DZPlayer 			= player;
		m_MeleeCombat		= m_DZPlayer.GetMeleeCombat();

		m_ComboHitCount 	= 0;
		m_ComboRangeMissed 	= false;
		m_RateLimited 		= true;
		m_IsInBlock 		= false;
		m_HitMask			= MeleeCombatHit.NONE;
	}

	void ~DayZPlayerMeleeFightLogic() {}

	bool IsInBlock()
	{
		return m_IsInBlock;
	}
	
	bool Process(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState)
	{
		InventoryItem itemInHands = InventoryItem.Cast(pEntityInHands);

		if (pInputs.IsUseButtonDown())
		{
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

			EntityAI target;
			target = m_MeleeCombat.GetTargetObject();

			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				//! first attack in raised erc
				if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
				{
					m_ComboHitCount = 0;
					m_HitMask = MeleeCombatHit.LIGHT;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					m_DZPlayer.StartCommand_Melee(target);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
					return true;
				}
				//! kick from raised pne
				else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
				{
					m_ComboHitCount = 0;
					m_HitMask = MeleeCombatHit.KICK;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					m_DZPlayer.StartCommand_Melee(target);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					return true;
				}
				//! sprint attack in erc stance
				else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && pMovementState.m_iMovement == 3 && m_DZPlayer.IsSprintFull())
				{
					m_ComboHitCount = 0;
					m_HitMask = MeleeCombatHit.SPRINT;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					m_DZPlayer.StartCommand_Melee(target);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					return true;
				}
			}
			//! combo(Light, Light, Heavy hits) - when we are already in Melee command and clicking UseButton
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE)
			{
				HumanCommandMelee hmc = m_DZPlayer.GetCommand_Melee();

				//! eraly exit if combo range time window(from anim event) missed or spamming UseButton
				if(m_RateLimited || m_ComboRangeMissed)
					return true;

				//! in combo range time
				if (hmc && hmc.IsInComboRange())
				{
					//! continue with next attack
					hmc.ContinueCombo();
					//! final attack (heavy)
					if(m_ComboHitCount == 2)
					{
						m_RateLimited = true;
						m_HitMask = MeleeCombatHit.HEAVY;
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
						return true;
					}
					m_RateLimited = true;
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
					return true;
				}
				else
				{
					m_ComboRangeMissed = true;
					return true;
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE)
		{
			HumanCommandMelee hmc2 = m_DZPlayer.GetCommand_Melee();
			if (hmc2)
			{
				//! on anim event Hit received
				if (hmc2.WasHit())
				{
					//! re-target (enemy can be already away)
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					//! procees hit - selection of cfg 'ammo' type
					m_MeleeCombat.ProcessHit();

					//! when the attack was finished - reset previously used data
					if((m_HitMask & (MeleeCombatHit.LIGHT | MeleeCombatHit.SPRINT | MeleeCombatHit.HEAVY) ) == (MeleeCombatHit.LIGHT | MeleeCombatHit.SPRINT | MeleeCombatHit.HEAVY))
					{
						m_MeleeCombat.SetTargetObject(NULL);
						m_MeleeCombat.SetHitZoneIdx(-1);
						m_ComboHitCount = 0;
						m_HitMask = MeleeCombatHit.NONE;
					}
					m_ComboRangeMissed = false;
					m_RateLimited = false;
					m_ComboHitCount += 1;
				}
				
				//! breaks the attack and start evade
				if(pInputs.IsMeleeEvade() && pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE))
				{
					m_DZPlayer.StartCommand_Move();
					//! cancel melee attack
					hmc2.Cancel();
					//! start melee evade
					m_DZPlayer.GetCommand_Move().StartMeleeEvade();
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
					//! invalidate previously selected hit types
					m_HitMask = MeleeCombatHit.NONE;
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{			
			HumanCommandMove cm = m_DZPlayer.GetCommand_Move();
			float angle;

			//! evades in raised erc stance while moving
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT && pInputs.IsMeleeEvade())
			{

				// moving and angle is below -80 or above 80
				if (cm.GetCurrentInputAngle(angle) && (angle < -80.0 || angle > 80) && m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE))
				{
					// start melee evade
					cm.StartMeleeEvade();
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
				}
			}
			//! blocks in raised erc/pro stance
			else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT || pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				if (cm.GetCurrentInputAngle(angle) && (angle < -130.0 || angle > 130))
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