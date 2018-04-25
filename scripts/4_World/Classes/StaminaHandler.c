/**@class	Defines Stamina Consumer
 * @brief	Holds information about Stamina Consumer
 *
 * @param[in]	threshold	value needed to allow consume stamina
 * @param[in]	state		keeps state of the consumer non-depleted/depleted
 */
class StaminaConsumer
{
	protected float m_Threshold;
	protected bool m_State;
	
	void StaminaConsumer(float threshold, bool state)
	{
		m_Threshold = threshold;
		m_State = state;
	}
	
	bool GetState() { return m_State; }
	void SetState(bool state) { m_State = state; }
	
	float GetThreshold() { return m_Threshold; }
	void SetThreshold(float threshold) { m_Threshold = threshold; }
	
}

class StaminaConsumers
{
	protected ref map<EStaminaConsumers, ref StaminaConsumer> m_StaminaConsumers;

	void StaminaConsumers()
	{
		m_StaminaConsumers = new map<EStaminaConsumers, ref StaminaConsumer>;
	}

	void RegisterConsumer(EStaminaConsumers consumer, float threshold)
	{
		if ( !m_StaminaConsumers.Contains(consumer) )
		{
			//! init of StaminaConsumer - threshold, state
			StaminaConsumer sc = new StaminaConsumer(threshold, true);
			m_StaminaConsumers.Set(consumer, sc);
		}
	}

	bool HasEnoughStaminaFor(EStaminaConsumers consumer, float curStamina, bool isDepleted)
	{
		if ( m_StaminaConsumers && m_StaminaConsumers.Contains(consumer) )
		{
			StaminaConsumer sc = m_StaminaConsumers.Get(consumer);
			if( isDepleted )
			{
				sc.SetState(false);
				return false;
			}
			
			if( curStamina <= sc.GetThreshold() && sc.GetState() )
				return true;
	
			if( curStamina >= sc.GetThreshold() )
			{
				sc.SetState(true);
				return true;
			}
		}

		return false;
	}
}


/**@class	Defines Stamina Modifier
 * @brief	Holds information about Stamina Modifier
 *
 * @param[in]	type	 	calculation method
 * @param[in]	minValue	min value substracted from stamina (if type > 0)
 * @param[in]	maxValue	max value substracted from stamina
 */
class StaminaModifier
{
	int m_Type;
	float m_MinValue, m_MaxValue;

	void StaminaModifier(int type, float min, float max)
	{
		m_Type = type;
		m_MinValue = min;
		m_MaxValue = max;
	}
	
	int GetType() { return m_Type; }
	
	float GetMinValue() { return m_MinValue; }
	void SetMinValue(float val) { m_MinValue = val; } 
	
	float GetMaxValue() { return m_MaxValue; }
	void SetMaxValue(float val) { m_MaxValue = val; }
}

class StaminaModifiers
{
	const int FIXED 		= 0;
	const int RANDOMIZED 	= 1;

	protected ref map<EStaminaModifiers, ref StaminaModifier> m_StaminaModifiers;

	void StaminaModifiers()
	{
		m_StaminaModifiers = new map<EStaminaModifiers, ref StaminaModifier>;
	}

	//! register single value modifier - depletes stamina for that value
	void RegisterFixed(EStaminaModifiers modifier, float value)
	{	
		if ( !m_StaminaModifiers.Contains(modifier) )
		{
			//! init of StaminaModifier - type and min, max values (min is not relevant for that type)
			StaminaModifier sm = new StaminaModifier(FIXED, -1, value);
			m_StaminaModifiers.Set(modifier, sm);
		}
	}
	
	//! register randomized modifier - stamina will be depleted by value between min and max value;
	void RegisterRandomized(EStaminaModifiers modifier, float minValue, float maxValue)
	{
		if ( !m_StaminaModifiers.Contains(modifier) )
		{
			//! init of StaminaModifier - type, min, max values
			StaminaModifier sm = new StaminaModifier(RANDOMIZED, minValue, maxValue);
			m_StaminaModifiers.Set(modifier, sm);
		}
	}
	
	StaminaModifier GetModifierData(EStaminaModifiers modifier)
	{
		return m_StaminaModifiers.Get(modifier);
	}
}


class StaminaHandler
{	
	protected int 							m_PlayerSpeed;
	protected int 							m_PlayerStance;
	protected float 							m_PlayerLoad;
	protected float 							m_StaminaDelta;
	protected float 							m_Stamina;
	protected float 							m_StaminaCap;
	protected float							m_StaminaDepletion;
	protected float 							m_Time;
	protected ref Param2<float,float>			m_StaminaParams; 
	protected ref HumanMovementState			m_State;
	protected PlayerBase						m_Player;

