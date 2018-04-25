class BurlapSackCover extends ClothingBase
{

	override void OnWasAttached(EntityAI parent, string slot_name)
	{
		super.OnWasAttached(parent, slot_name);
		PlayerBase player;
		Class.CastTo(player, parent.GetHierarchyRootPlayer());
		bool selected = player.IsPlayerSelected();//this is 0 upon player's connection to server


		if ( (GetGame().IsClient() || !GetGame().IsMultiplayer()) && player && player.IsControlledPlayer() && slot_name == "Headgear" )
		{
			//GetGame().GetWorld().SetAperture(100000);
			PPEffects.EnableBurlapSackBlindness();
		}
	}

	override void OnWasDetached(EntityAI parent, string slot_name)
	{
		PlayerBase player;
		Class.CastTo(player, parent.GetHierarchyRootPlayer());
		if ( (GetGame().IsClient() || !GetGame().IsMultiplayer()) && player && player.IsControlledPlayer() && slot_name == "Headgear" )
		{
			PPEffects.DisableBurlapSackBlindness();
			//GetGame().GetWorld().SetAperture(0);
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( parent != this )
		{
			return true;
		}
		return false;
	}

	override bool CanDetachAttachment( EntityAI parent )
	{
		return false;
	}
}