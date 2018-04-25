class ActionMineTreeCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 4;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineTree: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.03;
	
	void ActionMineTree()
	{
		m_CallbackClass = ActionMineTreeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CHOPTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting down the tree.";
		m_MessageSuccess = "I have cut down the tree.";
		m_MessageFail = "My tool is ruined.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_MINE_TREE;
	}
		
	override string GetText()
	{
		return "cut tree down";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsTree() )
		{ 
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};