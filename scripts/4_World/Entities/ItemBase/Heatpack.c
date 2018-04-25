class Heatpack : ItemBase
{
	override void OnWorkStart()
	{
		if (GetGame().IsServer())
		{
			SetTemperature(60);
		}
	}
	
	override void OnWorkStop()
	{
		if (GetGame().IsServer())
		{
			SetTemperature(0);
		}
	}
};
