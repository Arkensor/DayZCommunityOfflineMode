class CfgParamFloat extends CfgParamType
{	
	private float m_Value;
	
	void CfgParamFloat(string param_name)
	{
		m_Value = 0;
	}
	
	void SetValue(float value)
	{
		m_Value = value;
	}
	
	float GetValue()
	{
		return m_Value;
	}
	
	override int GetType()
	{
		return CFG_TYPE_FLOAT;
	}
}