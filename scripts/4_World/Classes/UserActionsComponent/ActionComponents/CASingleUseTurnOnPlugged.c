class CASingleUseTurnOnPlugged : CASingleUse
{	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !item )
		{
			return UA_ERROR;
		}
		if ( !item.GetCompEM().CanWork() )
		{
			return UA_SETEND_2;
		}
		return UA_FINISHED;
	}
};