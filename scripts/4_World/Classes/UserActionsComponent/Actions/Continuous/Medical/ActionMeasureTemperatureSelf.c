class ActionMeasureTemperatureSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.MEASURE_TEMP);
	}
};

class ActionMeasureTemperatureSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;

	void ActionMeasureTemperatureSelf()
	{
		m_CallbackClass = ActionMeasureTemperatureSelfCB;
		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "I have started measuring myself";
		m_MessageSuccess = "I have measured myself.";
		m_MessageFail = "I have moved and measuring was canceled.";
		m_MessageCancel = "I stopped measuring.";
		//m_Animation = "measure";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_MEASURE_TEMPERATURE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "measure temperature";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if (player.GetStatTemperature())
		{
			float temperature = Math.Floor(player.GetStatTemperature().Get()*10)/10;
			string message = "Thermometer displays "+temperature.ToString()+" ?C";
			SendMessageToClient(player, message);
		}
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};