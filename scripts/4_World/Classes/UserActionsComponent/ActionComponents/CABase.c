class CABase 
{	
	protected ActionBase				m_Action;
	protected float 					m_LastTick;
	protected ref Param					m_ACData;
	protected ref Param2<float,float>	m_ProgressParam;
	
	void Init( ActionBase action, PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_ProgressParam = new Param2<float,float>(0,0);
		m_Action = action;
		if ( !m_ACData ) 
		{
			m_ACData = new Param;
		}
		m_LastTick = GetGame().GetTime();
		Setup(player,target,item);
	}
	
	void Setup( PlayerBase player, ActionTarget target, ItemBase item  )
	{
	}

	int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		return UA_ERROR;
	}
	
	int Cancel( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return UA_CANCEL;
	}

	int Interrupt( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Cancel( player, target, item);
		return UA_INTERRUPT;
	}
		
	void SetACData(Param units) //overload this method if you want to send more than one parameter out of the action component
	{
		if ( m_ACData )
		{
			m_ACData = units;
		}
	}
	
	Param GetACData() 
	{
		if ( m_ACData ) 
		{
			return m_ACData;
		}
		return NULL;
	}
	
	bool IsContinuousAction() //informs command callback whether action is looped or oneshot
	{
		return false;
	}
	
	float GetProgress()
	{
		return 0;
	}
};