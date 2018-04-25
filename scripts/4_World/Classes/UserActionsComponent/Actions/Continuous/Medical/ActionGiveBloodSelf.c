class ActionGiveBloodSelfCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveBloodSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.03;

	void ActionGiveBloodSelf()
	{
		m_CallbackClass = ActionGiveBloodSelfCB;
		m_MessageStartFail = "There's no blood left.";
		m_MessageStart = "I have started giving blood myself";
		m_MessageSuccess = "I gave myself blood.";
		m_MessageFail = "I have moved and giving blood was canceled.";
		m_MessageCancel = "I stopped giving self a blood.";
		//m_Animation = "GIVEBLOODS";
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
		return AT_GIVE_BLOOD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "give blood";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Param1<float> nacdata = Param1<float>.Cast( acdata );
		float delta = nacdata.param1;
		
		player.AddHealth("","Blood",delta);
		//player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") );
		
		int itembloodtype = 0; //item.GetVar(BloodType);
		int bloodtypetarget = player.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(itembloodtype, bloodtypetarget);
		if ( !bloodmatch )
		{
			player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_HEMOLYTIC_REACTION);
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

class SyringeLambda : ReplaceItemWithNewLambda
{
	void SyringeLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if (new_item) 
		{
			ItemBase old_item_IB = ItemBase.Cast(old_item);
			ItemBase new_item_IB = ItemBase.Cast(new_item);
			
			MiscGameplayFunctions.TransferItemProperties(old_item_IB, new_item_IB, true, false, true);
		}
		else
		{
			Debug.LogError("SyringeLambda: failed to create new item","static");
		}
	}
};