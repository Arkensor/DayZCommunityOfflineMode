class Headtorch_ColorBase extends Clothing
{
	void Headtorch_ColorBase()
	{
		
	}

	override void OnWorkStart()
	{
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		SetPilotLight(false);
	}
}
