class CAContinuousFish : CAContinuousBase
{
	protected FishingRod_Base m_Rod;
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Class.CastTo(m_Rod, item);
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( m_Rod != item )
		{
			return UA_ERROR;
		}
		
		if ( GetGame().IsServer() ) 
		{
			m_Rod.AddPull(player.GetDeltaT());
		}
		return UA_PROCESSING;	
	}
};
