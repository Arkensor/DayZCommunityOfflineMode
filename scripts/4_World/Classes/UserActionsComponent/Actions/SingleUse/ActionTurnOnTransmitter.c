class ActionTurnOnTransmitterCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseTurnOnPlugged;
	}
}

class ActionTurnOnTransmitter: ActionTurnOnWhileInHands
{
	void ActionTurnOnTransmitter()
	{
		m_CallbackClass = ActionTurnOnTransmitterCB;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIEON;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I have turned it on.";
		m_MessageStartFail = "Cannot turn on the device without power source.";
		m_MessageFail = "It's broken.";
	}
	
	override int GetType()
	{
		return AT_TURN_ON_TRANSMITTER;
	}

	override string GetText()
	{
		return "turn on";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsTransmitter() )
		{
			if ( item.HasEnergyManager() && item.GetCompEM().CanSwitchOn() )
			{
				if ( item.IsInherited( TransmitterBase ) )
				{
					TransmitterBase transmitter = TransmitterBase.Cast( item );
					
					transmitter.DisplayRadioInfo( "OFF", player );
				}
				
				return true;
			}				
		}
		
		return false;
	}
		
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		item.GetCompEM().SwitchOn();
	}
}
