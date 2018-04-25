class Candle extends Inventory_Base
{
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( !parent.HasSelection("flame") )
		{
			return true;
		}
		return false;
	}
}