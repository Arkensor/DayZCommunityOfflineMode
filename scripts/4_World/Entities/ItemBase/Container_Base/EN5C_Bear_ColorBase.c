class Bear_ColorBase extends Container_Base
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		//better use something like super.GetName()
		if ( !(parent.IsKindOf("Bear_ColorBase")) )
		{
			return true;
		}
		
		return false;
	}
}

class Bear_Beige : Bear_ColorBase
{
}

class Bear_Dark : Bear_ColorBase
{
}

class Bear_Pink : Bear_ColorBase
{
}

class Bear_White : Bear_ColorBase
{
}