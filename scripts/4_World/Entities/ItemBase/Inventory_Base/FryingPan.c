class FryingPan extends Inventory_Base
{
	override bool IsContainer()
	{
		return true;		
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( !(parent.IsKindOf("FryingPan")) && !(parent.IsKindOf("Container_Base")) )
		{
			return true;
		}

		return false;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( parent.IsKindOf("FireplaceBase") )
		{
			return true;
		}
		
		return false;
	}
}