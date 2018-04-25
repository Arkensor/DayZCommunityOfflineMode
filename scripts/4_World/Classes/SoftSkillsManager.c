class SoftSkillsManager
{	
	//SoftSkillsManager members
	protected PlayerBase			m_Player;
	protected float 				m_SpecialtyLevel;
	protected float 				m_RoughLevel;
	protected float 				m_PreciseLevel;
	 
	protected bool					m_IsLinear;
	protected bool					m_IsActive;
	protected bool					m_IsCoolDown;

	protected int 					m_UserActionsCounter;	

	static protected const int 		DEFAULT_EFFICIENCY = 0;
	static protected const float 	PRECISE_WEIGHT_LIMIT = -1; 
	static protected const float 	ROUGH_WEIGHT_LIMIT = 1;
	static protected const float 	COOLDOWN_TIMER = 5; //default 5, 

	protected ref Timer 			m_CoolDownTimer = new Timer();

	//debug window members
	protected float 				m_IsDebugMode;
	protected float 				m_CoolDownValue;
	protected float 				m_LastUAValue;
	protected float 				m_ComponentBonusBefore;
	protected float 				m_ComponentBonusAfter;
	protected float 				m_GeneralBonusBefore;
	protected float 				m_GeneralBonusAfter;

	protected ref SoftSkillManagerDebug m_DebugWindow;

	protected ref Timer 			m_SynchTimer;

	//Constructor
	void SoftSkillsManager( PlayerBase player )
	{
		m_Player = player;
		m_IsCoolDown = false;
		m_IsLinear = false;
		m_IsActive = false;
		m_IsDebugMode = false;
	}

	void InitSpecialty( float specialty_level )
	{
		SetSpecialtyLevel( specialty_level );
		SynchSpecialtyLevel();
	}

	//Destructor
	void ~SoftSkillsManager()
	{
		delete m_CoolDownTimer;
	}

	// ----------------------------------------------------------------------------------------
	float AddLinearPrecise( float specialty_weight )
	{
		m_SpecialtyLevel += specialty_weight; 
		SetLastUAValue( specialty_weight );
		m_SpecialtyLevel = Math.Clamp( m_SpecialtyLevel, PRECISE_WEIGHT_LIMIT, ROUGH_WEIGHT_LIMIT );

		return m_SpecialtyLevel;
	}

	// ----------------------------------------------------------------------------------------
	float AddLinearRough( float specialty_weight )
	{
		m_SpecialtyLevel += specialty_weight;
		SetLastUAValue( specialty_weight );
		m_SpecialtyLevel = Math.Clamp( m_SpecialtyLevel, PRECISE_WEIGHT_LIMIT, ROUGH_WEIGHT_LIMIT );

		return m_SpecialtyLevel;
	}

	// ----------------------------------------------------------------------------------------
	float AddExponentialPrecise( float specialty_weight )
	{
		m_UserActionsCounter -= 1;

		if( m_UserActionsCounter == 0)
		{
			m_UserActionsCounter = -1;
			m_SpecialtyLevel = 0;
		}
		
		float adjusted_weight = specialty_weight / Math.Sqrt( Math.AbsInt( m_UserActionsCounter ) );
	
		SetLastUAValue( adjusted_weight );

		m_SpecialtyLevel += adjusted_weight;
		m_SpecialtyLevel = Math.Clamp( m_SpecialtyLevel, PRECISE_WEIGHT_LIMIT, ROUGH_WEIGHT_LIMIT );

		return m_SpecialtyLevel;
	}
		
	// ----------------------------------------------------------------------------------------
	float AddExponentialRough( float specialty_weight )
	{
		m_UserActionsCounter += 1;
			
		if( m_UserActionsCounter == 0)
		{
			m_UserActionsCounter = 1;
			m_SpecialtyLevel = 0;
		}
		
		float adjusted_weight = specialty_weight / Math.Sqrt( Math.AbsInt( m_UserActionsCounter ) );

		SetLastUAValue( adjusted_weight );

		m_SpecialtyLevel += adjusted_weight;
		m_SpecialtyLevel = Math.Clamp( m_SpecialtyLevel, PRECISE_WEIGHT_LIMIT, ROUGH_WEIGHT_LIMIT );

		return m_SpecialtyLevel;
	}

	// ----------------------------------------------------------------------------------------
	void AddSpecialty( float specialty_weight )
	{
		if( GetSoftSkillsState() )
		{
			if( !IsCoolDown() )
			{	
					if( specialty_weight < 0 )
					{	
						if( IsLinear() )
						{
							SetSpecialtyLevel( AddLinearPrecise( specialty_weight ) );	
						}
						else
						{
							 SetSpecialtyLevel( AddExponentialPrecise( specialty_weight ) );
						}

						m_Player.GetStatSpecialty().Set( m_SpecialtyLevel );
						StartCoolDownTimer( Math.AbsFloat( ( specialty_weight * 100 ) ) * COOLDOWN_TIMER );
						SynchSpecialtyLevel();
					}
					else if( specialty_weight > 0 )
					{
						if( IsLinear() )
						{							
							SetSpecialtyLevel( AddLinearRough( specialty_weight ) );
						}
						else
						{						
							SetSpecialtyLevel( AddExponentialRough( specialty_weight ) );		
						}

						m_Player.GetStatSpecialty().Set( m_SpecialtyLevel );
						StartCoolDownTimer( Math.AbsFloat( ( specialty_weight * 100 ) ) * COOLDOWN_TIMER );
						SynchSpecialtyLevel();
					}
					else
					{
						//if the specialty weight for a recipe or UA is set to 0, it will increase neither specialty, nor UA counter
						return;
					}
				}
			else
			{
				StartCoolDownTimer( Math.AbsFloat( ( specialty_weight * 100 ) ) * COOLDOWN_TIMER );		
			}
		}
	}

	// ----------------------------------------------------------------------------------------	
	// Use AddSpecialtyBonus if you want to increase a value, based on the specialty level of the player. e.g. more harvested material
	// limit_efficiency sets the max amount of bonus, the soft skill can provide: limit_efficiency = 2 is 50% bonus, limit_efficiency = 3 is 33% bonus etc
	float AddSpecialtyBonus( float base_value, float specialty_weight, bool is_cacomponent = false, float limit_efficiency = 2 )
	{	
		SetBonusBefore( is_cacomponent, base_value);

		float adjusted_value;
		
		GetPreciseRoughLevels();

		if ( specialty_weight < 0 )
		{			
			adjusted_value = base_value + ( ( base_value * m_PreciseLevel ) / limit_efficiency );
		}
		else
		{
			adjusted_value = base_value + ( ( base_value * m_RoughLevel ) / limit_efficiency );
		}

		SetBonusAfter( is_cacomponent, adjusted_value );

		return adjusted_value;
	}
	
	// ----------------------------------------------------------------------------------------
	// Use SubtractSpecialtyBonus if you want to decrease a value, based on the specialty level of the player. e.g. faster harvesting time
	// limit_efficiency sets the max amount of bonus, the soft skill can provide: limit_efficiency = 2 is 50% bonus, limit_efficiency = 3 is 33% bonus etc
	float SubtractSpecialtyBonus( float base_value, float specialty_weight, bool is_cacomponent = false, float limit_efficiency = 2 )
	{
		SetBonusBefore( is_cacomponent, base_value);

		float adjusted_value;
		
		GetPreciseRoughLevels();

		if ( specialty_weight < 0 )
		{
			adjusted_value = base_value - ( ( base_value * m_PreciseLevel  ) / limit_efficiency );
		}
		else
		{
			adjusted_value = base_value - ( ( base_value * m_RoughLevel ) / limit_efficiency );
		}

		SetBonusAfter( is_cacomponent, adjusted_value );

		return adjusted_value;	
	}

	// ----------------------------------------------------------------------------------------
	// Use AdjustCraftingTime if you want to decrease time for recipe crafting
	// limit_efficiency sets the max amount of bonus, the soft skill can provide: limit_efficiency = 2 is 50% bonus, limit_efficiency = 3 is 33% bonus etc
	float AdjustCraftingTime( float base_time, float specialty_weight, float limit_efficiency = 2 )
	{
		SetBonusBefore( false, base_time);

		float adjusted_time;

		GetPreciseRoughLevels();

		if ( specialty_weight < 0 )
		{
			adjusted_time = base_time - ( ( base_time * m_PreciseLevel ) / limit_efficiency );
		}
		else
		{
			adjusted_time = base_time - ( ( base_time * m_RoughLevel ) / limit_efficiency );
		}

		SetBonusAfter( false, adjusted_time );

		return adjusted_time;	
	}

	// ----------------------------------------------------------------------------------------
	void SetSpecialtyLevel( float specialty_level )
	{
		m_SpecialtyLevel = specialty_level;
	}
	
	// ----------------------------------------------------------------------------------------
	float GetSpecialtyLevel()
	{
		return m_SpecialtyLevel;
	}

	// ----------------------------------------------------------------------------------------
	void SynchSpecialtyLevel()
	{	
		ref Param1<float> specialty_level;
		
		if( GetGame().IsServer() )
		{
			specialty_level = new Param1<float>( m_SpecialtyLevel );
			GetGame().RPCSingleParam( m_Player, ERPCs.RPC_SOFT_SKILLS_SPECIALTY_SYNC, specialty_level, true, m_Player.GetIdentity() );
		}		
	}

	// ----------------------------------------------------------------------------------------
	void SetSoftSkillsState( bool state )
	{
		m_IsActive = state;
	}

	// ----------------------------------------------------------------------------------------
	bool GetSoftSkillsState()
	{
		return m_IsActive;
	}

	// ----------------------------------------------------------------------------------------
	void SetLinearState( bool model )
	{
		m_IsLinear = model;
	}

	// ----------------------------------------------------------------------------------------
	bool IsLinear()
	{
		return m_IsLinear;
	}

	// ----------------------------------------------------------------------------------------
	void GetPreciseRoughLevels()
	{
		if( m_SpecialtyLevel > 0)
		{
			m_RoughLevel = m_SpecialtyLevel;
			m_PreciseLevel = DEFAULT_EFFICIENCY;
		}
		else if ( m_SpecialtyLevel < 0)
		{
			m_RoughLevel = DEFAULT_EFFICIENCY;
			m_PreciseLevel =  Math.AbsFloat( m_SpecialtyLevel );
		}
		else
		{
			m_RoughLevel = DEFAULT_EFFICIENCY;
			m_PreciseLevel = DEFAULT_EFFICIENCY;
		}
	}

	// ----------------------------------------------------------------------------------------
	void StartCoolDownTimer( float cooldown_value )
	{
		SetCoolDown( true );
		SetCoolDownValue( cooldown_value );
		m_CoolDownTimer.Run( cooldown_value, this, "SetCoolDown", new Param1<bool>( false ) );
	}

	// ----------------------------------------------------------------------------------------
	bool IsCoolDown()
	{
		return m_IsCoolDown;
	}

	// ----------------------------------------------------------------------------------------
	void SetCoolDown( bool cool_down )
	{
		m_IsCoolDown = cool_down;
	}
	
// ----------------------------------------------------------------------------------------
//	SoftSkillManagerDebug support
// ----------------------------------------------------------------------------------------
	void CreateDebugWindow( bool create )
	{
		if( create )
		{
			m_DebugWindow = new SoftSkillManagerDebug( this );
			SetIsDebug( create );
		}
		else
		{
			ResetDebugWindow();
			SetIsDebug( create );
			delete m_DebugWindow;
		}
	}
	
	// ----------------------------------------------------------------------------------------
	void SynchDebugStats()
	{	
		if( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			ref Param5<float, float, float, float, bool> debug_stats;
			
			debug_stats = new Param5<float, float, float, float, bool>( m_GeneralBonusBefore, m_GeneralBonusAfter, m_LastUAValue, m_CoolDownValue, m_IsCoolDown );
			GetGame().RPCSingleParam( m_Player, ERPCs.RPC_SOFT_SKILLS_STATS_SYNC, debug_stats, true, m_Player.GetIdentity() );
		}		
	}

	// ----------------------------------------------------------------------------------------
	void SetIsDebug( bool is_debug )
	{
		m_IsDebugMode = is_debug;
	}

	// ----------------------------------------------------------------------------------------
	float IsDebug()
	{
		return m_IsDebugMode;
	}
	
	// ----------------------------------------------------------------------------------------
	void SetCoolDownValue( float cooldown_value )
	{
		m_CoolDownValue = cooldown_value;
	}

	// ----------------------------------------------------------------------------------------
	float GetCoolDownValue()
	{
		return m_CoolDownValue;
	}
	
	// ----------------------------------------------------------------------------------------
	float GetLastUAValue()
	{
		return m_LastUAValue;
	}

	// ----------------------------------------------------------------------------------------
	void SetLastUAValue( float last_ua_value )
	{
		m_LastUAValue = last_ua_value;
	}

	// ----------------------------------------------------------------------------------------
	void SetBonusBefore( bool is_cacomponent, float base_value)
	{
		if( IsDebug() )
		{
			if( is_cacomponent )
			{
				SetComponentBonusBefore(base_value);
			}
			else
			{
				SetGeneralBonusBefore(base_value);
			}
		}	
	}

	// ----------------------------------------------------------------------------------------
	void SetBonusAfter( bool is_cacomponent, float adjusted_value )
	{
		if( IsDebug() )
			{
				if( is_cacomponent )
				{
					SetComponentBonusAfter(adjusted_value);
				}
				else
				{
					SetGeneralBonusAfter(adjusted_value);
				}
			}
	}

	// ----------------------------------------------------------------------------------------
	float GetComponentBonusBefore()
	{
		return m_ComponentBonusBefore;
	}

	// ----------------------------------------------------------------------------------------
	void SetComponentBonusBefore( float component_bonus_before )
	{
		m_ComponentBonusBefore = component_bonus_before;
	}

	// ----------------------------------------------------------------------------------------
	float GetComponentBonusAfter()
	{
		return m_ComponentBonusAfter;
	}

	// ----------------------------------------------------------------------------------------
	void SetComponentBonusAfter( float component_bonus_after )
	{
		m_ComponentBonusAfter = component_bonus_after;
	}

	// ----------------------------------------------------------------------------------------
	float GetGeneralBonusBefore()
	{
		return m_GeneralBonusBefore;
	}

	// ----------------------------------------------------------------------------------------
	void SetGeneralBonusBefore( float general_bonus_before )
	{
		m_GeneralBonusBefore = general_bonus_before;
	}

	// ----------------------------------------------------------------------------------------
	float GetGeneralBonusAfter()
	{
		return m_GeneralBonusAfter;
	}

	// ----------------------------------------------------------------------------------------
	void SetGeneralBonusAfter( float general_bonus_after )
	{
		m_GeneralBonusAfter = general_bonus_after;
	}

	// ----------------------------------------------------------------------------------------
	void StartSynchTimer()
	{
		SetIsDebug( true );
		m_SynchTimer = new Timer;
		m_SynchTimer.Run( 2, this, "SynchDebugStats", NULL, true );
	}

	// ----------------------------------------------------------------------------------------
	void StopSynchTimer()
	{
		SetIsDebug( false );
		m_SynchTimer.Stop();
		delete m_SynchTimer;
	}
		
	// ----------------------------------------------------------------------------------------
	void ResetDebugWindow()
	{
		SetSpecialtyLevel( 0 );
		SetLastUAValue( 0 );
		SetComponentBonusBefore( 0 );
		SetComponentBonusAfter( 0 );
		SetGeneralBonusBefore( 0 );
		SetGeneralBonusAfter( 0 );
		SetCoolDownValue( 0 );
		
		SynchSpecialtyLevel();
	}
}

