class ActionCloseDoors: ActionInteractBase
{
	void ActionCloseDoors()
	{
		m_MessageSuccess = "";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_CLOSE_DOORS;
	}

	override string GetText()
	{
		return "Close door";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false;
		//if( IsDamageDestroyed(target) ) return false;
		if( !IsBuilding(target) ) return false;
		
		Building building;
		if( Class.CastTo(building, target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if ( doorIndex != -1 )
			{
				if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;
				return building.IsDoorOpen(doorIndex);
			}
		}		
		return false;
	}

	override void OnStartServer( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Building building;
		if( Class.CastTo(building, target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if( doorIndex != -1 )
			{
				building.CloseDoor(doorIndex);
			}
		}
	}
};