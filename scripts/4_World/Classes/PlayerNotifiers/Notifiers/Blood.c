class Blood: NotifierBase
{
	protected const float 	DEC_TRESHOLD_LOW 	= -0.001;
	protected const float 	DEC_TRESHOLD_MED 	= -1;
	protected const float 	DEC_TRESHOLD_HIGH	= -2;
	protected const float 	INC_TRESHOLD_LOW 	= 0.001;
	protected const float 	INC_TRESHOLD_MED 	= 1;
	protected const float 	INC_TRESHOLD_HIGH	= 2;

	void Blood(NotifiersManager manager)
	{
	}

	override int GetNotifierType()
	{
		return NTF_BLOOD;
	}

	override void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		//PrintString(delta.ToString());
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_TDCY_BLOOD,tendency);
		
		DSLevels level = DetermineLevel( GetObservedValue(), ModifierConstants.THRESHOLD_BLOOD_WARNING,  ModifierConstants.THRESHOLD_BLOOD_CRITICAL, ModifierConstants.THRESHOLD_ENERGY_EMPTY );
		GetDisplayStatus().SetLevel(eDisplayElements.DELM_TDCY_BLOOD, level);
		
	}

	override float GetObservedValue()
	{
		float blood = m_Player.GetHealth("GlobalHealth","Blood");
		return blood;
	}
};