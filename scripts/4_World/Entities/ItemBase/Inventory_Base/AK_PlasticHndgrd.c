class AK_PlasticHndgrd extends Inventory_Base
{
	override bool CanDetachAttachment( EntityAI attachment )
	{
		return false;
	}
}