class ActionTakeABiteCan: ActionTakeABite
{
	override int GetType()
	{
		return AT_TAKE_A_BITE_CAN;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return false;
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