class CCTNone : CCTBase
{
	override bool Can( PlayerBase player, ActionTarget target )
	{
		return true;
	}
	
	override bool CanContinue( PlayerBase player, ActionTarget target )
	{
		return true;
	}
};