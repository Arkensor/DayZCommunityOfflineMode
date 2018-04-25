class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 50;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFill(QUANTITY_FILLED_PER_SEC);
	}
};

class ActionFillBottleBase: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;
	
	void ActionFillBottleBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling the bottle.";
		m_MessageSuccess = "I have finished filling the bottle..";
		m_MessageFail = "Player moved and filling the bottle was canceled.";
		m_MessageCancel = "I stopped filling the bottle.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		
		m_CallbackClass = ActionFillBottleBaseCB;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_FILL_BOTTLE;
	}

	override bool Consumable()
	{
		return true;
	}

	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase item = player.GetItemInHands();

		return "fill "+item.GetDisplayName().Substring(0,(item.GetDisplayName().Length() )); //crops the '' bit from the displayname
		//return " fill bottle"; //default
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return true;
		
		vector pos_cursor = target.GetCursorHitPos();
		if( g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) || (target.GetObject() && (target.GetObject().GetType() == "Land_Misc_Well_Pump_Yellow" || target.GetObject().GetType() == "Land_Misc_Well_Pump_Blue")) )
		{
			if ( vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT && Liquid.CanFillContainer(item, LIQUID_WATER ) )
			{
				return true;
			}
		}
		return false;
	}	
	
	
	override void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		//item.TransferModifiers(player);
		Param1<float> nacdata;
		Class.CastTo(nacdata,  acdata );
		float delta = nacdata.param1;
		Liquid.FillContainerEnviro( item, LIQUID_WATER, delta );

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		OnCompleteLoopServer( player, target, item, acdata );
	}
};