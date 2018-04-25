class ActionBuryBodyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.BURY_BODY);
	}
};

class ActionBuryBody: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionBuryBody()
	{
		m_CallbackClass = ActionBuryBodyCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting with hand drill.";
		m_MessageSuccess = "I have ignited the fireplace with a hand drill.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		//m_Animation = "DIGGINGSHOVEL";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_BURY_BODY;
	}
		
	override string GetText()
	{
		return "bury the body";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		
		if ( body_EAI  &&  !body_EAI.IsAlive()  &&  (body_EAI.IsInherited(DayZCreature )  ||  body_EAI.IsInherited(Man)) )
		{
			string surface_type;
			vector position = body_EAI.GetPosition();
			GetGame().SurfaceGetType ( position[0], position[2], surface_type );
			array<string> surface_types = {"cp_dirt", "cp_grass", "cp_grass_tall", "cp_conifer_common1", "cp_conifer_common2", "cp_conifer_moss1", "cp_conifer_moss2", "cp_broadleaf_dense1", "cp_broadleaf_dense2", "cp_broadleaf_sparse1", "cp_broadleaf_sparse2", "cp_gravel" };
			
			if ( surface_types.Find(surface_type) )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object targetObject = target.GetObject();
		g_Game.ObjectDelete(targetObject);

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight ); 
	}
};