	protected bool 							m_Debug;
	protected bool							m_StaminaDepleted;

	protected ref Timer						m_CooldownTimer;
	protected bool							m_IsInCooldown;
	
	protected ref StaminaConsumers			m_StaminaConsumers;
	protected ref StaminaModifiers			m_StaminaModifiers;
	
	void StaminaHandler(PlayerBase player)
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			m_StaminaParams = new Param2<float,float>(0,0);		
		}
		m_State 			= new HumanMovementState();
		m_Player 			= player;
		m_Stamina 			= STAMINA_MAX; 
		m_StaminaCap 		= STAMINA_MAX;
		m_StaminaDepletion 	= 0;
		m_Time 				= 0;
		m_StaminaDepleted	= false;
		m_CooldownTimer		= new Timer(CALL_CATEGORY_GAMEPLAY);
		m_IsInCooldown		= false;
		m_Debug 			= false;

		Init();
	}
	
	void Init()
	{
		//! stamina consumers registration
		m_StaminaConsumers = new StaminaConsumers;
		m_StaminaConsumers.RegisterConsumer(EStaminaConsumers.HOLD_BREATH, STAMINA_HOLD_BREATH_THRESHOLD);
		m_StaminaConsumers.RegisterConsumer(EStaminaConsumers.SPRINT, STAMINA_SPRINT_THRESHOLD);
		m_StaminaConsumers.RegisterConsumer(EStaminaConsumers.MELEE_HEAVY, STAMINA_MELEE_HEAVY_THRESHOLD);
		m_StaminaConsumers.RegisterConsumer(EStaminaConsumers.MELEE_EVADE, STAMINA_MELEE_EVADE_THRESHOLD);
		
		//! stamina modifiers registration
		m_StaminaModifiers = new StaminaModifiers;
		m_StaminaModifiers.RegisterFixed(EStaminaModifiers.HOLD_BREATH, STAMINA_DRAIN_HOLD_BREATH);
		m_StaminaModifiers.RegisterRandomized(EStaminaModifiers.MELEE_LIGHT, 1, STAMINA_DRAIN_MELEE_LIGHT);
		m_StaminaModifiers.RegisterRandomized(EStaminaModifiers.MELEE_HEAVY, STAMINA_DRAIN_MELEE_LIGHT, STAMINA_DRAIN_MELEE_HEAVY);
		m_StaminaModifiers.RegisterRandomized(EStaminaModifiers.MELEE_EVADE, 3, STAMINA_DRAIN_MELEE_EVADE);
	}
	
	void Update(float deltaT)
	{
#ifdef DEVELOPER
		m_Debug = DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_STAMINA_DISABLE);
		if( m_Debug ) return;
