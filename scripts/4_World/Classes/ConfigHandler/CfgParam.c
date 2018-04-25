const int CFG_TYPE_STRING	= 0;
const int CFG_TYPE_INT		= 1;
const int CFG_TYPE_FLOAT	= 2;
const int CFG_TYPE_BOOL		= 3;
const int CFG_TYPE_ARRAY	= 4;
const int CFG_TYPE_PARAM	= 5;

class CfgParam: Managed
{
	protected string	m_ParamName;
	protected string	m_ParamValueString;
	
	void CfgParam(string param_name)
	{
		m_ParamName	= param_name;
	}
	
	void SetName(string param_name)
	{
		m_ParamName	= param_name;
	}
	
	string GetName()
	{
		return m_ParamName;
	}
	
	int GetType()
	{
		return -1;
	}

	bool IsArray()
	{
		return false;
	}
	
	static CfgParam CreateParam(local string name, local int cfg_type)
	{
		local CfgParam param;

		switch (cfg_type)
		{
			case CFG_TYPE_STRING:
			{
				param = new CfgParamString(name);
				break;
			}
			case CFG_TYPE_INT:
			{
				param = new CfgParamInt(name);
				break;
			}
			case CFG_TYPE_FLOAT:
			{
				param = new CfgParamFloat(name);
				break;
			}
			case CFG_TYPE_BOOL:
			{
				param = new CfgParamBool(name);
				break;
			}
			case CFG_TYPE_ARRAY:
			{
				param = new CfgParamArray(name);
				break;
			}
		}
		
		return param;
	}
}