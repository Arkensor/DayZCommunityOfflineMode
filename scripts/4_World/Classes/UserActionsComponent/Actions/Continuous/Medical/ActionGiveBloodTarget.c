class ActionGiveBloodTargetCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveBloodTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.03;
	
	void ActionGiveBloodTarget()
	{
		m_CallbackClass = ActionGiveBloodTargetCB;
		m_MessageStartFail = "It is empty.";
		m_MessageStart = "Player started giving you blood.";
		m_MessageSuccess = "Player finished giving you blood.";
		m_MessageFail = "Player moved and giving you blood was canceled.";
		m_MessageCancel = "You stopped giving blood.";
		//m_Animation = "giveblood";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_GIVE_BLOOD_T;
	}
		
	override string GetText()
	{
		return "give blood";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{	
		//TODO Daniel: integrate ss 2.0
		/*float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}*/
		
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		item.TransferModifiers(ntarget);
		Param1<float> nacdata = Param1<float>.Cast( acdata );
		float delta = nacdata.param1;
		
		ntarget.AddHealth("","Blood",delta);
		//ntarget.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		//float delta = nacdata.param1 * efficiency;
		//player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );

		int itembloodtype = 0; //item.GetVar(BloodType);
		int bloodtypetarget = ntarget.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(itembloodtype, bloodtypetarget);
		if ( !bloodmatch )
		{
			ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_HEMOLYTIC_REACTION);
		}	
		if ( item.IsKindOf("BloodSyringe") )
		{
			MiscGameplayFunctions.TurnItemIntoItemEx(player, new SyringeLambda(item, "Syringe", player));
		}
		else if ( item.GetQuantity() < 1 )
		{
			item.Delete();
		}

		//this condition protects spamming UAs for exp without using items quantity
		if( nacdata.param1 != 0 )
		{
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};