class ActionTestBloodTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.TEST_BLOOD);
	}
};

class ActionTestBloodTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
	
	void ActionTestBloodTarget()
	{
		m_CallbackClass = ActionTestBloodTargetCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "Player started testing your blood.";
		m_MessageSuccess = "Player finished testing your blood.";
		m_MessageFail = "Player moved and testing was canceled.";
		m_MessageCancel = "You stopped testing.";
	//	m_Animation = "testblood";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_TEST_BLOOD_T;
	}
		
	override string GetText()
	{
		return "test blood of target";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntraget = PlayerBase.Cast( target.GetObject() );
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		int blood_type = ntraget.GetStatBloodType().Get();
		
		module_lifespan.UpdateBloodTypeVisibility( ntraget, true );
		module_lifespan.UpdateBloodType( ntraget, blood_type );
		
		item.Delete();
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};