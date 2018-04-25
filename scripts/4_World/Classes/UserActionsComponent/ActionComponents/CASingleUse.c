class CASingleUse : CASingleUseBase
{	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
		{
			return UA_ERROR;
		}

		return UA_FINISHED;
	}
};