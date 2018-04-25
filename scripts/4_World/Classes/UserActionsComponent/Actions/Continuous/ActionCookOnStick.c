class ActionCookOnStickCB : ActionContinuousBaseCB
{
	private const float COOKING_ON_STICK_UPDATE_TIME = 1;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTimeCooking( COOKING_ON_STICK_UPDATE_TIME );
	}
};

class ActionCookOnStick: ActionContinuousBase
{
	void ActionCookOnStick()
	{
		m_CallbackClass = ActionCookOnStickCB;
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREFB_CAMPFIRE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "I have failed to start the action.";
		m_MessageStart = "I have started the action.";
		m_MessageFail = "I have failed the action.";
		m_MessageCancel = "I have stopped the action.";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_COOK_ON_STICK;
	}
		
	override string GetText()
	{
		return "cook on stick";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		FireplaceBase fireplace_target = FireplaceBase.Cast( target.GetObject() );
		Object targetObject = target.GetObject();
		Edible_Base item_on_stick = Edible_Base.Cast( item.GetAttachmentByType( Edible_Base ) );
		if ( fireplace_target && item_on_stick )
		{
			BarrelHoles_ColorBase barrel = BarrelHoles_ColorBase.Cast( fireplace_target );
			//barrel with holes
			if ( barrel )
			{
				if ( barrel.IsOpened() && fireplace_target.CanCookOnStick() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			
			//fireplace
			if ( fireplace_target.IsBaseFireplace() )
			{
				Fireplace fireplace = Fireplace.Cast( fireplace_target );
				if ( !fireplace.IsOven() && fireplace_target.CanCookOnStick() ) 
				{
					return true;
				}
			}
			
			//fireplace indoor
			if ( fireplace_target.IsFireplaceIndoor() )
			{
				if ( fireplace_target.CanCookOnStick() ) 
				{
					return true;
				}	
			}			
		}
		
		return false;
	}
};