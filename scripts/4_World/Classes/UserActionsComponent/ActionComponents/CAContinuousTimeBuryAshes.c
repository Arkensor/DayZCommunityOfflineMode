class CAContinuousTimeBuryAshes : CAContinuousBase
{
	protected float 					m_LocalTimeElpased;
	protected float						m_TimeElpased;
	protected float						m_TimeToComplete;
	protected float						m_DefaultTimeToComplete;	
	
	protected float 					m_TimeToRepeatCheck;
	protected float 					m_TimeElapsedRepeat;
	protected ActionBuryAshes			m_BuryAshesAction;
	
	protected ref Param1<float>			m_SpentUnits;
	
	void CAContinuousTimeBuryAshes( float time_to_complete_action, float time_to_repeat_check )
	{
		m_DefaultTimeToComplete = time_to_complete_action;
		m_TimeToRepeatCheck = time_to_repeat_check;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		
		m_BuryAshesAction = ActionBuryAshes.Cast( m_Action );
		
		m_TimeToComplete = player.GetSoftSkillManager().SubtractSpecialtyBonus( m_DefaultTimeToComplete, m_Action.GetSpecialtyWeight(), true);
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( m_TimeElpased < m_TimeToComplete )
		{
			m_TimeElpased += player.GetDeltaT();
			m_TimeElapsedRepeat += player.GetDeltaT();
			
			if ( m_TimeElapsedRepeat >= m_TimeToRepeatCheck )
			{
				m_TimeElapsedRepeat = 0;
				
				//get reason to cancel action
				string reason = GetReasonToCancel( target, item );
				if ( reason != "" )
				{
					m_BuryAshesAction.SetReasonToCancel( reason );
					
					return UA_CANCEL;
				}
			}
			
			return UA_PROCESSING;
		}
		else
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_TimeElpased;
				SetACData( m_SpentUnits );
			}
			return UA_FINISHED;
		}
	}

	override float GetProgress()
	{	
		if ( m_TimeToComplete > 0 )
		{
			return m_TimeElpased / m_TimeToComplete;
		}
		else
		{
			Print("USER ACTION COMPONENT CONTINUOUS TIME - WRONG TIME TO COMPLETE CALCULATION - Incorrect data for progress bar!");
		}
		return 1;
	}
	
	string GetReasonToCancel( ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );

		//check if fireplace cargo and attachments are empty
		if ( !fireplace_target.IsEmpty() )
		{
			return fireplace_target.MESSAGE_BURY_ASHES_FAILED_NOT_EMPTY;
		}
			
		//check hard surface
		string surface_type;
		vector position = target_object.GetPosition();
		GetGame().SurfaceGetType ( position[0], position[2], surface_type ); 
		if ( surface_type == "cp_dirt"  ||  surface_type == "cp_grass"  ||  surface_type == "cp_grass_tall"  ||  surface_type == "cp_conifer_common1"  ||  surface_type == "cp_conifer_common2" ||  surface_type == "cp_conifer_moss1"  ||  surface_type == "cp_conifer_moss2"  ||  surface_type == "cp_broadleaf_dense1"  ||  surface_type == "cp_broadleaf_dense2" ||  surface_type == "cp_broadleaf_sparse1"  ||  surface_type == "cp_broadleaf_sparse2" || surface_type == "cp_gravel" )
		{
			return fireplace_target.MESSAGE_BURY_ASHES_FAILED_SURFACE;
		}
		
		//check burning state
		if ( fireplace_target.IsBurning() )
		{
			return fireplace_target.MESSAGE_BURY_ASHES_FAILED_BURNING;
		}
		
		//check proper temperature
		if ( fireplace_target.GetTemperature() > fireplace_target.PARAM_MIN_FIRE_TEMPERATURE )
		{
			return fireplace_target.MESSAGE_BURY_ASHES_FAILED_TOO_HOT;
		}
		
		return "";
	}
}