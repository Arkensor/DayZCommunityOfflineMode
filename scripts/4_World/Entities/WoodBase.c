class WoodBase extends Plant
{
	override bool IsWoodBase()
	{
		return true;
	}

	int GetAmountOfDrops(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 3;
			}
			else
			{
				return 100; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return 1;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 1;
			}
			else
			{
				return 3;
			}
		}
	}

	string GetMaterial(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return "Bark_Oak";
			}
			else if ( item && item.KindOf("Axe") )
			{
				return "FireWood";
			}
			else
			{
				return "LongWoodenStick"; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return "LongWoodenStick";
			}
			else if ( item && item.KindOf("Axe") )
			{
				return "LongWoodenStick";
			}
			else
			{
				return "WoodenStick";
			}
		}
	}

	int GetAmountOfMaterialPerDrop(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return 1;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 1;
			}
			else
			{
				return 1; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return 1;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 1;
			}
			else
			{
				return 1;
			}
		}
	}

	float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 5;
			}
			else
			{
				return 0; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 5;
			}
			else
			{
				return 0;
			}
		}
	}
};