// ----------------------------------------------------------------------------------------
//	SoftSkillManagerDebug
// ----------------------------------------------------------------------------------------

class SoftSkillManagerDebug
{
	SoftSkillsManager m_SoftSkillManager;
	ref Widget m_PanelSoftSkills;
	TextWidget SpecialtyTotal;
	TextWidget SpecialtyChange;
	TextWidget ComponentBonusBefore;
	TextWidget ComponentBonusAfter;
	TextWidget GeneralBonusBefore;
	TextWidget GeneralBonusAfter;
	TextWidget CoolDown;
	TextWidget IsCoolDown;

	void SoftSkillManagerDebug( SoftSkillsManager softskill_manager )
	{		
		m_SoftSkillManager = softskill_manager;
		m_PanelSoftSkills = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud_debug_softskills.layout");
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.OnUpdate);

		Class.CastTo(SpecialtyTotal,  m_PanelSoftSkills.FindWidget("SpecialtyTotal"));
		Class.CastTo(SpecialtyChange,  m_PanelSoftSkills.FindWidget("SpecialtyChange"));
		Class.CastTo(ComponentBonusBefore,  m_PanelSoftSkills.FindWidget("ComponentBonusBefore"));
		Class.CastTo(ComponentBonusAfter,  m_PanelSoftSkills.FindWidget("ComponentBonusAfter"));
		Class.CastTo(GeneralBonusBefore,  m_PanelSoftSkills.FindWidget("GeneralBonusBefore"));
		Class.CastTo(GeneralBonusAfter,  m_PanelSoftSkills.FindWidget("GeneralBonusAfter"));
		Class.CastTo(CoolDown,  m_PanelSoftSkills.FindWidget("CoolDown"));
		Class.CastTo(IsCoolDown,  m_PanelSoftSkills.FindWidget("IsCoolDown"));
		
