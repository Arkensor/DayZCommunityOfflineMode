class ActionGiveSalineTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SALINE);
	}
};

class ActionGiveSalineTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionGiveSalineTarget()
	{
		m_CallbackClass = ActionGiveSalineTargetCB;
		m_MessageStartFail = "Bag is empty.";
		m_MessageStart = "Player started giving you saline.";
		m_MessageSuccess = "Player finished giving you saline.";
		m_MessageFail = "Player moved and giving you saline was canceled.";
		m_MessageCancel = "You stopped giving saline.";
		//m_Animation = "givesaline";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);	
	}

	override int GetType()
	{
		return AT_GIVE_SALINE_T;
	}
		
	override string GetText()
	{
		return "give saline";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		//TODO Daniel: integrate ss 2.0
		/*float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}*/
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		Param1<float> nacdata = Param1<float>.Cast( acdata );
		float delta = nacdata.param1;
		//ntarget.AddHealth("", "Blood", delta);
		//ntarget.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		//float delta = nacdata.param1 * efficiency;
		//player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );
		
		ntarget.GetModifiersManager().ActivateModifier(eModifiers.MDF_SALINE);
		
		//this condition protects spamming UAs for exp without using items quantity
		if ( nacdata.param1 != 0 )
		{
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}

		item.Delete();
	}
};