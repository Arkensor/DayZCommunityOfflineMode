class ActionSawPlanksCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CONSTRUCT);
	}
};

class ActionSawPlanks: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.02;
	
	void ActionSawPlanks()
	{
		m_CallbackClass = ActionSawPlanksCB;
		m_MessageStartFail = "The tool is ruined.";
		m_MessageStart = "I have started sawing the planks.";
		m_MessageSuccess = "I have sawn 3 planks.";
		m_MessageFail = "I've stopped sawing the planks.";
		m_MessageCancel = "I've stopped sawing the planks.";
		//m_Animation = "startFire";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_SAW_PLANKS;
	}

	override string GetText()
	{
		return "saw planks";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_O = target.GetObject();
		
		if ( item  &&  target_O.IsInherited(PileOfWoodenPlanks))
		{
			string item_type = item.GetType();
			
			switch(item_type)
			{
				case "Hacksaw":
					//m_TimeToCompleteAction = 6;
				break;
		  
				case "Chainsaw":
					//m_TimeToCompleteAction = 2;
				break;
			}
			
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PileOfWoodenPlanks item_POWP = PileOfWoodenPlanks.Cast( target.GetObject() );
		item_POWP.RemovePlanks(1);
		
		vector pos = player.GetPosition();
		ItemBase planks = ItemBase.Cast( GetGame().CreateObject("WoodenPlank", pos) );
		const float NEW_PLANKS = 3;

		planks.SetQuantity( Math.Round( player.GetSoftSkillManager().AddSpecialtyBonus( NEW_PLANKS, this.GetSpecialtyWeight() ) ), true );
		
		item.DecreaseHealth( "", "", 1);

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};