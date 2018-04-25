class BushHard_b_canina2s: BushHard 
{
	override int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 3;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 10;
		}
	}	

	override string GetMaterial(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return "WoodenStick";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "LongWoodenStick";
		}
		else
		{
			return "CaninaBerry"; 
		}
	}

	override int GetAmountOfMaterialPerDrop(ItemBase item)
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

	override float GetDamageToMiningItemEachDrop(ItemBase item)
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
};

class BushHard_b_sambucus: BushHard 
{
	override int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 3;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 10;
		}
	}	

	override string GetMaterial(ItemBase item)
	{
		Print("getmaterial: Sambucus");
		if ( item && item.KindOf("Knife") )
		{
			return "WoodenStick";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "LongWoodenStick";
		}
		else
		{
			return "SambucusBerry"; 
		}
	}

	override int GetAmountOfMaterialPerDrop(ItemBase item)
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

	override float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 0.1;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 0.05;
		}
		else
		{
			return 0;
		}
	}
};