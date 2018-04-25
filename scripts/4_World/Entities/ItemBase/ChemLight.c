class Chemlight_ColorBase : ItemBase
{
	override void OnWorkStart()
	{
		SetPilotLight(true);
	}
	
	override void OnWorkStop()
	{
		SetPilotLight(false);
		SetObjectTexture(0, "");
	}
};
