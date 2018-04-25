class ComponentsBank
{
	private EntityAI	m_EntityParent;
	private ref Component	m_Components[COMP_TYPE_COUNT];
		
	void ComponentsBank(EntityAI entity_parent)
	{
		m_EntityParent = entity_parent;
	}

	Component GetComponent(int comp_type, string extended_class_name="")
	{
		if ( !Component.IsTypeExist(comp_type) )
		{
			Component.LogErrorBadCompType(comp_type, "EntityAI.GetComponent(int comp_type)");
			return NULL;
		}
		
		if ( !IsComponentAlreadyExist(comp_type) )
		{
			CreateComponent(comp_type, extended_class_name);
		}
		
		return m_Components[comp_type];		
	}
	
	bool DeleteComponent(int comp_type)
	{
		if ( IsComponentAlreadyExist(comp_type) )
		{
			m_Components[comp_type] = NULL;
			return true;
		}
		
		return false;
	}

	private Component CreateComponent(int comp_type, string extended_class_name="")
	{
		if ( !Component.IsTypeExist(comp_type) )
		{
			Component.LogErrorBadCompType(comp_type, "EntityAI->CreateComponent(int comp_type)");
			return NULL;
		}
		
		if ( IsComponentAlreadyExist(comp_type) )
		{
			Component.LogWarningAlredyExist(comp_type, "EntityAI->CreateComponent(int comp_type)");
			return m_Components[comp_type];
		}
		
		
		string clas_name = extended_class_name;
		
		if ( clas_name == string.Empty )
		{
			clas_name = Component.GetNameByType(comp_type);
		}
		
		Component comp = clas_name.ToType().Spawn();
		
		comp.SetParentEntityAI(m_EntityParent);
		comp.Event_OnAwake();
		
		m_Components[comp_type] = comp;
		
		comp.Event_OnInit();
		
		return comp;
	}

	bool IsComponentAlreadyExist(int comp_type)
	{
		if ( m_Components[comp_type] != NULL )
		{
			return true;
		}
		
		return false;
	}
}