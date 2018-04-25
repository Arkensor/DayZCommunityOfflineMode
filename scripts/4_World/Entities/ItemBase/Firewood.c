class Firewood extends ItemBase
{
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( !( this.GetQuantity() > 5 ) )
		{
			return true;
		}
		return false;
	}
}