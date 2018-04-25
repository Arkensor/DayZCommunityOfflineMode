class ActionPickBerryCB : ActionInteractLoopBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAInteractLoop(UATimeSpent.DEFAULT_PICK);
	}
	/*override void OnAnimationEvent(int pEventID)	
	{
		Print("ActionInteractLoopBase.c | OnAnimationEvent | OnAnimationEvent called");
		if ( !m_Interrupted && pEventID == UA_ANIM_EVENT ) 
		{
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			m_State = UA_FINISHED;
			m_ActionData.Do(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
		}	
	}*/
};

class ActionPickBerry: ActionInteractLoopBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.02;
	
	void ActionPickBerry()
	{
		m_MessageSuccess = "I have harvested it.";
		m_MessageStartFail = "Fail..";
		//m_MaximalActionDistance = 1;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BERRIES;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CallbackClass = ActionPickBerryCB;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override int GetType()
	{
		return AT_PICK_BERRY;
	}

	override string GetText()
	{
		return "harvest";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( player && targetObject && targetObject.IsWoodBase() ) 
		{
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		WoodBase ntarget = WoodBase.Cast( targetObject );
		string drop_name = ntarget.GetMaterial(NULL);
		float drop_quantity = ntarget.GetAmountOfMaterialPerDrop(item);
		
		targetObject.DecreaseHealth( "", "", (1 / Math.Max(1,ntarget.GetAmountOfDrops(item)))*100, true );
				
		ItemBase drop = ItemBase.Cast( GetGame().CreateObject(drop_name, player.GetPosition(), false) );
		drop.SetQuantity(drop_quantity, false);

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};