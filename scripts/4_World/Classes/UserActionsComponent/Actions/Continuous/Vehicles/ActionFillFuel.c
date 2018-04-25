class ActionFillFuelCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		//should it really use CAContinuousQuantityEdible?
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.FUEL,UATimeSpent.DEFAULT);
	}
};

class ActionFillFuel: ActionContinuousBase
{
	void ActionFillFuel()
	{
		m_CallbackClass = ActionFillFuelCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I have started consuming.";
		m_MessageSuccess = "I have finished consuming.";
		m_MessageFail = "Player moved and consuming was canceled.";
		m_MessageCancel = "I stopped consuming.";
		//m_Animation = "EATTABLETS";		
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_FILL_FUEL;
	}

	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase item = player.GetItemInHands();
		
		//return "fill "+item.GetDisplayName().Substring(0,(item.GetDisplayName().Length() )); //crops the '' bit from the displayname
		return " fill bottle"; //default
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( !target ) return false;
		if( !IsTransport(target) ) return false;
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;
		
		Car car = Car.Cast(target.GetObject());

		if( car.IsActionComponentPartOfSelection(target.GetComponentIndex(), "refill") )
		{
			return true;
		}
		
		return false;
	}	
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Print("Refill Complete");
		Car car = Car.Cast(target.GetObject());

		if( car.IsActionComponentPartOfSelection(target.GetComponentIndex(), "refill") )
		{
			Param1<float> nacdata = Param1<float>.Cast( acdata );
			
			//float consumed_fuel = car.Fill( CarFluid.FUEL, nacdata.param1 );
			//int consumed_fuel = car.AddFuel( nacdata.param1 );
			
			item.AddQuantity( -0.1 );
		}
	}
};