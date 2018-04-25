class ActionTestBloodSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.TEST_BLOOD);
	}
};

class ActionTestBloodSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;

	void ActionTestBloodSelf()
	{
		m_CallbackClass = ActionTestBloodSelfCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "I have started testing my blood.";
		m_MessageSuccess = "I have tested my blood.";
		m_MessageFail = "I have moved and testing was canceled.";
		m_MessageCancel = "I stopped testing my blood.";
		//m_Animation = "INJECTS";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_TEST_BLOOD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "test blood";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		int blood_type = player.GetStatBloodType().Get();
		
		module_lifespan.UpdateBloodType( player, blood_type );
		module_lifespan.UpdateBloodTypeVisibility( player, true );
		
		item.Delete();
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};