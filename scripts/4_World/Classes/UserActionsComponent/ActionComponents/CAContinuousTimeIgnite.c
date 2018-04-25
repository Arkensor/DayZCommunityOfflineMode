class CAContinuousTimeIgnite : CAContinuousBase
{
	protected float 					m_LocalTimeElpased;
	protected float						m_TimeElpased;
	protected float						m_TimeToComplete;
	protected float						m_DefaultTimeToComplete;	
	
	protected float 					m_TimeToRepeatCheck;
	protected float 					m_TimeElapsedRepeat;
	protected ActionIgniteFireplace		m_IgniteFireplaceAction;
	
	protected ref Param1<float>			m_SpentUnits;
	
	void CAContinuousTimeIgnite( float time_to_complete_action, float time_to_repeat_check )
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
		
		m_IgniteFireplaceAction = ActionIgniteFireplace.Cast( m_Action );
		
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
				if ( GetGame() && GetGame().IsServer() )
				{
					string reason = GetReasonToCancel( target, item );
					if ( reason != "" )
					{
						m_IgniteFireplaceAction.SetReasonToCancel( reason );
						
						return UA_CANCEL;
					}					
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

		//COMMON REASONS
		//check kindling
		if ( !fireplace_target.HasAnyKindling() )
		{
			if ( item )
			{
				return fireplace_target.MESSAGE_IGNITE_NO_KINDLING;
			}
			else
			{
				return fireplace_target.MESSAGE_REIGNITE_NO_KINDLING;						//if no item is present, reigniting by air is in process
			}
		}
		
		//check roof
		if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsBarrelWithHoles() )	//base fireplace and barrel with Holes only
		{
			if ( fireplace_target.IsRoofAbove() )
			{
				return fireplace_target.MESSAGE_IGNITE_UNDER_ROOF;
			}
		}

		//check surface
		if ( fireplace_target.IsWaterSurface() )
		{
			return fireplace_target.MESSAGE_IGNITE_IN_WATER;
		}

		//check wetness
		if ( fireplace_target.IsWet() )
		{
			if ( item )
			{
				return fireplace_target.MESSAGE_IGNITE_TOO_WET;
			}
			else
			{
				return fireplace_target.MESSAGE_REIGNITE_TOO_WET;							//if no item is present, reigniting by air is in process
			}
		}
		
		//SPECIFIC REASONS
		//check damage
		if ( item )
		{
			if ( item.IsInherited( HandDrillKit ) )
			{
				if ( item.GetHealth("", "") < m_IgniteFireplaceAction.m_HandDrillDamagePerUse )
				{
					return fireplace_target.MESSAGE_IGNITE_IGNIT_ITEM_DAMAGED;
				}
			}
		}
		
		//check rain
		if ( fireplace_target.IsBaseFireplace() )	//base fireplace only
		{
			if ( fireplace_target.IsRainingAbove() )
			{
				if ( item )
				{
					if ( !item.IsInherited( Roadflare ) || !item.IsInherited( Torch ) )	//only flare and torch can ingnite fireplace in rain (if wetness is in proper levels)
					{
						return fireplace_target.MESSAGE_IGNITE_RAIN;
					}					
				}
				else
				{
					return fireplace_target.MESSAGE_REIGNITE_RAIN;									//if no item is present, reigniting by air is in process
				}
			}
		}
		
		//check wind
		if ( fireplace_target.IsBaseFireplace() )	//base fireplace only
		{
			if ( fireplace_target.IsWindy() )
			{
				if ( item )
				{
					if ( !item.IsInherited( Roadflare ) || !item.IsInherited( Torch ) )							//only flare and torch can ingnite fireplace in wind
					{
						if ( item.IsInherited( Matchbox ) )
						{
							item.AddQuantity ( -m_IgniteFireplaceAction.m_MatchSpentPerUse );								//remove match
						}
						
						if ( item.IsInherited( HandDrillKit ) ) 
						{
							item.DecreaseHealth ( "", "", m_IgniteFireplaceAction.m_HandDrillDamagePerUse / 2, true );		//add half damage as succesful action to hand drill
						}
						
						return fireplace_target.MESSAGE_IGNITE_WIND;
					}
				}
			}
		}
		
		return "";
	}
}