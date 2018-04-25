class TreeHard_t_BetulaPendula_1f: TreeHard 
{
	override string GetMaterial(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return "Bark_Birch";
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
};

class TreeHard_t_BetulaPendula_1fb: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_1s: TreeHard_t_BetulaPendula_1f
{              
};             
class TreeHard_t_BetulaPendula_2f: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_2fb: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_2fc: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_2s: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_3f: TreeHard_t_BetulaPendula_1f
{                
};               
                 
class TreeHard_t_BetulaPendula_3fb: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_3s: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_3fc: TreeHard_t_BetulaPendula_1f
{
}; 

class TreeSoft_t_malus1s: TreeSoft
{
	override int GetAmountOfDrops(ItemBase item)
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
			return 20;
		}
	}	

	override string GetMaterial(ItemBase item)
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
			return "Apple"; 
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