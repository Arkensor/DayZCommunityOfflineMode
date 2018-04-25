class NutritionalProfile
{
	float m_Energy;
	float m_WaterContent;
	float m_NutritionalIndex;
	float m_FullnessIndex;
	float m_Toxicity;
	
	void NutritionalProfile(float energy, float water_content, float nutritional_index, float fullness_index, float toxicity)
	{
		m_Energy = energy;
		m_WaterContent = water_content;
		m_NutritionalIndex = nutritional_index;
		m_FullnessIndex = fullness_index;
		m_Toxicity = toxicity;
	}
	
	float GetEnergy()
	{
		return m_Energy;
	}
	
	float GetWaterContent()
	{
		return m_WaterContent;
	}
	
	float GetNutritionalIndex()
	{
		return m_NutritionalIndex;
	}
	
	float GetFullnessIndex()
	{
		return m_FullnessIndex;
	}
	
	float GetToxicity()
	{
		return m_Toxicity;
	}
}
