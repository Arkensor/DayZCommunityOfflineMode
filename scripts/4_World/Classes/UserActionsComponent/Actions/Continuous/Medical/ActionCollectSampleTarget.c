class ActionCollectSampleTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.COLLECT_SAMPLE);
	}
};

class ActionCollectSampleTarget : ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionCollectSampleTarget()
	{
		m_CallbackClass = ActionCollectSampleTargetCB;
		m_MessageStartFail = "iT'S full.";
		m_MessageStart = "Player started collectin sample of your blood.";
		m_MessageSuccess = "Player finished collecting sample of your blood.";
		m_MessageFail = "Player moved and collecting of sample was canceled.";
		m_MessageCancel = "You stopped collecting.";
		//m_Animation = "splint";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_COLLECT_SAMPLE_T;
	}

	override string GetText()
	{
		return "collect sample";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		Param1<float> nacdata;
		Class.CastTo(nacdata,  acdata );
		float delta = (nacdata.param1 / UATimeSpent.COLLECT_SAMPLE);

		ActionCollectBloodTargetLambda lambda = new ActionCollectBloodTargetLambda(item, "BloodSyringe", player, m_SpecialtyWeight, ntarget, delta);
		player.LocalReplaceItemInHandsWithNew(lambda);
	}
	
	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		OnCompleteServer( player, target, item, acdata );
	}
};

