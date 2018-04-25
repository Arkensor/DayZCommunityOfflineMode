class ActionTuneFrequencyCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 2.0;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
}

class ActionTuneFrequency: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.01;
	
	void ActionTuneFrequency()
	{
		m_CallbackClass = ActionTuneFrequencyCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIETUNE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_WALKIETALKIETUNE;		
		m_MessageStartFail = "I have failed the tunning.";
		m_MessageStart = "I have started the tunning.";
		m_MessageFail = "I have failed the tunning.";
		m_MessageCancel = "I have stopped the tunning.";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCINonRuined;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_TUNE_FREQUENCY;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "tune frequency";
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( item.IsTransmitter() ) 
		{
			TransmitterBase transmitter = TransmitterBase.Cast( item );
			
			if ( transmitter.CanOperate() ) 
			{
				transmitter.DisplayRadioInfo( transmitter.GetTunedFrequency().ToString(), player );
				
				return true;
			}
		}
		
		return false;
	}

	override void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		TransmitterBase transmitter =  TransmitterBase.Cast( item );
		transmitter.SetNextFrequency( player );
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}