class ActionTuneFrequencyOnGround : ActionInteractBase
{
	void ActionTuneFrequencyOnGround()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I have tuned the frequency.";
		m_MessageFail = "Cannot tune the frequency on the device without power.";
	}

	override int GetType()
	{
		return AT_TUNE_FREQUENCY_ON_GROUND;
	}

	override string GetText()
	{
		return "tune frequency";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		Land_Radio_PanelBig transmitter = Land_Radio_PanelBig.Cast( target_object );
		string selection = target_object.GetActionComponentName( target.GetComponentIndex() );
		
		if ( transmitter.CanOperate() && selection == "control_panel" )
		{
			transmitter.DisplayRadioInfo( transmitter.GetTunedFrequency().ToString(), player );
			
			return true;
		}
		
		return false;
	}
		
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Land_Radio_PanelBig transmitter = Land_Radio_PanelBig.Cast( target.GetObject() );
		
		if ( transmitter.CanOperate() )
		{
			transmitter.SetNextFrequency( player );
		}
	}
}