class Container_Base extends ItemBase
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( parent != this && ( this.GetType() != parent.GetType() ) )
		{
			return true;
		}
		
		return false;
	}
}