#endif
		if( m_Player )
		{
			// Calculates actual max stamina based on player's load
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
				m_Stamina = m_Player.GetStatStamina().Get();

			m_PlayerLoad = m_Player.GetPlayerLoad();
			m_StaminaCap =  Math.Max((STAMINA_MAX - ((m_PlayerLoad/STAMINA_KG_TO_GRAMS) * STAMINA_KG_TO_STAMINAPERCENT_PENALTY)),STAMINA_MIN_CAP);
			
			// Calculates stamina gain/loss based on movement and load
			m_Player.GetMovementState(m_State);
			m_PlayerSpeed = m_State.m_iMovement;
			m_PlayerStance = m_State.m_iStanceIdx;
			if( !m_IsInCooldown )
			{
				switch ( m_PlayerSpeed )
				{
					case 3/* DayZPlayerConstants.MOVEMENTIDX_SPRINT */: //sprint
						if ( m_PlayerStance == DayZPlayerConstants.STANCEIDX_ERECT )
						{
							m_StaminaDelta = -STAMINA_DRAIN_STANDING_SPRINT_PER_SEC;
							break;
						}
						if ( m_PlayerStance == DayZPlayerConstants.STANCEIDX_CROUCH)
						{
							m_StaminaDelta = -STAMINA_DRAIN_CROUCHED_SPRINT_PER_SEC;
							break;
						}
						m_StaminaDelta = STAMINA_GAIN_JOG_PER_SEC;
					break;
						
					case 2/* DayZPlayerConstants.MOVEMENTIDX_RUN */: //jog
						m_StaminaDelta = (STAMINA_GAIN_JOG_PER_SEC + CalcStaminaGainBonus());
					break;
						
					case 1/* DayZPlayerConstants.MOVEMENTIDX_WALK */: //walk
						m_StaminaDelta = (STAMINA_GAIN_WALK_PER_SEC + CalcStaminaGainBonus());
					break;
						
					case 0/* DayZPlayerConstants.MOVEMENTIDX_IDLE */: //idle
						m_StaminaDelta = (STAMINA_GAIN_IDLE_PER_SEC + CalcStaminaGainBonus());
					break;
						
					default:
						m_StaminaDelta = STAMINA_GAIN_IDLE_PER_SEC;
					break;
				}
			}
			
			//Sets current stamina & stores + syncs data with client
			m_Stamina = Math.Max(0,Math.Min((m_Stamina + m_StaminaDelta*deltaT),m_StaminaCap));
			m_Stamina = m_Stamina - m_StaminaDepletion;
			
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			{
				m_Player.GetStatStamina().Set(m_Stamina);
				m_Time += deltaT;
				if ( m_StaminaParams && m_Time >= STAMINA_SYNC_RATE )
				{
					m_Time = 0;
					m_StaminaParams.param1 = m_Stamina;
					m_StaminaParams.param2 = m_StaminaCap;
					GetGame().RPCSingleParam(m_Player, ERPCs.RPC_STAMINA, m_StaminaParams, true, m_Player.GetIdentity());
				}
			}
			else
			{
				m_Player.SetStamina(m_Stamina, m_StaminaCap);
			}

			//! sets exhaustion look of player based on stamina level
			HumanCommandAdditives ad = m_Player.GetCommandModifier_Additives();
			float exhaustion_value = 1-((m_Stamina/(m_StaminaCap*0.01))*0.01);
			exhaustion_value = Math.Min(1,exhaustion_value);
			if ( ad )
			{
				ad.SetExhaustion(exhaustion_value, true);
			}

			CheckStaminaState();

			m_StaminaDelta = 0;
			m_StaminaDepletion = 0; // resets depletion modifier
		}
	}

	//! called from RPC on playerbase - syncs stamina values on server with client
	void SyncStamina(float stamina, float stamina_cap)
	{
		if ( Math.AbsFloat(stamina - m_Stamina) > 5 )
		{
			m_Stamina = stamina;
		}
		if ( stamina_cap != m_StaminaCap )
		{
			m_StaminaCap = stamina_cap;
		}
	}
	
	protected float CalcStaminaGainBonus()// Calulates stamina regain bonus based on current stamina level (So it's better to wait for stamina to refill completely and avoid overloading)
	{
		if (m_StaminaDepletion > 0)
			return 0;

		return Math.Min((m_Stamina/10),STAMINA_GAIN_BONUS_CAP); // exp version
		//return STAMINA_GAIN_BONUS_CAP; // linear version
	}

	
	//! check if the stamina is completely depleted
	protected void CheckStaminaState()
	{
		if ( m_Stamina <= 1 )
		{
			m_StaminaDepleted = true;
			//! in case of complete depletion - start a cooldown timer before the regeneration cycle start
			if(!m_IsInCooldown) SetCooldown(STAMINA_REGEN_COOLDOWN_EXHAUSTION); // set this only once
		}
		else
			m_StaminaDepleted = false;
	}

	//! set cooldown timer between each consume of stamina	
	protected void SetCooldown(float time)
	{
		if( m_StaminaDepleted ) return;

		m_IsInCooldown = true;
		if( m_CooldownTimer.IsRunning() )
			m_CooldownTimer.Stop();
		m_CooldownTimer.Run(time, this, "ResetCooldown", NULL);
	}
	
	protected void ResetCooldown()
	{
		m_IsInCooldown = false;
	}
	
	// ---------------------------------------------------
	bool HasEnoughStaminaFor(EStaminaConsumers consumer)
	{
		return m_StaminaConsumers.HasEnoughStaminaFor(consumer, m_Stamina, m_StaminaDepleted);
	}
	
	float GetStamina()
	{
		return m_Stamina;
	}

	float GetStaminaNormalized()
	{	
		return m_Stamina / STAMINA_MAX;
	}
	
	void DepleteStamina(EStaminaModifiers modifier)
	{
		float val = 0.0;
		StaminaModifier sm = m_StaminaModifiers.GetModifierData(modifier);

		//! select by modifier type and drain stamina
		switch (sm.GetType())
		{
			case m_StaminaModifiers.FIXED:
				m_StaminaDepletion = m_StaminaDepletion + sm.GetMaxValue();
			break;
			
			case m_StaminaModifiers.RANDOMIZED:
				val = Math.RandomFloat(sm.GetMinValue(), sm.GetMaxValue());
				m_StaminaDepletion = m_StaminaDepletion + val;
			break;
		}

		//! run cooldown right after depletion
		SetCooldown(STAMINA_REGEN_COOLDOWN_DEPLETION);
		m_StaminaDepletion = Math.Clamp(m_StaminaDepletion, 0, STAMINA_MAX);
	}
};