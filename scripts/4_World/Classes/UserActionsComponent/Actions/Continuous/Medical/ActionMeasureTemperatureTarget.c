class ActionMeasureTemperatureTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.MEASURE_TEMP);	
	}
};

class ActionMeasureTemperatureTarget : ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
	
	void ActionMeasureTemperatureTarget()
	{
		m_CallbackClass = ActionMeasureTemperatureTargetCB;
		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "Player started messuring you.";
		m_MessageSuccess = "Player finished measuring you.";
		m_MessageFail = "Player moved and measuring was canceled.";
		m_MessageCancel = "You stopped measuring.";
		//m_Animation = "measure";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_MEASURE_TEMPERATURE_T;
	}
		
	override string GetText()
	{
		return "measure temperature";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		if (ntarget.GetStatTemperature()) 
		{ 
			float temperature = Math.Floor(ntarget.GetStatTemperature().Get()*10)/10;
			string message = "Thermometer displays "+temperature.ToString()+" ?C";
			SendMessageToClient(player, message);
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};