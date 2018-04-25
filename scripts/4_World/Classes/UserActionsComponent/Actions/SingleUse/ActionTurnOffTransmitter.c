class ActionTurnOffTransmitterCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUse;
	}
}

class ActionTurnOffTransmitter: ActionTurnOffWhileInHands
{
	void ActionTurnOffTransmitter()
	{
		m_CallbackClass = ActionTurnOffTransmitterCB;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIEOFF;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;		
		m_MessageSuccess = "I have turned it off.";
		m_MessageFail = "I have failed to turn it off.";
	}
	
	override int GetType()
	{
		return AT_TURN_OFF_TRANSMITTER;
	}

	override string GetText()
	{
		return "turn off";
	}
	
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsTransmitter() )
		{
			if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOff() )
			{
				return true;
			}
		}

		return false;
	}
		
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		item.GetCompEM().SwitchOff();
	}	
}