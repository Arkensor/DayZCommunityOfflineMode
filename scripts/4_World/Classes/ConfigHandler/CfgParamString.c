class CfgParamString extends CfgParamType
{	
	private string m_Value;
	
	void CfgParamString(string param_name)
	{
		m_Value = STRING_EMPTY;
	}
	
	void SetValue(string value)
	{
		m_Value = value;
	}
	
	string GetValue()
	{
		return m_Value;
	}
	
	override int GetType()
	{
		return CFG_TYPE_STRING;
	}
}