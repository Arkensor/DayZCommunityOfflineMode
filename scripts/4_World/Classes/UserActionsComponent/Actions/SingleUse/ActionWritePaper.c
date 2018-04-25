class ActionWritePaperCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT);
	}
};

class ActionWritePaper: ActionContinuousBase
{
	const float TARGET_DISTANCE = 2;
	
	void ActionWritePaper()
	{
		m_CallbackClass = ActionWritePaperCB;
		m_MessageSuccess = "I broke chemlight.";
		m_MessageFail = "It's out of energy";
		//m_Animation = "break";
	}
	
	override void CreateConditionComponents()
	{	
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined(TARGET_DISTANCE);
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ((item.ConfigIsExisting("writingColor") && target.GetObject().GetType() == "Paper") || (target.GetObject().ConfigIsExisting("writingColor") && item.GetType() == "Paper"))
		{
			return true;
		}
		return false;
	}

	override int GetType()
	{
	 return AT_WRITE_PAPER;
	}

	/*override bool HasTarget()
	{
		return false;
	}*/

	override string GetText()
	{
		return "Write note";
	}

	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		//which is pen and which paper
		if (target.GetObject().ConfigIsExisting("writingColor"))
		{
			player.m_writingImplement = EntityAI.Cast(target.GetObject());
			player.m_paper = item;
		}
		else
		{
			player.m_writingImplement = item;
			player.m_paper = EntityAI.Cast(target.GetObject());
		}
		player.enterNoteMenuWrite = true;
	}
};