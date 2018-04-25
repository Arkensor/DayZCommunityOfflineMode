class ActionGiveSalineSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SALINE);
	}
};

class ActionGiveSalineSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionGiveSalineSelf()
	{
		m_CallbackClass = ActionGiveSalineSelfCB;
		m_MessageStartFail = "There's nothing in it.";
		m_MessageStart = "I have started giving me saline";
		m_MessageSuccess = "I have salined myself.";
		m_MessageFail = "I have moved and giving saline was canceled.";
		m_MessageCancel = "I stopped giving saline myself.";
		//m_Animation = "givesaline";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_GIVE_SALINE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "give saline";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{		
		item.TransferModifiers(player);
		Param1<float> nacdata = Param1<float>.Cast( acdata );		
		float delta = nacdata.param1;
		//player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );
		//player.AddHealth("", "Blood", delta);
		player.GetModifiersManager().ActivateModifier(eModifiers.MDF_SALINE);
		//this condition protects spamming UAs for exp without using items quantity
		if ( nacdata.param1 != 0 )
		{
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
		item.Delete();
	}
};