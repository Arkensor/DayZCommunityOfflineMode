class CAContinuousTimeCooking : CAContinuousBase
{
	protected float 			m_LocalTimeElpased;
	protected float				m_TimeElpased;
	protected float 			m_TimeElapsedRepeat;
	protected ref Param1<float>	m_SpentUnits;
	
	protected float 			m_TimeToCook;
	protected Cooking			m_CookingProcess;
	protected Edible_Base		m_ItemToCook;
	protected float 			m_CookingUpdateTime;
	
	private const float ROUGH_SPECIALTY_WEIGHT = 0.02;
	
	void CAContinuousTimeCooking( float cooking_update_time )
	{
		m_CookingUpdateTime = cooking_update_time;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TimeElapsedRepeat = 0;
		
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		
		//fireplace & cooking process
		FireplaceBase fireplace = FireplaceBase.Cast( target.GetObject() );
		m_CookingProcess = fireplace.GetCookingProcess();
		m_ItemToCook = m_CookingProcess.GetFoodOnStick( item );
		m_TimeToCook = m_CookingProcess.GetTimeToCook( m_ItemToCook, CookingMethodType.BAKING );
		
		//update elapsed time
		m_TimeElpased = m_ItemToCook.GetFoodStage().GetCookingTime();
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		int cooking_state_update = 0;
		float cooking_time = m_ItemToCook.GetFoodStage().GetCookingTime();
		if ( m_TimeToCook > cooking_time )
		{
			m_TimeElpased += player.GetDeltaT();
			m_TimeElapsedRepeat += player.GetDeltaT();
				
			if ( m_TimeElapsedRepeat >= m_CookingUpdateTime )
			{
				if ( GetGame() && GetGame().IsServer() )
				{
					cooking_state_update = m_CookingProcess.CookOnStick( m_ItemToCook, player.GetSoftSkillManager().AddSpecialtyBonus( m_CookingUpdateTime, m_Action.GetSpecialtyWeight(), true ) );
				}
				
				//add speciality
				if ( ( cooking_time / m_TimeToCook ) > 0 ) 
				{
					player.GetSoftSkillManager().AddSpecialty( ROUGH_SPECIALTY_WEIGHT * ( cooking_time / m_TimeToCook ) );
				}
				
				//refresh action when food changed state
				if ( cooking_state_update == 1 )
				{
					Setup( player, target, item );
				}
				
				m_TimeElapsedRepeat = 0;
			}				
			
			return UA_PROCESSING;
		}
		else
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_TimeElpased;
				SetACData(m_SpentUnits);
			}
			
			return UA_FINISHED;
		}
	}
	
	override float GetProgress()
	{	
		if ( m_TimeToCook != 0 )
		{
			return m_TimeElpased/m_TimeToCook;
		}

		return 1;
	}
};