class CfgParamArray extends CfgParam
{
	private ref array<ref CfgParam> m_Values;
	
	void CfgParamArray(string param_name)
	{
		m_Values = new array<ref CfgParam>;
	}
	
	void ~CfgParamArray()
	{
		if ( m_Values != NULL )
		{
			Clear();
		}
	}
	
	void Clear()
	{
		m_Values.Clear();
	}
	
	array<ref CfgParam> GetValues()
	{
		return m_Values;
	}
	
	CfgParam GetValue(local int i)
	{
		return m_Values.Get(i);
	}
	
	void InsertValue(local CfgParam value)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<ref CfgParam>;
		}
		
		m_Values.Insert(value);
	}
	
	bool IsParamExist(local string name, local int cfg_type)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<ref CfgParam>;
		}
		
		for ( local int i = 0; i < m_Values.Count(); ++i )
		{
			local CfgParam p = m_Values.Get(i);
			
			if ( name == p.GetName() )
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool RenameParam(local string name, local int cfg_type, string new_name)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<ref CfgParam>;
		}
		
		for ( local int i = 0; i < m_Values.Count(); ++i )
		{
			local CfgParam p = m_Values.Get(i);
			
			if ( name == p.GetName() )
			{
				p.SetName(new_name);
				return true;
			}
		}
		
		return false;		
	}
	
	CfgParam GetValueByName(local string name, local int cfg_type)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<ref CfgParam>;
		}
		
		for ( local int i = 0; i < m_Values.Count(); ++i )
		{
			local CfgParam p = m_Values.Get(i);
			
			if ( name == p.GetName() )
			{
				return p;
			}
		}
		
		CfgParam param = CreateParam(name, cfg_type);
		m_Values.Insert(param);
		
		return param;
	}
	
	void SetParams(array<ref CfgParam> value_array)
	{
		m_Values = value_array;
	}	
	
	bool IsValueArray()
	{
		return true;
	}
	
	override int GetType()
	{
		return CFG_TYPE_ARRAY;
	}
}