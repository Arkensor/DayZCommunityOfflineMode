#ifdef GAME_TEMPLATE

[EditorAttribute("box", "GameLib/Scripted", "Script light", "-0.25 -0.25 -0.25", "0.25 0.25 0.25", "255 0 0 255")]
class ScriptLightClass
{

}

ScriptLightClass ScriptLightSource;

class ScriptLight: GenericEntity
{
	[Attribute("1", "flags", "Flags", "", { ParamEnum("Point", "1"), ParamEnum("Spot", "2"), ParamEnum("Directional", "4") } )]
	int Flags;
	[Attribute("1", "combobox", "Type", "", { ParamEnum("Point", "1"), ParamEnum("Spot", "2"), ParamEnum("Directional", "3") } )]
	int Type;
	[Attribute("1", "editbox", "Radius", "", NULL )]
	float Radius;
	[Attribute("1 1 1", "color", "Color", "", NULL )]
	vector Color;
	[Attribute("1", "editbox", "Intensity", "", NULL )]
	float Intensity;
	HLIGHT m_light;
	
	void ScriptLight(IEntitySource src, IEntity parent)
	{
		//SetFlags(this, EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE);
		m_light = AddLight(this, LightType.POINT, LightFlags.DYNAMIC|LightFlags.CASTSHADOW, Radius, Color * Intensity);
	}

	void ~ScriptLight()
	{
		if(m_light)
		{
			RemoveLight(m_light);
		}
	}
}

#endif