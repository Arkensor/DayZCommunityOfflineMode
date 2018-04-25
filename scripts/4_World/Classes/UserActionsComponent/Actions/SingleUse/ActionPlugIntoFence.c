class ActionPlugIntoFence: ActionSingleUseBase
{
	void ActionPlugIntoFence()
	{
		m_MessageStartFail = "m_MessageStartFail";
		m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "m_MessageSuccess";
		m_MessageFail = "m_MessageFail";
		m_MessageCancel = "m_MessageCancel";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_PLUG_INTO_FENCE;
	}
	
	override string GetText()
	{
		return "plug in";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		if ( targetObject  &&  item  &&  targetObject.IsInherited(BaseBuildingBase))
		{
			BaseBuildingBase target_BBB = BaseBuildingBase.Cast( targetObject );
			
			BarbedWire bw = BarbedWire.Cast( target_BBB.GetAttachmentByType(BarbedWire) );
			
			if ( bw  &&  bw.GetCompEM().CanReceivePlugFrom(item) )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		BarbedWire bw = BarbedWire.Cast( target_IB.GetAttachmentByType(BarbedWire) );
		item.GetCompEM().PlugThisInto(bw);
		
		if ( !player.IsPlacingServer() )
		{
			//player.TogglePlacing( target, item );
		}
	}
};