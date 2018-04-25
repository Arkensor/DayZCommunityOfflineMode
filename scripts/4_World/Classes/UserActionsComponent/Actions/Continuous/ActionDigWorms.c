class ActionDigWormsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_WORMS);
	}
};

class ActionDigWorms: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.2;

	void ActionDigWorms()
	{
		m_CallbackClass = ActionDigWormsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGHOE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		m_MessageStartFail = "I have no beard to be shaved.";
		m_MessageStart = "I have started shaving myself.";
		m_MessageSuccess = "I have shaved myself.";
		m_MessageFail = "I have failed to shave myself.";
		m_MessageCancel = "I stopped shaving myself.";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !player.IsPlacingLocal() )
		{
			if ( target )
			{
				string surface_type;
				vector position;
				position = target.GetCursorHitPos();
				GetGame().SurfaceGetType ( position[0], position[2], surface_type );
				if ( surface_type == "cp_dirt"  ||  surface_type == "cp_grass"  ||  surface_type == "cp_grass_tall"  ||  surface_type == "cp_conifer_common1"  ||  surface_type == "cp_conifer_common2" ||  surface_type == "cp_conifer_moss1"  ||  surface_type == "cp_conifer_moss2"  ||  surface_type == "cp_broadleaf_dense1"  ||  surface_type == "cp_broadleaf_dense2" ||  surface_type == "cp_broadleaf_sparse1"  ||  surface_type == "cp_broadleaf_sparse2" || surface_type == "cp_gravel" )
				{
					return true;
				}
			}
		}
	
		return false;
	}

	override int GetType()
	{
		return AT_DIG_WORMS;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Dig up worms";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		ItemBase worms;
		Class.CastTo(worms,  GetGame().CreateObject("Worm", player.GetPosition()) );
		worms.SetQuantity(10,false);
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};