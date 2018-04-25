// This is just a test action!
class ActionRepairWithToolFromHands: ActionSingleUseBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.03;

	void ActionRepairWithToolFromHands()
	{
		Print("ActionRepairWithToolFromHands");
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		
		//m_Animation = "open";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return 0; //AT_REPAIR;
	}

	override string GetText()
	{
		return "repair";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject /*&&  targetObject.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = ItemBase.Cast( targetObject );
			bool can_repair = item_to_repair.CanRepair(item);
			Print(can_repair);
			return can_repair;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject  /* &&  targetObject.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = ItemBase.Cast( targetObject );
			Param1<float> nacdata = Param1<float>.Cast( acdata );
			bool was_repaired = item_to_repair.Repair(player, item, nacdata.param1);
			Print(was_repaired);
		}

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
