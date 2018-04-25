class ActionReadPaper: ActionSingleUseBase
{
	void ActionReadPaper()
	{
		m_MessageSuccess = "I broke chemlight.";
		m_MessageFail = "It's out of energy";
		//m_Animation = "break";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
	 return AT_READ_PAPER;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "read";
	}

	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		//display note
		//player.enterNoteMenuRead = true;
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Paper paper_item = Paper.Cast(item);
		PaperParams params = new PaperParams(paper_item.m_AdvancedText);
		//WritePaperParams params = new WritePaperParams("", player.m_penColor,player.m_handwriting);
		paper_item.RPCSingleParam(ERPCs.RPC_READ_NOTE, params, true);
	}
};