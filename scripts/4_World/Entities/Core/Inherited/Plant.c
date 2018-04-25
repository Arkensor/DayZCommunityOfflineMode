//-----------------------------------------------------------------------------
/** \brief Specific tree class can be declared as: class TreeHard + _ + p3d filename (without extension) 
@code
//exapmle for tree DZ\plants2\tree\t_acer2s.p3d
class TreeHard_t_acer2s: TreeHard 
{
};
@endcode
*/

class TreeHard extends PlantSuper
{
	override bool IsTree()
	{
		return true;
	}
};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
class TreeSoft extends PlantSuper
{
	override bool IsTree()
	{
		return true;
	}
};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
class BushHard extends PlantSuper
{
	override bool IsBush()
	{
		return true;
	}

};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
class BushSoft extends PlantSuper
{
	override bool IsBush()
	{
		return true;
	}
};