		m_PanelSoftSkills.Show( true );
		SpecialtyTotal.Show( true );
		SpecialtyChange.Show( true );
		ComponentBonusBefore.Show( true );
		ComponentBonusAfter.Show( true );
		GeneralBonusBefore.Show( true );
		GeneralBonusAfter.Show( true );
		CoolDown.Show( true );
		IsCoolDown.Show( true );
	}
	
	void ~SoftSkillManagerDebug( PlayerBase player )
	{
		delete m_PanelSoftSkills;
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.OnUpdate);
	}

	SoftSkillsManager GetActiveSoftSkillManager()
	{
		return m_SoftSkillManager;
	}
	
	void OnUpdate()
	{
		SpecialtyTotal.SetText( "Specialty level: " + GetActiveSoftSkillManager().GetSpecialtyLevel() );
		SpecialtyChange.SetText( "Specialty change: " + GetActiveSoftSkillManager().GetLastUAValue() );
		ComponentBonusBefore.SetText( "Component/craft default: " + GetActiveSoftSkillManager().GetComponentBonusBefore() );
		ComponentBonusAfter.SetText( "Component/craft with bonus: " + GetActiveSoftSkillManager().GetComponentBonusAfter() );
		GeneralBonusBefore.SetText( "General default: " + GetActiveSoftSkillManager().GetGeneralBonusBefore() );
		GeneralBonusAfter.SetText( "General with bonus: " + GetActiveSoftSkillManager().GetGeneralBonusAfter() );
		CoolDown.SetText( "CoolDown value: " + GetActiveSoftSkillManager().GetCoolDownValue() );
		IsCoolDown.SetText( "Cooldown active: " + GetActiveSoftSkillManager().IsCoolDown() );
	}

}
