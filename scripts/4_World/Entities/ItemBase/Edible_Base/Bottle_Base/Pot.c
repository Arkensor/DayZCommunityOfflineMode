class Pot extends Bottle_Base
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( !(parent.IsKindOf("Pot")) && !(parent.IsKindOf("Container_Base")))
		{
			return true;
		}
		
		return false;
	}
}