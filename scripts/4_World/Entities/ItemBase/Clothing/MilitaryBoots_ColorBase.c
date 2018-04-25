class MilitaryBoots_ColorBase extends ClothingBase
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( parent != this )
		{
			return true;
		}
		return false;
	}	
}