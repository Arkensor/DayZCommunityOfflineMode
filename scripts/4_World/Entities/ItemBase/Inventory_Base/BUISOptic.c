class BUISOptic extends Inventory_Base
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		/*if ( parent.IsKindOf("M4A1_Base") )
		{
			return true;
		}

		return false;*/
		return true;
	}
}