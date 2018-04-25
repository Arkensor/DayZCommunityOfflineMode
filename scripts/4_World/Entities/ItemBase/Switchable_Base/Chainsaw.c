class Chainsaw extends Switchable_Base
{
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if( !GetCompEM().IsSwitchedOn() )
		{
			return true;
		}
		return false;
	}
}