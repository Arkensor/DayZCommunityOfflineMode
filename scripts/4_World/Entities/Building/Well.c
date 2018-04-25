class Well extends Building
{
	override bool IsBuilding()
	{
		return false;
	}
	
	override bool IsWell()
	{
		return true;
	}
}