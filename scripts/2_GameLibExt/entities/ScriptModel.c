#ifdef GAME_TEMPLATE

[EditorAttribute("box", "GameLib/Scripted", "Script model", "-0.25 -0.25 -0.25", "0.25 0.25 0.25", "255 0 0 255", "0 0 0 0", true, true, true)]
class ScriptModelClass
{

}

ScriptModelClass ScriptModelSource;

class ScriptModel: GenericEntity
{
	[Attribute("", "resourceNamePicker", "Model", "xob")]
	string Model;
	[Attribute("1", "combobox", "Physics", "", { ParamEnum("None", "2"), ParamEnum("Static", "1"), ParamEnum("Dynamic", "0") } )]
	int Type;

	void ScriptModel(IEntitySource src, IEntity parent)
	{
		if (Model== "")
			return;

		SetFlags(EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false);
		
		vobject vobj = GetObject(Model);
		SetObject(vobj, "");
		ReleaseObject(vobj, false);

		if (Type == 1)
		{
			dBodyCreateStatic(this, 0xffffffff); // todo - defines for layer mask			
		}
		else if (Type == 0)
		{
			if(!dBodyCreateDynamic(this, 1.0, 0xffffffff))
			{
				//create implicit box
				vector mins, maxs;
				GetBounds(mins, maxs);
				vector center = (mins + maxs) * 0.5;
				vector size = maxs - mins;
								
				ref PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateBox(size), "material/default", 0xffffffff)};
				dBodyCreateDynamicEx(this, center, 1, geoms);
			}
			if(dBodyIsSet(this))
			{
				dBodySetMass(this, 1.0);
				dBodyActive(this, ActiveState.ACTIVE);
				dBodyDynamic(this, true);
			}
		}
	}

	void ~ScriptModel()
	{
		if(dBodyIsSet(this))
			dBodyDestroy(this);
	}
}

#endif