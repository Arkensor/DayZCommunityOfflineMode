class SodaCan_Empty extends ItemBase
{
	override void OnActivatedByTripWire()
	{
		PlaySound("trapWireCans_0", 50 );
	}
}