class ActionTurnOnTorch: ActionTurnOnWhileInHands
{
	void ActionTurnOnTorch()
	{
		m_MessageSuccess = "I ignitied torch.";
		m_MessageFail = "It's burnt out.";
		//m_Animation = "ignite";
	}

	override int GetType()
	{
		return AT_TURN_ON_TORCH;
	}

	override string GetText()
	{
		return "ignite";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(Torch) && item != NULL && !item.IsDamageDestroyed() )
		{
			Torch target_torch = Torch.Cast( targetObject );
			
			if ( target_torch.GetCompEM().CanWork()  &&  !target_torch.GetCompEM().IsWorking() ) // also checks damage
			{
				if ( item.IsKindOf( "Matchbox" ) && item.GetQuantity() > 0 )
				{
					return true;
				}
				else if ( item.IsInherited( Torch) )
				{
					Torch item_torch = Torch.Cast( item );
					
					if ( item_torch.GetCompEM().IsWorking() )
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

	override void OnStartServer( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item != NULL && item.IsKindOf( "Matchbox" ) )
		{
			item.PlaySound("matchboxStrike", 50);
		}
	}
	
	override void OnStartClient( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item != NULL && item.IsKindOf( "Matchbox" ) )
		{
			item.PlaySound("matchboxStrike", 50);
		}
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(Torch) )
		{
			if ( item.IsKindOf( "Matchbox" ) )
			{
				item.AddQuantity(-1,true);
			}
			
			Torch torch = Torch.Cast( targetObject );
			torch.GetCompEM().SwitchOn();
		}
	}
};