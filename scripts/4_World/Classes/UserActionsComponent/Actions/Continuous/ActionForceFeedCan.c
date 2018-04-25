class ActionForceFeedCan: ActionForceFeed
{	
	override int GetType()
	{
		return AT_FEED_CAN;
	}
		
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return true;
		/* 
		//if staging for cans is introduced
		if ( item && item.IsOpen() )
		{
			return true;
		}
		return false;
		*/
	}
};