class ActionForceABiteCan: ActionForceABite
{
	override int GetType()
	{
		return AT_FORCE_A_BITE_CAN;
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