class CCINone : CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		return true;
	}
	
	override bool CanContinue( PlayerBase player, ItemBase item )
	{
		return true;
	}
};