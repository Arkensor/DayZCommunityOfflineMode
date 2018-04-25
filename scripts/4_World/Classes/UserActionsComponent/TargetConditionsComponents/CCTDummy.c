class CCTDummy : CCTBase
{
	override bool Can( PlayerBase player, ActionTarget target )
	{
		if( target && target.GetObject() )
		{
			return true;
		}
		return false;
	}
};