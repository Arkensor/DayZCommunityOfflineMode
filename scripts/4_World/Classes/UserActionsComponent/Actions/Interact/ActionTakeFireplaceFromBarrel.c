class ActionTakeFireplaceFromBarrel: ActionInteractBase
{
	void ActionTakeFireplaceFromBarrel()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I took the fireplace.";
	}

	override int GetType()
	{
		return AT_TAKE_FIREPLACE_BARREL;
	}

	override string GetText()
	{
		return "take fireplace";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		
		if ( target_object )
		{
			BarrelHoles_ColorBase fireplace_barrel = BarrelHoles_ColorBase.Cast( target_object );
			
			//check barrel fireplace state
			if ( fireplace_barrel.IsOpened() && !fireplace_barrel.HasAshes() && !fireplace_barrel.IsBurning() )
			{
				//check cargo and attachments
				if ( fireplace_barrel.IsCargoEmpty() && fireplace_barrel.GetInventory().AttachmentCount() > 0 )
				{
					return true;
				}
			}
		}

		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object target_object = target.GetObject();
		BarrelHoles_ColorBase fireplace_barrel = BarrelHoles_ColorBase.Cast( target_object );
		
		MiscGameplayFunctions.TurnItemIntoItemEx(player, new TurnItemIntoItemLambda(fireplace_barrel, "Fireplace", player));
	}
}