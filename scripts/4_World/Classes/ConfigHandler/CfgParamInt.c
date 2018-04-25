class CfgParamInt extends CfgParamType
{	
	private int m_Value;
	
	void CfgParamInt(string param_name)
	{
		m_Value = 0;
	}
	
	void SetValue(int value)
	{
		m_Value = value;
	}
	
	int GetValue()
	{
		return m_Value;
	}
	
	override int GetType()
	{
		return CFG_TYPE_INT;
	}
}