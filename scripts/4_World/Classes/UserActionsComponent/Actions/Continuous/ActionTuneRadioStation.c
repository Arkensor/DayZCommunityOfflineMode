class ActionTuneRadioStationCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 1.0;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
}

class ActionTuneRadioStation: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.01;
	
	void ActionTuneRadioStation()
	{
		m_CallbackClass = ActionTuneRadioStationCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIETUNE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_WALKIETALKIETUNE;	
		m_MessageStartFail = "I've failed to change the radio station.";
		m_MessageStart = "I've changed the radio station.";
		m_MessageFail = "I've failed to change the radio station.";
		m_MessageCancel = "I have stopped the radio tunning.";
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
		return AT_TUNE_RADIO_STATION;
	}
		
	override string GetText()
	{
		return "tune radio station";
	}
	
	override bool HasTarget()
	{
		return false;
	}	

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Radio radio = Radio.Cast( item );
		
		if ( radio.CanOperate() ) 
		{
			return true;
		}
		
		return false;
	}

	override void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Radio radio = Radio.Cast( item );
		radio.TuneNextStation();
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}