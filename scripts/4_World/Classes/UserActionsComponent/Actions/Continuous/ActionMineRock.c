class ActionMineRockCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 8;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineRock(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineRock: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.03;

	void ActionMineRock()
	{
		m_CallbackClass = ActionMineRockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CHOPTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_CallbackClass = ActionMineRockCB;
		m_MessageStartFail = "My tool is too damaged to mine.";
		m_MessageStart = "I started mining the rock.";
		m_MessageSuccess = "I have mined the rock.";
		m_MessageFail = "My tool is ruined.";
		//m_AnimationOneHanded = "DRINK";
		//m_AnimationTwoHanded = "DIGGINGSHOVEL";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionTarget = new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsRock() )
		{
			return true;
		}
		return false;
	}

	override int GetType()
	{
		return AT_MINE_ROCK;
	}
		
	override string GetText()
	{
		return "mine";
	}


	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};