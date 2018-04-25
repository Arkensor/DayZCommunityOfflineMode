class AK_RailHndgrd extends Inventory_Base
{
	override bool CanDetachAttachment( EntityAI attachment )
	{
		return false;
	}
}