class Food_Lard extends Edible_Base
{
	override bool CanEat()
	{
		EntityAI entity_owner = GetHierarchyParent();
		
		if ( entity_owner && entity_owner.GetInventory().HasAttachment(this) )
		{
			return false;
		}
		
		return true;
	}
}
